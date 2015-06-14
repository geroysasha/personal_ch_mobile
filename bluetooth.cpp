
#include "bluetooth.h"
#include <QDebug>

vector<string> Bluetooth::bt_scan_adapter()
{
    char cmd[] = "hcitool dev";
    char buf[BUFSIZ];
    FILE *ptr;
    vector<string> vec;

    if ((ptr = popen(cmd, "r")) != NULL)
            while (fgets(buf, BUFSIZ, ptr) != NULL)
            {
                if(((string) buf).find("hci") != string::npos)
                {
                    vec.insert(vec.begin()
                               , ((string) buf).substr(((string) buf).find(":") - 2
                                                       , ((string) buf).find(":") + 14));
                }
            }
    (void) pclose(ptr);

    return vec;
}

Bluetooth::Bluetooth()
{
    th = new QThread;
    this->moveToThread(th);
}

void Bluetooth::bluetooth_init_dialogs()
{
    dlgProgress = new DialogProgress[1];

    QObject::connect( this,
                      SIGNAL(dlgProgress_run()),
                      dlgProgress,
                      SLOT(run())
                      );
    QObject::connect( this,
                      SIGNAL(dlgProgress_stop()),
                      dlgProgress,
                      SLOT(stop())
                      );
}

void Bluetooth::signals_and_slot(char *method)
{

    QObject::connect( th, SIGNAL(started()),
                      this, method );

}

void Bluetooth::bt_search_remote_dev()
{

    emit dlgProgress_run();

    char hci = adapter_number + '0';
    string str = "";

    vector<string> *dev_map = new vector<string>;

    char cmd[strlen("hcitool -i ") + strlen("hci") +strlen(&hci) + strlen(" scan")];
    strcpy(cmd, "hcitool -i ");
    strcat(cmd, "hci");
    strcat(cmd, &hci);
    strcat(cmd, " scan");
    char buf[BUFSIZ];

    FILE *ptr;

    if ((ptr = popen(cmd, "r")) != NULL)
    {
        dev_map->push_back(&hci);

         while (fgets(buf, BUFSIZ, ptr) != NULL)
         {
             if(((string) buf).find(":") != string::npos)
             {
                 str.append(((string) buf).substr(((string) buf).find(":") - 2, ((string) buf).find(":") + 14));
                 str.append("\n");
                 str.append(((string) buf).substr(((string) buf).find(":") + 16, ((string) buf).length()));
                 dev_map->push_back(str);
                 str.clear();
              }
         }
         (void) pclose(ptr);
    }

    dlgProgress_stop();
    emit bluetooth_remote_device(dev_map);
}

void Bluetooth::remote_dev(int adptr_number)
{
    adapter_number = adptr_number;


    th->quit();
    th->wait();
    th->disconnect(SIGNAL(started()));

    signals_and_slot( (char*) SLOT(bt_search_remote_dev()) );

    th->start();
    sleep(1);
}

void Bluetooth::check_device(vector<string> *vec_devices)
{
    vec_dev = *vec_devices;

    th->quit();
    th->wait();
    th->disconnect(SIGNAL(started()));

    signals_and_slot((char*)  SLOT(bt_check_connect_remote_dev()));

    th->start();
    sleep(1);
}

void Bluetooth::bt_check_connect_remote_dev()
{
    emit dlgProgress_run();

    char hci[strlen( ((string) (vec_dev)[0]).c_str() )];
    char mac[strlen( ((string) (vec_dev)[1]).substr(0, LENGTH_MAC_ADD).c_str() )];

    strcpy(hci, ((string) (vec_dev)[0]).c_str() );
    strcpy(mac, ((string) (vec_dev)[1]).substr(0, LENGTH_MAC_ADD).c_str() );

    take_local_mac_address();
    if(take_rfcomm_port(hci, mac) && take_headset_port(hci, mac))
    {

        remote_mac_address.clear();
        remote_mac_address.append(mac);

        bluetooth_checked_remote_device(&vec_dev, true);
        //emit request(HANDLER_CORE_CHECK_CONNECT_REMOTE_DEVICE_OK, &vec);
    } else
    {
        bluetooth_checked_remote_device(&vec_dev, false);
        //char err[] = "Error this is divice not support!";
        //emit request(HANDLER_CORE_CHECK_CONNECT_REMOTE_DEVICE_ERROR, &vec);
        //emit request(HANDLER_CORE_SYS_ERROR, err);
    }
    emit dlgProgress_stop();
}

bool Bluetooth::take_rfcomm_port(char *hci, char *mac){

    remote_rfcomm_port = 99;

    char cmd[strlen("sdptool -i ") + strlen("hci") + strlen(hci) + strlen(" search 0x1101")];

    strcpy(cmd, "sdptool -i ");
    strcat(cmd, "hci");
    strcat(cmd, hci);
    strcat(cmd, " search 0x1101");

    remote_rfcomm_port = take_port(cmd, mac);

    if(remote_rfcomm_port != 99)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Bluetooth::take_headset_port(char *hci, char *mac){

    remote_headset_port = 99;

    char cmd[strlen("sdptool -i ") + strlen("hci") + strlen(hci) + strlen(" search 0x1112")];

    strcpy(cmd, "sdptool -i ");
    strcat(cmd, "hci");
    strcat(cmd, hci);
    strcat(cmd, " search 0x1108");

    remote_headset_port = take_port(cmd, mac);

    if(remote_headset_port != 99)
    {
        return true;//keypass(hci, mac);
    }
    else
    {
        return false;
    }
}

int Bluetooth::take_port(char *cmd, char *mac)
{
    char buf[BUFSIZ];
    bool find = false;
    FILE *ptr;
    int port = 99;

    if ((ptr = popen(cmd, "r")) != NULL)
            while (fgets(buf, BUFSIZ, ptr) != NULL)
            {
                if(((string) buf).find(mac) != string::npos || find == true)
                {
                    if(((string) buf).find("Channel") != string::npos)
                    {
                        if( atoi(((string) buf).substr(((string) buf).find(":") + 2,((string) buf).length() - 1).c_str()) < port)
                        {
                            port = atoi(((string) buf).substr(((string) buf).find(":") + 2,((string) buf).length() - 1).c_str());
                        }
                    }
                    else if(((string) buf).find("Searching for") != string::npos && find == true)
                    {
                        find = false;
                    }
                    else
                    {
                        find = true;
                    }

                }

            }
            (void) pclose(ptr);
    return port;
}



void Bluetooth::take_local_mac_address()
{

    char buf[BUFSIZ];
    char num = adapter_number + '0';
    char hci[5];

    strcpy(hci, "hci0");
    //strcat(hci, &num);

    FILE *ptr;
    qDebug()<<hci<<"\n";
    if ((ptr = popen("hcitool dev", "r")) != NULL)
    {

         while (fgets(buf, BUFSIZ, ptr) != NULL)
         {
                              qDebug()<<buf;
             if(((string) buf).find(hci) != string::npos)
             {
                 local_mac_address.clear();
                 local_mac_address.append(((string) buf).substr( ((string) buf).find(hci) + 5,
                                                                 ((string) buf).find(hci) + 16) );
                 qDebug()<<local_mac_address.c_str();
                 break;
              }
         }
         (void) pclose(ptr);
    } else
    {
        char err[] = "Error in get_local_mac_address(), <<popen>> returtn NULL";
        //emit request(HANDLER_CORE_SYS_ERROR, err);
    }

}

int Bluetooth::get_remote_rfcomm_port()
{
    return remote_rfcomm_port;
}

int Bluetooth::get_remote_headset_port()
{
    return remote_headset_port;
}

string Bluetooth::get_remote_mac_address()
{
    return remote_mac_address;
}

string Bluetooth::get_local_mac_address()
{
    return local_mac_address;
}

//bool Bluetooth::keypass(char *hci, char *mac)
//{
//    bool stat = false;
//    string request;
//    char cmd[strlen("echo \"0000\" | bluez-simple-agent hci 00:00:00:00:00:00 remove") + strlen(hci) + 1];

//    popen_interaction("bluez-simple-agent -h", request);

//    if(request.find("Usage:") != string::npos)
//    {
//        strcpy(cmd, "echo \"0000\" | bluez-simple-agent hci");
//        strcat(cmd, hci);
//        strcat(cmd, " ");
//        strcat(cmd, mac);
//    }else
//    {
//        strcpy(cmd, "echo \"0000\" | simple-agent hci");
//        strcat(cmd, hci);
//        strcat(cmd, " ");
//        strcat(cmd, mac);
//    }

//    popen_interaction(cmd, request);
//    if(request.find("Already Exists") != string::npos)
//    {
//        strcat(cmd, " remove");
//        popen_interaction(cmd, request);
//    }

//    if(request.find("New device") != string::npos)
//    {
//        stat = true;
//    }

//    return stat;
//}


//void Bluetooth::popen_interaction(char *cmd, string &str)
//{
//    char buf[BUFSIZ];
//    FILE *ptr;
//    str.clear();

//    qDebug()<<cmd;
//    if ((ptr = popen(cmd, "r")) != NULL)
//    {
//        while (fgets(buf, BUFSIZ, ptr) != NULL)
//        {
//            str.append(buf);
//        }
//    }
//    (void) pclose(ptr);
//}
