#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#define LENGTH_MAC_ADD 17

#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/sco.h>
#include "const.h"
#include "dialogprogress.h"

using namespace std;

class Bluetooth : public QObject
{
    Q_OBJECT
public:
    Bluetooth();
    virtual ~Bluetooth()
    {
        delete [] dlgProgress;
    }
    static vector<string> bt_scan_adapter();
    int get_remote_rfcomm_port();
    int get_remote_headset_port();
    string get_remote_mac_address();
    string get_local_mac_address();


    void remote_dev( int adptr_number );
    void check_device(vector<string> *vec_devices);
    void bluetooth_init_dialogs();
private:
    int adapter_number;
    int remote_rfcomm_port;
    int remote_headset_port;
    string remote_mac_address;
    string local_mac_address;
    QThread *th;
    DialogProgress *dlgProgress;
    vector<string> vec_dev;
    void signals_and_slot(char *method);
    bool take_rfcomm_port(char *, char *);
    bool take_headset_port(char *, char *);
    int take_port(char *, char *);
    void take_local_mac_address();
//    bool keypass(char *hci, char *mac);
//    //float bluez_version();
//    void popen_interaction(char *cmd, string &str);

private slots:
    void bt_search_remote_dev();
    void bt_check_connect_remote_dev();
signals:
    //void request(int, void *);
    //void send(int, char *);
    void bluetooth_remote_device(vector<string> *vec_devices);
    void bluetooth_checked_remote_device(vector<string> *, bool);
    void dlgProgress_run();
    void dlgProgress_stop();
};

#endif // BLUETOOTH_H
