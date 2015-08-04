#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, QMdiArea *pmdi) :
    QMainWindow(parent),
    mdi(pmdi),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    alphabet_load();
    corr_table_load();
    ui->find_mu_progress->setVisible(false);

    QSettings set("crypto_analyzer_polyalphabetic","spliter_sizes_main_form");

    ui->splitter->restoreState(set.value("spliter", QByteArray()).toByteArray());
    ui->splitter_2->restoreState(set.value("spliter2", QByteArray()).toByteArray());

    block_all(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::alphabet_load()
{
    QTextStream in;
    QFile *in_file;
    in_file=new QFile;
    in_file->setFileName("alphabet.txt");
    in_file->open(QIODevice::ReadOnly);
    in.setDevice(in_file);

    in>>alphabet;


    in_file->close();
}

void MainWindow::corr_table_load()
{
    QTextStream in;
    QFile *in_file;
    in_file=new QFile;
    in_file->setFileName("corr_table.txt");
    in_file->open(QIODevice::ReadOnly);
    in.setDevice(in_file);

    int n,m;

    in>>n>>m;


    for(int i=0;i<n;i++)
    {
        QVector<float> temp;
        temp.resize(m);
        for(int j=0;j<m;j++)
            in>>temp[j];
        corr_table.push_back(temp);
    }

    in_file->close();


    QString info=QString().sprintf("μ     Ic(x)min     Ic(x)sr.     Ic(x)max\n");
    for(int i=0;i<n-1;i++)
        info+=QString().sprintf("%0.0f     %0.5f     %0.5f     %0.5f\n",corr_table[i][0],corr_table[i][1],corr_table[i][3],corr_table[i][2]);

    info+=QString().sprintf(">     %0.5f     %0.5f     %0.5f\n",corr_table[n-1][1],corr_table[n-1][3],corr_table[n-1][2]);

    ui->corr_table_text->setPlainText(info);
}

void MainWindow::selection_in_text(int s,int f,QPlainTextEdit *edit)
{


    QTextCursor tc = edit->textCursor();
      tc.select(QTextCursor::Document);
      QTextCharFormat format;
      QColor lineColor = QColor(102,205,0);//(0,191,255);
      format.setBackground(lineColor);
      for(int i = s; i <= f; ++i) {
        QTextEdit::ExtraSelection selection;
        selection.format = format;
        selection.format.setToolTip(QString::number(i));
        selection.cursor = tc;
        selection.cursor.clearSelection();
        selection.cursor.setPosition(i);
        selection.cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, 1);
        // selection.cursor.mergeCharFormat(selection.format);
        extraSelections.append(selection);
      }
      edit->setExtraSelections(extraSelections);
}


int MainWindow::Nod(int a, int b)
{
   return a && b ? (a >= b ? Nod(a % b, b) : Nod(a, b % a)) : a + b;
}

int MainWindow::Nod_n(int *a, int *end)
{
   return a < end ? Nod(Nod_n(a + 1, end), *a) : 0;
}




void MainWindow::find_mu()
{
    ui->mu_label->setText("μ:");
    ui->label_2->setText("Расстояния:");

    periods.clear();
    QString text=ui->open_text->toPlainText().toUpper();
    int sub_len=ui->mu_length->value();
    QString sub_str;

    //selection_in_text(2,5,ui->open_text);

    ui->find_mu_progress->setVisible(true);
    ui->find_mu_progress->setMinimum(0);
    ui->find_mu_progress->setMaximum(text.length()-1);


    QVector<int> distance;
    QVector< QPair<int,int> > indexes;


    for(int i=0;i<=text.length()-sub_len;i++)
    {
        sub_str="";



        for(int j=i;j<i+sub_len;j++)
            sub_str+=text[j];

        bool f=false;
        for(int j=0;j<sub_len;j++)
            if(alphabet.indexOf(sub_str[j],0)==-1)
            {
                f=true;
                break;
            }
        if(f)
            continue;


        distance.clear();
        indexes.clear();

        int j=text.indexOf(sub_str, 0);
        int last=j;
        indexes.push_back(QPair<int,int>(j,j+sub_len-1));
        j++;
        while((j = text.indexOf(sub_str, j)) != -1)
        {

            int temp=0;
            for(int l=last;l<j;l++)
                if(alphabet.indexOf(text[l],0)!=-1)
                   temp++;


            distance.push_back(temp);
            indexes.push_back(QPair<int,int>(j,j+sub_len-1));
            last=j;

            j++;
        }


        int *a=new int[distance.size()];
        for(int k=0;k<distance.size();k++)
            a[k]=distance[k];

        int nod=Nod_n(a, a + distance.size());
        if(nod>1 && nod<=10 && distance.size()>0)
        {
            three push;
            push.substr=sub_str;
            push.nod=nod;
            push.indexes=indexes;
            push.dist=distance;
            periods.push_back(push);
        }






        ui->find_mu_progress->setValue(i);
    }

    for(int i=0;i<periods.size();i++)
        for(int j=0;j<periods.size();j++)
            if(periods[i].indexes.size()>periods[j].indexes.size())
                qSwap(periods[i],periods[j]);


    for(int i=0;i<periods.size();i++)
        for(int j=0;j<periods.size();j++)
            if(periods[i].nod>periods[j].nod)
                qSwap(periods[i],periods[j]);

    QVector< three > vec=periods;

    periods.clear();

    for(int i=0;i<vec.size();i++)
    {
        bool f=true;
       for(int j=0;j<periods.size();j++)
           if(periods[j].substr==vec[i].substr)
               f=false;

       if(f)
           periods.push_back(vec[i]);
    }

    ui->find_mu_progress->setVisible(false);

    ui->comboBox->clear();


    for(int i=0;i<periods.size();i++)
    {
        ui->comboBox->addItem("Последовательность: \"" + periods[i].substr + QString().sprintf("\", Найдено: %d, μ: %d",
                                                periods[i].indexes.size(), periods[i].nod));

    }

}

void MainWindow::out_period_of_index(int index)
{
    if(ui->comboBox->count()==0)
    {
        return;
    }
    extraSelections.clear();
    for(int i=0;i<periods[index].indexes.size();i++)
        selection_in_text(periods[index].indexes[i].first,periods[index].indexes[i].second,ui->open_text);


    ui->label_2->setText("Расстояния: ");
    for(int i=0;i<periods[index].dist.size();i++)
    {
        ui->label_2->setText(ui->label_2->text()+QString().sprintf("%d",periods[index].dist[i]));
        if(i+1<periods[index].dist.size())
            ui->label_2->setText(ui->label_2->text()+", ");
    }
    ui->label_2->setText(ui->label_2->text()+" сим.");


    QColor color;
    mu=periods[index].nod;
    int ind=(mu>7)?(corr_table.size()-1):(mu-1);
    calc_icx();
    calc_many_icx();
    if(icx>corr_table[ind][3] && icx<corr_table[ind][2])
        color.setRgb(102,205,0);
    else
    {
        if(icx>corr_table[ind][1] && icx<corr_table[ind][3])
            color.setRgb(255,215,0);
        else
            color.setRgb(178,34,34);
    }

    mu=periods[index].nod;
    ui->icx_label_3->setText(QString().sprintf("%f",icx));
    ui->icx_label->setText("<p style=\"color:"+color.name()+"\">"+QString().sprintf("%d",periods[index].nod)+"</p>");



}

void MainWindow::calc_icx()
{
    QMap<QCharRef,int> f;
    int count=0;
    QString text=ui->open_text->toPlainText();

    for(int i=0;i<text.length();i++)
        if(alphabet.indexOf(text[i],0)!=-1)
        {
            f[text[i]]++;
            count++;
        }

    int N=count;
    int M=alphabet.length();

    float sigmaf=0;
    for(int i=0;i<M;i++)
        sigmaf+=f[alphabet[i]]*(f[alphabet[i]]-1);
    icx=sigmaf/(N*(N-1));
}

void MainWindow::calc_many_icx()
{
    many_icx.clear();
    QString only_text="";
    QVector<QString> mu_group;
    QString open_text=ui->open_text->toPlainText();

    for(int i=0;i<open_text.length();i++)
    {
        if(alphabet.indexOf(open_text[i],0)!=-1)
            only_text+=open_text[i];
    }

    mu_group.resize(mu);
    for(int i=0;i<only_text.length();i++)
    {
        mu_group[i%mu]+=only_text[i];
    }

    for(int i=0;i<mu_group.size();i++)
    {
        QMap<QCharRef,int> f;
        int count=0;
        QString text=mu_group[i];

        for(int i=0;i<text.length();i++)
            if(alphabet.indexOf(text[i],0)!=-1)
            {
                f[text[i]]++;
                count++;
            }

        int N=count;
        int M=alphabet.length();
        float ticx;

        float sigmaf=0;
        for(int i=0;i<M;i++)
            sigmaf+=f[alphabet[i]]*(f[alphabet[i]]-1);
        ticx=sigmaf/(N*(N-1));

        many_icx.push_back(ticx);
    }




    QString name_item_str="%1<span style=\" vertical-align:sub;\">%2</span>";
    QString value_item_str="<span style=\" color:%1;\">%2</span>";
    QString str;
    QString r;

    QColor color;
    for(int i=0;i<many_icx.size();i++)
    {
        if(many_icx[i]>0.050 && many_icx[i]<0.07)
            color.setRgb(102,205,0);
        else
            color.setRgb(178,34,34);
        if((i+1)<many_icx.size())
            r=",  ";
        else
            r=".";

        if((i+1)%2==0)
            r=r+"<br>";

        str+=name_item_str.arg("I").arg("c")+"("+name_item_str.arg("x").arg(i)+")="
                +value_item_str.arg(color.name()).arg(QString().sprintf("%0.4f",many_icx[i]))
                +r;
    }

    ui->many_icx_label->setText(str);
}



void MainWindow::on_find_mu_button_clicked()
{
    periods.clear();
    extraSelections.clear();
    many_icx.clear();

    ui->icx_label->clear();
    ui->icx_label_3->clear();
    extraSelections.clear();
    ui->many_icx_label->clear();

    QString text=ui->open_text->toPlainText().toUpper();

    for(int i=0;i<text.length();i++)
    {
        if(text[i]=='\n')
            text[i]=' ';
    }

    ui->open_text->setPlainText(text);

    ui->open_text->setExtraSelections(extraSelections);

    find_mu();



    create_graphic();
    block_all(true);
}



void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    out_period_of_index(index);
}


void MainWindow::on_pushButton_3_clicked()
{
    PolyAlphabetic *w=new PolyAlphabetic(this,ui->open_text->toPlainText(),mu);

    w->setWindowTitle("Анализатор шифра полиалфавитной подстановки");
    QMdiSubWindow *sww=mdi->addSubWindow(w);
    sww->setWindowIcon(QIcon(":/img/polyw.png"));

    this->showMinimized();
    w->show();

    //box->exec();
}

void MainWindow::on_pushButton_4_clicked()
{
    mu=ui->spinBox->value();
    ui->icx_label->setText(QString().sprintf("%d",mu));

}

void MainWindow::on_pushButton_2_clicked()
{
    this->showMinimized();

    IndexOfConsistency *w=new IndexOfConsistency(this,ui->open_text->toPlainText(),mu,alphabet,mdi);
    w->setWindowTitle("Анализатор шифра полиалфавитной подстановки");

    QMdiSubWindow *sw=mdi->addSubWindow(w);
    sw->setWindowIcon(QIcon(":/img/polyw.png"));
    w->show();
}



void MainWindow::create_graphic()
{
    ui->graphicsView->setRenderHint(QPainter::Antialiasing, true);
   /* int sizew=1536;//1400;
    int sizeh=668;
    int space=40;

    int const_for_ky=25.5;
    int const_for_kx=10.971;*/

    int sizew=1036;//1400;
    int sizeh=536;
    int space=40;

    int const_for_ky=20.5;
    int const_for_kx=10.971;

    int shift_y=300;




    sc=new QGraphicsScene(0,0,sizew,sizeh,ui->graphicsView);

    QPen pen(QBrush(QColor(139,134,130),Qt::SolidPattern),4,Qt::SolidLine, Qt::RoundCap,Qt::RoundJoin);
    pen.setColor(QColor(139,119,101));
    sc->addLine(space,space,space,sizeh-space,pen);
    sc->addLine(space,sizeh-space,sizew-space,sizeh-space,pen);

    float ky=sizeh*const_for_ky;//18000;
    float kx=sizew/const_for_kx;//140;
    float x1,y1,x2,y2;



    //mu_lines
    int max_len=45;
    QMap<int,QString> mu_text;
    for(int i=0;i<periods.size();i++)
    {
        if(mu_text[periods[i].nod].length()>max_len)
        {
            if(mu_text[periods[i].nod][mu_text[periods[i].nod].length()-1]!='.')
                mu_text[periods[i].nod]+=" ...";
            continue;
        }

        if(mu_text[periods[i].nod].length()==0)
            mu_text[periods[i].nod]+=periods[i].substr;//+QString().sprintf("(%d)",periods[i].indexes.size());
        else
            mu_text[periods[i].nod]+="," + periods[i].substr;//+QString().sprintf("(%d)",periods[i].indexes.size());
    }


    QMap<int,int> temp;
    for(int i=periods.size()-1;i>=0;i--)
    {
        if(temp[periods[i].nod]==0)
        {
            temp[periods[i].nod]=1;
            x1 = space + (periods[i].nod-1)*kx;
            y1 = (temp.size()+1)*space;

            x2 = space + (periods[i].nod-1)*kx;
            if(periods[i].nod-1<corr_table.size())
                y2 = sizeh + shift_y - space - (float)(corr_table[periods[i].nod-1][1]*ky);
            else
                y2 = sizeh + shift_y - space - (float)(corr_table[corr_table.size()-1][1]*ky);

            QPen p(QColor(139,119,101,200),2,Qt::DotLine);
            if(i==mu)
                p.setColor(QColor(102,205,0));

            int ind=(periods[i].nod>7)?(corr_table.size()-1):(periods[i].nod-1);
            if(icx>corr_table[ind][3] && icx<corr_table[ind][2])
                p.setColor(QColor(102,205,0));
            else
            {
                if(icx>corr_table[ind][1] && icx<corr_table[ind][3])
                    p.setColor(QColor(255,215,0));
                else
                    p.setColor(QColor(178,34,34));
            }



            sc->addLine(x1,y1,x2,y2,p);
            sc->addLine(x1-space/4,y1+space/4,(x1-space/4)+30+(mu_text[periods[i].nod].length()*7),y1+space/4,p);



            QGraphicsTextItem *t;
            t=new QGraphicsTextItem(mu_text[periods[i].nod]);
            t->setDefaultTextColor(QColor(p.color().red()-30,p.color().green()-30,p.color().blue()));//(238,130,98));
            t->setPos(x1,y1+space/4-22);
            sc->addItem(t);



        }
    }




    //icx
    x1=space;
    y1=sizeh + shift_y - space - (float)(icx*ky);
    x2=sizew-space;
    y2=sizeh + shift_y - space - (float)(icx*ky);
    sc->addLine(x1,y1,x2,y2,QPen(QColor(139,119,101,200),2,Qt::DashLine));


    int per_lim;
    if(periods.size()!=0 && corr_table.size()<periods[0].nod)
    {
        per_lim=periods[0].nod-1;
    }
    else
        per_lim=corr_table.size()-1;

    //оси и точки и icmni sr max
    pen.setColor(QColor(205,175,149));
    for(int j=1;j<corr_table[0].size();j++)
    {
        for(int i=0;i<per_lim;i++)
        {
            if(i>=corr_table.size()-1)
            {
                x1 = x2;
                y1 = y2;

                x2 = space + (i+1)*kx;
                y2 = y2;
            }
            else
            {
                x1 = space + (corr_table[i][0]-1)*kx;
                y1 = sizeh + shift_y - space - (float)(corr_table[i][j]*ky);

                x2 = space + (corr_table[i+1][0]-1)*kx;
                y2 = sizeh + shift_y - space - (float)(corr_table[i+1][j]*ky);
            }
            sc->addLine(x1,y1,x2,y2,QPen(QBrush(QColor(205,175,149),Qt::SolidPattern),4,Qt::SolidLine, Qt::RoundCap,Qt::RoundJoin));
            sc->addEllipse(x1-4,y1-4,8,8,QPen(QColor(139,119,101),2),QBrush(QColor(205,175,149)));


            sc->addLine(x2,sizeh-space-5,x2,sizeh-space+5,QPen(QBrush(QColor(139,134,130),Qt::SolidPattern),4,Qt::SolidLine, Qt::RoundCap,Qt::RoundJoin));

            QGraphicsTextItem *t;
            t=new QGraphicsTextItem(QString().sprintf("%d",i+2));
            t->setDefaultTextColor(QColor(139,134,130));
            t->setPos(x2-7,sizeh-space+3);
            sc->addItem(t);


            if(i==0)
            {
                sc->addEllipse(x1-5,sizeh-space-5,10,10,QPen(QColor(139,119,101),2),QBrush(QColor(139,134,130)));
                t=new QGraphicsTextItem(QString().sprintf("%d",i+1));
                t->setDefaultTextColor(QColor(139,134,130));
                t->setPos(x1-7,sizeh-space+3);
                sc->addItem(t);
            }

        }

    }

    sc->addEllipse(x2-4,y2-4,8,8,QPen(QColor(139,119,101),2),QBrush(QColor(205,175,149)));


    //подписи графиков

    for(int i=1;i<corr_table[0].size();i++)
    {
        x1=(corr_table[1][0]-1)*kx;
        y1 = sizeh + shift_y - space - (float)(corr_table[1][i]*ky);
        x2 = space + (corr_table[2][0]-1)*kx;
        y2 = sizeh + shift_y - space - (float)(corr_table[2][i]*ky);
        float xs,ys;
        xs=(x1+x2)/2;
        ys=(y1+y2)/2;

        QGraphicsTextItem *t;

        switch(i)
        {
        case 1:
            t=new QGraphicsTextItem("Ic(x)min");
            t->setPos(xs-15,ys-55);
            t->setRotation(23);
            break;
        case 2:
            t=new QGraphicsTextItem("Ic(x)max");
            t->setPos(xs+11,ys-70);
            t->setRotation(52);
            break;
        case 3:
            t=new QGraphicsTextItem("Ic(x)sr.");
            t->setPos(xs+11,ys-50);
            t->setRotation(34);
            break;
        }


        t->setDefaultTextColor(QColor(139,134,130));
        t->setFont(QFont(t->font().family(),15,11,true));

        sc->addItem(t);

    }

    QGraphicsTextItem *t;
    t=new QGraphicsTextItem("Ic(x)=" + QString().sprintf("%0.4f",icx));
    t->setDefaultTextColor(QColor(139,134,130));
    t->setFont(QFont(t->font().family(),15,11,true));
    t->setPos(sizew-5*space,sizeh + shift_y - space - (icx*ky)-40);
    sc->addItem(t);

    t=new QGraphicsTextItem("μ");
    t->setDefaultTextColor(QColor(139,134,130));
    t->setFont(QFont(t->font().family(),15,11,true));
    t->setPos(sizew-space-2,sizeh - space-2);
    sc->addItem(t);
    pen.setColor(QColor(139,119,101));
    sc->addLine(sizew-space,sizeh - space,sizew-space-10,sizeh - space-8,pen);
    sc->addLine(sizew-space,sizeh - space,sizew-space-10,sizeh - space+8,pen);
    t=new QGraphicsTextItem("Ic(x)");
    t->setDefaultTextColor(QColor(139,134,130));
    t->setFont(QFont(t->font().family(),15,11,true));
    t->setPos(space+2,space+2);
    sc->addItem(t);
    pen.setColor(QColor(139,119,101));
    sc->addLine(space,space,space-8,space+10,pen);
    sc->addLine(space,space,space+8,space+10,pen);


    for(int i=1;i<corr_table[0].size();i++)
    {
        t=new QGraphicsTextItem(QString().sprintf("%0.3f",corr_table[0][i]));
        t->setDefaultTextColor(QColor(139,134,130));
        t->setFont(QFont(t->font().family(),10,11,true));
        t->setPos(space + (corr_table[0][0]-1)*kx-52,sizeh + shift_y - space - (float)(corr_table[0][i]*ky)+25);
        t->setRotation(-45);
        sc->addItem(t);
    }




    ui->graphicsView->setScene(sc);
    //ui->graphicsView->fitInView(0,0,sizew,sizeh);
    // ui->graphicsView->scale(10,-1);
}

void MainWindow::block_all(bool fl)
{
    ui->comboBox->setEnabled(fl);
    ui->corr_table_text->setEnabled(fl);
    ui->find_mu_progress->setEnabled(fl);
    ui->graphicsView->setEnabled(fl);
    ui->icx_label->setEnabled(fl);
    ui->icx_label_2->setEnabled(fl);
    ui->icx_label_3->setEnabled(fl);
    ui->label->setEnabled(fl);
    ui->label_2->setEnabled(fl);
    ui->label_3->setEnabled(fl);
    ui->label_5->setEnabled(fl);
    ui->label_6->setEnabled(fl);
    ui->label_7->setEnabled(fl);
    ui->many_icx_label->setEnabled(fl);
    ui->mu_label->setEnabled(fl);
    ui->mu_length->setEnabled(fl);
    ui->pushButton_2->setEnabled(fl);
    ui->pushButton_3->setEnabled(fl);
    ui->pushButton_4->setEnabled(fl);
    ui->spinBox->setEnabled(fl);
    ui->find_mu_button->setEnabled(fl);

}

void MainWindow::writeSettings()
{
    QSettings set("crypto_analyzer_polyalphabetic","spliter_sizes_main_form");

    set.setValue("spliter", ui->splitter->saveState());
    set.setValue("spliter2", ui->splitter_2->saveState());
}
void MainWindow::closeEvent(QCloseEvent *e)
{
    /*QMessageBox msgBox;
     //msgBox.setInformativeText("Сохранить персональные настройки?");

    msgBox.setWindowTitle("Анализатор шифра полиалфавитной подстановки");

     msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
     msgBox.setDefaultButton(QMessageBox::Save);

     msgBox.button(QMessageBox::Save)->setText("Сохранить");
     msgBox.button(QMessageBox::Discard)->setText("Не сохранять");
     msgBox.button(QMessageBox::Cancel)->setText("Отмена");

     msgBox.setText("Сохранить персональные настройки(положение объектов внутри формы)?");

     int ret = msgBox.exec();

     if(ret==QMessageBox::Save)
        writeSettings();

     if(ret==QMessageBox::Cancel)
        e->ignore();*/

    writeSettings();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_T:
        if(event->modifiers() & Qt::CTRL)
        {
            ui->open_text->setPlainText("ЦЬЦЬИУЯ БОЫАОУШ ЛЙЦ ГЧЙДРЫУШ Х ЧЦХОШАТС, ЕЪЬЫЬЫНШВЦЧ ЪСУЩШУ: НЯУАОФОЧЦ М СЩЧЬНУСБХЦЦ РСТ ПЮЫАЗ ЬЦМСМОЦОХБ ГКПЦМИЕИПЩЗЫТЦИТМ, ЬЬТЖЖКФРЫМКВ, НЦМШМС ЕШЮЖРТС ЕЪЦМЩММЮЬКЬН ЕУБПУ Х ЭФНЖШТР. \"ЯРУИЪЦ, МЮДЧ, - ЩШРЩЮМР ЫЦБ ШУ ФЕЭ ПСЩ ЭФУКПЯЬЬФ, - Ш ИППК УХЧТ ЬОЦОСЧ; ЩЪУГЫТ ЖЙОУЭ, УХРЮ ЬЙ РСТ ФДНБМЦЬБ. ГШ АЖ ЫЙЧЫЪЭРЩМЗ. ИППК ЬИТД БАЩ-ЫМЖЙОК ТОРЕГТА, ЩТСУ НЬЬ-ШЦЕШЪЖ АРПЙ ФДХММЦЦЦХ - ЬЙ УЪ ЛЕГЙА, Л ЭФУНПУ Ю АЙЖХ ОЮКЫА, ФЖШЕРУ ЦКЧП ЫУЭТЬЫЦ, АЖ БК Н ЩХНПУЦБ ГК ЫРСТ ТЫ ЯЬВУЬБ. ЗЦЬЭЮМ, ЬИШП УХ ЦКЧЙ ЫР РЯЭЫХ ЪЩТСАЯ РЦНЬУНЗПД. Ю АЙЖХ Т АРЭЙХТ ЭФР ЕЦУ-ИШ ЫЛЕМТЦИА ЬЩМЭАШЪ МЬНПД ФЮУЕДЧТ ФЮЛЯОН. ЖТЯЮЫТП Й ЬУМН СК ЗЬЮЩС, Д ФДОЕЛЯ М ЗДМЯР ЯПЕЧ, ХЦШЦГ ТЫ МЦПЫД; ЧС ЭФ ОЬСДЫВКЬН ЛЕ ВШТШЙР УЗМУДУСНЫЦ, ХЛ АЙС, НЬЬ МКЙЧ ГК ЭРЮЖАЫ НЩЛХД. ЦВШАЫЦ, ОЕА ЪОТ ЭТФЦОУГК Ж ЕГНЩУЧХПЮУ ЮЩТ. ЕКЖПСУЯА; ЧЫЛН ЮФ СЕНТЫЛУЦ ЦШПА ЭНСШИЖ; БС Н ЖНЬЭ Б ЭУЕД ЮЧЬФ ЮДМ ГК ЖРУ ЭКЩШЩЗЯОУЯ ЩЩЛАТП, ОХНЪО Х РДЫШЩЪ... ТТД СОЧОСЬЮМЬ, ЧЬКТД ЩБЬАМЧТЫН ЪЦХЕИЖ ЪЩТСАЫ ФОЫАМТАТ, ЭЩЮЦХЫЬЦХЦ ЛЕ ЪПЫЗСМ. ЙЦ МУПК СЕ УЬЬЧ СЧЖЮЬЯК, О СК ЖКХНУФЧСМОРАХД ИКЩЛЫЦ. ЧЫЪЭУ. ЬЕЙЙЦЙНОН ЗЗЙШЮМ ФЕЧШАЮ, ПФУЗЖ ЗРСТРТЫАНЬ - УШЗЬК УГ СЕЧТЮЛМЦ ЙЖЭСУУ ИКГЖСУ. АЖУЫ ША ЭУЕД ГП БФТЙЧ\". ЕЪЬЯУХЦДЪ ПЖЩ ТЧНКЯЭЦ УХЦМ. ЦШЬЗЙЦ ЯЬЭУПУЗЖ, АЩЕСУ, ГКЖРЪЧ ЪЙОЬСЫМПЙ ФБЭЫЧЧТ, ГУОЬПБГЭАЗ - ЯПУШШЪ, ХЬЙ-ИЪП ЭЩШДМЦЬК ЬРТГ ВШЩЩТТЦИЖ. ЫЩ ЭФН ШЫУЧ АТС ДЧ ЪЩС ЖМХЬК ШОИ ЦДЛЬЙ РПЕЗЬК. НЮЙСЫЧОЧЦ ТТ ВШС ЪЬЛЕЧЕАЗ РХК, ЕЪЦШНЖЭЮЫК ТО ОНЗЬК, У ЬЦХСМОЦЯГ УИ ЧУР ЫЙ НГКЕР, ШДП ДЬ ЭЫУОХЦЫЫЩСТ ФЖПЮНОСТДНЬ ЬЫД. ЗАЭЯ РСТ ХЦСРУРДРЗЙ ХЛЪЙЧГШ. УДУ СК ЕШЫУЪДР ДЧ РЬУН ИБЭПУЫЯ ХЦЮОИЩГ, ТД ЭФР БЖРЫФОЦЯГ ЖСЫАЫЬН, ЭЮЪЬХЬН ПЮЫАЗМ ЗЗЮОО, ЩЯЦЕГКРЦЦЖЕБЫН ЪУФКЪ ЩЬЫАФКИКЪУ АМЫЮКЫЛ, РТЦЛТЗЛЩХД КХОЧОСЙМКЪУ. УЭК ЕШАРЪСКШВЦФ ЬЕРЮФ, ЬМЩЙПЦИЗУЧ ХЧЦЪЙР ШДХИТЫЖ, ЫЙ ЗЫЫК ЬЬЬКБ ЩЮРТ СНВ; ЧЬ ЩЫ ЧЛЫ ЩЮЩХФКШКЩ Н ЫМЪ АШУ-ВАТ, ЪДЬН НЫЧЧЖПЫШЬ СК ЗШСЦОЬЕБЫН Ь ЭФУКПЯЬЬФУВ, БАЩПЯ ЦИКЮУЫСАЫ ЦОЬАЙХЦ ЬОХ ЫЙЙДЫНООЙСД ЭЖЦЦ ТЧ ГКЯ; РЪЧ ПЦСОЦЬХБ ЪКФР, ЕЦУ ЪПРКАСЕЪАОЭЙН ЗЫФ ШЩУ Ж ЬЫЦ ХШОЫНИПЩЗЫТ НЛ ШЭРЮЙЙЮХ, ЕЭЬ УУЪЪОСОСНЫ ЩЮУЮТЙЫ ФОХ-АТ ЦЪПЩЛЩТЦТ ЬУЪУФБ ХЪЕР, ФМЗЫП, ПЦЦКК; ЗХЬНЬР, УГ ОБЧОП З УЬЬЧ ЯПШНКУ ЭОО, ПЦФ ТЮЪДКИ ЦЬЦЬИУЗЬК, ЮФЙ ФДЫАУСЬЕХ ФЬР-ЕЦУ Ю ББНЯЦЗЙИЗЛН БЧД М СЩЮИУВ МЫЮАФКГЧУЧ ЯТМГКЫУЦ. МТДНТЛ ЯЦЕГШРУЩТЦТ ПЪЮ ТТЦЦОЫЩ, ШТИЪК ЬШ РМЙЫХ, ШЛШ ЛЕЫСФУЧ КНШШЭУЯЙЫ, КЪОШДЧМ ЮХЦ ШУРКМ, ТЫЩСИЕ ЪКФР РТЗЗП ЫР ФМЗДЩЫЬУЪ ФД ЩЮУХЖЕГЖМ, ЩТСУЯ ЬЬЦКОУ ЕЪЦНЙЫТДУ ХЛЪДЭАШЧ, МЬНПДЫАЗМ ОНЗЬЦ У НФПДЫАЗМ ОХЦЫЬХ ЭФУЮСРЩТМР ШЫУЩПЭНЯ ВБЧ Ц ХПЦЩЩУРДР ЗППР РРНЩ ОУШУКТСУ ШЛЭМЧЦХ");
            on_find_mu_button_clicked();
        }
        break;


    default:
        QWidget::keyPressEvent(event);
    }

}

void MainWindow::on_action_triggered()
{
    close();
}

void MainWindow::on_action_2_triggered()
{
    QMessageBox::about(this,"О программе", "Данный модуль проргаммы предназначен для определения периода шифрования текста используя тест Касиски и тест Фридмана.");

}

void MainWindow::on_action_3_triggered()
{
    QMessageBox::about(this,"Автор", "Выполнил студент группы КИ10-06 Герасимчук Михаил 2014г.");
}

void MainWindow::on_open_text_textChanged()
{
    if(ui->open_text->toPlainText().length()==0)
    {
        ui->label->setEnabled(false);
        ui->mu_length->setEnabled(false);
        ui->find_mu_button->setEnabled(false);
    }
    else
    {
        ui->label->setEnabled(true);
        ui->mu_length->setEnabled(true);
        ui->find_mu_button->setEnabled(true);
    }
}
