#include "dialogselectlist.h"
#include "ui_dialogselectlist.h"

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
void DialogSelectList::add_data(vector<string> *dev_map)
{

    QStringList data;
    QStringListModel *stringListModel = new QStringListModel(this);

    dev = new vector<string>[2];

    dev->push_back( (*dev_map)[0] );

    string str = "Adapter:hci";
    str.append((*dev_map)[0]);
    this->setWindowTitle(str.c_str());

    for (unsigned i = 1; i < dev_map->size(); i++)
    {
        str = (*dev_map)[i];
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

void DialogSelectList::run(vector<string> *data)
{
    this->add_data(data);
    this->show();
    this->center_display();
}

void DialogSelectList::on_pushButton_select_clicked()
{
    this->close();
    dev->push_back( (ui->listView->currentIndex().data().toString()).toStdString() );
    emit dialogselectlist_select(dev);
}

void DialogSelectList::on_pushButton_close_clicked()
{
    this->close();
}

