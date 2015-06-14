#include "mobilechannel.h"

MobileChannel::Zigi_acc *zigi_acc = new MobileChannel::Zigi_acc;
MobileChannel* MobileChannel::s = 0;

MobileChannel::MobileChannel()
{
    QObject::connect(  Core::Instance(),
                       SIGNAL( mobilechannel_pjsua_account_create(Bluetooth *, const char *, const char *) ),
                       this,
                       SLOT( pjsua_account_create(Bluetooth *, const char *, const char *) )
                       );
    QObject::connect(  Core::Instance(),
                       SIGNAL( mobilechannel_pjsua_account_destroy() ),
                       this,
                       SLOT( pjsua_account_destroy() )
                       );
    QObject::connect(  Core::Instance(),
                       SIGNAL( close_application() ),
                       this,
                       SLOT( pjsua_destroy() )
                       );
    QObject::connect(  this,
                       SIGNAL( core_logger(char *) ),
                       Core::Instance(),
                       SLOT( core_logger(char *) )
                       );
    this->sock_rfcomm = -1;
    this->sock_rfcomm_hs = -1;
    this->sock_sco = -1;
    this->sock_client_sco = -1;
}

void MobileChannel::init()
{
    _pjsua_init();
}

void MobileChannel::pjsua_account_create(Bluetooth *ptr, const char *field_email, const char *field_code)
{

    pjsua_acc_config cfg;
    PostHTTP postHTTP;

    char *email = new char[strlen(field_email)];
    strcpy(email, field_email);

    char *pass = new char[strlen(field_code)];
    strcpy(pass, field_code);

    string data[3] = {"","",""};

    if(postHTTP.post_request(email, pass, data))
    {

        char sip_user[data[0].length()];
        char sip_domain[data[2].length()];
        char sip_password[data[1].length()];

        strcpy(sip_user, data[0].c_str());
        strcpy(sip_domain, data[2].c_str());
        strcpy(sip_password, data[1].c_str());

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
        cfg.cred_info[0].scheme = pj_str( (char *)"digest" );
        cfg.cred_info[0].username = pj_str(sip_user);
        cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
        cfg.cred_info[0].data = pj_str(sip_password);

        if (pjsua_acc_add(&cfg, PJ_TRUE, &zigi_acc->acc_id) != PJ_SUCCESS)
        {
            MobileChannel::Instance().logger( (char *)"ERROR -> pjsua_acc_add");
        } else
        {
            zigi_acc->bt_adp_pointer = ptr;
            bt_socket_rfcomm_create(true);

            logger( (char *) "OK -> account created" );
//            if(!bt_socket_rfcomm_create(true))
//            {
//                char message[] = "Authorization Error! \n Check email or code";
//                mob_Ch_emit(HANDLER_CORE_ALERT, message);
//            }else
//            {
//                MobileChannel::Instance().mob_Ch_emit(HANDLER_CORE_PJSUA_ACC_CREATE_OK , NULL);
//            }
        }

    } else
    {
        logger( (char *)"ALERT -> Authorization Error! \n Check email or code");
    }

    delete email;
    delete pass;

}

int MobileChannel::get_sock_client_sco()
{
    return this->sock_client_sco;
}


void MobileChannel::pjsua_account_destroy()
{

    if( pjsua_acc_del(zigi_acc->acc_id) != PJ_SUCCESS)
    {
        logger( (char *) "ERROR -> account destroy" );
    } else
    {
        logger( (char *) "OK -> account destroy" );
    }
}

void MobileChannel::pjsua_destroy()
{
    _pjsua_destroy();

    delete this;
}

void MobileChannel::logger(char *mess)
{
    char *str = new char[strlen(mess) + strlen(LOG_INFO) + 1];
    strcpy(str, LOG_INFO);
    strcat(str, mess);
    emit core_logger(str);
}

bool MobileChannel::bt_socket_rfcomm_create(bool socket_rfcomm_close)
{
    struct sockaddr_rc remote_rfcomm_sys_cmd;

    this->sock_rfcomm = ::socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    remote_rfcomm_sys_cmd.rc_family = AF_BLUETOOTH;
    remote_rfcomm_sys_cmd.rc_channel = (uint8_t) zigi_acc->bt_adp_pointer->get_remote_rfcomm_port();
    ::str2ba( zigi_acc->bt_adp_pointer->get_remote_mac_address().c_str(),
            &remote_rfcomm_sys_cmd.rc_bdaddr );


    struct sockaddr_rc local_rfcomm;

    ::memset(&local_rfcomm, 0, sizeof(local_rfcomm));

    local_rfcomm.rc_family = AF_BLUETOOTH;
    local_rfcomm.rc_channel = 0;

    ::str2ba( zigi_acc->bt_adp_pointer->get_local_mac_address().c_str(),
            &local_rfcomm.rc_bdaddr );

    if (bind(this->sock_rfcomm, (struct sockaddr *) &local_rfcomm, sizeof(local_rfcomm)) < 0 )
    {
        qDebug()<<"error .... BIND";
    }

    if( ::connect(this->sock_rfcomm, (struct sockaddr *)&remote_rfcomm_sys_cmd, sizeof(remote_rfcomm_sys_cmd)) < 0)
    {

        logger( (char *)"ERROR>>  Check connection with a device");
        return false;
    } else
    {

        if(socket_rfcomm_close)
        {
            ::close(this->sock_rfcomm);
        }
    }
    return true;
}

pj_status_t mob_put_frame( pjmedia_port *this_port,
                                   pjmedia_frame *frame)
{
    PJ_UNUSED_ARG(this_port);

    return PJ_SUCCESS;
}

pj_status_t mob_get_frame( pjmedia_port *this_port,
                                   pjmedia_frame *frame)
{
    PJ_UNUSED_ARG(this_port);

    return PJ_SUCCESS;
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
    name = pj_str( (char *)"mobile channel" );
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
    pjmedia_port *mob_prt;

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
                   &mob_prt   /* returned port	    */
                     ) != PJ_SUCCESS)
    {
        qDebug()<<"ERROR create_mobile_port()";
    }

    pjsua_conf_add_port(pool, mob_prt, &zigi_acc->mob_port);

    pjsua_conf_connect(ci->conf_slot, zigi_acc->mob_port);
    pjsua_conf_connect(zigi_acc->mob_port, ci->conf_slot);

    pjmedia_port_destroy( mob_prt );

    /* Release application pool */
    pj_pool_release( pool );

    /* Destroy media endpoint. */
    pjmedia_endpt_destroy( med_endpt );

    /* Destroy pool factory */
    pj_caching_pool_destroy( &cp );

    /* Shutdown PJLIB */
    pj_shutdown();

}

/* Callback called by the library upon receiving incoming call */
void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id,
                             pjsip_rx_data *rdata)
{
    pjsua_call_info ci;

    PJ_UNUSED_ARG(acc_id);
    PJ_UNUSED_ARG(rdata);

    pjsua_call_get_info(call_id, &ci);

    MobileChannel::Instance().logger( (char *) ci.remote_info.ptr );
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
        zigi_acc->callTimer.stopTimer();
        MobileChannel::Instance().logger( (char *) "OK>>SIP_STATE_DISCONNCTD" );

        pjsua_conf_disconnect(ci.conf_slot, zigi_acc->mob_port);
        pjsua_conf_disconnect(zigi_acc->mob_port, ci.conf_slot);

        if(zigi_acc->mob_port > 0)
        {

            pjsua_conf_remove_port(zigi_acc->mob_port);
            zigi_acc->mob_port = 0;
            MobileChannel::Instance().bt_socket_close();
            pjsua_call_hangup_all();

        }
    } else if(strcmp(ci.state_text.ptr, SIP_STATE_CONFIRMED) == 0)
    {
        if(MobileChannel::Instance().pjsua_bt_init(ci))
        {

            MobileChannel::Instance().logger( (char *) "OK>>BT init" );

            pjsua_conf_disconnect(pjsua_player_get_conf_port(zigi_acc->wav_player_id), ci.conf_slot);

            mobile_port(&ci);
            zigi_acc->callTimer.startTimer();
        } else
        {
            MobileChannel::Instance().logger( (char *) "ERROR>>initialize bluetooth channel" );
            zigi_acc->mob_port = 0;
            MobileChannel::Instance().bt_socket_close();
            pjsua_call_hangup_all();
        }
    }

}

/* Callback called by the library when call's media state has changed */
void on_call_media_state(pjsua_call_id call_id)
{
    pjsua_call_info ci;
    pjsua_call_get_info(call_id, &ci);

    if (ci.media_status == PJSUA_CALL_MEDIA_ACTIVE)
    {

        pj_str_t fName = pj_str( (char *)"wait_sound.wav" );
        pjsua_player_create(&fName, 0, &zigi_acc->wav_player_id);

        pjsua_conf_connect(pjsua_player_get_conf_port(zigi_acc->wav_player_id), ci.conf_slot);
    }
}

bool popen_reset_hci()
{
    char buf[BUFSIZ];
    FILE *ptr;
    string str;
    int counter = 0;

    if ((ptr = popen("hcitool dev 2>&1", "r")) != NULL)
    {
        while (fgets(buf, BUFSIZ, ptr) != NULL)
        {
            str.append(buf);
        }

        if(str.find("hci") == string::npos)
        {
            popen("hciconfig hci0 down 2>&1", "r");
            do
            {
                str.clear();
                if ((ptr = popen("hciconfig hci0 up  \n hcitool dev 2>&1", "r")) != NULL)
                {
                    while (fgets(buf, BUFSIZ, ptr) != NULL)
                    {
                        str.append(buf);
                    }
                }
                (void) pclose(ptr);
                counter++;
                if(counter > 5)
                {
                    return false;
                }
            } while ( str.find("hci") == string::npos);
        } else
        {
            (void) pclose(ptr);
        }
    }

    return true;
}

void logCalback(int level, const char *data, int len)
{
    string str = (const char *) data;
    if(str.find("Forbidden") != string::npos)
    {
        MobileChannel::Instance().logger( (char *) "ERROR>> Registration failed, \n status=403 (Forbiddden)" );
    } else if(str.find("pjsua_call_hangup()") != string::npos)
    {
        qDebug()<<"ERROR>>pjsua_call_hangup()";
    } else if(str.find("pjsua_acc.c") == string::npos &&
              str.find("pjsua_call.c") == string::npos &&
              str.find("pjsua_core.c") == string::npos )
    {
        MobileChannel::Instance().logger( (char *)data );
    }

}

void _pjsua_init()
{

    /* Create pjsua first! */
    if (pjsua_create() != PJ_SUCCESS)
    {
        MobileChannel::Instance().logger( (char *) "ERROR>> pjsua_create() error!!" );
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
    mcfg.ec_tail_len = 1000;
    mcfg.ec_options = PJMEDIA_ECHO_SIMPLE;


    pjsua_logging_config_default(&log_cfg);
    log_cfg.console_level = 3;
    log_cfg.cb = &logCalback;

    if (pjsua_init(&cfg, &log_cfg, &mcfg) != PJ_SUCCESS)
    {
        MobileChannel::Instance().logger( (char *) "ERROR -> pjsua_init(&cfg, &log_cfg, &mcfg) error!!" );
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
        MobileChannel::Instance().logger( (char *) "ERROR -> pjsua_start() error!!" );
    }

    pjsua_set_null_snd_dev();

    MobileChannel::Instance().logger( (char *) "OK -> pjsua initialized" );
}

void error_exit()
{
    pjsua_destroy();
    qDebug()<<"---PJSUA DESTROY---\n";
    sleep(5);
    qDebug()<<"---PJSUA INIT---";
    _pjsua_init();
}


void _pjsua_destroy()
{
    delete zigi_acc;
    pjsua_stop_worker_threads();
    pjsua_destroy();

    qDebug()<<"---PJSUA DESTROY---\n";
}

bool MobileChannel::pjsua_bt_init(pjsua_call_info &ci)
{

    struct sockaddr_sco local_sco;
    struct sockaddr_rc remote_rfcomm_hs;

    ::memset(&local_sco, 0, sizeof(local_sco));
    ::memset(&remote_rfcomm_hs, 0, sizeof(remote_rfcomm_hs));

    if(!popen_reset_hci())
    {
        return false;
    }

    if(!bt_socket_rfcomm_create(false))
    {
        MobileChannel::Instance().logger( (char *) "ERROR>> no create RFCOMM socket" );
        return false;
    }

    this->sock_rfcomm_hs = ::socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    this->sock_sco = ::socket(PF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_SCO);

    remote_rfcomm_hs.rc_family = AF_BLUETOOTH;
    remote_rfcomm_hs.rc_channel = (uint8_t) zigi_acc->bt_adp_pointer->get_remote_headset_port();
    ::str2ba( zigi_acc->bt_adp_pointer->get_remote_mac_address().c_str(),
            &remote_rfcomm_hs.rc_bdaddr );

    if( ::connect(this->sock_rfcomm_hs, (struct sockaddr *)&remote_rfcomm_hs, sizeof(remote_rfcomm_hs)) < 0)
    {
        qDebug()<<"return CONNECT 2";
        return false;
    }

    local_sco.sco_family = AF_BLUETOOTH;
    ::str2ba( zigi_acc->bt_adp_pointer->get_local_mac_address().c_str(),
            &local_sco.sco_bdaddr );

    if ( ::bind(this->sock_sco, (struct sockaddr *) &local_sco, sizeof(local_sco)) < 0 )
    {
        qDebug()<<"ERROR bind(zigi_acc.sock_sco, (struct sockaddr *) &local_sco, sizeof(local_sco))";
        return false;
    }

    string info = (const char *) ci.remote_info.ptr;
    char numb[strlen("ATD") + info.substr(2,info.find(" ") - 3).size() + strlen(";\r") + 1];
    strcpy(numb, "ATD");
    strcat(numb, info.substr(2,info.find(" ") - 3).c_str());
    strcat(numb, ";\r");
    at_send(this->sock_rfcomm, numb, 2);
    at_send(this->sock_rfcomm_hs, (char *)"AT+CKPD=200;\r", 0.2);
    at_send(sock_rfcomm_hs, (char *)"AT+CKPD=456;\r", 0.2);


    ::listen(this->sock_sco, 1);

    socklen_t optlen = sizeof(local_sco);

    int select_res = 0;
    fd_set s_set;
    FD_ZERO(&s_set);
    FD_SET(this->sock_sco, &s_set);
    timeval timeout = {3, 0};

    select_res = ::select(this->sock_sco + 1, &s_set, 0, 0, &timeout);

    if (!select_res)
    {
        qDebug()<<"SCO accep ERROR";
        return false;
    }

    this->sock_client_sco = ::accept(this->sock_sco, (struct sockaddr *) &local_sco, &optlen);
    MobileChannel::Instance().logger( (char *) "OK>> SCO create" );
    if (this->sock_client_sco < 0)
    {
        qDebug()<<"ERROR zigi_acc.sock_client_sco";
        return false;
    }

    return true;
}

void MobileChannel::bt_socket_close()
{

    at_send(this->sock_rfcomm_hs, (char *)"AT+CHUP;\r", 1);
    sleep(0.2);

    qDebug()<< ::close(this->sock_client_sco);
    sleep(0.5);

    qDebug()<< ::close(this->sock_sco);
    sleep(0.5);

    qDebug()<< ::close(this->sock_rfcomm_hs);
    sleep(0.5);

    qDebug()<<close(this->sock_rfcomm);
    sleep(0.5);

    MobileChannel::Instance().logger( (char *) "OK>> BT socket close" );
}


bool MobileChannel::at_send(int &s, char cmd[], unsigned int seconds)
{
   int status;

   status = ::write(s, cmd, strlen(cmd));
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

bool MobileChannel::rfcomm_read(int &s)
{
    char buf[32];
    ::memset(buf, 0, sizeof(buf));
    recv(s, buf, sizeof(buf), 0);
    string str = (const char *) buf;
    if(str.find("ERROR") != string::npos)
    {
        return false;
    }
    qDebug()<<buf;
    return true;
}

