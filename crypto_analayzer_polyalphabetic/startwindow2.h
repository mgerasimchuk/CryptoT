#ifndef STARTWINDOW2_H
#define STARTWINDOW2_H

#include <QMainWindow>
#include "monoalphabetic.h"
#include "mainwindow.h"
#include "polyalphabetic.h"

namespace Ui {
class startwindow2;
}

class startwindow2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit startwindow2(QWidget *parent = 0);
    ~startwindow2();

private slots:
    void on_commandLinkButton_clicked();

    void on_commandLinkButton_2_clicked();

    void on_commandLinkButton_3_clicked();

    void on_commandLinkButton_4_clicked();

private:
    Ui::startwindow2 *ui;
};

#endif // STARTWINDOW2_H
