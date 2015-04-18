#ifndef DIALOGALERT_H
#define DIALOGALERT_H

#include <QDialog>
#include <QDesktopWidget>

namespace Ui {
class DialogAlert;
}

class DialogAlert : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogAlert(QWidget *parent = 0);
    ~DialogAlert();
    
private:
    Ui::DialogAlert *ui;

public slots:
    void run(char *);

private slots:
    void on_pushButton_ok_clicked();
};

#endif // DIALOGALERT_H
