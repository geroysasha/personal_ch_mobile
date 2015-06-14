#include "calltimer.h"

CallTimer::CallTimer()
{
    callTimer = new QTimer( this );
    callTimer->setInterval( 1000 );
    QObject::connect( callTimer,
                      SIGNAL(timeout()),
                      this,
                      SLOT(updateTime())
                      );
    QObject::connect( this,
                      SIGNAL(calltimer_set_time(char *)),
                      Core::Instance(),
                      SLOT(core_logger(char *))
                      );
}

void CallTimer::startTimer()
{
    callTimer->start();
    startTime = QDateTime::currentDateTime();
    updateTime();
}

void CallTimer::stopTimer()
{
    callTimer->stop();
    secTalk =  startTime.time().msecsTo( QDateTime::currentDateTime().time() ) / 1000 ;
}

void CallTimer::resetTimer()
{
    startTime = QDateTime::currentDateTime();
    updateTime();
}

void CallTimer::updateTime()
{

    QString str = "call time -> ";
    str.append(QDateTime::fromTime_t( startTime.time().msecsTo( QDateTime::currentDateTime().time() ) / 1000 ).toString("mm:ss"));

    char *time = new char[strlen( (char *)str.toStdString().c_str() ) + 1];
    strcpy( time, (char *)str.toStdString().c_str() );
    emit calltimer_set_time( time );

}

int CallTimer::getSec()
{
    return 0;
}

void CallTimer::calltimer_destroy()
{
    delete this;
}
