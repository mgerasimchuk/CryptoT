#ifndef MONOALPHABETIC_H
#define MONOALPHABETIC_H

#include <QMainWindow>
#include <QtGui>
#include <QMap>

#include <QMessageBox>
#include <QDir>
#include <QFile>

#include <QTextEdit>
#include <QGraphicsTextItem>
#include <QFileDialog>

namespace Ui {
class MonoAlphabetic;
}

class MonoAlphabetic : public QMainWindow
{
    Q_OBJECT

public:
    explicit MonoAlphabetic(QWidget *parent = 0);
    ~MonoAlphabetic();
    void char_prob_load();
    void cipher_text_alphabet_load();
    void static_analyze();
    QString get_best(QString s);
    void out_text_after_static_analyze();
    void on_action_3_triggered(QString filename);

public slots:
    //void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void create_grafic(int disp);
    void out_busy_to_table();
    void UpdateData(const QModelIndex & indexA, const QModelIndex & indexB);
    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_tableView_clicked(const QModelIndex &index);
    void out_info();
    //---
    void load_bigram();
    void create_bigram_table();
    void selection_in_text(int s, int f);
    void select_zero_bigram();
    void on_add_select_clicked();
    void on_add_simbol_clicked();
    void create_busy();
    void on_pushButton_2_clicked();
    void show_graphics();
    void update_all();
    void on_spinBox_valueChanged(int arg1);
    void writeSettings();
    void closeEvent(QCloseEvent *);
    void on_lineEdit_textChanged(const QString &arg1);
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void load_dictionary();
    void on_lineEdit_2_returnPressed();
    void first_mask_search();
    void on_plainTextEdit_2_selectionChanged();

    void on_action_4_triggered();
    void on_action_6_triggered();
    //void on_action_7_triggered();
    void on_action_9_triggered();
    void on_action_triggered();
    void on_action_5_triggered();
    void on_action_10_triggered();
    void on_action_2_triggered();
    void out_simbols();
    void create_mask();




    void on_plainTextEdit_2_customContextMenuRequested(const QPoint &pos);
    void context_menu_check_item();
    void set_in_busy(int start,int finish, QString s);
    void next_find_list();
    void keyPressEvent(QKeyEvent *event);


    void on_pushButton_clicked();


    void on_action_3_triggered();
private slots:
    void changeEvent(QEvent *event);
    void resizeEvent(QResizeEvent *);

    void on_plainTextEdit_textChanged();

private:
    Ui::MonoAlphabetic *ui;
    QMap<QString,float> char_prob;
    QString alphabet;
    QMap<QString,int> char_count_text;
    QMap<QString,float> char_prob_text;
    QGraphicsScene *sc;
    QVector< QPair<QString,QString> > busy;//1-A 2-00
    QStandardItemModel *model;
    QString oldvar;
    QString bigram_alphabet;
    QMap< QString, QHash<QString,int> > char_bigram;
    QMap<QString,int> char_bigram_text;
    QStandardItemModel *model2;
    QList<QTextEdit::ExtraSelection> extraSelections;
    int limit;
    int char_count;
    int cc;
    int last_spin;
    QVector<int> last_table_item;
    QVector<QString> dictionary;

    QString dictionary_file_name;
    bool selection_on;
    QVector< QPair<QString,QString> > for_mask;
    QVector<QString> find_result;
    QMenu *menu;
    int lefti,righti;
    bool text_edit_sovet;
    bool grid_sovet;

    QPoint pos_for_menu_find_list;
    int ind_find_list;

    struct hist{
        QVector< QPair<QString,QString> > busy;
        int char_count;
        QString lineedit;
    };

    QVector< hist  > history;

    int now_history_ind;



};

#endif // MONOALPHABETIC_H
