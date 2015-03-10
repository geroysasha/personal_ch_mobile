#include "dialogselectlist.h"
#include "ui_dialogselectlist.h"

vector<string> dev(2);

DialogSelectList::DialogSelectList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSelectList)
{
    ui->setupUi(this);
}

DialogSelectList::~DialogSelectList()
{
    delete ui;
}
void DialogSelectList::add_data(vector<string> dev_map)
{

    QStringList data;
    QStringListModel *stringListModel = new QStringListModel(this);

    dev[0] = dev_map[0];

    string str = "Adapter:hci";
    str.append(dev_map[0]);
    this->setWindowTitle(str.c_str());

    for (int i = 1; i < dev_map.size(); i++)
    {
        str = dev_map[i];
        data << str.c_str();
    }

    stringListModel->setStringList(data);
    ui->listView->setModel(stringListModel);

}

void DialogSelectList::center_display()
{
    QDesktopWidget desktop;
    QRect rect = desktop.availableGeometry(desktop.primaryScreen());

    this->move(rect.width()/2 - this->width()/2,rect.height()/2 - this->width()/2);
}

void DialogSelectList::run(vector<string> data)
{
    this->add_data(data);
    this->show();
    this->center_display();
    //this->exec();
}

void DialogSelectList::on_pushButton_select_clicked()
{
    this->close();
    dev[1] = (ui->listView->currentIndex().data().toString()).toStdString();
    emit send(HANDLER_CORE_DIALOG_SELECT_ITEM, dev);
}

void DialogSelectList::on_pushButton_close_clicked()
{
    this->close();
}


//void DialogSelectList::on_listView_clicked(const QModelIndex &index)
//{
//    emit send(HANDLER_CORE_DIALOG_CLICKED_ITEM, index.data().toString());
//ui->listView->currentIndex().row()
//}
