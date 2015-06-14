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

    ui->label_text->setText(QString::fromStdString(  ((std::string) mess).substr( ((std::string) mess).rfind("-> ") + 3,
                                                                             ((std::string) mess).length() - ((std::string) mess).rfind("-> ") + 3 ).c_str()  ));

    this->show();

    delete [] mess;
}

void DialogAlert::on_pushButton_ok_clicked()
{
    this->close();
}
