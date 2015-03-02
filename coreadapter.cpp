#include "coreadapter.h"

CoreAdapter::CoreAdapter()
{
}

void CoreAdapter::core_receiver(int cmd, void *data)
{
    Core::Instance().core_handler(cmd, data);
}

void CoreAdapter::receiver(int cmd, void *data)
{
    Core::Instance().core_handler(cmd, data);
}

void CoreAdapter::core_send(int id, void *data)
{
    switch(id)
    {
    case EMIT_DIALOG_PROGRESS_SHOW:
    {
        emit dialog_progress_show();
        break;
    }
    case EMIT_DIALOG_SELECT_LIST_SHOW:
    {
        emit dialog_select_list_show(data);
        break;
    }
    case EMIT_DIALOG_PROGRESS_STOP:
        emit dialog_progress_stop();
        break;
    }

}

