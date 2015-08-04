#ifndef MAINWINDOW123_H
#define MAINWINDOW123_H

#include <QMainWindow>

namespace Ui {
class MainWindow123;
}

class MainWindow123 : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow123(QWidget *parent = 0);
    ~MainWindow123();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow123 *ui;
};

#endif // MAINWINDOW123_H
