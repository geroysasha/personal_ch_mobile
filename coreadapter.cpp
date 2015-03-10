#include "coreadapter.h"

CoreAdapter::CoreAdapter()
{
    Core::Instance().core_adapter_pointer(this);
}

void CoreAdapter::receiver(int cmd, void *data)
{
    Core::Instance().core_handler(cmd, data);
}

void CoreAdapter::receiver(int cmd, vector<string> data)
{
    void *pointer;
    pointer = &data;
    Core::Instance().core_handler(cmd, pointer);
}

void CoreAdapter::receiver(int cmd, int data)
{
    void *pointer;
    pointer = &data;
    Core::Instance().core_handler(cmd, pointer);
}

void CoreAdapter::receiver(int cmd, char *data)
{
    Core::Instance().core_handler(cmd, data);
}

void CoreAdapter::emit_core_signal(int id)
{
    switch(id)
    {
    case EMIT_DIALOG_PROGRESS_SHOW:

        emit dialog_progress_show();

        break;

    case EMIT_DIALOG_SELECT_LIST_SHOW:

        emit dialog_select_list_show(Core::Instance().get_buff_vector());

        break;

    case EMIT_DIALOG_PROGRESS_STOP:

        emit dialog_progress_stop();

        break;

    case EMIT_CHECK_CONNECT_REMOTE_DEVICE_OK:

        qDebug()<<"EMIT_CHECK_CONNECT_REMOTE_DEVICE_OK";

        emit mainwindow_selected_remote_dev(Core::Instance().get_buff_vector()
                                            , true);

        break;
    case EMIT_CHECK_CONNECT_REMOTE_DEVICE_ERROR:

        qDebug()<<"EMIT_CHECK_CONNECT_REMOTE_DEVICE_ERROR";

        emit mainwindow_selected_remote_dev(Core::Instance().get_buff_vector()
                                            , false);

        break;
    }
}
