#include "systhread.h"

void *var;

SysThread::SysThread(void *data)
{
    var = data;
}

void SysThread::run() {
    emit work(var);
}

void SysThread::request(int request, void *data)
{
    Core::Instance().core_handler(request, data);
/*
    map<string,string> *dev = static_cast<map<string,string> *>(data);
    map<string,string> tmp = *dev;
    map<string, string>::iterator it_map = tmp.begin();
    for (it_map=tmp.begin(); it_map!=tmp.end(); ++it_map)
       cout << it_map->first << " => " << it_map->second << '\n';
       */
    qDebug()<< "thread_end()";
    emit thread_end();
}
