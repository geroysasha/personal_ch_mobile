#ifndef SIGNALS_H
#define SIGNALS_H

#define EMIT_DIALOG_PROGRESS_SHOW 1
#define EMIT_DIALOG_SELECT_LIST_SHOW 2
#define EMIT_DIALOG_PROGRESS_STOP 3
#define EMIT_CHECK_CONNECT_REMOTE_DEVICE_OK 4
#define EMIT_CHECK_CONNECT_REMOTE_DEVICE_ERROR 5

#include <QObject>
#include <map>
#include "core.h"
#include "mainwindow.h"

using namespace std;

class CoreAdapter : public QObject
{
    Q_OBJECT
public:
    CoreAdapter();
    virtual ~CoreAdapter(){}
    void emit_core_signal(int);
public slots:
    void receiver(int, void *);
    void receiver(int, vector<string>);
    void receiver(int, int);
    void receiver(int, char *);
signals:
    void dialog_progress_show();
    void dialog_progress_stop();
    void dialog_select_list_show(vector<string>);
    void mainwindow_selected_remote_dev(vector<string>, bool);
};

#endif // SIGNALS_H
