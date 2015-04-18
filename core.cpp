#include "core.h"

Core::Core()
{
    bt = 0;
    sysThread = 0;
}

void Core::core_handler(int ch, void *data)
{
    switch(ch)
    {
    case HANDLER_CORE_BLUETOOTH_LOCAL_ADAPTER:
        break;
    case HANDLER_CORE_BLUETOOTH_LOCAL_ADAPTER_ERROR:
        break;
    case HANDLER_CORE_BLUETOOTH_REMOTE_DEVICE:
        break;
    case HANDLER_CORE_BLUETOOTH_REMOTE_DEVICE_ERROR:
        break;
    case HANDLER_CORE_GUI_SEARCH_REMOTE_DEVICE:

        if(*static_cast<int*>(data) < Bluetooth::bt_scan_adapter().size() && bt != 0)
        {
            core_thread_connect(sysThread[*static_cast<int*>(data)],
                                      bt[*static_cast<int*>(data)],
                                      SLOT(bt_search_remote_dev(void *)),
                                      data);
            sysThread[*static_cast<int*>(data)]->start();
            emit dialog_progress_show();
        } else
        {
            if(bt != 0)
            {
                delete[] bt;
                delete[] sysThread;
                bt = 0;
                sysThread = 0;
            }

            bt_adapter_checked();
            char err[] = "There is not enough \n BT adapters";
            core_handler(HANDLER_CORE_SYS_ERROR, &err);
        }
        break;
    case HANDLER_CORE_REQUEST_SEARCH_REMOTE_DEVICE:
        emit dialog_progress_stop();
        emit dialog_select_list_show(*static_cast<vector<string> *>(data));
        break;
    case HANDLER_CORE_LOAD_ADAPTER_POINTER:
        break;
    case HANDLER_CORE_DIALOG_SELECT_ITEM:
        buff_vec = *static_cast<vector<string> *>(data);
        core_thread_connect(sysThread[atoi(static_cast<string>(buff_vec[0]).c_str())],
                            bt[atoi(static_cast<string>(buff_vec[0]).c_str())],
                            SLOT(bt_check_connect_remote_dev(void *)),
                            data);
        sysThread[buff_int]->start();
        emit dialog_progress_show();
        break;
    case HANDLER_CORE_CHECK_CONNECT_REMOTE_DEVICE_OK:
        emit dialog_progress_stop();
        emit mainwindow_selected_remote_dev(*static_cast<vector<string> *>(data), true);
        qDebug()<<"HANDLER_CORE_CHECK_CONNECT_REMOTE_DEVICE_OK";
        break;
    case HANDLER_CORE_CHECK_CONNECT_REMOTE_DEVICE_ERROR:
        emit dialog_progress_stop();
        emit mainwindow_selected_remote_dev(*static_cast<vector<string> *>(data), false);
        qDebug()<<"HANDLER_CORE_CHECK_CONNECT_REMOTE_DEVICE_ERROR";
        break;
    case HANDLER_CORE_SYS_ERROR:
        emit dialog_progress_stop();
        emit mainwindow_btn_state(false, false);
        emit dialog_alert_show(static_cast<char *>(data));
        break;
    case HANDLER_CORE_GUI_START:
        emit pjsua_account_create(bt[*static_cast<int*>(data)]);
        break;
    case HANDLER_CORE_GUI_STOP:
        emit pjsua_account_destroy();
        break;
    case HANDLER_CORE_PJSUA_INIT_OK:
        bt_adapter_checked();
        break;
    case HANDLER_CORE_PJSUA_ACC_CREATE_OK:
        qDebug()<<"HANDLER_CORE_PJSUA_ACC_CREATE_OK";
        break;
    default:

        qDebug()<<"HANDLER_CORE unknown code";

        break;
    }

}

void Core::core_thread_connect(SysThread *sysThread, QObject *obj, char *method, void *data)
{
    sysThread->set_buff(data);

    QObject::connect(sysThread,SIGNAL(started()), sysThread, SLOT(run()));

    QObject::connect(sysThread,SIGNAL(work(void *)), obj, method);

    QObject::connect(obj, SIGNAL(request(int,void *)), sysThread, SLOT(request(int, void *)));

    QObject::connect(sysThread,SIGNAL(thread_end(int, void *)), &Core::Instance(), SLOT(core_handler(int, void *)));

    QObject::connect(sysThread,SIGNAL(thread_end(int, void *)), sysThread, SLOT(terminate()));
    QObject::connect(sysThread, SIGNAL(thread_end(int, void *)), sysThread, SLOT(quit()));

    QObject::connect(sysThread, SIGNAL(finished()), obj, SLOT(deleteLater()));
    QObject::connect(sysThread, SIGNAL(finished()), sysThread, SLOT(deleteLater()));

    obj->moveToThread(sysThread);

}

char* Core::get_buff_char()
{
    return buff_char;
}

vector<string> Core::get_buff_vector()
{
    return buff_vec;
}

bool Core::get_buff_bool()
{
    return buff_bool;
}


void Core::receiver(int cmd, void *data)
{
    core_handler(cmd, data);
}

void Core::receiver(int cmd, vector<string> data)
{
    void *pointer;
    pointer = &data;
    core_handler(cmd, pointer);
}

void Core::receiver(int cmd, int data)
{
    void *pointer;
    pointer = &data;
    core_handler(cmd, pointer);
}

void Core::receiver(int cmd, char *data)
{
    core_handler(cmd, data);
}

void Core::bt_adapter_checked()
{
    buff_vec = Bluetooth::bt_scan_adapter();

    if(buff_vec.size() != 0)
    {
        bt = new Bluetooth* [buff_vec.size()];
        sysThread = new SysThread* [buff_vec.size()];

        for(unsigned i = 0; i < buff_vec.size(); i++)
        {
            bt[i] = new Bluetooth();
            sysThread[i] = new SysThread();
        }
    } else
    {
        char err[] = "Not search BT adapter";
        core_handler(HANDLER_CORE_SYS_ERROR, &err);
    }
}
