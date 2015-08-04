#include "polyalphabetic.h"
#include "ui_polyalphabetic.h"

PolyAlphabetic::PolyAlphabetic(QWidget *parent, QString fopen_text, int fmu, bool ffiks) :
    QMainWindow(parent),
    ui(new Ui::PolyAlphabetic),
    par(parent),
    open_text(fopen_text),
    mu(fmu),
    fiks(ffiks)
{

    ui->setupUi(this);

    QSettings set("crypto_analyzer_polyalphabetic","spliter_sizes");

    ui->splitter->restoreState(set.value("spliter", QByteArray()).toByteArray());
    ui->splitter_2->restoreState(set.value("spliter2", QByteArray()).toByteArray());
    ui->splitter_3->restoreState(set.value("spliter3", QByteArray()).toByteArray());
    ui->splitter_4->restoreState(set.value("spliter4", QByteArray()).toByteArray());
    init(fopen_text, fmu, ffiks);
}

void PolyAlphabetic::init(QString fopen_text, int fmu, bool ffiks)
{
    open_text=fopen_text;
    mu=fmu;
    fiks=ffiks;

    dictionary_file_name="dictionary.txt";
    selection_on=true;
    char_prob_load();
    cipher_text_alphabet_load();

    load_bigram();
    load_dictionary();
    ui->progressBar->setVisible(false);
    char_count=0;
    limit=6;
    char_count=0;
    on_group_selected=false;
    ui->spinBox->setValue(limit);
    last_spin=ui->spinBox->value();

    ui->pushButton->setVisible(false);
    ui->plainTextEdit->setVisible(false);
    ui->plainTextEdit_2->setEnabled(false);
    ui->plainTextEdit_3->setEnabled(false);
    ui->graphicsView->setEnabled(false);
    ui->tableView->setEnabled(false);
    ui->tableView_2->setEnabled(false);
    ui->groupBox->setEnabled(false);
    //ui->groupBox_2->setEnabled(false);
    //ui->pushButton_6->setEnabled(false);
    ui->commandLinkButton->setEnabled(false);
    ui->commandLinkButton_2->setEnabled(false);
    ui->commandLinkButton_3->setEnabled(false);
    ui->commandLinkButton_4->setEnabled(false);
    ui->commandLinkButton_5->setEnabled(false);
    ui->toolBox->setEnabled(false);





//    ui->label_9->setVisible(false);

    simple_mask=true;


    ui->listView->setFlow(QListView::LeftToRight);
    ui->listView->setLayoutMode(QListView::SinglePass);
    ui->listView->setResizeMode(QListView::Adjust);
    ui->listView->setViewMode(QListView::IconMode);


    //ui->groupBox_2->setVisible(false);
    finditem a;

    a.le=ui->lineEdit_3;
    a.pb=ui->pushButton_12;
    a.l=ui->label_11;
    a.rb1=ui->radioButton_3;
    a.rb2=ui->radioButton_4;
    a.pt=ui->plainTextEdit_5;
    search_tab.push_back(a);


    a.le=ui->lineEdit_4;
    a.pb=ui->pushButton_14;
    a.l=ui->label_12;
    a.rb1=ui->radioButton_5;
    a.rb2=ui->radioButton_6;
    a.pt=ui->plainTextEdit_6;
    search_tab.push_back(a);

    a.le=ui->lineEdit_5;
    a.pb=ui->pushButton_15;
    a.l=ui->label_13;
    a.rb1=ui->radioButton_7;
    a.rb2=ui->radioButton_8;
    a.pt=ui->plainTextEdit_7;
    search_tab.push_back(a);


    a.le=ui->lineEdit_6;
    a.pb=ui->pushButton_16;
    a.l=ui->label_14;
    a.rb1=ui->radioButton_9;
    a.rb2=ui->radioButton_10;
    a.pt=ui->plainTextEdit_8;
    search_tab.push_back(a);

    out_list();
    sovet=false;
}

void PolyAlphabetic::char_prob_load()
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
void PolyAlphabetic::cipher_text_alphabet_load()
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


void PolyAlphabetic::static_analyze()
{
    QString s=ui->plainTextEdit->toPlainText();
    QString temp;
    for(int i=0;i<s.length();i++)
    {
        if(bigram_alphabet.indexOf(s[i],0)!=-1)
        {
            temp=s[i];
            char_count_text[temp]++;
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


    if(many_busy.size()-1<now_group)
    {
        if(fiks)
            create_fiks_busy();
        else
            create_busy();
        save temp;

        temp.busy=busy;
        temp.char_count=char_count;
        temp.index=for_list_view_2.size();

        many_busy.push_back(temp);
        for_list_view_2.push_back(ui->plainTextEdit->toPlainText());
    }
    else
    {

    busy=many_busy[now_group].busy;
    char_count=many_busy[now_group].char_count;
    ui->lineEdit->setText(many_busy[now_group].lineEdit);
    }

}

void PolyAlphabetic::create_busy()
{
    QMapIterator<QString,float> it(char_prob_text);


    while(it.hasNext())
    {
        it.next();
        bool fl=true;
        for(int i=0;i<busy.size();i++)
            if(busy[i].second==it.key())
            {
                fl=false;
                break;
            }

        if(fl)
            busy.push_back(QPair<QString,QString>("",it.key()));
    }

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


void PolyAlphabetic::out_busy_to_table()
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
                QStandardItem *item;
                item = new QStandardItem(busy[j].second);
                item->setSizeHint(QSize(itemw,itemh));

                model->setItem(0,i,item);
                break;
            }
            else
            {
                QStandardItem *item;
                item = new QStandardItem("");
                item->setSizeHint(QSize(itemw,itemh));

                model->setItem(0,i,item);
            }
        }
        i++;
    }

    ui->tableView->setModel(model);
}


PolyAlphabetic::~PolyAlphabetic()
{
    delete ui;
}

void PolyAlphabetic::on_pushButton_clicked()
{
    char_count_text.clear();
    char_prob_text.clear();
    busy.clear();
    extraSelections.clear();
    cc=0;


    static_analyze();
    out_busy_to_table();
    show_graphics();
    out_info();

    //select_zero_bigram();
    create_bigram_table();

    //char_count=1;
    out_simbols();
    //first_mask_search();



    ui->plainTextEdit_2->setEnabled(true);
    ui->plainTextEdit_3->setEnabled(true);
    ui->graphicsView->setEnabled(true);
    ui->tableView->setEnabled(true);
    ui->tableView_2->setEnabled(true);
    ui->groupBox->setEnabled(true);
    //ui->groupBox_2->setEnabled(true);
    //ui->pushButton_6->setEnabled(true);
    ui->commandLinkButton->setEnabled(true);
    ui->commandLinkButton_2->setEnabled(true);
    ui->commandLinkButton_3->setEnabled(true);
    ui->commandLinkButton_4->setEnabled(true);
    ui->commandLinkButton_5->setEnabled(true);
    ui->toolBox->setEnabled(true);
    //settings.setValue("splitterSizes", ui->splitter->saveGeometry());


}
void PolyAlphabetic::on_pushButton_3_clicked()
{
    extraSelections.clear();
    selection_on=false;
    out_simbols();
}

void PolyAlphabetic::show_graphics()
{
    ui->graphicsView->setRenderHint(QPainter::Antialiasing, true);//установка сглаживания области отображения графика
    sc=new QGraphicsScene(0,0,ui->graphicsView->width(),ui->graphicsView->height());//установка размеров области вывода графика
    create_grafic(1);
    create_grafic(2);
}

void PolyAlphabetic::create_grafic(int disp)
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
            ch->setPos(points_a[i].first+(step_a_x-6)/2,points_a[i].second-20);
            //ch->rotate(-90);
            sc->addItem(ch);

        }


    ui->graphicsView->setScene(sc);//привзка сцены к элементу вывода графической информации graphicView

}

void PolyAlphabetic::UpdateData(const QModelIndex & indexA, const QModelIndex & indexB)
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

void PolyAlphabetic::on_tableView_doubleClicked(const QModelIndex &index)
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


    many_busy[now_group].busy=busy;
    many_busy[now_group].char_count=char_count;
    many_busy[now_group].lineEdit=ui->lineEdit->text();

    update_all();
    last_table_item.clear();
}

void PolyAlphabetic::on_tableView_clicked(const QModelIndex &index)
{

    last_table_item.push_back(index.column());
}

void PolyAlphabetic::out_info()
{
    QString s1,s2;
    int col_count=1000;//бесполезная штука

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


void PolyAlphabetic::load_bigram()
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

void PolyAlphabetic::select_zero_bigram()
{


    QTextCursor tc = ui->plainTextEdit_2->textCursor();
  tc.select(QTextCursor::Document);
  QTextCharFormat format;
  QColor lineColor = QColor(Qt::red);
  format.setBackground(lineColor);

    QTextEdit::ExtraSelection selection;
    selection.format = format;

    selection.cursor = tc;
    selection.cursor.clearSelection();


    QList<QTextEdit::ExtraSelection> res;



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
            {


                selection.cursor.setPosition(i);
                selection.cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, 2);
                extraSelections.append(selection);



            }

            i++;
        }
        else
        {
            i++;
        }
    }

    out_marker_select();

    //ui->plainTextEdit_2->setExtraSelections(extraSelections);
}


void PolyAlphabetic::selection_in_text(int s,int f)
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

void PolyAlphabetic::create_bigram_table()
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

void PolyAlphabetic::on_add_select_clicked()
{
    selection_on=true;
     limit=ui->spinBox->value();
     extraSelections.clear();

    out_simbols();

}




void PolyAlphabetic::out_simbols()
{
    /*QVector< QPair<QString,QString> > vec;
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


    QString s=open_text;//ui->plainTextEdit->toPlainText();
    QString temp,res;
    for(int i=0;i<s.length();i++)
    {
        if(bigram_alphabet.indexOf(s[i],0)!=-1)
        {
            temp=s[i];

            temp=top[temp];
            if(temp=="")
                temp="*";

            res+=temp;

            if(i>=s.length())
                break;
        }
        else
        {
            res+=s[i];

        }
    }


    ui->plainTextEdit_2->setPlainText(res);
    if(selection_on)
        on_add_select_clicked();


    open_all_simbol_text=res;*/



    //новый out_simbols
    /*
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


    QString s=open_text;//ui->plainTextEdit->toPlainText();


    */

    if(many_busy.size()!=mu)
        return;


    many_busy[now_group].busy=busy;
    many_busy[now_group].char_count=char_count;
    many_busy[now_group].lineEdit=ui->lineEdit->text();





    QString result;
    result.resize(text_and_index.size());
    for(int i=0;i<result.length();i++)
        result[i]='*';

    for(int mbi=0;mbi<many_busy.size();mbi++)
    {

        QHash<QString,QString> top;
        QVector< QPair<QString,QString> > vec;
        vec=many_busy[mbi].busy;


        for(int i=0;i<many_busy[mbi].char_count && i<vec.size();i++)
            top[vec[i].second]=vec[i].first;

        QString ss=many_busy[mbi].lineEdit.toUpper();
        for(int i=0;i<ss.length();i++)
        {
            for(int j=0;j<vec.size();j++)
                if(vec[j].first==(QString)ss[i] && bigram_alphabet.indexOf(vec[j].second,0)!=-1)
                {
                    top[vec[j].second]=vec[j].first;
                    break;
                }
        }

        for(int i=0;i<text_and_index.size();i++)
        {
            if(bigram_alphabet.indexOf(text_and_index[i].first,0)!=-1)
            {


                if(text_and_index[i].second==many_busy[mbi].index)
                {
                    if(top[QString(text_and_index[i].first)]!="")
                        result[i]=top[QString(text_and_index[i].first)][0];
                }
            }
            else
            {
                result[i]=text_and_index[i].first[0];
            }
        }






    }

    ui->plainTextEdit_2->setPlainText(result);

    history.push_back(many_busy);
    now_history_ind=history.size()-1;

    select_zero_bigram();
    out_marker_select();
    selected_mu_group();
}

void PolyAlphabetic::out_list()
{



    QStandardItemModel *model = new QStandardItemModel (this);
    QList<QStandardItem *> items;


   /* for(int i=0;i<open_text.length();)
    {
        QString str="";

        int count=0;
        while(count<mu-1 && i<open_text.length())
        {
            if(bigram_alphabet.indexOf(open_text[i],0)!=-1)
                count++;
            str+=open_text[i];
            i++;
        }

        items.append(new QStandardItem (str));

    }

    model->appendColumn(items);
    ui->listView->setModel(model);*/


    for(int i=0;i<open_text.length();i++)
    {

        QStandardItem *t = new QStandardItem (QString(open_text[i]));

        if(open_text[i]==' ')
            t->setSizeHint(QSize(25,18));

        else
            ;//t->setSizeHint(QSize(18,18));

        items.append(t);



    }

    model->appendColumn(items);
    ui->listView->setModel(model);
}

void PolyAlphabetic::on_pushButton_2_clicked()
{
    if(char_count!=0)
    {
        char_count--;
        update_all();
    }
    else
        update_all();
}
void PolyAlphabetic::update_all()
{
    out_busy_to_table();
    show_graphics();
    out_info();

    out_simbols();

    selected_mu_group();
    select_zero_bigram();


}



void PolyAlphabetic::on_spinBox_valueChanged(int arg1)
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

void PolyAlphabetic::writeSettings()
{
    QSettings set("crypto_analyzer_polyalphabetic","spliter_sizes");

    set.setValue("spliter", ui->splitter->saveState());
    set.setValue("spliter2", ui->splitter_2->saveState());
    set.setValue("spliter3", ui->splitter_3->saveState());
    set.setValue("spliter4", ui->splitter_4->saveState());
}


void PolyAlphabetic::closeEvent(QCloseEvent *e)
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

     if(par)
        par->showNormal();
}

void PolyAlphabetic::on_lineEdit_textChanged(const QString &arg1)
{
    update_all();
}


void PolyAlphabetic::on_pushButton_4_clicked()
{
    char_count=0;
    out_simbols();
}

void PolyAlphabetic::on_pushButton_5_clicked()
{
    char_count=busy.size();
    out_simbols();
}

void PolyAlphabetic::load_dictionary()
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

void PolyAlphabetic::on_pushButton_6_clicked()
{
    QString mask=search_tab[ui->toolBox->currentIndex()].le->text().toUpper();
    ui->toolBox->setItemText(ui->toolBox->currentIndex(), "Запрос: \"" + QString(mask +"\""));


    if(simple_mask)
    {
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

                ui->progressBar->setValue(i);
                count++;
            }

        }


        for (int i = 0; i < mask.length(); i++)
        if (mask[i] != '*')
            ind.push_back(i);
        bool f;

        for (int i = 0; i < dictionary.size(); i++)
        {
               //ui->progressBar->setValue(dictionary.size() + i);
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

        if(count!=0)
        {
            search_tab[ui->toolBox->currentIndex()].l->setVisible(true);
            search_tab[ui->toolBox->currentIndex()].l->setText(QString().sprintf("Всего найдено: %d",count));
        }
        else
             search_tab[ui->toolBox->currentIndex()].l->setText("В словаре нет слов соотвутствующих маске \"" + mask + "\"!");


        //ui->progressBar->setVisible(false);

        //ui->plainTextEdit_4->setPlainText(result);
        search_tab[ui->toolBox->currentIndex()].pt->setPlainText(result);

        return;
    }

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
               result+="\r\n";
               count++;
            }

        }
    }

    if(count!=0)
    {
        search_tab[ui->toolBox->currentIndex()].l->setVisible(true);
        search_tab[ui->toolBox->currentIndex()].l->setText(QString().sprintf("Всего найдено: %d",count));
    }
    else
         search_tab[ui->toolBox->currentIndex()].l->setText("В словаре нет слов соотвутствующих маске \"" + mask + "\"!");


    //ui->progressBar->setVisible(false);


    search_tab[ui->toolBox->currentIndex()].pt->setPlainText(result);


}

void PolyAlphabetic::on_lineEdit_2_returnPressed()
{
    on_pushButton_6_clicked();
}

void PolyAlphabetic::first_mask_search()
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

    ui->lineEdit_2->setText(max_s);
    on_pushButton_6_clicked();

*/
}

void PolyAlphabetic::on_plainTextEdit_2_selectionChanged()
{
    QString substr="";

    QHash<QCharRef,int> m;
    int ind=1;
    QString res="";
    QString not_open_text=ui->plainTextEdit_2->toPlainText();



    for(int i=ui->plainTextEdit_2->textCursor().selectionStart();i<ui->plainTextEdit_2->textCursor().selectionEnd();i++)
    {

        if(not_open_text[i]=='*')
        {
            if(search_tab[ui->toolBox->currentIndex()].rb1->isChecked())
            {
               substr+="*";
               continue;
            }

            if(m[open_text[i]]==0)
                m[open_text[i]]=ind++;


            substr+=QString().sprintf("$%d",m[open_text[i]]);
        }
        else
            substr+=not_open_text[i];
    }




    search_tab[ui->toolBox->currentIndex()].le->setText(substr);
    //ui->lineEdit_2->setText(substr);

    ui->toolBox->setItemText(ui->toolBox->currentIndex(),QString("Запрос: \""+search_tab[ui->toolBox->currentIndex()].le->text())+"\"");

    /*if(ui->plainTextEdit_2->textCursor().selectedText().length()>=1)
        ui->lineEdit_2->setText(ui->plainTextEdit_2->textCursor().selectedText());*/
}

void PolyAlphabetic::on_action_3_triggered(QString filename="")
{
    QTextStream in;
    QFile *in_file;
    in_file=new QFile;

    if(filename=="")
        in_file->setFileName(QFileDialog::getOpenFileName(this, "Открыть файл", "", "Файлы анализатора(*.ppaf)"));
    else
        in_file->setFileName(filename);

    if(in_file->fileName()=="")
        return;

    this->setWindowTitle("Полиалфавитный Анализатор " + in_file->fileName());

    in_file->open(QIODevice::ReadOnly);
    in.setDevice(in_file);

    QString fopen_text;
    int fmu;
    bool ffiks=true;
    int length;

    fmu=in.readLine().toInt();
    length=in.readLine().toInt();
    fopen_text=in.readLine();
    init(fopen_text,fmu,ffiks);
    on_pushButton_7_clicked();
    ffiks=false;


    int mbc;
    fopen_text=in.readLine();
    mbc=fopen_text.toInt();
    many_busy.clear();
    QString t1,t2;

    for(int i=0;i<mbc;i++)
    {
        save tb;
        t1=in.readLine();
        t2=in.readLine();

        for(int j=0;j<t1.size();j++)
            tb.busy.push_back(QPair<QString,QString>(QString(t1[j]),QString(t2[j])));

        tb.char_count=in.readLine().toInt();
        tb.index=in.readLine().toInt();
        tb.lineEdit=in.readLine();

        many_busy.push_back(tb);
    }

    in_file->close();

    busy=many_busy[many_busy.size()-1].busy;
    char_count=many_busy[many_busy.size()-1].char_count;
    ui->lineEdit->setText(many_busy[many_busy.size()-1].lineEdit);


    update_all();
}

void PolyAlphabetic::on_action_4_triggered()
{
    update_all();
    QTextStream out;
    QFile *out_file;
    out_file=new QFile;

    QString filename=ui->plainTextEdit_2->toPlainText().left(20).replace('*',"_") + ".ppaf";

    out_file->setFileName(QFileDialog::getSaveFileName(this, "Сохранить файл", filename , "Файлы анализатора(*.ppaf)"));

    if(out_file->fileName()=="")
        return;

    out_file->open(QIODevice::WriteOnly);
    out.setDevice(out_file);

    out<<mu<<"\n";
    out<<open_text.length()<<"\n";
    out<<open_text<<"\n";
    out<<many_busy.size()<<"\n";

    for(int i=0;i<many_busy.size();i++)
    {
        QString b1,b2;
        for(int j=0;j<many_busy[i].busy.size();j++)
        {
            b1+=many_busy[i].busy[j].first;
            b2+=many_busy[i].busy[j].second;
        }

        out<<b1<<"\n";
        out<<b2<<"\n";

        out<<many_busy[i].char_count<<"\n";
        out<<many_busy[i].index<<"\n";
        out<<many_busy[i].lineEdit<<"\n";

    }

    out_file->close();
}

void PolyAlphabetic::on_action_6_triggered()
{
    close();

}

/*void PolyAlphabetic::on_action_7_triggered()
{
    QString temp;
    temp=QFileDialog::getOpenFileName(this, "Открыть файл словаря", "", "Текстовые файлы(*.txt)");

    if(temp!="")
    {
            dictionary_file_name=temp;
            load_dictionary();
    }
}*/

void PolyAlphabetic::on_action_9_triggered()
{
    QMessageBox::about(this,"Автор", "Выполнил студент группы КИ10-06 Герасимчук Михаил 2014г.");
}

void PolyAlphabetic::on_action_triggered()
{
    QMessageBox::about(this,"О программе", "Данная программа предназначена для помощи криптоаналитику при дешифровании шифра полиалфавитной подстановки.");
}

void PolyAlphabetic::on_action_5_triggered()
{
    dictionary_file_name="dictionary.txt";
    load_dictionary();
    ui->action_10->setChecked(false);
    ui->action_5->setChecked(true);
}

void PolyAlphabetic::on_action_10_triggered()
{
    dictionary_file_name="dictionary_big.txt";
    load_dictionary();
    ui->action_5->setChecked(false);
    ui->action_10->setChecked(true);
}

void PolyAlphabetic::on_action_2_triggered()
{
    QMessageBox::about(this,"Инструкция", "1.   Для начала анализа зашифрованного текста нажмите кнопку «Анализировать» расположенную в главном окне программы;\r\n2.   Для показа элемента расшифрованной последовательности нажмите кнопку «Добавить элемент имеющий большую частоту появления»;\r\n3.   Для перестановки символов алфавит нормативного/шифротекста используйте таблицу «Таблица заменяемых символов», для замены символов кликните один раз по символу который требуется заменить, далее двойным кликом выберите символ замещения.\r\n4.   Для выделения биграмм с частотой меньше пороговой измените значение «Порог биграмм», для снятия выделений нажмите «Снять выделения»;\r\n5.   Для отображения символов нормативного алфавита так же можно воспользоваться строкой «Добавить к показу буквы нормативного алфавит», для этого требуется ввести соответствующие символы;\r\n6.   Для поиска слов по маске, выделите слово в окне «Дешифрованный текст» слово автоматически скопируется в строку поиска, после чего нажмите «Найти», ниже будут отображены результаты поиска;\r\n7.   Для изменения словаря используемого для поиска слов по маске, выберите в меню пункт «Файл->Выбор словаря».\r\n8.   Для выхода из приложения выберите «Файл->Выход» или нажмите кнопку закрытие окна.");
}

void PolyAlphabetic::on_add_simbol_clicked()
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


    //out_simbols();
    update_all();
}

void PolyAlphabetic::on_listView_clicked(const QModelIndex &index)
{
    if(!sovet)
    {
        QMessageBox::information(this,"Совет", "Для переключения между группами можно исопльзовать клавиши \"Z\",\"X\"!");
        sovet=true;
    }

    if(bigram_alphabet.indexOf(index.model()->data(index).toString(),0)==-1)
    {
        ui->listView->selectionModel()->clearSelection();
        return;
    }

    if(many_busy.size()==0)
        first_start();





    on_listView_2_clicked(index);
    //click_on_list(index);
}

void PolyAlphabetic::click_on_list(const QModelIndex &index)
{
    //many_busy[now_group].busy=busy;
    //many_busy[now_group].char_count=char_count;


    QModelIndexList mod = ui->listView->selectionModel()->QItemSelectionModel::selectedRows(0);

    int start=index.row();
    int start_for_cicl=start;
    int simbol_count=0;
    for(int i=0;i<start;i++)
        if(bigram_alphabet.indexOf(open_text[i],0)!=-1)
            simbol_count++;

    start=simbol_count%mu;
    ui->listView->selectionModel()->clearSelection();

    int count=0;
    QString res="";

    ui->listView->selectionModel()->select(ui->listView->model()->index(start_for_cicl,0),QItemSelectionModel::Select);
    now_group=start;

    res+=open_text[start_for_cicl];
    for(int i=start_for_cicl;i<open_text.length();i++)
    {
        if(bigram_alphabet.indexOf(open_text[i],0)!=-1)
        {

            if(count==mu)
            {
                ui->listView->selectionModel()->select(ui->listView->model()->index(i,0),QItemSelectionModel::Select);
                res+=open_text[i];
                count=0;
            }

        count++;
        }
    }

    ui->plainTextEdit->setPlainText(res);

   on_pushButton_clicked();
    selected_mu_group();
}

void PolyAlphabetic::selected_mu_group()
{
    if(!on_group_selected)
    {
        out_marker_select();
        return;
    }

    QTextCharFormat format;
    QColor lineColor = QColor(0,245,255);

    format.setBackground(lineColor);

    QTextCursor tc = ui->plainTextEdit_2->textCursor();
    tc.select(QTextCursor::Document);
    QTextEdit::ExtraSelection selection;
    selection.format = format;
    selection.cursor = tc;
    selection.cursor.clearSelection();



    for(int j=0;j<text_and_index.size();j++)
    {
        if(text_and_index[j].second!=now_group)
            continue;

        selection.cursor.setPosition(j);
        selection.cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, 1);
        group_selections.append(selection);
    }

    out_marker_select();
}

void PolyAlphabetic::on_listView_pressed(const QModelIndex &index)
{
    on_listView_clicked(index);
}

void PolyAlphabetic::on_listView_entered(const QModelIndex &index)
{
    on_listView_clicked(index);
}

void PolyAlphabetic::on_listView_activated(const QModelIndex &index)
{
    on_listView_clicked(index);
}

void PolyAlphabetic::first_start()
{
    int start,simbol_count;
    for(int i=0;i<open_text.length();i++)
    {

        if(bigram_alphabet.indexOf(open_text[i],0)!=-1)
        {
            start=i;

            simbol_count=0;
            for(int i=0;i<start;i++)
                if(bigram_alphabet.indexOf(open_text[i],0)!=-1)
                    simbol_count++;

            start=simbol_count%mu;
            text_and_index.push_back( QPair<QString,int>(QString(open_text[i]),start) );
        }
        else
            text_and_index.push_back( QPair<QString,int>(QString(open_text[i]),-1) );
    }



    int countt=0;
    for(int i=0;countt<mu;i++)
    {
        if(bigram_alphabet.indexOf(open_text[i],0)==-1)
        {

            continue;
        }

        ui->listView->clearSelection();
        ui->listView->selectionModel()->select(ui->listView->model()->index(i,0),QItemSelectionModel::Select);

        QModelIndexList il = ui->listView->selectionModel()->QItemSelectionModel::selectedRows(0);

        foreach (QModelIndex index, il)
        {

            click_on_list(index);

        }

        countt++;

    }

    QStandardItemModel *model = new QStandardItemModel (this);
    QList<QStandardItem *> items;




    for(int i=0;i<for_list_view_2.size();i++)
    {
        QStandardItem *t = new QStandardItem (QString().sprintf("Группа %d:   ",i) + QString(for_list_view_2[i]+QString().sprintf(", количество символов в группе - %d сим.",for_list_view_2[i].length())));
        items.append(t);
    }






    model->appendColumn(items);
    ui->listView_2->setModel(model);
    out_simbols();

    on_pushButton_6_clicked();
}

void PolyAlphabetic::on_pushButton_7_clicked()
{
    if(ui->listView->model()->rowCount()==0)
        return;
    if(mu==0)
    {
        QMessageBox::critical(this,"Ошибка периода", "Неверно задан период!");
        return;}


    extraSelections.clear();
    limit=6;
    char_count=0;
    selection_on=false;


    many_busy.clear();
    for_list_view_2.clear();

    text_and_index.clear();
    on_group_selected=false;

    show_all_simbol=false;


    first_start();
    ui->listView->setFocus();
}

void PolyAlphabetic::on_listView_2_clicked(const QModelIndex &index)
{
    if(!sovet)
    {
        QMessageBox::information(this,"Совет", "Для переключения между группами можно исопльзовать клавиши \"Z\",\"X\"!");
        sovet=true;
    }


    int ind=0;
    int i;
    for(i=0;i<open_text.length();i++)
    {
        if(bigram_alphabet.indexOf(open_text[i],0)!=-1)
            ind++;
        if(ind-1==text_and_index[index.row()].second)
            break;
    }


    ui->listView->clearSelection();
    ui->listView->selectionModel()->select(ui->listView->model()->index(i,0),QItemSelectionModel::Select);
    ui->listView_2->clearSelection();
    ui->listView_2->selectionModel()->select(ui->listView_2->model()->index(i,0),QItemSelectionModel::Select);

    QModelIndexList il = ui->listView->selectionModel()->QItemSelectionModel::selectedRows(0);

    foreach (QModelIndex index, il)
    {

        click_on_list(index);

    }
}

void PolyAlphabetic::on_pushButton_8_clicked()
{
    on_group_selected=true;
    selected_mu_group();

}

void PolyAlphabetic::on_pushButton_9_clicked()
{
    on_group_selected=false;
    out_simbols();
}

void PolyAlphabetic::on_pushButton_10_clicked()
{
    for(int i=0;i<mu;i++)
        if(ui->checkBox->isChecked())
        {
            many_busy[i].char_count=many_busy[i].busy.size();
            many_busy[i].lineEdit=bigram_alphabet;
        }
        else
            many_busy[i].char_count=many_busy[i].busy.size();

    if(ui->checkBox->isChecked())
    {
        char_count=busy.size();
        ui->lineEdit->setText(bigram_alphabet);
    }
    else
        char_count=busy.size();

    out_simbols();
}

void PolyAlphabetic::on_pushButton_11_clicked()
{
    for(int i=0;i<mu;i++)
        if(ui->checkBox->isChecked())
        {
            many_busy[i].lineEdit="";
            many_busy[i].char_count=0;
        }
        else
            many_busy[i].char_count=0;


    if(ui->checkBox->isChecked())
    {
        ui->lineEdit->clear();
        char_count=0;
    }
    else
        char_count=0;

    out_simbols();

}



void PolyAlphabetic::on_radioButton_3_clicked()
{
    simple_mask=true;
}

void PolyAlphabetic::on_radioButton_4_clicked()
{
    simple_mask=false;
}

void PolyAlphabetic::on_pushButton_12_clicked()
{
    on_pushButton_6_clicked();
}

void PolyAlphabetic::on_lineEdit_3_returnPressed()
{
    on_pushButton_6_clicked();
}

void PolyAlphabetic::on_pushButton_14_clicked()
{
    on_pushButton_6_clicked();
}

void PolyAlphabetic::on_lineEdit_4_returnPressed()
{
    on_pushButton_6_clicked();
}

void PolyAlphabetic::on_pushButton_15_clicked()
{
    on_pushButton_6_clicked();
}

void PolyAlphabetic::on_lineEdit_5_returnPressed()
{
    on_pushButton_6_clicked();
}

void PolyAlphabetic::on_pushButton_16_clicked()
{
    on_pushButton_6_clicked();
}

void PolyAlphabetic::on_lineEdit_6_returnPressed()
{
    on_pushButton_6_clicked();
}

void PolyAlphabetic::on_pushButton_13_clicked()
{
    ww t;
    t.start=ui->plainTextEdit_2->textCursor().selectionStart();
    t.sdv=ui->plainTextEdit_2->textCursor().selectionEnd()-ui->plainTextEdit_2->textCursor().selectionStart();
    t.color=lineColor;

    marker_select_ind.push_back(t);

    out_marker_select();


}


void PolyAlphabetic::out_marker_select()
{

    QTextCharFormat format;




    QTextCursor tc = ui->plainTextEdit_2->textCursor();
    tc.select(QTextCursor::Document);
    QTextEdit::ExtraSelection selection;
    selection.format = format;
    selection.cursor = tc;
    selection.cursor.clearSelection();

     QList<QTextEdit::ExtraSelection> res;

    for(int i=0;i<marker_select_ind.size();i++)
    {
        format.setBackground(marker_select_ind[i].color);
        selection.format = format;

        selection.cursor.setPosition(marker_select_ind[i].start);
        selection.cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, marker_select_ind[i].sdv);
        marker_select.append(selection);
        res.push_back(selection);
    }


    if(selection_on)
        for(int i=0;i<extraSelections.size();i++)
            res.push_back(extraSelections[i]);

    if(on_group_selected)
    for(int i=0;i<group_selections.size();i++)
        res.push_back(group_selections[i]);


    ui->plainTextEdit_2->setExtraSelections(res);
    ui->plainTextEdit_2->textCursor().clearSelection();
}


void PolyAlphabetic::on_commandLinkButton_clicked()
{
    lineColor = QColor(238,0,238);
    on_pushButton_13_clicked();
}

void PolyAlphabetic::on_commandLinkButton_2_clicked()
{
    lineColor = QColor(255,165,0);
    on_pushButton_13_clicked();
}

void PolyAlphabetic::on_commandLinkButton_3_clicked()
{
    lineColor = QColor(0,255,127);
    on_pushButton_13_clicked();
}

void PolyAlphabetic::on_commandLinkButton_4_clicked()
{
    lineColor = QColor(255,255,0);
    on_pushButton_13_clicked();
}

void PolyAlphabetic::on_commandLinkButton_5_clicked()
{
  /*  lineColor = QColor(255,255,255);
    on_pushButton_13_clicked();*/

    int s,f;
    s=ui->plainTextEdit_2->textCursor().selectionStart();
    f=ui->plainTextEdit_2->textCursor().selectionEnd();

    qDebug()<<"s="<<s<<", f="<<f;



    int n=marker_select_ind.size();
    for(int i=0;i<n;i++)
    {
        int start=marker_select_ind[i].start;
        int finish=marker_select_ind[i].start+marker_select_ind[i].sdv;

        qDebug()<<"start="<<start<<", finish="<<finish;

        if(s<=start && f>=finish)
        {
            marker_select_ind.remove(i);
            i--;
            n=marker_select_ind.size();
            continue;
        }
        else
        {
            if(s>start && f<finish)
            {
                marker_select_ind[i].sdv=s-start;
                ww new_w;
                new_w.start=f;
                new_w.sdv=finish-f;
                new_w.color=marker_select_ind[i].color;
                marker_select_ind.insert(i,new_w);
                n=marker_select_ind.size();
                continue;
            }
            if(s<=start && f<=finish && f>=start)
            {
                marker_select_ind[i].start=f;
                marker_select_ind[i].sdv=finish-f;
                continue;
            }
            if(s>=start && f>=finish && s<=finish)
            {
                marker_select_ind[i].sdv=s-start;
                continue;
            }
        }

    }

    out_marker_select();
}


void PolyAlphabetic::keyPressEvent(QKeyEvent *event)
{
    int ind;
    int key=event->key();
    if( QChar::toCaseFolded( (uint) key ) == 0x427 ||  QChar::toCaseFolded( (uint) key ) == 0x447 )
        key=Qt::Key_X;

    if( QChar::toCaseFolded( (uint) key ) == 0x42F ||  QChar::toCaseFolded( (uint) key ) == 0x44F )
        key=Qt::Key_Z;

    switch (key)
    {
    case Qt::Key_X:
         ind=ui->listView_2->currentIndex().row()+1;

         if(ind>=mu)
             ind=0;
         if(ind<0)
             ind=mu-1;


         ui->listView_2->setCurrentIndex(ui->listView_2->model()->index(ind,0));
         on_listView_2_clicked(ui->listView_2->model()->index(ind,0));
         break;


    case Qt::Key_Z:

        if(event->modifiers() & Qt::CTRL)
        {
            if(now_history_ind>1)
            {
                now_history_ind-=2;
                many_busy=history[now_history_ind];


                busy=many_busy[now_group].busy;
                ui->lineEdit->setText(many_busy[now_group].lineEdit);
                char_count=many_busy[now_group].char_count;


                history.clear();
                history.push_back(many_busy);
                update_all();


            }
            break;
        }


        ind=ui->listView_2->currentIndex().row()-1;

        if(ind>=mu)
            ind=0;
        if(ind<0)
            ind=mu-1;


        ui->listView_2->setCurrentIndex(ui->listView_2->model()->index(ind,0));
        on_listView_2_clicked(ui->listView_2->model()->index(ind,0));
        break;


    default:
        QWidget::keyPressEvent(event);
    }

}

void PolyAlphabetic::create_fiks_busy()
{
    QMapIterator<QString,float> it(char_prob_text);


    while(it.hasNext())
    {
        it.next();
        bool fl=true;
        for(int i=0;i<busy.size();i++)
            if(busy[i].second==it.key())
            {
                fl=false;
                break;
            }

        if(fl)
            busy.push_back(QPair<QString,QString>(it.key(),it.key()));
    }

    for(int i=0;i<busy.size();i++)
        for(int j=0;j<busy.size();j++)
            if(char_prob_text[busy[i].second]>char_prob_text[busy[j].second])
                qSwap(busy[i],busy[j]);
/*
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
    }   */
}

void PolyAlphabetic::on_plainTextEdit_2_customContextMenuRequested(const QPoint &pos)
{
    QTextCursor tc = ui->plainTextEdit_2->cursorForPosition(pos);


    QString left,right;


    QString text=ui->plainTextEdit_2->toPlainText();

    lefti=righti=tc.selectionStart();


    if(lefti<0 || lefti>=text.length())
        return;

    while(true)
    {
        if(lefti==0)
            break;

        if(text[lefti]=='*' || (bigram_alphabet.indexOf(text[lefti],0)!=-1))
        {
            lefti--;
        }
        else
            break;
    }
    while(true)
    {
        if(text[lefti]=='*' || (bigram_alphabet.indexOf(text[lefti],0)!=-1))
            break;
        lefti++;
    }



    while(true)
    {
        if(righti==(text.length()-1))
            break;

        if(text[righti]=='*' || (bigram_alphabet.indexOf(text[righti],0)!=-1))
        {
            righti++;
        }
        else
            break;
    }
    while(true)
    {
        if(text[righti]=='*' || (bigram_alphabet.indexOf(text[righti],0)!=-1))
            break;
        righti--;
    }

    QString my_mask;
    for(int i=lefti;i<=righti;i++)
        my_mask+=text[i];

    find_result.clear();
    QVector<int> ind;
    for (int i = 0; i < my_mask.length(); i++)
    if (my_mask[i] != '*')
        ind.push_back(i);
    bool f;
    for (int i = 0; i < dictionary.size(); i++)
    {
           ui->progressBar->setValue(dictionary.size() + i);
        if (my_mask.length() == dictionary[i].length())
        {
            f = true;
            for (int j = 0; j < ind.size(); j++)
            if (my_mask[ind[j]] != dictionary[i][ind[j]])
            {
                f = false;
                break;
            }

            if (f)
               find_result.push_back(dictionary[i]);

        }
    }

    menu =new QMenu(this);
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

void PolyAlphabetic::context_menu_check_item()
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


void PolyAlphabetic::set_in_busy(int start, int finish, QString s)
{

    QString text=ui->plainTextEdit_2->toPlainText();

    for(int i=start,si=0;i<=finish;i++,si++)
    {
        int bi=text_and_index[i].second;


        for(int j=0;j<many_busy[bi].busy.size();j++)
        {
            if(many_busy[bi].busy[j].second==text_and_index[i].first)
            {
                if(many_busy[bi].busy[j].first==QString(s[si]))
                {
                    if(many_busy[bi].lineEdit.indexOf(s[si],0)==-1)
                        many_busy[bi].lineEdit+=s[si];
                    break;
                }

                int fjb=-1;
                for(int l=0;l<many_busy[bi].busy.size();l++)
                    if(many_busy[bi].busy[l].first==QString(s[si]))
                    {
                        fjb=l;
                        break;
                    }

                if(fjb!=-1)
                {
                     many_busy[bi].busy[fjb].first=many_busy[bi].busy[j].first;
                     many_busy[bi].busy[j].first=s[si];
                }
                else
                    many_busy[bi].busy[j].first=s[si];

                if(many_busy[bi].lineEdit.indexOf(s[si],0)==-1)
                    many_busy[bi].lineEdit+=s[si];
                break;
            }
        }



    }

    busy=many_busy[now_group].busy;
    char_count=many_busy[now_group].char_count;
    ui->lineEdit->setText(many_busy[now_group].lineEdit);


    update_all();


    /*  QVector< QPair<int,bool> > v;

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

    ui->lineEdit->setText(ui->lineEdit->text()+s);*/
    //update_all();

}

void PolyAlphabetic::next_find_list()
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



void PolyAlphabetic::on_action_3_triggered()
{
    on_action_3_triggered("");
}

void PolyAlphabetic::changeEvent(QEvent *event)
{
    if(ui->plainTextEdit_2->toPlainText().length()>0 && event->type()==QEvent::WindowStateChange)
        update_all();
}

void PolyAlphabetic::resizeEvent(QResizeEvent *)
{
    if(ui->plainTextEdit_2->toPlainText().length()>0)
        update_all();
}
