#include "startwindow2.h"
#include "ui_startwindow2.h"

startwindow2::startwindow2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::startwindow2)
{
    ui->setupUi(this);


}

startwindow2::~startwindow2()
{
    delete ui;
}

void startwindow2::on_commandLinkButton_clicked()
{
    MonoAlphabetic *w=new MonoAlphabetic(this);
    w->setWindowTitle("Анализатор шифра моноалфавитной подстановки");
    w->showMaximized();
    w->show();
}

void startwindow2::on_commandLinkButton_2_clicked()
{
    MainWindow *w=new MainWindow(this);
    w->setWindowTitle("Анализатор шифра полиалфавитной подстановки");
    w->showMaximized();
    w->show();
}

void startwindow2::on_commandLinkButton_3_clicked()
{
    PolyAlphabetic *w=new PolyAlphabetic(this);
    w->setWindowTitle("Анализатор шифра полиалфавитной подстановки");
    w->showMaximized();
    w->show();
}

void startwindow2::on_commandLinkButton_4_clicked()
{
    this->close();
}
