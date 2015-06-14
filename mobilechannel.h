#ifndef MOBILECHANNEL_H
#define MOBILECHANNEL_H

#define SIP_STATE_DISCONNCTD "DISCONNCTD"
#define SIP_STATE_CONFIRMED "CONFIRMED"
#define LOG_INFO "mobilechannel.c -> "

#include <pjsua-lib/pjsua.h>
#include <pjlib.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/sco.h>

#include "calltimer.h"
#include "posthttp.h"
#include "core.h"

pj_status_t mob_put_frame( pjmedia_port *, pjmedia_frame *);
pj_status_t mob_get_frame( pjmedia_port *, pjmedia_frame *);
void mobile_port(pjsua_call_info *);
pj_status_t create_mobile_port(pj_pool_t *,
                                      unsigned,
                                      unsigned,
                                      pjmedia_port **
                                      );
void _pjsua_init();
void _pjsua_destroy();
void error_exit();
void on_incoming_call(pjsua_acc_id, pjsua_call_id, pjsip_rx_data *);
void on_call_state(pjsua_call_id, pjsip_event *);
void on_call_media_state(pjsua_call_id );
void logCalback(int level, const char *data, int len);

bool popen_reset_hci();

class MobileChannel : public QObject
{
    Q_OBJECT
public:
    static MobileChannel& Instance()
    {
        if(!s)
             s = new MobileChannel();
        return *s;
    }

    struct Zigi_acc
    {
        pjsua_acc_id acc_id;
        Bluetooth *bt_adp_pointer;
        CallTimer callTimer;
        string name_acc;
        pjsua_player_id wav_player_id;
        pjsua_conf_port_id mob_port;
    };

    void init();
    void logger( char * );
    int get_sock_client_sco();
    bool pjsua_bt_init(pjsua_call_info &);
    bool at_send(int &s, char cmd[], unsigned int seconds);
    bool rfcomm_read(int &s);
    void bt_socket_close();
    bool bt_socket_rfcomm_create(bool socket_rfcomm_close);

public slots:
    void pjsua_account_create(Bluetooth *bluetooth, const char *field_email, const char *field_code);
    void pjsua_account_destroy();
    void pjsua_destroy();

signals:
    void core_logger(char *mess);


private:
    static MobileChannel *s;
    int sock_rfcomm;
    int sock_rfcomm_hs;
    int sock_sco;
    int sock_client_sco;

    MobileChannel();
    virtual ~MobileChannel()
    {
        qDebug()<<"destroys MobileChannel";
    }

    MobileChannel(MobileChannel const&);
    MobileChannel& operator= (MobileChannel const&);

};

#endif // MOBILECHANNEL_H
