#ifndef POLYALPHABETIC_H
#define POLYALPHABETIC_H

#include <QMainWindow>
#include <QtGui>
#include <QMap>

#include <QMessageBox>
#include <QDir>
#include <QFile>

#include <QTextEdit>
#include <QGraphicsTextItem>
#include <QFileDialog>

#include <QStringListModel>
#include <QStandardItemModel>

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QRadioButton>
#include <QPlainTextEdit>
#include <QAbstractItemModel>

namespace Ui {
class PolyAlphabetic;
}

class PolyAlphabetic : public QMainWindow
{
    Q_OBJECT

public:
    explicit PolyAlphabetic(QWidget *parent = 0, QString fopen_text="", int fmu=0, bool ffiks=false);
    ~PolyAlphabetic();

    void char_prob_load();
    void cipher_text_alphabet_load();
    void static_analyze();
    QString get_best(QString s);
    void out_text_after_static_analyze();
    void on_pushButton_13_clicked();
public slots:
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void create_grafic(int disp);
    void out_busy_to_table();
    void UpdateData(const QModelIndex & indexA, const QModelIndex & indexB);
    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_tableView_clicked(const QModelIndex &index);
    void out_info();

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
    void on_action_3_triggered(QString filename);
    void on_action_4_triggered();
    void on_action_6_triggered();
    //void on_action_7_triggered();
    void on_action_9_triggered();
    void on_action_triggered();
    void on_action_5_triggered();
    void on_action_10_triggered();
    void on_action_2_triggered();
    void out_simbols();
    void out_list();

    void on_listView_clicked(const QModelIndex &index);
    void selected_mu_group();
    void on_listView_pressed(const QModelIndex &index);

    void on_listView_entered(const QModelIndex &index);

    void on_listView_activated(const QModelIndex &index);
    void first_start();
    void click_on_list(const QModelIndex &index);

    void on_pushButton_7_clicked();

    void on_listView_2_clicked(const QModelIndex &index);

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();



    void on_radioButton_3_clicked();

    void on_radioButton_4_clicked();


    void on_pushButton_12_clicked();

    void on_lineEdit_3_returnPressed();

    void on_pushButton_14_clicked();

    void on_lineEdit_4_returnPressed();

    void on_pushButton_15_clicked();

    void on_lineEdit_5_returnPressed();

    void on_pushButton_16_clicked();

    void on_lineEdit_6_returnPressed();




    void out_marker_select();
    void on_commandLinkButton_clicked();

    void on_commandLinkButton_2_clicked();

    void on_commandLinkButton_3_clicked();

    void on_commandLinkButton_4_clicked();

    void on_commandLinkButton_5_clicked();
    void keyPressEvent(QKeyEvent *event);
    void create_fiks_busy();
    void init(QString fopen_text, int fmu, bool ffiks);


    void on_plainTextEdit_2_customContextMenuRequested(const QPoint &pos);
    void context_menu_check_item();
    void set_in_busy(int start,int finish, QString s);
    void next_find_list();

    void on_action_3_triggered();

private slots:
    void changeEvent(QEvent *event);
    void resizeEvent(QResizeEvent *);


private:
    Ui::PolyAlphabetic *ui;
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


    QString open_text;
    int mu;
    QString open_all_simbol_text;

    struct save{
        QVector< QPair<QString,QString> > busy;
        int char_count;
        int index;
        QString lineEdit;

    };
    QVector<save> many_busy;
    QVector<QString> for_list_view_2;
    QVector< QPair<QString,int> > text_and_index;
    bool on_group_selected;

    QList<QTextEdit::ExtraSelection> group_selections;
    bool show_all_simbol;

    int now_group;


    bool simple_mask;
    QString search_str;
    QString search_res;


    struct finditem
    {
        QLineEdit *le;
        QPushButton *pb;
        QLabel *l;
        QRadioButton *rb1;
        QRadioButton *rb2;
        QPlainTextEdit *pt;

    };

    QVector< finditem > search_tab;

    struct ww{int start;int sdv;QColor color;};
    QVector< ww > marker_select_ind;
    QList<QTextEdit::ExtraSelection> marker_select;
    QColor lineColor;

    bool sovet;
    bool fiks;

    int lefti,righti;

    QVector<QString> find_result;
    QPoint pos_for_menu_find_list;
    int ind_find_list;
    QMenu *menu;

    QVector< QVector<save>  > history;
    int now_history_ind;

    QWidget *par;
};

#endif // POLYALPHABETIC_H
