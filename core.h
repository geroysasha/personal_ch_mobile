#ifndef CORE_H
#define CORE_H

#include "bluetooth.h"
#include "systhread.h"
#include "const.h"

class SysThread;

class Core : public QObject
{
    Q_OBJECT
public:
    static Core& Instance()
    {
        static Core s;
        return s;
    }
    void core_thread_connect(SysThread *, QObject *, char *, void *);
    vector<string> get_buff_vector();
    char* get_buff_char();
    bool get_buff_bool();

public slots:
    void core_handler(int, void *);
    void receiver(int, void *);
    void receiver(int, vector<string>);
    void receiver(int, int);
    void receiver(int, char *);

signals:
    void dialog_progress_show();
    void dialog_progress_stop();
    void dialog_select_list_show(vector<string>);
    void dialog_alert_show(char *mess);
    void mainwindow_selected_remote_dev(vector<string>, bool);
    void mainwindow_btn_state(bool btn_start, bool btn_stop);
    void pjsua_account_create(void *);
    void pjsua_account_destroy();

private:
    Core();
    virtual ~Core()
    {
        delete[] bt;
        delete[] sysThread;
    }

    Core(Core const&);
    Core& operator= (Core const&);

    void bt_adapter_checked();

    Bluetooth **bt;
    SysThread **sysThread;

    vector<string> buff_vec;
    char *buff_char;
    bool buff_bool;
    int buff_int;

};

#endif // CORE_H

