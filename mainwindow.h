#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <vector>
#include <QMainWindow>
#include <QDebug>
#include "const.h"

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void selected_remote_dev(vector<string>, bool);
    //void check_connect_remote_dev(char *, bool);
private slots:
    void on_button_scan_clicked();
private:
    Ui::MainWindow *ui;
signals:
    void send(int, int);
};

#endif // MAINWINDOW_H
