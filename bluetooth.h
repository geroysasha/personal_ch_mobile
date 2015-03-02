#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <iostream>
#include <cstring>
#include <list>
#include <map>
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
public slots:
    void bt_search_remote_dev(void *);
    void deleteLater(){
        qDebug()<< "delete bluetooth";
        QObject::deleteLater();
    }
signals:
    void request(int, void *);
};

#endif // BLUETOOTH_H
