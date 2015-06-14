#ifndef CORE_H
#define CORE_H

#include <QProcess>
#include "bluetooth.h"
#include "dialogselectlist.h"
#include "dialogalert.h"

class SysThread;

class Core : public QObject
{
    Q_OBJECT
private:
    static Core *s;
    Core();
    virtual ~Core()
    {
        for(unsigned i = 0; i < buff_vec.size(); i++)
        {
            delete bt[i];
        }

        delete bt;
        delete [] dlgSelectList;
        delete [] dlgAlert;
        qDebug()<<"delete Core";
    }

    Core(Core const&);
    Core& operator= (Core const&);

    Bluetooth **bt;
    vector<string> buff_vec;
    DialogSelectList *dlgSelectList;
    DialogAlert *dlgAlert;
    void bt_adapter_checked();
public:

    static Core* Instance()
    {
        if(!s)
        {
             s = new Core();
        }
        return s;
    }

public slots:
    void core_init_dialogs();
    void mainwindow_remote_device( int adapter_number );
    void mainwindow_start_listener(string *registration_string);
    void mainwindow_stop_listener();
    void mainwindow_close_applicatrion();
    void bluetooth_remote_device(vector<string> *vec_devices);
    void bluetooth_checked_remote_device(vector<string> *vec_devices, bool stat);
    void dialogselectlist_select(vector<string> *select_device);
    void core_logger(char *mess);


signals:
    void mainwindow_selected_remote_dev(vector<string> *data, bool stat);
    void mainwindow_logger(char *);
    void mainwindow_btn_state(bool btn_start, bool btn_stop);
    void mobilechannel_pjsua_account_create(Bluetooth *ptr, const char *field_email, const char *field_code);
    void mobilechannel_pjsua_account_destroy();
    void close_application();

};

#endif // CORE_H

