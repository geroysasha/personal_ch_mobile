#ifndef DIALOGSELECTLIST_H
#define DIALOGSELECTLIST_H
#include <QDesktopWidget>
#include <QRect>
#include <QDialog>
#include <QDebug>
#include <QStringList>
#include <QStringListModel>
#include <map>
#include <iostream>
#include <cstring>

using namespace std;

namespace Ui {
class DialogSelectList;
}

class DialogSelectList : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogSelectList(QWidget *parent = 0);
    ~DialogSelectList();   
private:
    Ui::DialogSelectList *ui;
    void center_display();
    void add_data(void *);
signals:
    void send(int, void*);
public slots:
    void run(void *);
private slots:
    void on_pushButton_select_clicked();
    void on_pushButton_close_clicked();
};

#endif // DIALOGSELECTLIST_H
