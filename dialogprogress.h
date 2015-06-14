#ifndef DIALOGPROGRESS_H
#define DIALOGPROGRESS_H

#include <QDialog>
#include <QDebug>
#include <QDesktopWidget>
#include <QRect>
#include <QTimer>

namespace Ui {
class DialogProgress;
}

class DialogProgress : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogProgress(QWidget *parent = 0);
    ~DialogProgress();
public slots:
    void run();
    void stop();
    void updateProgressBar();
private:
    Ui::DialogProgress *ui;
    void center_display();
    QTimer *timer;
    int count;
};

#endif // DIALOGPROGRESS_H
