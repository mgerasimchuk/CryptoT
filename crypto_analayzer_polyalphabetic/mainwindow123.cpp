#include "mainwindow123.h"
#include "ui_mainwindow123.h"
#include "startwindow.h"
#include "startwindow2.h"

MainWindow123::MainWindow123(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow123)
{
    ui->setupUi(this);
}

MainWindow123::~MainWindow123()
{
    delete ui;
}

void MainWindow123::on_pushButton_clicked()
{
    StartWindow *w=new StartWindow(this);
    w->show();


}

void MainWindow123::on_pushButton_2_clicked()
{
    startwindow2 *w=new startwindow2(this);
    w->show();
}
