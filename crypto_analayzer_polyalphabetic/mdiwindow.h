#ifndef MDIWINDOW_H
#define MDIWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "monoalphabetic.h"
#include "mainwindow.h"
#include "polyalphabetic.h"
#include "QMdiSubWindow"
#include <QTimer>
namespace Ui {
class MDIWindow;
}

class MDIWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MDIWindow(QWidget *parent = 0);
    ~MDIWindow();

private slots:
    void on_open_project_triggered();

    void on_mono_hovered();

    void on_mono_triggered();

    void on_poly_hovered();

    void on_poly_triggered();

    void on_open_project_hovered();

    void on_action_3_triggered();

    void on_close_all_hovered();

    void on_action_3_hovered();

    void on_close_all_triggered();

    void closeEvent(QCloseEvent *e);

    void on_about_hovered();

    void on_about_triggered();


    void on_close_tab_hovered();

    void on_close_tab_triggered();

    void create_icon_toolbar();
    void set_icon_toolbar_theme();


    void keyPressEvent(QKeyEvent *event);
    void ficha_event();
    void ficha2_event();
    void fficha();

private:
    Ui::MDIWindow *ui;

    enum win_type{mono,poly,kasis};
    struct childs_win
    {
        QMainWindow* win;
        QMdiSubWindow* sub_win;
        win_type t;
    };

    QVector< childs_win > childs;
    QTimer *ficha;
    QTimer *ficha2;
    QVector<QAction*> act;
    int fichai;
    QVector<QString> aname;
    int pause;


};

#endif // MDIWINDOW_H
