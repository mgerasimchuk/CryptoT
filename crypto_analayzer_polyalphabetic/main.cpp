#include "mainwindow.h"
#include "monoalphabetic.h"
#include "startwindow2.h"
#include "mainwindow123.h"
#include "mdiwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
 /*   MainWindow w;

    w.setWindowTitle("Анализатор шифра полиалфавитной подстановки");
    w.showMaximized();
*/
    /*MonoAlphabetic w;
    w.show();*/
    MDIWindow w;
    w.setWindowTitle("Программный комплекс CryptoT. Статистический криптоанализ моноалфавитных и полиалфавитных шифров");
    w.showMaximized();

/*
    w.setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    w.show();
    w.setMinimumWidth(w.width());
    w.setMinimumHeight(w.height());
    w.setMaximumWidth(w.width());
    w.setMaximumHeight(w.height());*/
    return a.exec();
}
