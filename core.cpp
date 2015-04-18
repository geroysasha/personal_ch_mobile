#include "core.h"

CoreAdapter *ca;

Bluetooth **bt;
SysThread **thread;

vector<string> buff_list;
char buff_char;
bool buff_bool;
int buff_int;

Core::Core()
{

    list<string> lst = Bluetooth::bt_scan_adapter();


    bt = new Bluetooth* [lst.size()];
    thread = new SysThread* [lst.size()];

    if(lst.size() > 0)
    {
        for(int i = 0; i < lst.size(); i++)
        {
            bt[i] = new Bluetooth();
            thread[i] = new SysThread();
        }
    }
    else
    {
        Core::core_handler(HANDLER_CORE_BLUETOOTH_LOCAL_ADAPTER_ERROR, NULL);
    }
}

void Core::core_adapter_pointer(CoreAdapter *pointer)
{
    ca = pointer;
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

        Core::core_thread_connect(thread[*static_cast<int*>(data)]
                                  , bt[*static_cast<int*>(data)]
                                  , SLOT(bt_search_remote_dev(void *))
                                  , data);

        thread[*static_cast<int*>(data)]->start();

        ca->emit_core_signal(EMIT_DIALOG_PROGRESS_SHOW);

        break;
    case HANDLER_CORE_REQUEST_SEARCH_REMOTE_DEVICE:

        ca->emit_core_signal(EMIT_DIALOG_PROGRESS_STOP);

        buff_list = *static_cast<vector<string> *>(data);

        ca->emit_core_signal(EMIT_DIALOG_SELECT_LIST_SHOW);

        break;
    case HANDLER_CORE_LOAD_ADAPTER_POINTER:
        break;
    case HANDLER_CORE_DIALOG_SELECT_ITEM:
        buff_list = *static_cast<vector<string> *>(data);
        buff_int = atoi(static_cast<string>(buff_list[0]).c_str());

        Core::core_thread_connect(thread[buff_int]
                                  , bt[buff_int]
                                  , SLOT(bt_check_connect_remote_dev(void *))
                                  , data);
        thread[buff_int]->start();

        ca->emit_core_signal(EMIT_DIALOG_PROGRESS_SHOW);

        break;
    case HANDLER_CORE_CHECK_CONNECT_REMOTE_DEVICE_OK:

        buff_list = *static_cast<vector<string> *>(data);

        ca->emit_core_signal(EMIT_DIALOG_PROGRESS_STOP);
        ca->emit_core_signal(EMIT_CHECK_CONNECT_REMOTE_DEVICE_OK);

        qDebug()<<"HANDLER_CORE_CHECK_CONNECT_REMOTE_DEVICE_OK";

        break;
    case HANDLER_CORE_CHECK_CONNECT_REMOTE_DEVICE_ERROR:

        buff_list = *static_cast<vector<string> *>(data);

        ca->emit_core_signal(EMIT_DIALOG_PROGRESS_STOP);
        ca->emit_core_signal(EMIT_CHECK_CONNECT_REMOTE_DEVICE_ERROR);

        qDebug()<<"HANDLER_CORE_CHECK_CONNECT_REMOTE_DEVICE_ERROR";

        break;
    }

}

void Core::core_thread_connect(SysThread *thread, QObject *obj, char *method, void *data)
{
    thread->set_buff(data);

    QObject::connect(thread,SIGNAL(started()), thread, SLOT(run()));

    QObject::connect(thread,SIGNAL(work(void *)), obj, method);

    QObject::connect(obj, SIGNAL(request(int,void *)), thread, SLOT(request(int, void *)));

    QObject::connect(thread,SIGNAL(thread_end(int, void *)), ca, SLOT(receiver(int, void *)));

    QObject::connect(thread,SIGNAL(thread_end(int, void *)), thread, SLOT(terminate()));
    QObject::connect(thread, SIGNAL(thread_end(int, void *)), thread, SLOT(quit()));

    QObject::connect(thread, SIGNAL(finished()), obj, SLOT(deleteLater()));
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    obj->moveToThread(thread);

}

char Core::get_buff_char()
{
    return buff_char;
}

vector<string> Core::get_buff_vector()
{
    return buff_list;
}

bool Core::get_buff_bool()
{
    return buff_bool;
}
