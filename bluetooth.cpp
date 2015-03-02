
#include "bluetooth.h"

Bluetooth::Bluetooth()
{

}

void Bluetooth::bt_search_remote_dev(void *data)
{
    char *hci_dev = static_cast<char *>(data);

    map<string, string> dev_map;

    char cmd[strlen("hcitool -i ") + strlen(hci_dev) + strlen(" scan")];
    strcpy( cmd, "hcitool -i ");
    strcat(cmd, hci_dev);
    strcat(cmd, " scan");

    char buf[BUFSIZ];

    FILE *ptr;

    if ((ptr = popen(cmd, "r")) != NULL)
            while (fgets(buf, BUFSIZ, ptr) != NULL)
            {
                if(((string) buf).find(":") != string::npos)
                {
                    dev_map.insert(pair<string,string>(((string) buf).substr(((string) buf).find(":") - 2, ((string) buf).find(":") + 14)
                                                       ,((string) buf).substr(((string) buf).find(":") + 16, ((string) buf).length())));
                }
            }
            (void) pclose(ptr);

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
