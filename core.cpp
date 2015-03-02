#include "core.h"

DialogProgress *dlgProgress;
DialogSelectList *dlgSelectList;
CoreAdapter *ca;
MainWindow *mWin;

Core::Core()
{
    dlgProgress = new DialogProgress();
    dlgSelectList = new DialogSelectList();
    ca = new CoreAdapter();

    list<string> lst = Bluetooth::bt_scan_adapter();
    void *data;
    data = &lst;
    if(lst.size() > 0)
    {
        Core::core_handler(HANDLER_CORE_BLUETOOTH_LOCAL_ADAPTER, data);
    }
    else
    {
        Core::core_handler(HANDLER_CORE_BLUETOOTH_LOCAL_ADAPTER_ERROR, data);
    }

    QObject::connect(dlgSelectList,SIGNAL(send(int, void *)), ca, SLOT(receiver(int, void *)));
    QObject::connect(dlgProgress,SIGNAL(send(int, void *)), ca, SLOT(receiver(int, void *)));
    QObject::connect(ca, SIGNAL(dialog_progress_show()), dlgProgress, SLOT(run()));
    QObject::connect(ca, SIGNAL(dialog_progress_stop()), dlgProgress, SLOT(close()));
    QObject::connect(ca, SIGNAL(dialog_select_list_show(void *)), dlgSelectList, SLOT(run(void *)));
}

void Core::core_handler(int ch, void *data)
{
    switch(ch)
    {
    case HANDLER_CORE_BLUETOOTH_LOCAL_ADAPTER:
    {
        list<string> *lst = static_cast<list<string> *>(data);
        break;
    }
    case HANDLER_CORE_BLUETOOTH_LOCAL_ADAPTER_ERROR:
        break;
    case HANDLER_CORE_BLUETOOTH_REMOTE_DEVICE:
        break;
    case HANDLER_CORE_BLUETOOTH_REMOTE_DEVICE_ERROR:
        break;
    case HANDLER_CORE_GUI_SEARCH_REMOTE_DEVICE:
        Core::core_thread(new Bluetooth(), SLOT(bt_search_remote_dev(void *)), data);
        ca->core_send(EMIT_DIALOG_PROGRESS_SHOW, NULL);
        break;
    case HANDLER_CORE_REQUEST_SEARCH_REMOTE_DEVICE:
        ca->core_send(EMIT_DIALOG_PROGRESS_STOP, NULL);
        ca->core_send(EMIT_DIALOG_SELECT_LIST_SHOW, data);
        break;
    case HANDLER_CORE_LOAD_POINTER:
        MainWindow *mWin = static_cast<MainWindow *>(data);
        QObject::connect(mWin,SIGNAL(send(int, void *)), ca, SLOT(receiver(int, void *)));
        break;
    }

}

void Core::core_thread(QObject *obj, char *method, void *data)
{
    SysThread *thread = new SysThread(data);

    QObject::connect(thread,SIGNAL(started()), thread, SLOT(run()));

    QObject::connect(thread,SIGNAL(work(void *)), obj, method);

    QObject::connect(obj, SIGNAL(request(int,void *)), thread, SLOT(request(int, void *)));

    QObject::connect(thread,SIGNAL(thread_end()),thread,SLOT(terminate()));
    QObject::connect(thread, SIGNAL(thread_end()), thread, SLOT(quit()));

    QObject::connect(thread, SIGNAL(finished()), obj, SLOT(deleteLater()));
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    obj->moveToThread(thread);

    thread->start();
}
