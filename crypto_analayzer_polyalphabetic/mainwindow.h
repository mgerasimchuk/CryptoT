#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>

#include <QStringListModel>
#include <QStandardItemModel>
#include <QMdiArea>
#include <QMdiSubWindow>

#include "polyalphabetic.h"
#include "indexofconsistency.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, QMdiArea *pmdi=0);
    ~MainWindow();
    void alphabet_load();
    void corr_table_load();
    int Nod_n(int *a, int *end);
    int Nod(int a, int b);
    void selection_in_text(int s,int f,QPlainTextEdit *edit);
    void find_mu();
    void out_period_of_index(int index);
    void calc_icx();
    void calc_many_icx();
    void create_graphic();
    void block_all(bool fl);


private slots:

    void on_find_mu_button_clicked();


    void on_comboBox_currentIndexChanged(int index);


    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    //void on_pushButton_clicked();
    void writeSettings();
    void closeEvent(QCloseEvent *);
    void on_action_triggered();

    void on_action_2_triggered();

    void on_action_3_triggered();
    void keyPressEvent(QKeyEvent *event);

    void on_open_text_textChanged();

private:
    Ui::MainWindow *ui;
    int mu;
    QString alphabet;
    struct three
    {
        QString substr;
        int nod;
        QVector< QPair<int,int> > indexes;
        QVector< int > dist;
    };
    QVector< three > periods;
    QList<QTextEdit::ExtraSelection> extraSelections;
    QVector< QVector<float> > corr_table;
    float icx;//индекс соответствия

    QGraphicsScene *sc;

    QVector<float> many_icx;

    QMdiArea *mdi;
};

#endif // MAINWINDOW_H
