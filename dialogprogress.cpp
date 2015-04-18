#include "dialogprogress.h"
#include "ui_dialogprogress.h"

#include <QLabel>
#include <QMovie>

DialogProgress::DialogProgress(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogProgress)
{
    ui->setupUi(this);
    QMovie *movie = new QMovie(":/new/zigi/res/001.gif");

    ui->label->setMovie(movie);
    movie->start();
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
}

void DialogProgress::stop()
{
    this->close();
}
