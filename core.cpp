#include "core.h"

Core* Core::s = 0;

Core::Core()
{
    bt_adapter_checked();
}

void Core::core_init_dialogs()
{
    dlgSelectList = new DialogSelectList[1];
    dlgAlert = new DialogAlert[1];

    QObject::connect( dlgSelectList, SIGNAL(dialogselectlist_select(vector<string> *)),
                      this, SLOT(dialogselectlist_select(vector<string> *)) );
    for(int i = 0; i < buff_vec.size(); i++)
    {
        bt[i]->bluetooth_init_dialogs();
    }
}

void Core::bt_adapter_checked()
{
    buff_vec = Bluetooth::bt_scan_adapter();

    if(buff_vec.size() != 0)
    {
        bt = new Bluetooth* [buff_vec.size()];

        for(unsigned i = 0; i < buff_vec.size(); i++)
        {
            bt[i] = new Bluetooth();
            connect( bt[i], SIGNAL(bluetooth_remote_device(vector<string> *)),
                     this, SLOT(bluetooth_remote_device(vector<string> *))
                     );
            connect( bt[i], SIGNAL( bluetooth_checked_remote_device(vector<string> *, bool)),
                     this, SLOT( bluetooth_checked_remote_device(vector<string> *, bool))
                     );
        }
    } else
    {
        char *str = new char[strlen("No BT adapter!") + 1];
        strcpy(str, "No BT adapter!");
        dlgAlert->run(str);
    }
}

void Core::mainwindow_remote_device(int adapter_number)
{
    bt[adapter_number]->remote_dev(adapter_number);
}

void Core::mainwindow_start_listener(string *registration_string)
{
    emit mobilechannel_pjsua_account_create(bt[atoi(registration_string->substr(0, registration_string->find("/")).c_str())],
                                                   registration_string->substr(registration_string->find("/") + 1, registration_string->rfind("/") - 2).c_str(),
                                                   registration_string->substr(registration_string->rfind("/") + 1, registration_string->length() - 1).c_str()
                                                   );
}

void Core::mainwindow_stop_listener()
{
    emit mobilechannel_pjsua_account_destroy();
}

void Core::mainwindow_close_applicatrion()
{
    emit close_application();
    delete this;
}

void Core::bluetooth_remote_device(vector<string> *vec_devices)
{
    dlgSelectList->run(vec_devices);
    delete vec_devices;
}

void Core::bluetooth_checked_remote_device(vector<string> *vec_devices, bool stat)
{
    emit mainwindow_selected_remote_dev(vec_devices, stat);
}

void Core::dialogselectlist_select(vector<string> *select_device)
{
    bt[atoi(((string) (*select_device)[0]).c_str())]->check_device(select_device);
    delete [] select_device;
}

void Core::core_logger(char *mess)
{
    char *str = new char[strlen(mess) + 1];
    strcpy(str, mess);

    if( ((string) str).find("ALERT") != string::npos )
    {
        dlgAlert->run(str);
        emit mainwindow_btn_state(true, false);
    } else
    {
        emit mainwindow_logger(str);
    }

    delete [] mess;
}

