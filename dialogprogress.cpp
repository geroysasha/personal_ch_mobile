#include "dialogprogress.h"
#include "ui_dialogprogress.h"

#include <QLabel>
#include <QMovie>

DialogProgress::DialogProgress(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogProgress)
{
    ui->setupUi(this);
}

DialogProgress::~DialogProgress()
{
    delete ui;
}

void DialogProgress::center_display()
{
    QDesktopWidget desktop;
    QRect rect = desktop.availableGeometry(desktop.primaryScreen());

    this->move(rect.width()/2 - this->width()/2,rect.height()/2 - this->width()/2);
}

void DialogProgress::run()
{
    this->show();
    this->center_display();

    count = 0;
    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()), this, SLOT(updateProgressBar()) );
    timer->start(100);
}

void DialogProgress::stop()
{
    timer->stop();
    delete timer;
    timer = 0;
    this->close();
}

void DialogProgress::updateProgressBar()
{
    QString frame = ":/new/res/res/";
    frame.append(count + '0');
    frame.append(".jpg");
    QPixmap mypix(frame);
    ui->label->setPixmap(mypix);
    count++;
    if(count == 8)
    {
        count = 0;
    }
}
