#include "systhread.h"

void *buff;

SysThread::SysThread()
{
}

void SysThread::run() {
    emit work(buff);
    buff = NULL;
}

void SysThread::request(int request, void *data)
{

    qDebug()<< "thread_end()";
    emit thread_end(request, data);
}

void SysThread::set_buff(void *data)
{
    buff = data;
}

