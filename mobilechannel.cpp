#include "mobilechannel.h"

MobileChannel::Zigi_acc zigi_acc;


void MobileChannel::init()
{
    _pjsua_init();
}

void MobileChannel::pjsua_account_create(void *ptr)
{

    Bluetooth *pointer = static_cast<Bluetooth *>(ptr);
    pjsua_acc_config cfg;
    //pjsua_acc_id acc_id;

    //вызов функции получения данных регистрации на SIP сервере
    //а пока задаем все вручную
    char sip_user[] = "SIP_USER" ;
    char sip_domain[] = "SIP_DOMAIN";
    char sip_password[] = "SIP_PASSWD";
    //

    char str_id[strlen("sip:") + strlen(sip_user) + strlen("@") + strlen(sip_domain) + 1];
    char str_reg_uri[strlen("sip:") + strlen(sip_domain) + 1];

    strcpy(str_id, "sip:");
    strcat(str_id, sip_user);
    strcat(str_id, "@");
    strcat(str_id, sip_domain);

    strcpy(str_reg_uri, "sip:");
    strcat(str_reg_uri, sip_domain);

    pjsua_acc_config_default(&cfg);
    cfg.id = pj_str(str_id);
    cfg.reg_uri = pj_str(str_reg_uri);
    cfg.cred_count = 1;
    cfg.cred_info[0].realm = pj_str(sip_domain);
    cfg.cred_info[0].scheme = pj_str("digest");
    cfg.cred_info[0].username = pj_str(sip_user);
    cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
    cfg.cred_info[0].data = pj_str(sip_password);

    if (pjsua_acc_add(&cfg, PJ_TRUE, &zigi_acc.acc_id) != PJ_SUCCESS)
    {
        char err[] = "pjsua_acc_add(&cfg, PJ_TRUE, &zigi_acc.acc_id) error!!";
        MobileChannel::Instance().mob_Ch_emit(HANDLER_CORE_SYS_ERROR , err);
    }

    zigi_acc.bt_adp_pointer = pointer;

    qDebug() << pointer->get_remote_headset_port();
    qDebug() << pointer->get_remote_mac_address().c_str();
    qDebug() << pointer->get_remote_rfcomm_port();

    MobileChannel::Instance().mob_Ch_emit(HANDLER_CORE_PJSUA_ACC_CREATE_OK , NULL);
}

void MobileChannel::pjsua_account_destroy()
{
    if( pjsua_acc_del(zigi_acc.acc_id) != PJ_SUCCESS)
    {
        cout << "ERROR";
    }
}

void MobileChannel::mob_Ch_emit(int cmd, char *mess)
{
    emit send(cmd, mess);
}

pj_status_t mob_put_frame( pjmedia_port *this_port,
                                   pjmedia_frame *frame)
{
    PJ_UNUSED_ARG(this_port);
    PJ_UNUSED_ARG(frame);
}

pj_status_t mob_get_frame( pjmedia_port *this_port,
                                   pjmedia_frame *frame)
{
    PJ_UNUSED_ARG(this_port);
    PJ_UNUSED_ARG(frame);
}

pj_status_t create_mobile_port(pj_pool_t *pool,
                    unsigned sampling_rate,
                    unsigned channel_count,
                    pjmedia_port **p_port)
{
    pjmedia_port *port;
    pj_str_t name;

    PJ_ASSERT_RETURN(pool && channel_count > 0 && channel_count <= 2,
             PJ_EINVAL);

    port = static_cast< pjmedia_port* >(pj_pool_zalloc(pool, sizeof(pjmedia_port)));
    PJ_ASSERT_RETURN(port != NULL, PJ_ENOMEM);

    /* Fill in port info. */
    name = pj_str("mobile channel");
    pjmedia_port_info_init(&port->info, &name,
                           PJMEDIA_SIG_CLASS_PORT_AUD('s', 'i'),
                           sampling_rate,
                           channel_count,
                           16, 24);

    /* Set the function to feed frame */
    port->get_frame = &mob_get_frame;
    port->put_frame = &mob_put_frame;

    *p_port = port;

    return PJ_SUCCESS;
}

void mobile_port(pjsua_call_info *ci)
{

    pj_caching_pool cp;
    pjmedia_endpt *med_endpt;
    pj_pool_t *pool;
    pjmedia_port *mobile_port;

    /* Must init PJLIB first: */
    if(pj_init() != PJ_SUCCESS)
    {
        qDebug()<<"ERROR pj_init()";
    }

    /* Must create a pool factory before we can allocate any memory. */
    pj_caching_pool_init(&cp, &pj_pool_factory_default_policy, 0);

    /*
     * Initialize media endpoint.
     * This will implicitly initialize PJMEDIA too.
     */
    if(pjmedia_endpt_create(&cp.factory, NULL, 1, &med_endpt) != PJ_SUCCESS)
    {
        qDebug()<<"ERROR pjmedia_endpt_create(&cp.factory, NULL, 1, &med_endpt)";
    }

    /* Create memory pool for our sine generator */
    pool = pj_pool_create( &cp.factory,	    /* pool factory	    */
               "wav",	    /* pool name.	    */
               4000,	    /* init size	    */
               4000,	    /* increment size	    */
               NULL		    /* callback on error    */
               );

    /* Create a media port to generate sine wave samples. */
    if( create_mobile_port( pool,	    /* memory pool	    */
                   8000,	    /* sampling rate	    */
                   1,/* # of channels	    */
                   &mobile_port   /* returned port	    */
                     ) != PJ_SUCCESS)
    {
        qDebug()<<"ERROR create_mobile_port()";
    }

    pjsua_conf_add_port(pool, mobile_port, &zigi_acc.mob_port);

    pjsua_conf_connect(ci->conf_slot, zigi_acc.mob_port);
    pjsua_conf_connect(zigi_acc.mob_port, ci->conf_slot);

//    status = pjmedia_port_destroy( mobile_port );

//    /* Release application pool */
//    pj_pool_release( pool );

//    /* Destroy media endpoint. */
//    pjmedia_endpt_destroy( med_endpt );

//    /* Destroy pool factory */
//    pj_caching_pool_destroy( &cp );

//    /* Shutdown PJLIB */
//    pj_shutdown();

}

/* Callback called by the library upon receiving incoming call */
void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id,
                             pjsip_rx_data *rdata)
{
    pjsua_call_info ci;

    PJ_UNUSED_ARG(acc_id);
    PJ_UNUSED_ARG(rdata);

    pjsua_call_get_info(call_id, &ci);


    /* Automatically answer incoming calls with 200/OK */
    pjsua_call_answer(call_id, 200, NULL, NULL);
}

/* Callback called by the library when call's state has changed */
void on_call_state(pjsua_call_id call_id, pjsip_event *e)
{
    pjsua_call_info ci;

    PJ_UNUSED_ARG(e);

    pjsua_call_get_info(call_id, &ci);

    if(strcmp(ci.state_text.ptr, SIP_STATE_DISCONNCTD) == 0)
    {

        qDebug()<<"SIP_STATE_DISCONNCTD";
        bt_socket_close();
    }
}

/* Callback called by the library when call's media state has changed */
void on_call_media_state(pjsua_call_id call_id)
{
    pjsua_call_info ci;

    pjsua_call_get_info(call_id, &ci);
        qDebug() << ci.remote_info.ptr;
    if (ci.media_status == PJSUA_CALL_MEDIA_ACTIVE) {
        if(pjsua_bt_init(ci))
        {
            mobile_port(&ci);
        } else
        {
            qDebug()<<"error pjsua_bt_init(ci)";
            pjsua_call_hangup_all();
            bt_socket_close();
        }
    }
}

void logCalback(int level, const char *data, int len)
{
    string str = (const char *) data;
    if(str.find("Forbidden") != string::npos)
    {
        char err[] = "Registration failed, \n status=403 (Forbiddden)";
        MobileChannel::Instance().mob_Ch_emit(HANDLER_CORE_SYS_ERROR , err);
    }

}

void _pjsua_init()
{
    /* Create pjsua first! */
    if (pjsua_create() != PJ_SUCCESS)
    {
        char err[] = "pjsua_create() error!!";
        MobileChannel::Instance().mob_Ch_emit(HANDLER_CORE_SYS_ERROR , err);
    }

    /* Init pjsua */
    pjsua_config cfg;
    pjsua_media_config mcfg;
    pjsua_logging_config log_cfg;

    pjsua_media_config_default(&mcfg);
    pjsua_config_default(&cfg);
    cfg.cb.on_incoming_call = &on_incoming_call;
    cfg.cb.on_call_media_state = &on_call_media_state;
    cfg.cb.on_call_state = &on_call_state;
    mcfg.clock_rate = 8000;
    mcfg.snd_clock_rate = 44100;

    pjsua_logging_config_default(&log_cfg);
    log_cfg.console_level = 3;
    log_cfg.cb = &logCalback;

    if (pjsua_init(&cfg, &log_cfg, &mcfg) != PJ_SUCCESS)
    {
        char err[] = "pjsua_init(&cfg, &log_cfg, &mcfg) error!!";
        MobileChannel::Instance().mob_Ch_emit(HANDLER_CORE_SYS_ERROR , err);
    }

    /* Add UDP transport. */
    {
        pjsua_transport_config cfg;

        pjsua_transport_config_default(&cfg);
        cfg.port = 5060;
        pjsua_transport_create(PJSIP_TRANSPORT_UDP, &cfg, NULL);
    }

    /* Initialization is done, now start pjsua */
    if (pjsua_start() != PJ_SUCCESS)
    {
        char err[] = "pjsua_start() error!!";
        MobileChannel::Instance().mob_Ch_emit(HANDLER_CORE_SYS_ERROR , err);
    }

    MobileChannel::Instance().mob_Ch_emit(HANDLER_CORE_PJSUA_INIT_OK , NULL);
}

void error_exit()
{
    qDebug()<<"---PJSUA DESTROY---";
    pjsua_destroy();
    pj_shutdown();
    sleep(5);
    qDebug()<<"---PJSUA INIT---";
    _pjsua_init();
}


void _pjsua_destroy()
{
    qDebug()<<"---PJSUA DESTROY---";
    pjsua_destroy();
}

bool pjsua_bt_init(pjsua_call_info &ci)
{

    struct sockaddr_rc local_rfcomm;
    struct sockaddr_rc remote_rfcomm_sys_cmd;
    struct sockaddr_rc remote_rfcomm_hs;
    struct sockaddr_sco local_sco;

    zigi_acc.sock_rfcomm = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    zigi_acc.sock_rfcomm_hs = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    zigi_acc.sock_sco = socket(PF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_SCO);

    local_rfcomm.rc_family = AF_BLUETOOTH;
    str2ba( zigi_acc.bt_adp_pointer->get_local_mac_address().c_str(),
            &local_rfcomm.rc_bdaddr );

    if (bind(zigi_acc.sock_rfcomm, (struct sockaddr *) &local_rfcomm, sizeof(local_rfcomm)) < 0 &&
            bind(zigi_acc.sock_rfcomm_hs, (struct sockaddr *) &local_rfcomm, sizeof(local_rfcomm)) < 0)
    {
        return false;
    }

    remote_rfcomm_sys_cmd.rc_family = AF_BLUETOOTH;
    str2ba( zigi_acc.bt_adp_pointer->get_remote_mac_address().c_str(),
            &remote_rfcomm_sys_cmd.rc_bdaddr );
    remote_rfcomm_sys_cmd.rc_channel = (uint8_t) zigi_acc.bt_adp_pointer->get_remote_rfcomm_port();

    remote_rfcomm_hs.rc_family = AF_BLUETOOTH;
    str2ba( zigi_acc.bt_adp_pointer->get_remote_mac_address().c_str(),
            &remote_rfcomm_hs.rc_bdaddr );
    remote_rfcomm_hs.rc_channel = (uint8_t) zigi_acc.bt_adp_pointer->get_remote_headset_port();


    if( connect(zigi_acc.sock_rfcomm, (struct sockaddr *)&remote_rfcomm_sys_cmd, sizeof(remote_rfcomm_sys_cmd)) < 0)
    {
        return false;
    } else
    {
        string info = (const char *) ci.remote_info.ptr;

        char numb[strlen("ATD") + info.substr(2,info.find(" ") - 3).size() + strlen(";\r") + 1];
        strcpy(numb, "ATD");
        strcat(numb, info.substr(2,info.find(" ") - 3).c_str());
        strcat(numb, ";\r");

        if(at_send(zigi_acc.sock_rfcomm, numb, 3))
        {

            if( connect(zigi_acc.sock_rfcomm_hs, (struct sockaddr *)&remote_rfcomm_hs, sizeof(remote_rfcomm_hs)) < 0)
            {
                return false;
            } else
            {
                at_send(zigi_acc.sock_rfcomm_hs, "AT+CKPD=200;\r", 0.1);
                at_send(zigi_acc.sock_rfcomm_hs, "AT+CKPD=456;\r", 0.1);

            }
        } else
        {
            return false;
        }

    }


    local_sco.sco_family = AF_BLUETOOTH;
    str2ba( zigi_acc.bt_adp_pointer->get_local_mac_address().c_str(),
            &local_sco.sco_bdaddr );

    if (bind(zigi_acc.sock_sco, (struct sockaddr *) &local_sco, sizeof(local_sco)) < 0)
    {
        qDebug()<<"ERROR bind(zigi_acc.sock_sco, (struct sockaddr *) &local_sco, sizeof(local_sco))";
        return false;
    }

    listen(zigi_acc.sock_sco, 1);

    socklen_t optlen = sizeof(local_sco);
    zigi_acc.sock_client_sco = accept(zigi_acc.sock_sco, (struct sockaddr *) &local_sco, &optlen);
    qDebug()<<"accept client:"<<zigi_acc.sock_client_sco;

    if (zigi_acc.sock_client_sco < 0)
    {
        qDebug()<<"ERROR zigi_acc.sock_client_sco";
        return false;
    }
    return true;
}

void bt_socket_close()
{

    at_send(zigi_acc.sock_rfcomm, "AT+CHUP;\r", 1);
    sleep(0.2);

    close(zigi_acc.sock_rfcomm);
    //zigi_acc.sock_rfcomm = 0;
    sleep(0.2);

    close(zigi_acc.sock_rfcomm_hs);
    //zigi_acc.sock_rfcomm = 0;
    sleep(0.2);

    close(zigi_acc.sock_client_sco);
    //zigi_acc.sock_client_sco = 0;
    sleep(0.2);

    close(zigi_acc.sock_sco);
    //zigi_acc.sock_sco = 0;
    sleep(0.2);
    qDebug()<<"bt_socket_close()";
}


bool at_send(int &s, char cmd[], unsigned int seconds)
{
   int status;
   status = write(s, cmd, strlen(cmd));
   sleep(seconds);
   if(status > 0)
   {
       if(rfcomm_read(s))
       {
           return true;
       }
   }
   return false;
}

bool rfcomm_read(int &s)
{
    char buf[32];
    recv(s, buf, sizeof(buf), 0);
    string str = (const char *) buf;
    if(str.find("ERROR") != string::npos)
    {
        return false;
    }
    qDebug()<<buf;
    return true;
}

