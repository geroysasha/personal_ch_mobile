#ifndef SYSTHREAD_H
#define SYSTHREAD_H

#include <QDebug>
#include <QThread>
#include "coreadapter.h"

class SysThread : public QThread
{
Q_OBJECT
public:
    SysThread();
    void set_buff(void *);
public slots:
    void run();
    void deleteLater()
    {
        qDebug()<< "delete thread";
        this->disconnect(SIGNAL(work(void*)));
        this->disconnect(SIGNAL(thread_end(int, void *)));
        this->disconnect(SIGNAL(started()));
        this->disconnect(SIGNAL(finished()));
    }
    void request(int, void *);
signals:
    void work(void *);
    void thread_end(int, void *);
};


#endif // SYSTHREAD_H
