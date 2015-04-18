#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#define LENGTH_MAC_ADD 17

#include <iostream>
#include <cstring>
#include <vector>
#include <QObject>
#include <QDebug>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include "const.h"

using namespace std;

class Bluetooth : public QObject
{
    Q_OBJECT
public:
    Bluetooth();
    virtual ~Bluetooth(){}
    static vector<string> bt_scan_adapter();
    int get_remote_rfcomm_port();
    int get_remote_headset_port();
    string get_remote_mac_address();
    string get_local_mac_address();
private:
    int dev_num;
    int remote_rfcomm_port;
    int remote_headset_port;
    string remote_mac_address;
    string local_mac_address;
    bool take_rfcomm_port(char *, char *);
    bool take_headset_port(char *, char *);
    int take_port(char *, char *);
    void take_local_mac_address();
public slots:
    void bt_search_remote_dev(void *);
    void bt_check_connect_remote_dev(void *);
    void deleteLater(){
        qDebug()<< "delete bluetooth";
        this->disconnect(SIGNAL(request(int,void*)));
    }
signals:
    void request(int, void *);
};

#endif // BLUETOOTH_H
