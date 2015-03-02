#ifndef SIGNALS_H
#define SIGNALS_H

#include <QObject>
#include "core.h"
#include "mainwindow.h"

class CoreAdapter : public QObject
{
    Q_OBJECT
public:
    CoreAdapter();
    virtual ~CoreAdapter(){}
    void core_receiver(int, void *);
    void core_send(int, void *);
public slots:
    void receiver(int, void *);
signals:
    void dialog_progress_show();
    void dialog_select_list_show(void *);
    void dialog_progress_stop();

};

#endif // SIGNALS_H
