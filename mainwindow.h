#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <vector>
#include <fstream>
#include <QMainWindow>
#include <QDebug>
#include <stdio.h>
#include "const.h"
#include "core.h"

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
    void selected_remote_dev(vector<string> *, bool);
    void btn_state(bool btn_start, bool btn_stop);
protected:
      void showEvent(QShowEvent *ev);
private slots:
    void on_button_scan_clicked();
    void on_pushButton_start_clicked();
    void on_pushButton_stop_clicked();
    void on_checkBox_clicked();
    void on_pushButton_clearLog_clicked();
    void quit();
    void logger(char *);

private:
    Ui::MainWindow *ui;
    bool read_file();
    bool write_file();
    string dat;

signals:
    void mainwindow_remote_device(int);
    void mainwindow_start_listener(string *);
    void mainwindow_stop_listener();
    void mainwindow_close_applicatrion();
    void core_init_dialogs();
    void send(int, string *);
    void show_win();
};

#endif // MAINWINDOW_H
