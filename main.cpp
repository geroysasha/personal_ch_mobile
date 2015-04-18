#include <QtGui/QApplication>
#include <pjsua-lib/pjsua.h>
#include <QDesktopWidget>
#include <QRect>

#include "mainwindow.h"
#include "coreadapter.h"
#include "dialogprogress.h"
#include "dialogselectlist.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    DialogProgress dlgProgress;
    DialogSelectList dlgSelectList;
    CoreAdapter ca;

    QObject::connect(&ca
                     , SIGNAL(dialog_progress_show())
                     , &dlgProgress
                     , SLOT(run()));
    QObject::connect(&ca
                     , SIGNAL(dialog_progress_stop())
                     , &dlgProgress
                     , SLOT(stop()));
    QObject::connect(&ca
                     , SIGNAL(dialog_select_list_show(vector<string>))
                     , &dlgSelectList
                     , SLOT(run(vector<string>)));
    QObject::connect(&ca
                     , SIGNAL(mainwindow_selected_remote_dev(vector<string>, bool))
                     , &w
                     , SLOT(selected_remote_dev(vector<string>, bool)));
    QObject::connect(&dlgSelectList
                     , SIGNAL(send(int, vector<string>))
                     , &ca
                     , SLOT(receiver(int, vector<string>)));
    QObject::connect(&w
                     , SIGNAL(send(int, int))
                     , &ca
                     , SLOT(receiver(int, int)));


    QDesktopWidget desktop;
    QRect rect = desktop.availableGeometry(desktop.primaryScreen());

    w.move(rect.width()/2 - w.width()/2,rect.height()/2 - w.width()/2);
    w.show();
    return a.exec();
}
