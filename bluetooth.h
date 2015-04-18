#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#define LENGTH_MAC_ADD 17

#include <iostream>
#include <cstring>
#include <list>
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
#include "core.h"

using namespace std;

class Bluetooth : public QObject
{
    Q_OBJECT
public:
    Bluetooth();
    virtual ~Bluetooth(){}
    static list<string> bt_scan_adapter();
private:
    bool get_rfcomm_port(char *, char *);
    bool get_headset_port(char *, char *);
    int get_port(char *, char *);
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
