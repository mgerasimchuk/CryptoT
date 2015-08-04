/*
порог перенести из меню на форму
сделать дополнительную таблицу где проявляется только по одной букве по нажатию кнопки(или просто апдейтитьтекущую таблицу и метод добавления)

анализ биграм вроде нормальный ни че менять не надо

*/

#include "monoalphabetic.h"
#include "ui_monoalphabetic.h"


MonoAlphabetic::MonoAlphabetic(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MonoAlphabetic)
{
    ui->setupUi(this);
    dictionary_file_name="dictionary.txt";
    selection_on=true;
    char_prob_load();
    cipher_text_alphabet_load();

    load_bigram();
    load_dictionary();
    ui->progressBar->setVisible(false);
    char_count=0;
    limit=6;
    ui->spinBox->setValue(limit);
    last_spin=ui->spinBox->value();

    ui->plainTextEdit_2->setEnabled(false);
    ui->plainTextEdit_3->setEnabled(false);
    ui->graphicsView->setEnabled(false);
    ui->tableView->setEnabled(false);
    ui->tableView_2->setEnabled(false);
    ui->groupBox->setEnabled(false);
    ui->groupBox_2->setEnabled(false);
     ui->pushButton_6->setEnabled(false);

    QSettings set("crypto_analyzer","spliter_sizes");

    ui->splitter->restoreState(set.value("spliter", QByteArray()).toByteArray());
    ui->splitter_2->restoreState(set.value("spliter2", QByteArray()).toByteArray());
    ui->splitter_3->restoreState(set.value("spliter3", QByteArray()).toByteArray());
    ui->splitter_4->restoreState(set.value("spliter4", QByteArray()).toByteArray());


    text_edit_sovet=grid_sovet=false;
    ui->label_9->setVisible(false);
    on_plainTextEdit_textChanged();
}

void MonoAlphabetic::char_prob_load()
{
    QTextStream in;
    QFile *in_file;
    in_file=new QFile;
    in_file->setFileName("char_probability.txt");
    in_file->open(QIODevice::ReadOnly);
    in.setDevice(in_file);

    QString c;
    float p;
    int n,m;
    in>>n>>m;
    in.readLine(255);
    for(int i=0;i<n*m && !in.atEnd();i++)
    {

        c=in.read(2).trimmed();
        if((i+1)%m==0)
            p=in.read(7).toFloat();
        else
            p=in.read(6).toFloat();

        char_prob[c]=p;
    }

    in_file->close();
}
void MonoAlphabetic::cipher_text_alphabet_load()
{
    QTextStream in;
    QFile *in_file;
    in_file=new QFile;
    in_file->setFileName("cipher_text_alphabet.txt");
    in_file->open(QIODevice::ReadOnly);
    in.setDevice(in_file);

    alphabet=in.readLine(2048);

    in_file->close();
}


void MonoAlphabetic::static_analyze()
{
    QString s=ui->plainTextEdit->toPlainText();
    QString temp;
    for(int i=0;i<s.length()-1;)
    {
        if(alphabet.indexOf(s[i],0)!=-1 && alphabet.indexOf(s[i+1],0)!=-1)
        {
            temp=s[i];
            temp+=s[i+1];
            char_count_text[temp]++;

            i+=2;
            if(i>=s.length())
                break;
        }
        else
        {
            i++;
        }
    }


    QMapIterator<QString, int> it(char_count_text);


    int sum=0;
    while(it.hasNext())
    {
        sum+=it.next().value();
    }

   // float k=(float)char_count_text.size()/sum;

    it.toFront();

    while(it.hasNext())
    {
        it.next();
        char_prob_text[it.key()]=(float)it.value()/sum;
    }

    create_busy();
}

void MonoAlphabetic::create_busy()
{
    QMapIterator<QString,float> it(char_prob_text);

    while(it.hasNext())
        busy.push_back(QPair<QString,QString>("",it.next().key()));

    for(int i=0;i<busy.size();i++)
        for(int j=0;j<busy.size();j++)
            if(char_prob_text[busy[i].second]>char_prob_text[busy[j].second])
                qSwap(busy[i],busy[j]);

    bool fl;
    float best;
    QMapIterator<QString,float> it2(char_prob);
    for(int i=0;i<busy.size();i++)
    {
        it2.toFront();
        best=0;
        while(it2.hasNext())
        {
            it2.next();
            fl=true;
            for(int j=0;j<busy.size();j++)
                if(it2.key()==busy[j].first)
                {
                    fl=false;
                    break;
                }

            if(fl && it2.value()>best)
            {
                best=it2.value();
                busy[i].first=it2.key();
            }
        }
    }
}


void MonoAlphabetic::out_busy_to_table()
{
    model = new QStandardItemModel(1,char_prob.size(),this);
    connect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(UpdateData(QModelIndex,QModelIndex)));
    QMapIterator<QString, float> it(char_prob);
    int i=0;

    int itemw=ui->tableView->size().width()/bigram_alphabet.length();
    int itemh=25;
    QHeaderView *hv;
    hv=ui->tableView->horizontalHeader();
    hv->setDefaultSectionSize(itemw);
    ui->tableView->setHorizontalHeader(hv);

    while(it.hasNext())
    {
        QStandardItem *item;
        item = new QStandardItem(it.next().key());
        item->setSizeHint(QSize(itemw,itemh));
        model->setHorizontalHeaderItem(i++, item);
    }



    it.toFront();
    i=0;
    while(it.hasNext())
    {
        it.next();
        for(int j=0;j<busy.size();j++)
        {
            if(it.key()==busy[j].first)
            {
                model->setItem(0,i,new QStandardItem(busy[j].second));
                break;
            }
            else
                model->setItem(0,i,new QStandardItem(""));
        }
        i++;
    }

    ui->tableView->setModel(model);
}


MonoAlphabetic::~MonoAlphabetic()
{
    delete ui;
}

void MonoAlphabetic::on_pushButton_clicked()
{

    QString text=ui->plainTextEdit->toPlainText().toUpper();

    for(int i=0;i<text.length();i++)
    {
        if(text[i]=='\n')
            text[i]=' ';
    }

    ui->plainTextEdit->setPlainText(text);

    char_count_text.clear();
    char_prob_text.clear();
    history.clear();
    busy.clear();
    extraSelections.clear();
    cc=0;
    ui->lineEdit->clear();


    static_analyze();
    out_busy_to_table();
    show_graphics();
    out_info();

    //select_zero_bigram();
    create_bigram_table();

    char_count=1;
    out_simbols();
    first_mask_search();


    ui->plainTextEdit_2->setEnabled(true);
    ui->plainTextEdit_3->setEnabled(true);
    ui->graphicsView->setEnabled(true);
    ui->tableView->setEnabled(true);
    ui->tableView_2->setEnabled(true);
    ui->groupBox->setEnabled(true);
    ui->groupBox_2->setEnabled(true);
    ui->pushButton_6->setEnabled(true);
    //settings.setValue("splitterSizes", ui->splitter->saveGeometry());


}


void MonoAlphabetic::on_pushButton_3_clicked()
{
    extraSelections.clear();
    ui->plainTextEdit_2->setExtraSelections(extraSelections);
    selection_on=false;
}

void MonoAlphabetic::show_graphics()
{
    ui->graphicsView->setRenderHint(QPainter::Antialiasing, true);//установка сглаживания области отображения графика
    sc=new QGraphicsScene(0,0,ui->graphicsView->width(),ui->graphicsView->height());//установка размеров области вывода графика
    create_grafic(1);
    create_grafic(2);
}

void MonoAlphabetic::create_grafic(int disp)
{

    QVector< double > a;//вектор содержащий информацию для отображения
    QHash<QString,float> cct;
    int point_count=char_prob.size();
    //формирования данных для вывода(i-ый ээлемент равен сумме элементов столбца до текущей строки)

    QVector< QPair<QString,QString> > label1,label2;

    if(disp==1)
    {
        QMapIterator<QString, float> it(char_prob);
        while(it.hasNext())
        {
            a.push_back(it.next().value()*1000);
            label1.push_back( QPair<QString,QString>(it.key(), QString().sprintf("%f", it.value()) ) );
        }
    }
    else
    {
        QMapIterator<QString, float> it(char_prob);
        while(it.hasNext())
        {
            it.next();
            bool fl=false;
            for(int i=0;i<busy.size();i++)
            {
                if(it.key()==busy[i].first)
                {
                    a.push_back(char_prob_text[busy[i].second]*1000);
                    label2.push_back( QPair<QString,QString>(busy[i].second, QString().sprintf("%f", char_prob_text[busy[i].second]) ) );
                    fl=true;
                }

            }
            if(!fl)
            {
                a.push_back(0);
                label2.push_back( QPair<QString,QString>("","" ) );
            }
        }



    }

    double max_a=a[0],min_a=a[0],step_a_y,step_a_x;
    for(int i=0;i<point_count;i++)
    {
        if(a[i]>max_a) max_a=a[i];
        if(a[i]<min_a) min_a=a[i];
    }

    step_a_x=(ui->graphicsView->width()-10)/a.size();//определения шага расстановки точек графика по оси Ox
    step_a_y=(ui->graphicsView->height()-80)/(max_a-min_a);//определения шага расстановки точек графика по оси Oy

    //массив точек выводимых в область вывода графика с учетом масштабирования
    //элементом вектора(массива) является элемент QPair<,> содержащий координаты x и y точки
    QVector< QPair<int,int> > points_a;
    points_a.resize(point_count);

    //формирования массива точек для последующего вывода и соеденения
    for(int i=0;i<points_a.size();i++)
    {
        points_a[i].first=i*step_a_x+15;
        points_a[i].second=ui->graphicsView->height()-(a[i]-min_a)*step_a_y/*-80*/;
    }


    //соеденение точек линиями
    for(int i=0;i<points_a.size();i++)
        if(disp==1)
        {

            //sc->addLine(points_a[i].first,points_a[i].second,points_a[i+1].first,points_a[i+1].second,QPen(QColor(205,181,205),2));


            sc->addRect(points_a[i].first,points_a[i].second,(step_a_x-6)/2,ui->graphicsView->height()-points_a[i].second, QPen(QColor(205,181,205),2),QBrush(QColor(205,181,205,100)));

        }
        else
        {
            sc->addRect(points_a[i].first+(step_a_x-6)/2,points_a[i].second,(step_a_x-6)/2,ui->graphicsView->height()-points_a[i].second, QPen(QColor(238,130,98),2),QBrush(QColor(238,130,98,100)));


        }

    QGraphicsTextItem *t1,*t2;//текст "легенды" графика
    t1=new QGraphicsTextItem("Частота появлений элементов нормативного алфавита");
    t2=new QGraphicsTextItem("Частота появления элементов алфавита шифротекста");
    t1->setDefaultTextColor(QColor(205,181,205));//задание цвета выводимого текста
    t2->setDefaultTextColor(QColor(238,130,98));
    sc->addRect(10,6,40,10,QPen(QColor(205,181,205)),QBrush(QColor(205,181,205,100)));//задание цвета для линии в "легенде"
    sc->addRect(10,26,40,10,QPen(QColor(238,130,98)),QBrush(QColor(238,130,98,100)));

    t1->setPos(55,0);//задание позиции вывода текста
    t2->setPos(55,20);
    sc->addItem(t1);//добавление текста на сцену
    sc->addItem(t2);


    if(disp==1)
        for(int i=0;i<point_count;i++)
        {
            QGraphicsTextItem *ch;

            ch=new QGraphicsTextItem(label1[i].first);
            ch->setDefaultTextColor(QColor(111,111,111));
            ch->setPos(points_a[i].first,points_a[i].second-20);
            //ch->rotate(-90);
            sc->addItem(ch);

        }
    else
        for(int i=0;i<point_count;i++)
        {
            QGraphicsTextItem *ch;

            ch=new QGraphicsTextItem(label2[i].first);
            ch->setDefaultTextColor(QColor(111,111,111));
            ch->setPos(points_a[i].first+(step_a_x-6)/2-2,points_a[i].second-20);
            //ch->rotate(-90);
            sc->addItem(ch);

        }


    ui->graphicsView->setScene(sc);//привзка сцены к элементу вывода графической информации graphicView

}

void MonoAlphabetic::UpdateData(const QModelIndex & indexA, const QModelIndex & indexB)
{




    /*QString newvar,temp;

    newvar=indexA.model()->data(indexA).toString();

    QStandardItem* i=model->itemFromIndex(indexA);
    int ii=indexA.column();
    temp=oldvar;

    oldvar=oldvar.remove(oldvar.length()-1,1);

    int i1=-1,i2=-1;
    for(int i=0;i<busy.size();i++)
    {
        if(busy[i].second==oldvar)
            i1=i;
        if(busy[i].second==newvar)
            i2=i;

    }

    if(temp.length()>0 && temp[temp.length()-1]=='!' && i1==-1)
    {
        busy.remove(i2);
        QPair<QString,QString> t( model->horizontalHeaderItem(indexA.column())->text(),newvar);
        busy.push_back(t);
        oldvar="-";

        update_all();
    }

    if(i1!=-1)
    {
        busy[i1].second=newvar;
        busy[i2].second=oldvar;

        oldvar="-";

        update_all();
    }
*/
}

void MonoAlphabetic::on_tableView_doubleClicked(const QModelIndex &index)
{
    int lti;
    if(last_table_item.size()>1)
        lti=last_table_item[last_table_item.size()-2];
    else
        return;




    QPair<QString,QString> last,now;

    last.first=bigram_alphabet[lti];
    last.second=model->item(0,lti)->text();

    now.first=bigram_alphabet[index.column()];
    now.second=model->item(0,index.column())->text();

    int il=-1,in=-1;
    for(int i=0;i<busy.size();i++)
    {
        if(busy[i].first==last.first)
            il=i;
        if(busy[i].first==now.first)
            in=i;
    }

    if(il!=-1 && in!=-1)
    {
        busy[il].second=now.second;
        busy[in].second=last.second;
    }

    if(il==-1 && in!=-1)
    {
        busy.remove(in);
        busy.push_back(QPair<QString,QString>(last.first,now.second));
    }

    if(in==-1 && il!=-1)
    {
        busy.remove(il);
        busy.push_back(QPair<QString,QString>(now.first,last.second));
    }



    update_all();
    last_table_item.clear();
}

void MonoAlphabetic::on_tableView_clicked(const QModelIndex &index)
{
    if(!grid_sovet)
    {
        grid_sovet=true;
        QMessageBox::information(this,"Совет", "Для смены симвлов в таблице, используйте одиночный и двойной клик по соответствующим ячейкам таблицы");
    }

    last_table_item.push_back(index.column());

}

void MonoAlphabetic::out_info()
{
    QString s1,s2;
    int col_count=1000;//можно вообще удалить это

    QMapIterator<QString, float> it(char_prob);
    s1+="Элементы нормативного алфавита и частота их встречаемости:\r\n";
    int i=1;
    while(it.hasNext())
    {
        it.next();
        s1+=it.key()+"="+(QString()).sprintf("%0.4f\t",it.value());
        if(i%col_count==0)
            s1+="\r\n";
        i++;
    }
    s1+="\r\n";
    s1+="\r\nЭлементы алфавита шфированного текста и частота их встречаемости:\r\n";

    i=1;
    QMapIterator<QString, float> it2(char_prob_text);
    while(it2.hasNext())
    {
        it2.next();

        s1+=it2.key()+"="+(QString()).sprintf("%0.3f\t",it2.value());
        if(i%col_count==0)
            s1+="\r\n";
        i++;
    }
    s1+="\r\n";
    s1+="\r\nЭлементы алфавита шфированного текста и их количество в введенном тексте:\r\n";
    i=1;
    QMapIterator<QString, int> it3(char_count_text);
    while(it3.hasNext())
    {
        it3.next();

        s1+=it3.key()+"="+(QString()).sprintf("%d\t",it3.value());
        if(i%col_count==0)
            s1+="\r\n";
        i++;
    }

    ui->plainTextEdit_3->setPlainText(s1);
}


void MonoAlphabetic::load_bigram()
{
    QTextStream in;
    QFile *in_file;
    in_file=new QFile;
    in_file->setFileName("char_bigram.txt");
    in_file->open(QIODevice::ReadOnly);
    in.setDevice(in_file);

    in>>bigram_alphabet;
    int t;
    QString si,sj;
    for(int i=0;i<bigram_alphabet.length();i++)
    {


        for(int j=0;j<bigram_alphabet.length();j++)
        {
            in>>t;
            QHash<QString,int> temp,a;
            si=bigram_alphabet[i];
            sj=bigram_alphabet[j];

            temp=char_bigram[si];
            temp[sj]=t;

            char_bigram[si]=temp;
        }
    }

    in_file->close();
}

void MonoAlphabetic::select_zero_bigram()
{
    QString s=ui->plainTextEdit_2->toPlainText();
    for(int i=0;i<s.length()-1;)
    {
        if(bigram_alphabet.indexOf(s[i],0)!=-1 && bigram_alphabet.indexOf(s[i+1],0)!=-1)
        {
            QString si,sj;
            int t;
            QHash<QString,int> temp;
            si=s[i];
            sj=s[i+1];

            temp=char_bigram[si];
            t=temp[sj];
            if(t<limit)
                selection_in_text(i,i+1);

            i++;
        }
        else
        {
            i++;
        }
    }
}


void MonoAlphabetic::selection_in_text(int s,int f)
{
    QTextCursor tc = ui->plainTextEdit_2->textCursor();
      tc.select(QTextCursor::Document);
      QTextCharFormat format;
      QColor lineColor = QColor(Qt::red);
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
      ui->plainTextEdit_2->setExtraSelections(extraSelections);
}

void MonoAlphabetic::create_bigram_table()
{
    model2 = new QStandardItemModel(bigram_alphabet.length(),bigram_alphabet.length(),this);
    ui->tableView->setModel(model);
    for(int i=0;i<bigram_alphabet.length();i++)
    {
        model2->setHorizontalHeaderItem(i, new QStandardItem((QString)bigram_alphabet[i]));
        model2->setVerticalHeaderItem(i, new QStandardItem((QString)bigram_alphabet[i]));
    }
    QString si,sj,t;
    for(int i=0;i<bigram_alphabet.length();i++)
    {
        for(int j=0;j<bigram_alphabet.length();j++)
        {
            QHash<QString,int> temp,a;
            si=bigram_alphabet[i];
            sj=bigram_alphabet[j];

            temp=char_bigram[si];
            t.sprintf("%d",temp[sj]);
            model2->setItem(j,i,new QStandardItem(t));
        }
    }

    ui->tableView_2->setModel(model2);
}

void MonoAlphabetic::on_add_select_clicked()
{
    selection_on=true;
     limit=ui->spinBox->value();
     extraSelections.clear();

     select_zero_bigram();
     ui->plainTextEdit_2->setExtraSelections(extraSelections);
}




void MonoAlphabetic::out_simbols()
{
    QVector< QPair<QString,QString> > vec;
    vec=busy;

    for(int i=0;i<vec.size();i++)
        for(int j=0;j<vec.size();j++)
        {
            if(char_prob_text[vec[i].second]>char_prob_text[vec[j].second] || (char_prob_text[vec[i].second]==char_prob_text[vec[j].second] && vec[i].second>vec[j].second))
                qSwap(vec[i],vec[j]);

        }

    QHash<QString,QString> top;

    for(int i=0;i<char_count && i<vec.size();i++)
        top[vec[i].second]=vec[i].first;

    QString ss=ui->lineEdit->text().toUpper();
    for(int i=0;i<ss.length();i++)
    {
        for(int j=0;j<vec.size();j++)
            if(vec[j].first==(QString)ss[i] && vec[j].second.length()>1)
            {
                top[vec[j].second]=vec[j].first;
                break;
            }
    }


    QString s=ui->plainTextEdit->toPlainText();
    QString temp,res;
    for(int i=0;i<s.length()-1;)
    {
        if(alphabet.indexOf(s[i],0)!=-1 && alphabet.indexOf(s[i+1],0)!=-1)
        {
            temp=s[i];
            temp+=s[i+1];

            temp=top[temp];
            if(temp=="")
                temp="*";

            res+=temp;
            i+=2;
            if(i>=s.length())
                break;
        }
        else
        {
            res+=s[i];
            i++;
        }
    }

    if(alphabet.indexOf(s[s.length()-1],0)==-1)
        res+=s[s.length()-1];

    ui->plainTextEdit_2->setPlainText(res);
    if(selection_on)
        on_add_select_clicked();


    create_mask();

    hist t;
    t.char_count=char_count;
    t.lineedit=ui->lineEdit->text();
    t.busy=busy;
    history.push_back(t);
    now_history_ind=history.size()-1;
}

void MonoAlphabetic::create_mask()
{
    for_mask.clear();
    QMap<QString, int> m;

    QString s=ui->plainTextEdit->toPlainText();
    QString temp,res;
    int global=1;
    for(int i=0;i<s.length()-1;)
    {
        if(alphabet.indexOf(s[i],0)!=-1 && alphabet.indexOf(s[i+1],0)!=-1)
        {
            temp=s[i];
            temp+=s[i+1];

            m[temp]=global;
            global++;

            i+=2;

            if(i>=s.length())
                break;


        }
        else
        {
            i++;
        }
    }



    for(int i=0;i<s.length()-1;)
    {
        if(alphabet.indexOf(s[i],0)!=-1 && alphabet.indexOf(s[i+1],0)!=-1)
        {
            temp=s[i];
            temp+=s[i+1];

            for_mask.push_back( QPair<QString,QString>("$" + QString::number(m[temp]),temp));

            i+=2;

            if(i>=s.length())
                break;
        }
        else
        {
            for_mask.push_back(QPair<QString,QString>(QString(s[i]),QString(s[i])));
            i++;
        }
    }

    if(alphabet.indexOf(s[s.length()-1])==-1)
        for_mask.push_back(QPair<QString,QString>(QString(s[s.length()-1]),QString(s[s.length()-1])));


}


void MonoAlphabetic::on_pushButton_2_clicked()
{
    if(char_count!=0)
    {
        char_count--;
        out_simbols();
    }
    else
        out_simbols();
}
void MonoAlphabetic::update_all()
{
    out_busy_to_table();
    show_graphics();
    out_info();

    out_simbols();
}



void MonoAlphabetic::on_spinBox_valueChanged(int arg1)
{
    if(qAbs(last_spin-arg1)==1)
    {
       int next=-1;
       QString si,sj,t;
       for(int i=0;i<bigram_alphabet.length();i++)
       {
           for(int j=0;j<bigram_alphabet.length();j++)
           {
               QHash<QString,int> temp;
               si=bigram_alphabet[i];
               sj=bigram_alphabet[j];

               temp=char_bigram[si];
               if(last_spin-arg1==-1)
               {
                   if((temp[sj]>arg1 && temp[sj]<next) || (temp[sj]>arg1 && next==-1))
                       next=temp[sj];
               }
               else
                   if((temp[sj]<arg1 && temp[sj]>next) || (temp[sj]<arg1 && next==-1))
                       next=temp[sj];
           }
       }

       if(next!=-1)
        ui->spinBox->setValue(next);
       last_spin=next;
       on_add_select_clicked();
       return;

    }

    last_spin=arg1;
}

void MonoAlphabetic::writeSettings()
{
    QSettings set("crypto_analyzer","spliter_sizes");

    set.setValue("spliter", ui->splitter->saveState());
    set.setValue("spliter2", ui->splitter_2->saveState());
    set.setValue("spliter3", ui->splitter_3->saveState());
    set.setValue("spliter4", ui->splitter_4->saveState());
}


void MonoAlphabetic::closeEvent(QCloseEvent *e)
{
    QMessageBox msgBox;
     //msgBox.setInformativeText("Сохранить персональные настройки?");

    msgBox.setWindowTitle("Анализатор шифра моноалфавитной подстановки");

    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);

    msgBox.button(QMessageBox::Save)->setText("Сохранить");
    msgBox.button(QMessageBox::Discard)->setText("Не сохранять");
    msgBox.button(QMessageBox::Cancel)->setText("Отмена");

    msgBox.setText("Сохранить персональные настройки(положение объектов внутри формы)?");

    int ret;
    /*ret = msgBox.exec();

    if(ret==QMessageBox::Save)
       writeSettings();

    if(ret==QMessageBox::Cancel)
    {
        e->ignore();
        return;
    }*/

    msgBox.setText("Сохранить проект?");

    ret = msgBox.exec();

    if(ret==QMessageBox::Save)
       on_action_4_triggered();

    if(ret==QMessageBox::Cancel)
    {
        e->ignore();
        return;
    }


}

void MonoAlphabetic::on_lineEdit_textChanged(const QString &arg1)
{
    update_all();
}


void MonoAlphabetic::on_pushButton_4_clicked()
{
    char_count=0;
    out_simbols();
}

void MonoAlphabetic::on_pushButton_5_clicked()
{
    char_count=busy.size();
    out_simbols();
}

void MonoAlphabetic::load_dictionary()
{
    QTextStream in;
    QFile *in_file;
    in_file=new QFile;
    in_file->setFileName(dictionary_file_name);
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

void MonoAlphabetic::on_pushButton_6_clicked()
{
    QString mask=ui->lineEdit_2->text().toUpper();
    QString result;
    QVector<int> ind;
    int count=0;

    //ui->progressBar->setVisible(true);
    //ui->progressBar->setMinimum(0);
    //ui->progressBar->setMaximum(dictionary.size()*2);

    if(mask.length()==0)
    {

        for(int i=0;i<dictionary.size();i++)
        {
            result+=dictionary[i]+"\r\n";

            //ui->progressBar->setValue(i);
            count++;
        }

    }

    /*
    for (int i = 0; i < mask.length(); i++)
    if (mask[i] != '*')
        ind.push_back(i);
    bool f;

    for (int i = 0; i < dictionary.size(); i++)
    {
           ui->progressBar->setValue(dictionary.size() + i);
        if (mask.length() == dictionary[i].length())
        {
            f = true;
            for (int j = 0; j < ind.size(); j++)
            if (mask[ind[j]] != dictionary[i][ind[j]])
            {
                f = false; break;
            }

            if (f)
            {
               result += dictionary[i];
               result+="\r\n";
               count++;
            }

        }
    }
    */

    find_result.clear();
    QVector<QString> ms;
    for(int i=0;i<mask.length();)
    {
        QString substr="";
        if(bigram_alphabet.indexOf(mask[i])!=-1)
        {
            substr+=mask[i];
            i++;
        }
        else
        {
            if(mask[i]=='*')
            {
                substr+="*";
                ms.push_back(substr);
                i++;
                continue;
            }
            i++;
            while(i<mask.length() && mask[i]!='$' && mask[i]!='*')
            {
                if(bigram_alphabet.indexOf(mask[i])!=-1)
                    break;

                substr+=mask[i];
                i++;
            }
        }

        ms.push_back(substr);
    }

    bool f;
    for (int i = 0; i < dictionary.size(); i++)
    {
           //ui->progressBar->setValue(dictionary.size() + i);
        if (ms.size() == dictionary[i].length())
        {
            f = true;

            for (int j = 0; j < ms.size(); j++)
            {



                if(bigram_alphabet.indexOf(ms[j])!=-1 && ms[j]!=QString(dictionary[i][j]))
                {
                    f=false;
                    break;
                }

                if(ms[j]=="*")
                    continue;

                for(int l=0;l<ms.size();l++)
                    if( (ms[j]==ms[l] && dictionary[i][j]!=dictionary[i][l]) || (ms[j]!=ms[l] && dictionary[i][j]==dictionary[i][l]) )
                    {
                        f=false;
                        break;
                    }
                if(!f)
                    break;
            }

            if (f)
            {
               result += dictionary[i];
               find_result.push_back(dictionary[i]);
               result+="\r\n";
               count++;
            }

        }
    }


    if(count!=0)
    {
        ui->label_9->setVisible(true);
        ui->label_9->setText(QString().sprintf("Всего найдено: %d",count));
    }
    else
         ui->label_9->setText("В словаре нет слов соотвутствующих маске \"" + mask + "\"!");


    //ui->progressBar->setVisible(false);

    ui->plainTextEdit_4->setPlainText(result);
}

void MonoAlphabetic::on_lineEdit_2_returnPressed()
{
    on_pushButton_6_clicked();
}

void MonoAlphabetic::first_mask_search()
{
    /*QString s=ui->plainTextEdit_2->toPlainText();

    QString max_s="";
    QString temp;
    for(int i=0;i<s.length();i++)
    {
        temp="";
        bool f=false;
        if(bigram_alphabet.indexOf(s[i],0)!=-1 || s[i]=='*')
        {
            while((bigram_alphabet.indexOf(s[i],0)!=-1 || s[i]=='*') && i<s.length())
            {
                if(bigram_alphabet.indexOf(s[i],0)!=-1)
                    f=true;
                temp+=s[i];
                        i++;
            }

            if(f && temp.length()>max_s.length())
                max_s=temp;
        }

    }

    ui->lineEdit_2->setText(max_s);*/
    on_pushButton_6_clicked();


}

void MonoAlphabetic::on_plainTextEdit_2_selectionChanged()
{
    if(!text_edit_sovet)
    {
        text_edit_sovet=true;
        QMessageBox::information(this,"Совет", "Для поиска подходящих слов кликните по слову правой кнопкой мыши");
    }
    if(ui->plainTextEdit_2->textCursor().selectedText().length()>=1)
    {
        //ui->lineEdit_2->setText(ui->plainTextEdit_2->textCursor().selectedText());

        QString mask;
        QString s=ui->plainTextEdit_2->toPlainText();
        for(int i=ui->plainTextEdit_2->textCursor().selectionStart();i<=ui->plainTextEdit_2->textCursor().selectionEnd()-1;i++)
        {
            if(s[i]=='*')
            {
                mask+=for_mask[i].first;
            }
            else
            {
               mask+=s[i];
            }
        }
        ui->lineEdit_2->setText(mask);
    }
}

void MonoAlphabetic::on_action_3_triggered(QString filename="")
{
    QTextStream in;
    QFile *in_file;
    in_file=new QFile;
    if(filename=="")
        in_file->setFileName(QFileDialog::getOpenFileName(this, "Открыть файл", "", "Файл анализатора(*.pmaf)"));
    else
        in_file->setFileName(filename);

    if(in_file->fileName()=="")
        return;

    this->setWindowTitle("Моноалфавитный Анализатор " + in_file->fileName());

    in_file->open(QIODevice::ReadOnly);
    in.setDevice(in_file);


    ui->plainTextEdit->setPlainText(in.readLine());

    on_pushButton_clicked();

    ui->lineEdit->setText(in.readLine());
    char_count=in.readLine().toInt();

    busy.resize(in.readLine().toInt());
    for(int i=0;i<busy.size();i++)
        busy[i].first=in.readLine();
    for(int i=0;i<busy.size();i++)
        busy[i].second=in.readLine();
    in_file->close();

    update_all();
}

void MonoAlphabetic::on_action_4_triggered()
{
    QTextStream out;
    QFile *out_file;
    out_file=new QFile;

    QString filename=ui->plainTextEdit_2->toPlainText().left(20).replace('*',"_") + ".pmaf";

    out_file->setFileName(QFileDialog::getSaveFileName(this, "Сохранить файл", filename, "Файл анализатора(*.pmaf)"));

    if(out_file->fileName()=="")
        return;

    out_file->open(QIODevice::WriteOnly);
    out.setDevice(out_file);

    out<<ui->plainTextEdit->toPlainText()<<"\n";
    out<<ui->lineEdit->text()<<"\n";
    out<<char_count<<"\n";

    out<<busy.size()<<"\n";
    for(int i=0;i<busy.size();i++)
        out<<busy[i].first<<"\n";
    for(int i=0;i<busy.size();i++)
        out<<busy[i].second<<"\n";

    out_file->close();
}

void MonoAlphabetic::on_action_6_triggered()
{
    close();

}

void MonoAlphabetic::on_action_9_triggered()
{
    QMessageBox::about(this,"Автор", "Выполнил студент группы КИ10-06 Герасимчук Михаил 2014г.");
}

void MonoAlphabetic::on_action_triggered()
{
    QMessageBox::about(this,"О программе", "Данная программа предназначена для помощи криптоаналитику при дешифровании шифра моноалфавитной подстановки.");
}

void MonoAlphabetic::on_action_5_triggered()
{
    dictionary_file_name="dictionary.txt";
    load_dictionary();
    ui->action_10->setChecked(false);
    ui->action_5->setChecked(true);
}

void MonoAlphabetic::on_action_10_triggered()
{
    dictionary_file_name="dictionary_big.txt";
    load_dictionary();
    ui->action_5->setChecked(false);
    ui->action_10->setChecked(true);
}

void MonoAlphabetic::on_action_2_triggered()
{
    QMessageBox::about(this,"Инструкция", "1.   Для начала анализа зашифрованного текста нажмите кнопку «Анализировать» расположенную в главном окне программы;\r\n2.   Для показа элемента расшифрованной последовательности нажмите кнопку «Добавить элемент имеющий большую частоту появления»;\r\n3.   Для перестановки символов алфавит нормативного/шифротекста используйте таблицу «Таблица заменяемых символов», для замены символов кликните один раз по символу который требуется заменить, далее двойным кликом выберите символ замещения.\r\n4.   Для выделения биграмм с частотой меньше пороговой измените значение «Порог биграмм», для снятия выделений нажмите «Снять выделения»;\r\n5.   Для отображения символов нормативного алфавита так же можно воспользоваться строкой «Добавить к показу буквы нормативного алфавит», для этого требуется ввести соответствующие символы;\r\n6.   Для поиска слов по маске, выделите слово в окне «Дешифрованный текст» слово автоматически скопируется в строку поиска, после чего нажмите «Найти», ниже будут отображены результаты поиска;\r\n7.   Для изменения словаря используемого для поиска слов по маске, выберите в меню пункт «Файл->Выбор словаря».");
}

void MonoAlphabetic::on_add_simbol_clicked()
{
    QMap<QString,int> temp;
    QString s=ui->lineEdit->text();

    for(int i=0;i<s.length();i++)
        if(bigram_alphabet.indexOf(s[i].toUpper(),0)!=-1)
            temp[s[i].toUpper()]++;

    if(char_count<temp.size())
        char_count=temp.size()-1;



    if(char_count<char_prob_text.size())
        char_count++;
    out_simbols();


}


void MonoAlphabetic::on_plainTextEdit_2_customContextMenuRequested(const QPoint &pos)
{
    QTextCursor tc = ui->plainTextEdit_2->cursorForPosition(pos);


        QString left,right;



        lefti=righti=tc.selectionStart();


        if(lefti<0 || lefti>=for_mask.size())
            return;

        while(true)
        {
            if(lefti==0)
                break;

            if(for_mask[lefti].first[0]=='$')
            {
                lefti--;
            }
            else
                break;
        }
        while(true)
        {
            if(for_mask[lefti].first[0]=='$')
                break;
            lefti++;
        }



        while(true)
        {
            if(righti==(for_mask.size()-1))
                break;

            if(for_mask[righti].first[0]=='$')
            {
                righti++;
            }
            else
                break;
        }
        while(true)
        {
            if(for_mask[righti].first[0]=='$')
                break;
            righti--;
        }

        QString ttt;
        QString s=ui->plainTextEdit_2->toPlainText();
        for(int i=lefti;i<=righti;i++)
        {

            if(s[i]!='*')
                ttt+=s[i];
            else
                ttt+=for_mask[i].first;
        }

        ui->lineEdit_2->setText(ttt);
        on_pushButton_6_clicked();

        menu=new QMenu(this);





        int max_count=250;

        for(int i=0;i<find_result.size() && i<max_count;i++)
        {
            QAction *act=new QAction(QIcon(":/img/marker.png"),find_result[i], this);

            /*bool fl=true;
            for(int j=lefti,si=0;j<=righti;j++,si++)
            {
                for(int l=0;l<busy.size();l++)
                {
                    if(for_mask[j].second==busy[l].second)
                    {
                        if(busy[l].first==QString(find_result[i][si]))
                            break;

                        for(int k=0;k<busy.size();k++)
                            if(busy[k].first==QString(find_result[i][si]))
                            {
                                fl=false;
                                break;
                            }
                        break;
                    }
                }

            }

            QFont f=ui->label->font();

            if(fl)
            {
                f.setBold(true);
            }
            else
                f.setBold(false);



            act->setFont(f);*/



            act->setCheckable(true);

            menu->addAction(act);
            connect(act,SIGNAL(triggered()),this,SLOT(context_menu_check_item()));
        }

        if(find_result.size()==0)
        {
            QAction *act=new QAction(QIcon(":/img/not_found.png"),"Соответствий не найдено...", this);
            act->setEnabled(false);
            menu->addAction(act);
        }


        if(find_result.size()>max_count)
        {
            QAction *act=new QAction(QIcon(":/img/next.png"),"Далее", this);
            connect(act,SIGNAL(triggered()),this,SLOT(next_find_list()));
            menu->addAction(act);
            ind_find_list=max_count-1;
        }


        pos_for_menu_find_list=ui->plainTextEdit_2->viewport()->mapToGlobal(pos);

        menu->popup(ui->plainTextEdit_2->viewport()->mapToGlobal(pos));
}

void MonoAlphabetic::context_menu_check_item()
{
    QString s;
    QList<QAction*> list=menu->actions();
    for(int i=0;i<list.size();i++)
        if(list[i]->isChecked())
        {
            s=list[i]->text();
            break;
        }



    set_in_busy(lefti, righti, s);
}

void MonoAlphabetic::set_in_busy(int start, int finish, QString s)
{
    QVector< QPair<int,bool> > v;

    v.resize(s.length());
    for(int i=start,si=0;i<=finish;i++,si++)
    {


        for(int j=0;j<busy.size();j++)
        {
            if(busy[j].second==for_mask[i].second)
            {
                v[si].first=j;
                v[si].second=false;

                for(int l=0;l<busy.size();l++)
                {
                    if(busy[l].first==QString(s[si]))
                    {
                        busy[l].first=busy[j].first;
                        busy[j].first=s[si];
                        v[si].second=true;
                        break;
                    }
                }


                break;
            }
        }
    }

    for(int i=0;i<v.size();i++)
        if(!v[i].second)
            busy[v[i].first].first=QString(s[i]);

    bool fl=false;
    for(int i=0;i<s.length();i++)
        if(ui->lineEdit->text().indexOf(s[i],0)==-1)
        {    ui->lineEdit->setText(ui->lineEdit->text()+s[i]);
            fl=true;
        }
    if(!fl)
        update_all();

}

void MonoAlphabetic::next_find_list()
{
    int max_count=250;
    menu=new QMenu(this);

    for(int i=ind_find_list;i<find_result.size() && i<ind_find_list+max_count;i++)
    {
        QAction *act=new QAction(QIcon(":/img/marker.png"),find_result[i], this);
        act->setCheckable(true);
        menu->addAction(act);
        connect(act,SIGNAL(triggered()),this,SLOT(context_menu_check_item()));
    }

    if(find_result.size()==0)
    {
        QAction *act=new QAction(QIcon(":/img/not_found.png"),"Соответствий не найдено...", this);
        act->setEnabled(false);
        menu->addAction(act);
    }


    if(find_result.size()>ind_find_list+max_count)
    {
        QAction *act=new QAction(QIcon(":/img/next.png"),"Далее", this);
        connect(act,SIGNAL(triggered()),this,SLOT(next_find_list()));
        menu->addAction(act);
        ind_find_list=ind_find_list+max_count-1;
    }

    menu->popup(pos_for_menu_find_list);
}

void MonoAlphabetic::keyPressEvent(QKeyEvent *event)
{
    int key=event->key();

    if( QChar::toCaseFolded( (uint) key ) == 0x42F ||  QChar::toCaseFolded( (uint) key ) == 0x44F )
        key=Qt::Key_Z;



    switch (key)
    {

    case Qt::Key_Z:
        if(event->modifiers() & Qt::CTRL)
        {
            if(now_history_ind>0)
            {
                now_history_ind--;
                busy=history[now_history_ind].busy;
                char_count=history[now_history_ind].char_count;

                ui->lineEdit->setText(history[now_history_ind].lineedit);
                history.clear();
                update_all();

            }

        }
        break;

    case Qt::Key_T:
        if(event->modifiers() & Qt::CTRL)
        {
            ui->plainTextEdit->setPlainText("81647544 41 752152, 487521 21843973 923913481375 5239213721, 13 849464377376 52134021. 2184397317 5564846475 15215239737544, 13 849464377317 92135564846475.");
            on_pushButton_clicked();

        }
        break;


    default:
        QWidget::keyPressEvent(event);
    }




}


void MonoAlphabetic::on_action_3_triggered()
{
    on_action_3_triggered("");
}

void MonoAlphabetic::changeEvent(QEvent *event)
{
    if(ui->plainTextEdit_2->toPlainText().length()>0 && event->type()==QEvent::WindowStateChange)
        update_all();
}

void MonoAlphabetic::resizeEvent(QResizeEvent *)
{
    if(ui->plainTextEdit_2->toPlainText().length()>0)
        update_all();
}

void MonoAlphabetic::on_plainTextEdit_textChanged()
{
    if(ui->plainTextEdit->toPlainText().length()==0)
    {
        ui->pushButton->setEnabled(false);
    }
    else
    {
        ui->pushButton->setEnabled(true);
    }
}
