#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "const.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_button_scan_clicked();
signals:
    void send(int, void *);
private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
