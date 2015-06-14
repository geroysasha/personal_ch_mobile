#ifndef DIALOGSELECTLIST_H
#define DIALOGSELECTLIST_H
#include <QDesktopWidget>
#include <QRect>
#include <QDialog>
#include <QDebug>
#include <QStringList>
#include <QStringListModel>
#include <vector>
#include <iostream>
#include <cstring>
#include "const.h"

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
    void run(vector<string>*);
private:
    Ui::DialogSelectList *ui;
    void center_display();
    void add_data(vector<string>*);
    vector<string> *dev;

private slots:
    void on_pushButton_select_clicked();
    void on_pushButton_close_clicked();
    //void on_listView_clicked(const QModelIndex &index);
signals:
    //void send(int, vector<string>&);
    void dialogselectlist_select(vector<string> *);
};

#endif // DIALOGSELECTLIST_H
