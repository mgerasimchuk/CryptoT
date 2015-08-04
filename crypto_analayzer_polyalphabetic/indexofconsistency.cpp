#include "indexofconsistency.h"
#include "ui_indexofconsistency.h"
#include <qlabel.h>

IndexOfConsistency::IndexOfConsistency(QWidget *parent, QString fopen_text, int fmu, QString falphabet, QMdiArea *pmdi) :
    QMainWindow(parent),
    ui(new Ui::IndexOfConsistency),
    open_text(fopen_text),
    par(parent),
    mdi(pmdi),
    mu(fmu),
    alphabet(falphabet)
{
    ui->setupUi(this);

    llabel=new QLabel("Вычисление сдвигов и вариантов ключевого слова...", this);
    llabel->setWordWrap(true);
    llabel->setStyleSheet("background-color: rgba(189, 189, 189, 100);"
                          "font: 25pt \"MS Shell Dlg 2\";");
    llabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    llabel->hide();

    ui->plainTextEdit->setPlainText(open_text);
    ui->lineEdit->setMaxLength(mu);



    for(int i=0;i<alphabet.length();i++)
    {
        get_index_of_simbol[QString(alphabet[i])]=i;
        get_simbol_of_index[i]=QString(alphabet[i]);
    }



    QSettings set("crypto_analyzer_polyalphabetic","spliter_sizes_for_index");

    ui->splitter->restoreState(set.value("spliter", QByteArray()).toByteArray());
    ui->splitter_2->restoreState(set.value("spliter2", QByteArray()).toByteArray());
    ui->splitter_3->restoreState(set.value("spliter3", QByteArray()).toByteArray());
    ui->splitter_4->restoreState(set.value("spliter4", QByteArray()).toByteArray());

    ui->spinBox->setMaximum(mu);
    ui->progressBar->setVisible(false);
    ui->pushButton_4->setEnabled(false);



    ui->label_2->setEnabled(false);
    ui->label_3->setEnabled(false);
    ui->label_4->setEnabled(false);
    ui->label_7->setEnabled(false);
    ui->tableView->setEnabled(false);
    ui->tableView_2->setEnabled(false);
    ui->listView->setEnabled(false);
    ui->listView_2->setEnabled(false);
}

IndexOfConsistency::~IndexOfConsistency()
{
    delete ui;
}




void IndexOfConsistency::calc_all_micxy()
{
    only_text="";
    mu_group.clear();
    icx_mu.clear();
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

    load_shift();

    for(int i=0;i<mu;i++)
    {
        for(int j=0;j<mu;j++)
        {
            double f=calc_icx(mu_group[i],mu_group[j]);
            int sh=get_shift_of_icx(f);



            icx_mu[QPair<int,int>(i,j)]=QPair<double,int>(f,sh);


            //ui->plainTextEdit_2->setPlainText(ui->plainTextEdit_2->toPlainText()+"\r\n"+QString().sprintf("shift(%d,%d)=%d",i,j,sh));
        }
    }

}

double IndexOfConsistency::calc_icx(QString a, QString b)
{
    QHash<QCharRef,int> fa;
    QHash<QCharRef,int> fb;

    for(int i=0;i<a.length();i++)
        fa[a[i]]++;

    for(int i=0;i<b.length();i++)
        fb[b[i]]++;

/*
    ui->plainTextEdit_2->setPlainText(ui->plainTextEdit_2->toPlainText()+ a + "\r\n");
    for(int i=0;i<alphabet.size();i++)
    {
        ui->plainTextEdit_2->setPlainText(ui->plainTextEdit_2->toPlainText()+alphabet[i] + "=" + QString().sprintf("%d",fa[alphabet[i]]) + ", ");
    }

    ui->plainTextEdit_2->setPlainText(ui->plainTextEdit_2->toPlainText()+  + "\r\n" + b + "\r\n");
    for(int i=0;i<alphabet.size();i++)
    {
        ui->plainTextEdit_2->setPlainText(ui->plainTextEdit_2->toPlainText()+alphabet[i] + "=" + QString().sprintf("%d",fb[alphabet[i]]) + ", ");
    }

*/

    int sum=0;
    for(int i=0;i<alphabet.length();i++)
    {
        sum+=fa[alphabet[i]]*fb[alphabet[i]];
    }

    return (double)sum/(a.length()*b.length());
}

void IndexOfConsistency::load_shift()
{
    QTextStream in;
    QFile *in_file;
    in_file=new QFile;
    in_file->setFileName("shift.txt");
    in_file->open(QIODevice::ReadOnly);
    in.setDevice(in_file);

    int n;
    in>>n;

    for(int i=0;i<n;i++)
    {
        QPair<int,double> tt;
        in>>tt.first>>tt.second;
        shift.push_back(tt);
    }


    in_file->close();
}

int IndexOfConsistency::get_shift_of_icx(double icx)
{

    double min=qAbs(icx-shift[0].second);
    int m=shift[0].first;

    for(int i=0;i<shift.size();i++)
    {
        if(qAbs(icx-shift[i].second)<min)
        {
            min=qAbs(icx-shift[i].second);
            m=shift[i].first;
        }
    }

    return m;
}

void IndexOfConsistency::out_in_table()
{
    shift_model_all = new QStandardItemModel(mu_group.size(),5,this);

    int itemw=ui->tableView->size().width()/5;
    int itemh=25;
    QHeaderView *hh,*vh;

    hh=ui->tableView->horizontalHeader();
    hh->setDefaultSectionSize(itemw);
    ui->tableView->setHorizontalHeader(hh);

    shift_model_all->setHorizontalHeaderItem(0,new QStandardItem("μ"));
    shift_model_all->setHorizontalHeaderItem(1,new QStandardItem("Символы группы"));
    shift_model_all->setHorizontalHeaderItem(2,new QStandardItem("Кол-во символов"));
    shift_model_all->setHorizontalHeaderItem(3,new QStandardItem("MIc(X0,Xμ)"));
    shift_model_all->setHorizontalHeaderItem(4,new QStandardItem("Сдвиг"));

    for(int i=0;i<mu_group.size();i++)
    {QStandardItem *h0,*h1,*h2,*h3,*h4;
        h0=new QStandardItem(QString().sprintf("%d",i));
        h0->setTextAlignment(Qt::AlignHCenter);

        shift_model_all->setItem(i,0,h0);

        h1=new QStandardItem(mu_group[i]);
        //h1->setToolTip(mu_group[i]);
        shift_model_all->setItem(i,1,h1);

        h2=new QStandardItem(QString().sprintf("%d",mu_group[i].length()));
        h2->setTextAlignment(Qt::AlignHCenter);
        shift_model_all->setItem(i,2,h2);

        if(i!=0)
        {

           if(icx_mu[QPair<int,int>(0,i)].first!=0)
           {
               h3=new QStandardItem(QString().sprintf("MIc(X%d,X%d)=%f",0,i,icx_mu[QPair<int,int>(0,i)].first));
               h3->setTextAlignment(Qt::AlignHCenter);
           }
           else
           {
               h3=new QStandardItem("-");
               h3->setTextAlignment(Qt::AlignHCenter);
           }


           h4=new QStandardItem(QString().sprintf("%d",-1*icx_mu[QPair<int,int>(0,i)].second));
           h4->setTextAlignment(Qt::AlignHCenter);



           shift_model_all->setItem(i,3,h3);
           shift_model_all->setItem(i,4,h4);

        }
        else
        {
            h3=new QStandardItem("-");
            h3->setTextAlignment(Qt::AlignHCenter);

            h4=new QStandardItem("-");
            h4->setTextAlignment(Qt::AlignHCenter);

            shift_model_all->setItem(i,3,h3);
            shift_model_all->setItem(i,4,h4);
        }
    }


    ui->tableView->setModel(shift_model_all);
    ui->tableView->resizeColumnToContents(0);
    ui->tableView->resizeColumnToContents(2);
    ui->tableView->resizeColumnToContents(3);
     ui->tableView->resizeColumnToContents(4);

    int size=ui->tableView->width() -
            (ui->tableView->columnWidth(0)+
            ui->tableView->columnWidth(2)+
            ui->tableView->columnWidth(3)+
             ui->tableView->columnWidth(4));
    ui->tableView->horizontalHeader()->resizeSection(1,size-25);
}

void IndexOfConsistency::on_pushButton_clicked()
{


    open_text=ui->plainTextEdit->toPlainText();


    QString res="",res2="";
    QVector<QString> v;
    for(int i=0;i<open_text.length();i++)
    {
        if(alphabet.indexOf(open_text[i],0)!=-1)
        {
            res+=open_text[i];
            res2+=open_text[i];
            if(res.length()%mu==0)
            {
                v.push_back(res2);
                res2="";
            }
        }
    }
    if(res2.length()!=mu)
        v.push_back(res2);



    calc_all_micxy();
    out_in_table();
    create_all_key();
    out_to_list1();
    out_to_list2();
    create_group_info();

    llabel->hide();
    ui->label_2->setEnabled(true);
    ui->label_3->setEnabled(true);
    ui->label_4->setEnabled(true);
    ui->label_7->setEnabled(true);
    ui->tableView->setEnabled(true);
    ui->tableView_2->setEnabled(true);
    ui->listView->setEnabled(true);

    if(ui->listView_2->model()->index(0,0).data().toString()!="Нет соответствий словарю!")
        ui->listView_2->setEnabled(true);
    else
        ui->listView_2->setEnabled(false);


}


void IndexOfConsistency::load_dictionary()
{
    QTextStream in;
    QFile *in_file;
    in_file=new QFile;
    in_file->setFileName("dictionary_big.txt");
    in_file->open(QIODevice::ReadOnly);
    in.setDevice(in_file);


    QString s;
    dictionary.clear();
    while(!in.atEnd())
    {
        in>>s;
        s=s.toUpper();
        s=s.trimmed();
        dictionary.push_back(s);
    }


    in_file->close();
}

void IndexOfConsistency::out_to_list1()
{

    QStandardItemModel *model = new QStandardItemModel (this);
    QList<QStandardItem *> items;

    for(int i=0;i<all_keys.size();i++)
    {
        QStandardItem *t = new QStandardItem (all_keys[i]);
        items.append(t);

    }


    model->appendColumn(items);
    ui->listView->setModel(model);
}


void IndexOfConsistency::out_to_list2()
{
    ui->listView_2->setEnabled(true);
    load_dictionary();
    QStandardItemModel *model = new QStandardItemModel (this);
    QList<QStandardItem *> items;


    QString tt;
    for(int i=0;i<mu;i++)
        tt.push_back('-');
    for(int i=0;i<ui->spinBox->value();i++)
        tt[i]='*';

    QVector<QString> masks;
    masks.push_back(tt);
    while(std::next_permutation(tt.begin(),tt.end()))
    {
        masks.push_back(tt);
    }

    int maxi=all_keys.size()+1;//*masks.size()*dictionary.size();
    //ui->progressBar->setValue(0);
    //ui->progressBar->setVisible(true);




    int it=0;
    //ui->progressBar->setMaximum(maxi);

    for(int i=0;i<all_keys.size();i++)
    {
        //ui->progressBar->setValue(it++);

        for(int nki=0;nki<masks.size();nki++)
        {
            QString now_key=all_keys[i];

            for(int l=0;l<masks[nki].length();l++)
                if(masks[nki][l]=='*')
                    now_key[l]='*';

            //qDebug()<<now_key;

            for(int j=0;j<dictionary.size();j++)
            {
                bool f=false;
                int count;
                if(dictionary[j].length()==mu && mu==ui->spinBox->value())
                {
                    QStandardItem *t = new QStandardItem (dictionary[j]);
                    items.append(t);
                    continue;
                }

                if(dictionary[j].length()==mu)
                {
                    count=0;
                    for(int k=0;k<dictionary[j].length();k++)
                        if(now_key[k]=='*' || dictionary[j][k]==now_key[k])
                            count++;
                    f=true;
                    if(count==mu)
                    {
                        for(int jj=0;jj<items.size();jj++)
                            if(dictionary[j]==items[jj]->text())
                                f=false;
                        if(f)
                        {
                            QStandardItem *t = new QStandardItem (dictionary[j]);
                            items.append(t);
                        }
                    }
                }
            }
        }
    }



  /*
    for(int i=0;i<all_keys.size();i++)
    {
        bool f=false;
        int count;

        int nn;

        if(ui->checkBox->isChecked())
            nn=all_keys[i].length();
        else
            nn=1;

        for(int iii=0;iii<all_keys[i].length();iii++)
        {
            QString now_key=all_keys[i];

            if(ui->checkBox->isChecked())
                now_key[iii]='*';


            for(int j=0;j<dictionary.size();j++)
            {

                if(dictionary[j].length()==mu)
                {
                    count=0;
                    for(int k=0;k<dictionary[j].length();k++)
                        if(now_key[k]=='*' || dictionary[j][k]==now_key[k])
                        {
                            count++;

                        }

                    f=true;
                    if(count==mu)
                    {
                        for(int jj=0;jj<items.size();jj++)
                        {
                            if(dictionary[j]==items[jj]->text())
                                f=false;

                        }

                        if(f)
                        {
                            QStandardItem *t = new QStandardItem (dictionary[j]);
                            items.append(t);
                        }
                    }
                }
            }
        }


    }
*/
    if(items.empty())
    {
        QStandardItem *t = new QStandardItem ("Нет соответствий словарю!");
        items.append(t);
        ui->listView_2->setEnabled(false);
    }
    model->appendColumn(items);
    ui->listView_2->setModel(model);
    //ui->progressBar->setValue(it++);
    //ui->progressBar->setVisible(false);

}

void IndexOfConsistency::create_all_key()
{
    all_keys.clear();

    for(int i=0;i<alphabet.length();i++)
    {
        QString key="";
        key+=alphabet[i];
        for(int j=1;j<mu_group.size();j++)
        {
            int sdvig=i-icx_mu[QPair<int,int>(0,j)].second;

            if(sdvig>=alphabet.length())
                sdvig-=alphabet.length();

            if(sdvig<0)
                key[j]=alphabet[alphabet.length()+sdvig];
            else
                key[j]=alphabet[sdvig];
        }
        all_keys.push_back(key);
    }
}

void IndexOfConsistency::create_group_info()
{
    group_info.clear();

    for(int i=0;i<mu_group.size();i++)
    {
        QHash<QCharRef,int> temp;
        for(int j=0;j<mu_group[i].length();j++)
            if(alphabet.indexOf(mu_group[i][j],0)!=-1)
                temp[mu_group[i][j]]++;

        group_info.push_back(temp);
    }


    out_group_info(0);


}

void IndexOfConsistency::out_group_info(int index)
{
    group_info_model = new QStandardItemModel(1,alphabet.length(),this);

    int itemw=ui->tableView_2->size().width()/alphabet.length();
    QHeaderView *hh;

    hh=ui->tableView_2->horizontalHeader();
    hh->setDefaultSectionSize(itemw);
    ui->tableView_2->setHorizontalHeader(hh);

    for(int i=0;i<alphabet.length();i++)
        group_info_model->setHorizontalHeaderItem(i,new  QStandardItem(QString(alphabet[i])));

    for(int i=0;i<alphabet.length();i++)
    {
        QStandardItem *h0;
        h0=new QStandardItem(QString().sprintf("%d",group_info[index][alphabet[i]]));
        h0->setTextAlignment(Qt::AlignHCenter);

        group_info_model->setItem(0,i,h0);
    }

    ui->tableView_2->setModel(group_info_model);
}

void IndexOfConsistency::get_shift(QString a, QString b)
{




}

QString IndexOfConsistency::do_shift(QString str,int sh)
{
    int ind;

    int sign=1;
    if(sh<0)
    {
        sign=-1;
        sh=sh*(-1);
    }

    for(int j=0;j<sh;j++)
    for(int i=0;i<str.length();i++)
    {
        ind=get_index_of_simbol[QString(str[i])]+sign;
        if(ind>=get_index_of_simbol.size())
            ind=0;

        if(ind<0)
            ind=get_index_of_simbol.size()-1;

        str[i]=get_simbol_of_index[ind][0];
    }

    return str;
}

void IndexOfConsistency::on_pushButton_2_clicked()
{
    QString res;
    QHash<QCharRef,int> lang;
    QString key=ui->lineEdit->text().toUpper();

    if(key.length()!=mu)
    {
        QMessageBox::critical(this,"Ошибка ключа!", "Ключ задан некорректно!");
        return;

    }

    QString temp=key;
    while(temp.length()<open_text.length())
        temp+=key;

    key.clear();
    for(int i=0,j=0;i<open_text.length();i++)
    {
        if(alphabet.indexOf(open_text[i],0)!=-1)
        {
            key+=temp[j];
            j++;
        }
        else
            key+=open_text[i];
    }

    for(int i=0;i<alphabet.length();i++)
        lang[alphabet[i]]=i;

    res.resize(open_text.length());
    for(int i=0;i<open_text.length();i++)
    {
        if(alphabet.indexOf(open_text[i],0)!=-1)
            res[i]=alphabet[(lang[open_text[i]]-lang[key[i]]+lang.size())%lang.size()];
        else
            res[i]=open_text[i];

    }

    ui->plainTextEdit_3->setPlainText(res);

}

void IndexOfConsistency::on_tableView_clicked(const QModelIndex &index)
{
    out_group_info(index.row());
}

void IndexOfConsistency::on_tableView_2_pressed(const QModelIndex &index)
{
    out_group_info(index.row());
}

void IndexOfConsistency::on_tableView_2_activated(const QModelIndex &index)
{
    out_group_info(index.row());
}

void IndexOfConsistency::on_tableView_2_entered(const QModelIndex &index)
{
    out_group_info(index.row());
}

void IndexOfConsistency::on_listView_clicked(const QModelIndex &index)
{
    ui->lineEdit->setText(all_keys[index.row()]);
}

void IndexOfConsistency::on_pushButton_3_clicked()
{


    /*QLabel *l;
    l=new QLabel("ssfddsfsdgsdfgs",this);
    l->setGeometry(0,0,this->size().width(),this->size().height());
    l->setLayout(ui->verticalLayout_6
                 );*/
    QPalette p;
//    p=QPalette::ColorRole;
  //  p.setColor( QPalette::Base, QColor(qRgba(255,255,255,50)));
//    l->setBackgroundRole((QPalette::ColorRole));


    /*
     * open_text=ui->plainTextEdit->toPlainText();


    QString res="",res2="";
    QVector<QString> v;
    for(int i=0;i<open_text.length();i++)
    {
        if(alphabet.indexOf(open_text[i],0)!=-1)
        {
            res+=open_text[i];
            res2+=open_text[i];
            if(res.length()%mu==0)
            {
                v.push_back(res2);
                res2="";
            }
        }
    }
    if(res2.length()!=mu)
        v.push_back(res2);

    ui->plainTextEdit_2->clear();
    for(int i=0;i<v.size();i++)
    {QString out="";
        for(int j=0;j<v[i].length();j++)
        {
            out+=v[i][j]+"\t";

        }
        ui->plainTextEdit_2->setPlainText(ui->plainTextEdit_2->toPlainText()+out+"\r\n");
    }

    //calc_all_micxy();



    only_text="";
    mu_group.clear();
    icx_mu.clear();
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



    for(int i=0;i<mu;i++)
    {
        for(int j=0;j<mu;j++)
        {
            double f=calc_icx(mu_group[i],mu_group[j]);
            int sh=get_shift_of_icx(f);



            icx_mu[QPair<int,int>(i,j)]=QPair<double,int>(f,sh);



            //ui->plainTextEdit_2->setPlainText(ui->plainTextEdit_2->toPlainText()+"\r\n"+QString().sprintf("shift(%d,%d)=%d",i,j,sh));
        }
    }



    for(int i=0;i<mu_group.size();i++)
    {
        int sh=0;

        for(int j=0;j<16;j++)
        {

            double picx=calc_icx(mu_group[0],do_shift(mu_group[i],j));

            if(picx>0.053 && picx<0.07)
            {    sh=j;
            }
        }

        icx_mu[QPair<int,int>(0,i)]=QPair<double,int>(0.0,sh);
    }

    out_in_table();
    //create_all_key();
    //out_to_list1();
    create_group_info();


    */


    open_text=ui->plainTextEdit->toPlainText();


    QString res="",res2="";
    QVector<QString> v;
    for(int i=0;i<open_text.length();i++)
    {
        if(alphabet.indexOf(open_text[i],0)!=-1)
        {
            res+=open_text[i];
            res2+=open_text[i];
            if(res.length()%mu==0)
            {
                v.push_back(res2);
                res2="";
            }
        }
    }
    if(res2.length()!=mu)
        v.push_back(res2);

    /*ui->plainTextEdit_2->clear();
    for(int i=0;i<v.size();i++)
    {QString out="";
        for(int j=0;j<v[i].length();j++)
        {
            out+=v[i][j]+"\t";

        }
        ui->plainTextEdit_2->setPlainText(ui->plainTextEdit_2->toPlainText()+out+"\r\n");
    }*/

    calc_all_micxy();


    QString str;
    for(int ii=0;ii<mu_group[0].length();ii++)
        str+=mu_group[0][ii]+",";

    for(int i=0;i<mu_group.size();i++)
    {
        int sh=0;

        QString temp_group=mu_group[i];


        for(int j=-16;j<=16;j++)
        {
            //temp_group=do_shift(temp_group,1);
            double picx=calc_icx(mu_group[0],do_shift(temp_group,j));

            if(picx>0.051 && picx<0.07)
            {
                sh=j;
            }



        }

        icx_mu[QPair<int,int>(0,i)]=QPair<double,int>(0.0,sh);
    }




    //calc_all_micxy();
    out_in_table();
    create_all_key();
    out_to_list1();
    out_to_list2();

    create_group_info();





    llabel->hide();
   //wait->stop();
    ui->label_2->setEnabled(true);
    ui->label_3->setEnabled(true);
    ui->label_4->setEnabled(true);
    ui->label_7->setEnabled(true);
    ui->tableView->setEnabled(true);
    ui->tableView_2->setEnabled(true);
    ui->listView->setEnabled(true);

    if(ui->listView_2->model()->index(0,0).data().toString()!="Нет соответствий словарю!")
        ui->listView_2->setEnabled(true);
    else
        ui->listView_2->setEnabled(false);

}

void IndexOfConsistency::on_pushButton_4_clicked()
{

    PolyAlphabetic *w=new PolyAlphabetic(this,ui->plainTextEdit_3->toPlainText(),mu,true);
    w->setWindowTitle("Анализатор шифра полиалфавитной подстановки");
    QMdiSubWindow *sww=mdi->addSubWindow(w);
    sww->setWindowIcon(QIcon(":/img/polyw.png"));

    this->showMinimized();
    w->show();

}

void IndexOfConsistency::writeSettings()
{
    QSettings set("crypto_analyzer_polyalphabetic","spliter_sizes_for_index");

    set.setValue("spliter", ui->splitter->saveState());
    set.setValue("spliter2", ui->splitter_2->saveState());
    set.setValue("spliter3", ui->splitter_3->saveState());
    set.setValue("spliter4", ui->splitter_4->saveState());
}


void IndexOfConsistency::closeEvent(QCloseEvent *e)
{
    QMessageBox msgBox;
     //msgBox.setInformativeText("Сохранить персональные настройки?");

    msgBox.setWindowTitle("Анализатор шифра полиалфавитной подстановки");

     msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
     msgBox.setDefaultButton(QMessageBox::Save);

     msgBox.button(QMessageBox::Save)->setText("Сохранить");
     msgBox.button(QMessageBox::Discard)->setText("Не сохранять");
     msgBox.button(QMessageBox::Cancel)->setText("Отмена");




     msgBox.setText("Сохранить персональные настройки(положение объектов внутри формы)?");

     /*int ret = msgBox.exec();

     if(ret==QMessageBox::Save)
        writeSettings();

     if(ret==QMessageBox::Cancel)
        e->ignore();*/

     par->showNormal();

}

void IndexOfConsistency::on_listView_2_clicked(const QModelIndex &index)
{
    ui->lineEdit->setText(ui->listView_2->model()->data(index).toString());
}

void IndexOfConsistency::on_pushButton_3_pressed()
{
    llabel->setGeometry(0,0,this->width(),this->height());
    llabel->show();
}

void IndexOfConsistency::on_pushButton_3_released()
{
    llabel->hide();
}

void IndexOfConsistency::on_plainTextEdit_3_textChanged()
{
    if(ui->plainTextEdit_3->toPlainText().length()==0)
    {
        ui->pushButton_4->setEnabled(false);

    }
    else
    {
        ui->pushButton_4->setEnabled(true);
    }
}

void IndexOfConsistency::on_pushButton_pressed()
{
    llabel->setGeometry(0,0,this->width(),this->height());
    llabel->show();
}

void IndexOfConsistency::on_pushButton_released()
{
    llabel->hide();
}
