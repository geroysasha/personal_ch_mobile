#ifndef CALLTIMER_H
#define CALLTIMER_H

#include <QTimer>
#include <QTime>
#include <QDateTime>
#include <QObject>
#include <QDebug>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "core.h"

using namespace std;

class CallTimer : public QObject
{
    Q_OBJECT
public:
    CallTimer();
    virtual ~CallTimer()
    {
        delete callTimer;
        qDebug()<<"destroys CallTimer";
    }
    void startTimer();
    void stopTimer();
    void resetTimer();
    int getSec();
private:
    QTimer *callTimer;
    QDateTime startTime;
    int secTalk;
public slots:
    void updateTime();
    void calltimer_destroy();
signals:
    void calltimer_set_time(char *);
};

#endif // CALLTIMER_H
