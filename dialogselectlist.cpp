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
void DialogSelectList::add_data(void *m)
{
    map<char *, char *> *temp_pointer = static_cast<map<char *, char *> *>(m);
    map<char *, char *> dev_map = *temp_pointer;

    QStringList data;
    QStringListModel *stringListModel = new QStringListModel(this);

    map<char *, char *>::iterator it_map = dev_map.begin();
    for (it_map=dev_map.begin(); it_map!=dev_map.end(); ++it_map)
    {
        data << it_map->second;
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

void DialogSelectList::run(void *data)
{
    this->add_data(data);
    this->show();
    this->center_display();
    this->exec();
}

void DialogSelectList::on_pushButton_select_clicked()
{
    /*QStringListModel *model = ui->listView->model();
    int row = model->rowCount();   // model = new QStringListModel
    model->insertRow(row);

    QModelIndex index = model->index(row);

    qDebug() << index;*/
    //emit send(HANDLER_CORE_DIALOG_SELECT_LIST, hci);
}

void DialogSelectList::on_pushButton_close_clicked()
{
    this->close();
}

