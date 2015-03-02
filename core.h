#ifndef CORE_H
#define CORE_H

#include "bluetooth.h"
#include "systhread.h"
#include "dialogprogress.h"
#include "dialogselectlist.h"
#include "coreadapter.h"
#include "mainwindow.h"
#include "const.h"

class Core
{

public:
    static Core& Instance()
    {
        static Core s;
        return s;
    }
    static void core_thread(QObject *, char *, void *);
    void core_handler(int, void *);
private:
    Core();
    virtual ~Core(){}

    Core(Core const&);
    Core& operator= (Core const&);

};

#endif // CORE_H

