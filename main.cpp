#include <QtGui/QApplication>
#include <pjsua-lib/pjsua.h>
#include <QDesktopWidget>
#include <QRect>

#include "mainwindow.h"
#include "coreadapter.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    CoreAdapter ca;
    void *data;
    data = &w;
    ca.core_receiver(HANDLER_CORE_LOAD_POINTER, data);

    QDesktopWidget desktop;
    QRect rect = desktop.availableGeometry(desktop.primaryScreen());

    w.move(rect.width()/2 - w.width()/2,rect.height()/2 - w.width()/2);
    w.show();
    return a.exec();
}
