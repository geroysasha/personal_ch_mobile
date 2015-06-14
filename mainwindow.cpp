#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    read_file();
    QObject::connect(  this,
                       SIGNAL( core_init_dialogs() ),
                       Core::Instance(),
                       SLOT( core_init_dialogs() )
                       );
    QObject::connect(  this,
                       SIGNAL( mainwindow_remote_device(int) ),
                       Core::Instance(),
                       SLOT( mainwindow_remote_device(int) )
                       );
    QObject::connect(  this,
                       SIGNAL( mainwindow_start_listener(string *) ),
                       Core::Instance(),
                       SLOT( mainwindow_start_listener(string *) )
                       );
    QObject::connect(  this,
                       SIGNAL( mainwindow_stop_listener() ),
                       Core::Instance(),
                       SLOT( mainwindow_stop_listener() )
                       );
    QObject::connect(  this,
                       SIGNAL( mainwindow_close_applicatrion() ),
                       Core::Instance(),
                       SLOT( mainwindow_close_applicatrion() )
                       );
    QObject::connect(  Core::Instance(),
                       SIGNAL( mainwindow_selected_remote_dev(vector<string> *, bool) ),
                       this,
                       SLOT( selected_remote_dev(vector<string> *, bool) )
                       );
    QObject::connect(  Core::Instance(),
                       SIGNAL( mainwindow_logger(char *) ),
                       this,
                       SLOT( logger(char *) )
                       );
    QObject::connect(  Core::Instance(),
                       SIGNAL( mainwindow_btn_state(bool, bool) ),
                       this,
                       SLOT( btn_state(bool, bool) )
                       );
}


MainWindow::~MainWindow()
{
    emit mainwindow_close_applicatrion();
    delete ui;
}

void MainWindow::quit()
{
    exit(0);
}

void MainWindow::showEvent(QShowEvent *)
{
    emit core_init_dialogs();
}

void MainWindow::selected_remote_dev(vector<string> *data, bool stat)
{

    ui->lineEdit_device->setText( ((string) (*data)[data->size() - 1]).c_str() );
    if(stat)
    {
        QPalette palette;
        palette.setColor(QPalette::WindowText, Qt::green);

        ui->label_device->setText("Device connected");
        ui->label_device->setPalette(palette);
        ui->pushButton_start->setEnabled(true);
    }
    else
    {
        QPalette palette;
        palette.setColor(QPalette::WindowText, Qt::red);

        ui->label_device->setText("Device is not available");
        ui->label_device->setPalette(palette);
        ui->pushButton_start->setEnabled(false);
    }
}

void MainWindow::btn_state(bool btn_start, bool btn_stop)
{

    ui->pushButton_start->setEnabled(btn_start);
    ui->pushButton_stop->setEnabled(btn_stop);

    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::red);

    if(!btn_start)
    {
        ui->label_device->setText("Error process");
        ui->label_device->setPalette(palette);
    }

}

void MainWindow::on_button_scan_clicked()
{

    emit mainwindow_remote_device(0);

}


void MainWindow::on_pushButton_start_clicked()
{
    dat.clear();

    if(ui->lineEdit_email->text() != "mainwindow_")
    {
        if(ui->lineEdit_code->text() != "")
        {

            if(ui->checkBox->checkState() != 0)
            {
                write_file();
            }
            ui->pushButton_start->setEnabled(false);
            ui->pushButton_stop->setEnabled(true);
            ui->button_scan->setEnabled(false);

//            dat = new char[strlen("0/") +
//                    ui->lineEdit_email->text().length() +
//                    strlen("/") +
//                    ui->lineEdit_code->text().length()];
//            strcpy(dat, "0/");
//            strcat(dat, ui->lineEdit_email->text().toStdString().c_str());
//            strcat(dat, "/");
//            strcat(dat, ui->lineEdit_code->text().toStdString().c_str());
            dat.append("0/");
            dat.append(ui->lineEdit_email->text().toStdString().c_str());
            dat.append("/");
            dat.append(ui->lineEdit_code->text().toStdString().c_str());

            emit mainwindow_start_listener(&dat);
        } else
        {
            dat.append("Empty field 'Your e-mail'");
            emit send(HANDLER_CORE_ALERT, &dat);
        }
    } else
    {
        dat.append("Empty field 'Registration code'");
        emit send(HANDLER_CORE_ALERT, &dat);
    }

}

void MainWindow::on_pushButton_stop_clicked()
{
    ui->pushButton_start->setEnabled(true);
    ui->pushButton_stop->setEnabled(false);
    ui->button_scan->setEnabled(true);
    emit mainwindow_stop_listener();
}

void MainWindow::on_checkBox_clicked()
{
    if(ui->checkBox->checkState() != 0)
    {
        if(ui->lineEdit_email->text() != "" &&
                ui->lineEdit_code->text() != "" &&
                ui->lineEdit_device->text() != "")
        {

        }
    }
}

bool MainWindow::read_file()
{
    ifstream file ("settings.txt");
    string line;


    if(file.is_open())
    {
        getline (file,line);

        if(line == "2")
        {
            ui->checkBox->setChecked(true);
            getline (file,line);
            ui->lineEdit_email->setText(line.c_str());
            getline (file,line);
            ui->lineEdit_code->setText(line.c_str());
        } else
        {
            file.close();
            return false;
        }
    }
    file.close();
    return true;
}

bool MainWindow::write_file()
{
    ofstream file ("settings.txt");
    if(file.is_open())
    {
        file << ui->checkBox->checkState()
             << "\n"
             << ui->lineEdit_email->text().toStdString()
             << "\n"
             << ui->lineEdit_code->text().toStdString();
        file.close();
    } else
    {
        return false;
    }
    return true;
}

void MainWindow::logger(char *log_line)
{
    string str = (const char *) log_line;

    delete [] log_line;

    QTextCursor cursor( ui->logText->textCursor() );

    QTextCharFormat format;

    if(str.find(LOGGIN_ERROR) != string::npos || str.find(LOGGIN_ERROR_EXT) != string::npos)
    {
        format.setForeground( QBrush( QColor( "red" ) ) );
    } else if( str.find(LOGGIN_ATTENTION) != string::npos )
    {
        format.setForeground( QBrush( QColor( "#000000" ) ) );
    } else if( str.find(LOGGIN_OK) != string::npos )
    {
        format.setForeground( QBrush( QColor( "grey" ) ) );
    }

    if(str.substr(str.length() - 1, 1).find("\n") == string::npos )
    {
        str.append("\n");
    }


    if(str.find("call time") != string::npos )
    {
        cursor.setPosition(cursor.position() - str.length());
        ui->logText->setTextCursor(cursor);

        if(ui->logText->find("call time"))
        {
            cursor.movePosition(QTextCursor::End);
            ui->logText->setTextCursor(cursor);

            for(int i = 0; i <str.length(); i++)
            {
                cursor.deletePreviousChar();
            }
        } else
        {
            cursor.movePosition(QTextCursor::End);
        }

    }

    cursor.setCharFormat( format );
    cursor.insertText( str.c_str() );
    ui->logText->setTextCursor(cursor);

}

void MainWindow::on_pushButton_clearLog_clicked()
{
    ui->logText->clear();
}


