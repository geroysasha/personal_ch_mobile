
#include "bluetooth.h"
#include <QDebug>
Bluetooth::Bluetooth()
{

}

void Bluetooth::bt_search_remote_dev(void *data)
{
    dev_num = *static_cast<int *>(data);
    char hci = dev_num + '0';
    string str = "";

    vector<string> dev_map;

    char cmd[strlen("hcitool -i ") + strlen("hci") +strlen(&hci) + strlen(" scan")];
    strcpy(cmd, "hcitool -i ");
    strcat(cmd, "hci");
    strcat(cmd, &hci);
    strcat(cmd, " scan");
    char buf[BUFSIZ];

    FILE *ptr;

    if ((ptr = popen(cmd, "r")) != NULL)
    {
        dev_map.push_back(&hci);

         while (fgets(buf, BUFSIZ, ptr) != NULL)
         {
             if(((string) buf).find(":") != string::npos)
             {
                 str.append(((string) buf).substr(((string) buf).find(":") - 2, ((string) buf).find(":") + 14));
                 str.append("\n");
                 str.append(((string) buf).substr(((string) buf).find(":") + 16, ((string) buf).length()));
                 dev_map.push_back(str);
                 str.clear();
              }
         }
         (void) pclose(ptr);
    }
    if(dev_map.size() != 1)
    {
        data = &dev_map;
        emit request(HANDLER_CORE_REQUEST_SEARCH_REMOTE_DEVICE, data);
    } else
    {
        char err[] = "No find remote devices";
        emit request(HANDLER_CORE_SYS_ERROR, &err);
    }
    sleep(1);
}

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
//        char buf[32];
//        write(s, "ATD0000000000", 13);
//        sleep(0.5);
//        recv(s, buf, sizeof(buf), 0);
//        write(s, "AT+CHUP;\r", 10);
//        sleep(0.5);
//        recv(s, buf, sizeof(buf), 0);

        return true;
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
                    qDebug()<<buf;
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

void Bluetooth::bt_check_connect_remote_dev(void *data)
{
    vector<string> vec = *static_cast<vector<string> *>(data);

    char hci[strlen(static_cast<string>(vec[0]).c_str())];
    char mac[strlen(static_cast<string>(vec[1]).substr(0, LENGTH_MAC_ADD).c_str())];

    strcpy(hci, static_cast<string>(vec[0]).c_str());
    strcpy(mac, static_cast<string>(vec[1]).substr(0, LENGTH_MAC_ADD).c_str());

    data = &vec;
    take_local_mac_address();
    if(take_rfcomm_port(hci, mac) && take_headset_port(hci, mac))
    {

        remote_mac_address.clear();
        remote_mac_address.append(mac);

        emit request(HANDLER_CORE_CHECK_CONNECT_REMOTE_DEVICE_OK, &vec);
    }
    else
    {
        char err[] = "Error this is divice not support!";
        emit request(HANDLER_CORE_CHECK_CONNECT_REMOTE_DEVICE_ERROR, &vec);
        emit request(HANDLER_CORE_SYS_ERROR, err);
    }
    sleep(1);
}

void Bluetooth::take_local_mac_address()
{

    char buf[BUFSIZ];
    char num = dev_num + '0';
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
        emit request(HANDLER_CORE_SYS_ERROR, err);
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
