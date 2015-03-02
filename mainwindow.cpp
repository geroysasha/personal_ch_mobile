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

void MainWindow::on_button_scan_clicked()
{
    char *hci = "hci0";
    emit send(HANDLER_CORE_GUI_SEARCH_REMOTE_DEVICE, hci);

}
