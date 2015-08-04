#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>
#include"monoalphabetic.h"
#include"mainwindow.h"

namespace Ui {
class StartWindow;
}

class StartWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StartWindow(QWidget *parent = 0);
    ~StartWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_toolBox_currentChanged(int index);

    void on_pushButton_3_clicked();

private:
    Ui::StartWindow *ui;
};

#endif // STARTWINDOW_H
