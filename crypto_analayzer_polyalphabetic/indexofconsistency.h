#ifndef INDEXOFCONSISTENCY_H
#define INDEXOFCONSISTENCY_H

#include <QMainWindow>
#include <QTextStream>
#include <QStandardItemModel>
#include <QDebug>
#include <QMdiArea>
#include <QMdiSubWindow>

#include "polyalphabetic.h"
namespace Ui {
class IndexOfConsistency;
}

class IndexOfConsistency : public QMainWindow
{
    Q_OBJECT

public:
    explicit IndexOfConsistency(QWidget *parent = 0, QString fopen_text="", int fmu=0, QString falphabet="", QMdiArea *pmdi=0);
    ~IndexOfConsistency();

    void calc_all_micxy();
    double calc_icx(QString a, QString b);
    void load_shift();
    int get_shift_of_icx(double icx);
    void out_in_table();
    void load_dictionary();
    void out_to_list1();
    void out_to_list2();
    void create_all_key();
    void create_group_info();
    void out_group_info(int index);
    void get_shift(QString a, QString b);
    QString do_shift(QString str, int sh);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_tableView_2_pressed(const QModelIndex &index);

    void on_tableView_2_activated(const QModelIndex &index);

    void on_tableView_2_entered(const QModelIndex &index);

    void on_listView_clicked(const QModelIndex &index);

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void closeEvent(QCloseEvent *);
    void writeSettings();

    void on_listView_2_clicked(const QModelIndex &index);


    void on_pushButton_3_pressed();

    void on_pushButton_3_released();

    void on_plainTextEdit_3_textChanged();

    void on_pushButton_pressed();

    void on_pushButton_released();

private:
    Ui::IndexOfConsistency *ui;

    QString open_text;
    int mu;
    QString alphabet;

    QString only_text;
    QVector<QString> mu_group;

    QHash< QPair<int,int> , QPair<double,int> > icx_mu;
    QVector< QPair<int,double> > shift;
    QStandardItemModel *shift_model_all;
    QVector<QString> dictionary;
    QVector<QString> all_keys;

    QVector< QHash<QCharRef,int> > group_info;

    QStandardItemModel *group_info_model;

    QHash<int, QString> get_simbol_of_index;
    QHash<QString, int> get_index_of_simbol;



    QMdiArea *mdi;
    QWidget *par;
    QLabel *llabel;
};

#endif // INDEXOFCONSISTENCY_H
