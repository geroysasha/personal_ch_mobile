#ifndef SYSTHREAD_H
#define SYSTHREAD_H

#include <QDebug>
#include <QThread>
#include "core.h"

class SysThread : public QThread
{
Q_OBJECT
public:
    SysThread(void *);
public slots:
    void run();
    void deleteLater()
    {
        qDebug()<< "delete thread";
        QObject::deleteLater();
    }
    void request(int, void *);

signals:
    void work(void *);
    void thread_end();
};


#endif // SYSTHREAD_H
