#include "startwindow.h"
#include "ui_startwindow.h"

StartWindow::StartWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);
    ui->toolBox->setCurrentIndex(0);
}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::on_pushButton_clicked()
{
    MonoAlphabetic *w=new MonoAlphabetic(this);
    w->setWindowTitle("Анализатор шифра моноалфавитной подстановки");
    w->showMaximized();
    w->show();
}

void StartWindow::on_pushButton_2_clicked()
{
    MainWindow *w=new MainWindow(this);
    w->setWindowTitle("Анализатор шифра полиалфавитной подстановки");
    w->showMaximized();
    w->show();
}

void StartWindow::on_toolBox_currentChanged(int index)
{
   // QIcon icon;
   // icon.addFile(QStringLiteral(":/img/list_marker_on.png"), QSize(), QIcon::Normal, QIcon::Off);

}

void StartWindow::on_pushButton_3_clicked()
{
    PolyAlphabetic *w=new PolyAlphabetic(this);
    w->setWindowTitle("Анализатор шифра полиалфавитной подстановки");
    w->showMaximized();
    w->show();
}
