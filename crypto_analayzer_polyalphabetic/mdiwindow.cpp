#include "mdiwindow.h"
#include "ui_mdiwindow.h"

MDIWindow::MDIWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MDIWindow)
{
    ui->setupUi(this);

    setCentralWidget(ui->mdiArea);
    ficha2=new QTimer;
    connect(ficha2,SIGNAL(timeout()),SLOT(ficha2_event()));
    ficha=new QTimer;
    connect(ficha,SIGNAL(timeout()),SLOT(ficha_event()));

    set_icon_toolbar_theme();
}

MDIWindow::~MDIWindow()
{
    delete ui;
}

void MDIWindow::on_open_project_triggered()
{
    QFile *in_file;
    in_file=new QFile;
    in_file->setFileName(QFileDialog::getOpenFileName(this, "Открыть файл", "", "Файлы анализатора(*.pmaf *.ppaf)"));

    if(in_file->fileName().right(4)=="pmaf")
    {
        MonoAlphabetic *child = new MonoAlphabetic;
        QMdiSubWindow *sw=ui->mdiArea->addSubWindow(child);
        sw->setWindowIcon(QIcon(":/img/monow.png"));
        sw->setMinimumWidth(1350);
        child->show();
        child->setWindowTitle("Анализ моноалфавитных шифров");
        child->on_action_3_triggered(in_file->fileName());

        /*childs_win ch;
        ch.win=child;
        ch.t=mono;
        ch.sub_win=sw;*/

        //childs.push_back(ch);

    }

    if(in_file->fileName().right(4)=="ppaf")
    {
        PolyAlphabetic *child = new PolyAlphabetic;
        QMdiSubWindow *sw=ui->mdiArea->addSubWindow(child);
        sw->setWindowIcon(QIcon(":/img/polyw.png"));
        sw->setMinimumWidth(1350);

        child->setWindowTitle("Анализ полиалфавитных шифров");
        child->show();
        child->on_action_3_triggered(in_file->fileName());

        /*childs_win ch;
        ch.win=child;
        ch.t=poly;
        ch.sub_win=sw;

        childs.push_back(ch);*/
    }

}

void MDIWindow::on_mono_hovered()
{
    statusBar()->showMessage("Создать проект (Анализ моноалфавитных шифров)",2000);
}

void MDIWindow::on_mono_triggered()
{
    MonoAlphabetic *child = new MonoAlphabetic;
    QMdiSubWindow *sw=ui->mdiArea->addSubWindow(child);
    sw->setWindowIcon(QIcon(":/img/monow.png"));
    sw->setMinimumWidth(1350);
    child->show();

    child->setWindowTitle("Анализ моноалфавитных шифров");


    /*childs_win ch;
    ch.win=child;
    ch.t=mono;
    ch.sub_win=sw;

    childs.push_back(ch);*/
}

void MDIWindow::on_poly_hovered()
{
    statusBar()->showMessage("Создать проект (Анализ полиалфавитных шифров)",2000);
}

void MDIWindow::on_poly_triggered()
{
    MainWindow *child = new MainWindow(this,ui->mdiArea);
    QMdiSubWindow *sw=ui->mdiArea->addSubWindow(child);
    sw->setWindowIcon(QIcon(":/img/polyw.png"));
    sw->setGeometry(sw->pos().x(),sw->pos().y(),1454,835);

    child->setWindowIcon(QIcon(":/img/polyw.png"));
    child->setWindowTitle("Анализ полиалфавитных шифров");
    child->show();

    /*childs_win ch;
    ch.win=child;
    ch.t=kasis;
    ch.sub_win=sw;

    childs.push_back(ch);*/
}

void MDIWindow::on_open_project_hovered()
{
    statusBar()->showMessage("Открыть проект",2000);

}

void MDIWindow::on_action_3_triggered()
{
    this->close();

}

void MDIWindow::on_close_all_hovered()
{
    statusBar()->showMessage("Закрыть все проекты",2000);
}

void MDIWindow::on_action_3_hovered()
{
    statusBar()->showMessage("Выход",2000);
}

void MDIWindow::on_close_all_triggered()
{
    if(ui->mdiArea->subWindowList().size()==0)
        return;

    QMessageBox msgBox;

    msgBox.setWindowTitle("Программный комплекс CryptoT");

    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);

    msgBox.button(QMessageBox::Save)->setText("Сохранить");
    msgBox.button(QMessageBox::Discard)->setText("Закрыть все");
    msgBox.button(QMessageBox::Cancel)->setText("Отмена");

    msgBox.setText("Закрыть проекты без сохранения?");



    int ret = msgBox.exec();

    if(ret==QMessageBox::Save)
        ui->mdiArea->closeAllSubWindows();
    else
    {

        if(ret==QMessageBox::Cancel)
            return;

        ui->mdiArea->~QMdiArea();
        ui->mdiArea = new QMdiArea(this);
        ui->mdiArea->setObjectName(QStringLiteral("mdiArea"));
        ui->mdiArea->setGeometry(QRect(11, 11, 766, 515));
        ui->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        ui->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        ui->mdiArea->setViewMode(QMdiArea::SubWindowView);

        this->setCentralWidget(ui->mdiArea);
        ui->mdiArea->show();
    }

}

void MDIWindow::closeEvent(QCloseEvent *e)
{
    QMessageBox msgBox;

    msgBox.setWindowTitle("Программный комплекс CryptoTи");

    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);

    msgBox.button(QMessageBox::Save)->setText("Сохранить");
    msgBox.button(QMessageBox::Discard)->setText("Закрыть все");
    msgBox.button(QMessageBox::Cancel)->setText("Отмена");

    msgBox.setText("Закрыть проекты без сохранения?");

    if(ui->mdiArea->subWindowList().size()>0)
    {
        int ret = msgBox.exec();

        if(ret==QMessageBox::Save)
        {

            ui->mdiArea->closeAllSubWindows();
            this->close();
        }

        if(ret==QMessageBox::Cancel)
        {
            e->ignore();
            return;
        }

        this->close();

    }
}


void MDIWindow::on_about_hovered()
{
    statusBar()->showMessage("О программе",2000);
}

void MDIWindow::on_about_triggered()
{
    QString info="Программный комплекс CryptoT предназначен для студентов и магистрантов, обучающимся по дисциплинам «Защита информации», «Информационная безопасность и защита данных», «Информационная безопасность и защита информации», «Криптографические основы защиты информации» и др. освоить базовые основы методов криптоанализа, основанных на статистическом исследовании симметричных моноалфавитных и полиалфавитных шифров, таких как: шифр Цезаря, шифр Вижинера и т.п. Программа также может быть полезна и для преподавателей, обучающих в области информационной безопасности, а также всех лиц, интересующихся криптографическими методами защиты информации.\r\n\r\nАвторы программы:\r\n   Герасимчук М.Г. (студент ИКИТ СФУ, гр. КИ10-06) – Разработка алгоритмов программы, интерфейсов пользователя, программная реализация программного комплекса.\r\n   Шниперов А.Н. (доц. Каф. ПМиКБ ИКИТ СФУ) – научное руководство проектом.";
    QMessageBox::about(this,"О программе",info);
}

void MDIWindow::on_close_tab_hovered()
{
    statusBar()->showMessage("Закрыть активный проект",2000);
}

void MDIWindow::on_close_tab_triggered()
{

    ui->mdiArea->closeActiveSubWindow();
}

void MDIWindow::create_icon_toolbar()
{
    act.clear();
    aname.clear();
    act.push_back(ui->open_project);
    act.push_back(ui->close_tab);
    act.push_back(ui->close_all);
    act.push_back(ui->action_3);
    act.push_back(ui->mono);
    act.push_back(ui->poly);
    act.push_back(ui->about);


    aname.push_back(":/img/import.png");
    aname.push_back(":/img/close_tab.png");
    aname.push_back(":/img/close_all.png");
    aname.push_back(":/img/exit.png");
    aname.push_back(":/img/mono.png");
    aname.push_back(":/img/poly.png");
    aname.push_back(":/img/about.png");


}

void MDIWindow::set_icon_toolbar_theme()
{
    create_icon_toolbar();
    for(int i=0;i<act.size();i++)
    {
        act[i]->setIcon(QIcon(aname[i].left(aname[i].length()-4)+"_t2.png"));
    }
}




void MDIWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {

    case Qt::Key_G:
        if((event->modifiers() & Qt::CTRL) && (event->modifiers() & Qt::SHIFT) && (event->modifiers() & Qt::ALT))
        {
            if(ficha!=NULL && ficha2!=NULL && !ficha->isActive() && !ficha2->isActive())
                fficha();

            break;
        }

    default:
        QWidget::keyPressEvent(event);
    }
}

void MDIWindow::ficha_event()
{
    if(fichai>=act.size())
    {
        ficha->stop();
        fichai=act.size()-1;

        pause=0;
        ficha2->start(30);

        return;
    }
    act[fichai]->setIcon(QIcon( QString(":/img/cryptot%1.png").arg(fichai)));
    fichai++;
}

void MDIWindow::ficha2_event()
{
    pause++;
    if(pause<20)
        return;
    if(fichai<0)
    {
        ficha2->stop();
        fichai=0;

        return;
    }

    act[fichai]->setIcon(QIcon( aname[fichai]));
    fichai--;
}

void MDIWindow::fficha()
{
    create_icon_toolbar();

    fichai=0;
    ficha->start(30);
}

