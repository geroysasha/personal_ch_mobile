#ifndef CORE_H
#define CORE_H

#include "coreadapter.h"
#include "bluetooth.h"
#include "systhread.h"
#include "const.h"

class CoreAdapter;
class SysThread;

class Core
{

public:
    static Core& Instance()
    {
        static Core s;
        return s;
    }
    static void core_thread_connect(SysThread *, QObject *, char *, void *);
    void core_adapter_pointer(CoreAdapter *);
    void core_handler(int, void *);
    vector<string> get_buff_vector();
    char get_buff_char();
    bool get_buff_bool();
private:
    Core();
    virtual ~Core(){}

    Core(Core const&);
    Core& operator= (Core const&);

};

#endif // CORE_H

