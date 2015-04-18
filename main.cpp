#include <QtGui/QApplication>
#include <pjsua-lib/pjsua.h>
#include <QDesktopWidget>
#include <QRect>

#include "mainwindow.h"
#include "core.h"
#include "mobilechannel.h"
#include "dialogprogress.h"
#include "dialogselectlist.h"
#include "dialogalert.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    DialogProgress dlgProgress;
    DialogSelectList dlgSelectList;
    DialogAlert dlgAlert;

    QObject::connect(&Core::Instance()
                     , SIGNAL(dialog_progress_show())
                     , &dlgProgress
                     , SLOT(run()));
    QObject::connect(&Core::Instance()
                     , SIGNAL(dialog_progress_stop())
                     , &dlgProgress
                     , SLOT(stop()));
    QObject::connect(&Core::Instance()
                     , SIGNAL(dialog_select_list_show(vector<string>))
                     , &dlgSelectList
                     , SLOT(run(vector<string>)));
    QObject::connect(&Core::Instance()
                     , SIGNAL(dialog_alert_show(char *))
                     , &dlgAlert
                     , SLOT(run(char *)));
    QObject::connect(&Core::Instance()
                     , SIGNAL(mainwindow_selected_remote_dev(vector<string>, bool))
                     , &w
                     , SLOT(selected_remote_dev(vector<string>, bool)));
    QObject::connect(&Core::Instance()
                     , SIGNAL(mainwindow_btn_state(bool, bool))
                     , &w
                     , SLOT(btn_state(bool, bool)));


    QObject::connect(&dlgSelectList
                     , SIGNAL(send(int, vector<string>))
                     , &Core::Instance()
                     , SLOT(receiver(int, vector<string>)));
    QObject::connect(&w
                     , SIGNAL(send(int, int))
                     , &Core::Instance()
                     , SLOT(receiver(int, int)));
    QObject::connect(&Core::Instance()
                     , SIGNAL(pjsua_account_create(void *))
                     , &MobileChannel::Instance()
                     , SLOT(pjsua_account_create(void *)));
    QObject::connect(&Core::Instance()
                     , SIGNAL(pjsua_account_destroy())
                     , &MobileChannel::Instance()
                     , SLOT(pjsua_account_destroy()));
    QObject::connect(&MobileChannel::Instance()
                     , SIGNAL(send(int, char *))
                     , &Core::Instance()
                     , SLOT(receiver(int, char *)));

    //инициализируем PJSUA
    MobileChannel::Instance().init();

    QDesktopWidget desktop;
    QRect rect = desktop.availableGeometry(desktop.primaryScreen());

    w.move(rect.width()/2 - w.width()/2,rect.height()/2 - w.width()/2);
    w.show();
    return a.exec();
}
