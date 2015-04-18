#include "dialogalert.h"
#include "ui_dialogalert.h"

DialogAlert::DialogAlert(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAlert)
{
    ui->setupUi(this);
    DialogAlert::setWindowFlags(Qt::WindowStaysOnTopHint);
}

DialogAlert::~DialogAlert()
{
    delete ui;
}

void DialogAlert::run(char *mess)
{
    QDesktopWidget desktop;
    QRect rect = desktop.availableGeometry(desktop.primaryScreen());

    this->move(rect.width()/2 - this->width()/2,rect.height()/2 - this->width()/2);

    ui->label_text->setText(QString::fromStdString(mess));

    this->show();
}

void DialogAlert::on_pushButton_ok_clicked()
{
    this->close();
}
