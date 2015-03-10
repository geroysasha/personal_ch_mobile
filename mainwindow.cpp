#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::selected_remote_dev(vector<string> data, bool stat)
{
    vector<string> remote_dev_info = data;
    QString remote_dev_name = static_cast<string>(remote_dev_info[remote_dev_info.size() - 1]).c_str();
    ui->lineEdit_device->setText(remote_dev_name);
    if(stat)
    {
        QPalette palette;
        palette.setColor(QPalette::WindowText, Qt::green);

        ui->label_device->setText("Device connected");
        ui->label_device->setPalette(palette);
    }
    else
    {
        QPalette palette;
        palette.setColor(QPalette::WindowText, Qt::red);

        ui->label_device->setText("Device is not available");
        ui->label_device->setPalette(palette);
    }
}

void MainWindow::on_button_scan_clicked()
{
    emit send(HANDLER_CORE_GUI_SEARCH_REMOTE_DEVICE, 0);//0(default) - it is number bluetooth adapter
}

