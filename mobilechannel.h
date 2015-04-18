#ifndef MOBILECHANNEL_H
#define MOBILECHANNEL_H

#define SIP_STATE_DISCONNCTD "DISCONNCTD"
#define SIP_STATE_CONFIRMED "CONFIRMED"

#include <pjsua-lib/pjsua.h>
#include <pjlib.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/sco.h>

#include "bluetooth.h"

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


bool pjsua_bt_init(pjsua_call_info &);
bool at_send(int &s, char cmd[], unsigned int seconds);
bool rfcomm_read(int &s);
void bt_socket_close();

class MobileChannel : public QObject
{
    Q_OBJECT
public:
    static MobileChannel& Instance()
    {
        static MobileChannel s;
        return s;
    }

    struct Zigi_acc
    {
//        Zigi_acc()
//        {
//            acc_id = 0;
//            bt_adp_pointer = 0;
//            name_acc = "";
//            sock_rfcomm = 0;
//            sock_rfcomm_hs = 0;
//            sock_sco = 0;
//            sock_client_sco = 0;
//            mob_port = 0;
//        }

        pjsua_acc_id acc_id;
        Bluetooth *bt_adp_pointer;
        string name_acc;
        int sock_rfcomm;
        int sock_rfcomm_hs;
        int sock_sco;
        int sock_client_sco;
        pjsua_conf_port_id mob_port;
    };

    void init();
    void mob_Ch_emit(int, char *);

public slots:
    void pjsua_account_create(void *);
    void pjsua_account_destroy();

signals:
    void send(int, char *);

private:
    MobileChannel(){}
    virtual ~MobileChannel(){_pjsua_destroy();}

    MobileChannel(MobileChannel const&);
    MobileChannel& operator= (MobileChannel const&);

};

#endif // MOBILECHANNEL_H
