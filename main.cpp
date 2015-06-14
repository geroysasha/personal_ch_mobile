#include <QtGui/QApplication>
#include <QDesktopWidget>
#include <QRect>
#include <QFile>

#include "mainwindow.h"
#include "mobilechannel.h"

static void update();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    update();

    MainWindow w;

    QDesktopWidget desktop;
    QRect rect = desktop.availableGeometry(desktop.primaryScreen());

    w.move(rect.width()/2 - w.width()/2,rect.height()/2 - w.width()/2);
    w.show();

    MobileChannel::Instance().init();

    QFile::copy (":///new/zigi/res/wait_sound.wav", "wait_sound.wav");

    return a.exec();
}

void update()
{
    char buf[BUFSIZ];
    FILE *ptr;
    string str_one;
    char cmd_info[] = "curl -I http://zigi.net.ua/voip/zigi 2>&1";
    char cmd_upload[] = "curl -z zigi  http://zigi.net.ua/voip/zigi -o zigi_new 2>&1 ";
    char cmd_update[] = "rm zigi && mv zigi_new zigi && chmod +x zigi";

    if ((ptr = popen(cmd_info, "r")) != NULL)
    {
        while (fgets(buf, BUFSIZ, ptr) != NULL)
        {
            str_one.append(buf);
        }
        if(str_one.find("Not Found") == string::npos)
        {
            str_one.clear();
            if ((ptr = popen(cmd_upload, "r")) != NULL)
            {
                while (fgets(buf, BUFSIZ, ptr) != NULL)
                {
                    str_one.append(buf);
                }
                qDebug()<<str_one.c_str();
//                if(str_one.find("Total") == string::npos)
//                {
//                    core_handler(HANDLER_CORE_LOGGING,(void *)"ATTENTION>> please install -curl-");

//                }
                if(str_one.find("100") != string::npos)
                {
                    ptr = popen(cmd_update, "r");
                    (void) pclose(ptr);
                    if ( QProcess::startDetached(QString("./zigi") ) )
                        exit(0);
                }

            }
            (void) pclose(ptr);
        }
    }

}
