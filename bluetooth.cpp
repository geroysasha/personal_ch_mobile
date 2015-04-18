
#include "bluetooth.h"

int remote_rfcomm_port;
int remote_headset_port;


Bluetooth::Bluetooth()
{

}

void Bluetooth::bt_search_remote_dev(void *data)
{
    int *dev = static_cast<int *>(data);
    char hci = *dev + '0';
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
    data = &dev_map;
    emit request(HANDLER_CORE_REQUEST_SEARCH_REMOTE_DEVICE, data);
    sleep(1);
}

list<string> Bluetooth::bt_scan_adapter()
{
    char *cmd = "hcitool dev";
    char buf[BUFSIZ];
    FILE *ptr;
    list<string> lst;

    if ((ptr = popen(cmd, "r")) != NULL)
            while (fgets(buf, BUFSIZ, ptr) != NULL)
            {
                if(((string) buf).find("hci") != string::npos)
                {
                    lst.insert(lst.begin()
                               , ((string) buf).substr(((string) buf).find(":") - 2
                                                       , ((string) buf).find(":") + 14));
                }
            }
            (void) pclose(ptr);
    return lst;
}

bool Bluetooth::get_rfcomm_port(char *hci, char *mac){

    remote_rfcomm_port = 99;

    char cmd[strlen("sdptool -i ") + strlen("hci") + strlen(hci) + strlen(" search 0x1101")];

    strcpy(cmd, "sdptool -i ");
    strcat(cmd, "hci");
    strcat(cmd, hci);
    strcat(cmd, " search 0x1101");

    remote_rfcomm_port = get_port(cmd, mac);

    if(remote_rfcomm_port != 99)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Bluetooth::get_headset_port(char *hci, char *mac){

    remote_headset_port = 99;

    char cmd[strlen("sdptool -i ") + strlen("hci") + strlen(hci) + strlen(" search 0x1112")];

    strcpy(cmd, "sdptool -i ");
    strcat(cmd, "hci");
    strcat(cmd, hci);
    strcat(cmd, " search 0x1112");

    remote_headset_port = get_port(cmd, mac);

    if(remote_headset_port != 99)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int Bluetooth::get_port(char *cmd, char *mac)
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

void Bluetooth::bt_check_connect_remote_dev(void *data)
{
    vector<string> vec = *static_cast<vector<string> *>(data);

    char hci[strlen(static_cast<string>(vec[0]).c_str())];
    char mac[strlen(static_cast<string>(vec[1]).substr(0, LENGTH_MAC_ADD).c_str())];

    strcpy(hci, static_cast<string>(vec[0]).c_str());
    strcpy(mac, static_cast<string>(vec[1]).substr(0, LENGTH_MAC_ADD).c_str());

    data = &vec;
    if(get_rfcomm_port(hci, mac) && get_headset_port(hci, mac))
    {
        emit request(HANDLER_CORE_CHECK_CONNECT_REMOTE_DEVICE_OK, data);
    }
    else
    {
        emit request(HANDLER_CORE_CHECK_CONNECT_REMOTE_DEVICE_ERROR, data);
    }
    sleep(1);
}
