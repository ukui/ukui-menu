#include "letterwidget.h"
#include "ui_letterwidget.h"
#include "src/color.h"
#include <QDebug>
#include <QHeaderView>
#include <QScrollBar>
#include "kylin-start-menu-interface.h"
#include <QPushButton>
#include <QTableWidgetItem>
#include <QFrame>
#include <QSlider>

LetterWidget::LetterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LetterWidget)
{
    ui->setupUi(this);

    init_widget();
}

LetterWidget::~LetterWidget()
{
    delete ui;
    if(menu!=nullptr)
        delete menu;
    if(letterbtnwid!=nullptr)
        delete letterbtnwid;
}

/**
 * 主界面初始化
 */
void LetterWidget::init_widget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    char widgetcolor[100];
    sprintf(widgetcolor, "border:0px;background-color:%s;",MAINVIEWWIDGETCOLOR);
    this->setStyleSheet(QString::fromLocal8Bit(widgetcolor));
//    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
//    this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
//    this->setFixedSize(330,462);
//    this->resize(330,462);
//    this->setMinimumSize(330,462);

    mainLayout=new  QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    applistWid=new QWidget(this);
    applistWid->setStyleSheet("border:0px;background: transparent;");
    applistWid->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
//    applistWid->setMinimumSize(this->width(),this->height());
    letterlistWid=new QWidget(this);
    letterlistWid->setStyleSheet("border:0px;background: transparent;");
    mainLayout->addWidget(applistWid);
    mainLayout->addWidget(letterlistWid);
    this->setLayout(mainLayout);

    letterbtnwid=new LetterButtonWidget();
    connect(this,SIGNAL(send_letterbtn_list(QStringList)),letterbtnwid,SLOT(recv_letterbtn_list(QStringList)));
    connect(letterbtnwid, SIGNAL(send_letterbtn_signal(QString)),this,SLOT(recv_letterbtn_signal(QString)));


    init_applist_widget();
    init_letterlist_widget();

}

/**
 * 初始化应用列表界面
 */
void LetterWidget::init_applist_widget()
{
    QHBoxLayout* layout=new QHBoxLayout(applistWid);
    layout->setContentsMargins(2,0,2,0);
    scrollarea=new ScrollArea;
    scrollareawid=new QWidget;
    scrollarea->setWidget(scrollareawid);
    scrollarea->setWidgetResizable(true);
    scrollareawidLayout=new QVBoxLayout;
    scrollareawidLayout->setContentsMargins(0,0,0,0);
    scrollareawidLayout->setSpacing(0);
    scrollareawid->setLayout(scrollareawidLayout);
    layout->addWidget(scrollarea);
    applistWid->setLayout(layout);
//    init_app_tablewidget();
    fill_app_list();
    load_min_wid();

}

/**
 * 填充应用列表
 */
void LetterWidget::fill_app_list()
{
    letterposlist=KylinStartMenuInterface::get_app_diff_first_letter_pos();
    appsortlist=KylinStartMenuInterface::sort_app_name();
    for(int i=0;i<letterposlist.count();i++)
    {
//        QStringList applist=KylinStartMenuInterface::get_app_diff_first_letter_pos();
        QString startstr=letterposlist.at(i);
        int start=startstr.toInt();
        int end;
        if(i==letterposlist.count()-1)
            end=appsortlist.count();
        else{
            QString endstr=letterposlist.at(i+1);
            end=endstr.toInt();
        }

        QStringList applist=get_alphabetic_classification_applist(start,end);
        if(!applist.isEmpty())
        {
            QString lettersrt=KylinStartMenuInterface::get_app_name_pinyin(applist.at(0));
            QChar letter=lettersrt.at(0);
            if(letter<48 || (letter>57 && letter<65) || letter>90)
                letter='&';
            else if(letter>=48 && letter<=57)
                letter='#';
            else letter=lettersrt.at(0);
            letterbtnlist.append(QString(letter));//存储分类字符

            PushButton* letterbtn=new PushButton(QString(letter),0,1);
            scrollareawidLayout->addWidget(letterbtn);
            connect(letterbtn, SIGNAL(clicked()), this,SLOT(app_classificationbtn_clicked_slot()));

            QFlowLayout* flowlayout=new QFlowLayout(0,0,0);
            QWidget* wid=new QWidget;
            wid->setLayout(flowlayout);
            scrollareawidLayout->addWidget(wid);
            for(int i=0;i<applist.count();i++)
            {
                PushButton* btn=new PushButton(applist.at(i),1,1);
                flowlayout->addWidget(btn);
                connect(btn,SIGNAL(clicked()),
                        this,SIGNAL(send_hide_mainwindow_signal()));
                connect(btn,SIGNAL(send_update_applist_signal(QString)),
                        this,SIGNAL(send_update_applist_signal(QString)));

            }
        }
    }
}

QStringList LetterWidget::get_alphabetic_classification_applist(int start, int end)
{
    QStringList applist;
    applist.clear();
    for(int i=start;i<end;i++)
    {
        applist.append(appsortlist.at(i));
    }
    return applist;
}

void LetterWidget::load_min_wid()
{
    letterbtnrowlist.clear();
    is_fullscreen=false;
    this->setGeometry(QRect(60,QApplication::desktop()->availableGeometry().height()-462,
                                 330,462));
    applistWid->setFixedSize(this->width(),this->height());
    applistWid->layout()->setContentsMargins(2,0,2,0);
    scrollarea->setFixedSize(applistWid->width()-4,applistWid->height());
    mainLayout->removeWidget(letterlistWid);
    letterlistWid->setParent(nullptr);
    if(is_letterbtnwid)
        letterbtnwid->load_min_wid();
    int pos=0;
    letterbtnrowlist.append(QString::number(pos));
    int row=0;
    while(row<scrollareawidLayout->count()/2)
    {
        //分类按钮
        QLayoutItem* letterbtnwidItem=scrollareawidLayout->itemAt(row*2);
        QWidget* letterbtnwid=letterbtnwidItem->widget();
        QPushButton* letterbtn=qobject_cast<QPushButton*>(letterbtnwid);
        letterbtn->setFixedSize(scrollarea->width()-14,20);
        letterbtn->layout()->setContentsMargins(15,0,0,0);
        letterbtn->layout()->setSpacing(6);
        QLayoutItem* item=letterbtn->layout()->itemAt(0);
        QWidget* linewid=item->widget();
        QFrame* line=qobject_cast<QFrame*>(linewid);
        line->setFixedSize(letterbtn->width()-41,1);
        //应用按钮界面
        QLayoutItem* widItem=scrollareawidLayout->itemAt(row*2+1);
        QWidget* wid=widItem->widget();
        int num=wid->layout()->count();
        wid->setFixedSize(scrollarea->width()-14,num*42);
        pos+=(20+wid->height());
        letterbtnrowlist.append(QString::number(pos));
        row++;

    }
    scrollarea->widget()->adjustSize();
}

void LetterWidget::load_max_wid()
{
    letterbtnrowlist.clear();
    is_fullscreen=true;
    this->setGeometry(QRect(160,
                            70,
                            QApplication::desktop()->availableGeometry().width()-160,
                            QApplication::desktop()->availableGeometry().height()-70));

    applistWid->setFixedSize(this->width(),this->height()-70);
    applistWid->layout()->setContentsMargins(30,0,2,0);
    scrollarea->setFixedSize(applistWid->width()-32-12,applistWid->height());
    letterlistWid->setFixedSize(this->width(),70);
    if(!is_letterbtnwid)
        mainLayout->addWidget(letterlistWid);
    else{
        letterbtnwid->load_max_wid();
    }
    int pos=0;
    letterbtnrowlist.append(QString::number(pos));
    int row=0;
    while(row<scrollareawidLayout->count()/2)
    {
        //分类按钮
        QLayoutItem* letterbtnwidItem=scrollareawidLayout->itemAt(row*2);
        QWidget* letterbtnwid=letterbtnwidItem->widget();
        QPushButton* letterbtn=qobject_cast<QPushButton*>(letterbtnwid);
        letterbtn->setFixedSize(scrollarea->width()-32,20);
        letterbtn->layout()->setContentsMargins(0,0,0,0);
        letterbtn->layout()->setSpacing(12);
        QLayoutItem* item=letterbtn->layout()->itemAt(0);
        QWidget* linewid=item->widget();
        QFrame* line=qobject_cast<QFrame*>(linewid);
        line->setFixedSize(letterbtn->width()-26,1);
        //应用按钮界面
        int dividend=scrollarea->width()/312;
        QLayoutItem* widItem=scrollareawidLayout->itemAt(row*2+1);
        QWidget* wid=widItem->widget();
        int num=wid->layout()->count();
        if(num%dividend>0)
            num=num/dividend+1;
        else num=num/dividend;
        wid->setFixedSize(scrollarea->width()-14,num*42);
        pos+=(20+wid->height());
        letterbtnrowlist.append(QString::number(pos));
        row++;
    }
    scrollarea->widget()->adjustSize();

}

/**
 * 更新应用列表
 */
void LetterWidget::update_app_list(QString name, int arg)
{
   if(arg==0)//有新软件安装
   {
       qDebug()<<"---有新软件安装---";
       QString appname=KylinStartMenuInterface::get_app_name(QString("/usr/share/applications/"+name));
       QString lettersrt=KylinStartMenuInterface::get_app_name_pinyin(appname);
       QChar letter=lettersrt.at(0);
       if(letter<48 || (letter>57 && letter<65) || letter>90)
       {
           letter='&';
       }
       else if(letter>=48 && letter<=57)
       {
           letter='#';
       }
       else letter=lettersrt.at(0);
       if(letterbtnlist.indexOf(QString(letter))==-1)//该软件没有对应分类
       {
           int index=0;
           for(index=0;index<letterbtnlist.count();index++)
           {
               QString indexstr=letterbtnlist.at(index);
               QChar indexch=indexstr.at(0);
               if(letter=='&')
               {
                   if(indexch>=65 && indexch<=90)
                       continue;
                   else
                       break;
               }
               else if(letter=='#')
                   break;
               else{
                   if(indexch=='&' || indexch=='#')
                       break;
                   else{
                       if(indexch>letter)
                           break;
                   }

               }
           }

           int pos=0;
           if(letter!='#')
           {
               if(index==letterbtnlist.count())
               {
                   pos=scrollareawidLayout->count();

               }
               else{
                   pos=index*2;
               }
           }
           else{
               index=letterbtnlist.count();
               pos=scrollareawidLayout->count();
           }
           letterbtnlist.insert(index,QString(letter));
           insert_letter_btn(QString(letter),pos);
           insert_appbtn_wid(name,pos+1);

       }
       else{//该软件有对应分类
           QStringList appnamelist;
           QStringList appsortlist=KylinStartMenuInterface::sort_app_name();
           for(int i=0;i<appsortlist.count();i++)
           {
               QString indexstr=KylinStartMenuInterface::get_app_name_pinyin(appsortlist.at(i));
               QChar indexch=indexstr.at(0);
               if(letter=='&')
               {
                   if(indexch<48 || (indexch>57 && indexch<65) || indexch>90)
                       appnamelist.append(appsortlist.at(i));
                   if(indexch>=48 && indexch<=57)
                       break;
               }
               else if(letter=='#')
               {
                   if(indexch>=48 && indexch<=57)
                       appnamelist.append(appsortlist.at(i));
               }
               else{
                   if(indexch==letter)
                       appnamelist.append(appsortlist.at(i));
                   if(indexch>letter || indexch<65 || indexch>90)
                       break;
               }
           }
           int pos=letterbtnlist.indexOf(QString(letter));
           QLayoutItem* item=scrollareawidLayout->itemAt(2*pos+1);
           QWidget* wid=item->widget();
           int count=wid->layout()->count();
           for(int i=count-1;i>=0;i--)
           {
               QLayoutItem* btnitem=wid->layout()->takeAt(i);
               QWidget* btnwid=btnitem->widget();
               wid->layout()->removeWidget(btnwid);
               btnwid->setParent(nullptr);
               delete btnwid;
           }
           for(int i=0;i<appnamelist.count();i++)
           {
                PushButton* btn=new PushButton(appnamelist.at(i),1,1);
                wid->layout()->addWidget(btn);
                connect(btn,SIGNAL(clicked()),
                        this,SIGNAL(send_hide_mainwindow_signal()));
                connect(btn,SIGNAL(send_update_applist_signal(QString)),
                        this,SIGNAL(send_update_applist_signal(QString)));
           }
            wid->layout()->update();

       }
   }
   else{//有软件被卸载
       qDebug()<<"---有软件被卸载---";
       QString appname=name;
       QString letterstr=KylinStartMenuInterface::get_app_name_pinyin(appname);
       QChar letter=letterstr.at(0);
       if(letter<48 || (letter>57 && letter<65) || letter>90)
           letter='&';
       else if(letter>=48 && letter<=57)
           letter='#';
       else letter=letterstr.at(0);
       int pos=letterbtnlist.indexOf(QString(letter));
       QLayoutItem* item=scrollareawidLayout->itemAt(2*pos+1);
       QWidget* wid=item->widget();
       int count=wid->layout()->count();
       for(int i=0;i<count;i++)
       {
           QLayoutItem* btnitem=wid->layout()->itemAt(i);
           QWidget* btnwid=btnitem->widget();
           QPushButton* btn=qobject_cast<QPushButton*>(btnwid);
           QLayoutItem* labelitem=btn->layout()->itemAt(1);
           QWidget* labelwid=labelitem->widget();
           QLabel* label=qobject_cast<QLabel*>(labelwid);
           QString name=label->text();
           if(QString::compare(appname,name)==0)
           {
               wid->layout()->removeWidget(btn);
               btn->setParent(nullptr);
               delete btn;
               break;
           }
       }
        if(wid->layout()->count()==0)
        {
            for(int i=1;i>=0;i--)
            {
                QLayoutItem* item=scrollareawidLayout->itemAt(2*pos+i);
                QWidget* wid=item->widget();
                scrollareawidLayout->removeWidget(wid);
                wid->setParent(nullptr);
                delete wid;
            }

            letterbtnlist.removeAll(letter);
        }
   }

   letterbtnrowlist.clear();
   if(is_fullscreen)
   {
       int pos=0;
       letterbtnrowlist.append(QString::number(pos));
       int row=0;
       while(row<scrollareawidLayout->count()/2)
       {
           //分类按钮
           QLayoutItem* letterbtnwidItem=scrollareawidLayout->itemAt(row*2);
           QWidget* letterbtnwid=letterbtnwidItem->widget();
           QPushButton* letterbtn=qobject_cast<QPushButton*>(letterbtnwid);
           letterbtn->setFixedSize(scrollarea->width()-32,20);
           letterbtn->layout()->setContentsMargins(0,0,0,0);
           letterbtn->layout()->setSpacing(12);
           QLayoutItem* item=letterbtn->layout()->itemAt(0);
           QWidget* linewid=item->widget();
           QFrame* line=qobject_cast<QFrame*>(linewid);
           line->setFixedSize(letterbtn->width()-26,1);
           //应用按钮界面
           int dividend=scrollarea->width()/312;
           QLayoutItem* widItem=scrollareawidLayout->itemAt(row*2+1);
           QWidget* wid=widItem->widget();
           int num=wid->layout()->count();
           if(num%dividend>0)
               num=num/dividend+1;
           else num=num/dividend;
           wid->setFixedSize(scrollarea->width()-14,num*42);
           if(row<scrollareawidLayout->count()/2-1)
           {
               pos+=(20+wid->height());
               letterbtnrowlist.append(QString::number(pos));
           }
           row++;
       }
       scrollarea->widget()->adjustSize();
   }
   else {
       int pos=0;
       letterbtnrowlist.append(QString::number(pos));
       int row=0;
       while(row<scrollareawidLayout->count()/2)
       {
           //分类按钮
           QLayoutItem* letterbtnwidItem=scrollareawidLayout->itemAt(row*2);
           QWidget* letterbtnwid=letterbtnwidItem->widget();
           QPushButton* letterbtn=qobject_cast<QPushButton*>(letterbtnwid);
           letterbtn->setFixedSize(scrollarea->width()-14,20);
           letterbtn->layout()->setContentsMargins(15,0,0,0);
           letterbtn->layout()->setSpacing(6);
           QLayoutItem* item=letterbtn->layout()->itemAt(0);
           QWidget* linewid=item->widget();
           QFrame* line=qobject_cast<QFrame*>(linewid);
           line->setFixedSize(letterbtn->width()-41,1);
           //应用按钮界面
           QLayoutItem* widItem=scrollareawidLayout->itemAt(row*2+1);
           QWidget* wid=widItem->widget();
           int num=wid->layout()->count();
           wid->setFixedSize(scrollarea->width()-14,num*42);
           if(row<scrollareawidLayout->count()/2-1)
           {
               pos+=(20+wid->height());
               letterbtnrowlist.append(QString::number(pos));
           }
           row++;

       }
       scrollarea->widget()->adjustSize();
   }

    qDebug()<<letterbtnlist.count()<<letterbtnrowlist.count();
}

void LetterWidget::insert_letter_btn(QString letter, int pos)
{
    PushButton* letterbtn=new PushButton(QString(letter),0,1);
    scrollareawidLayout->insertWidget(pos,letterbtn);
    connect(letterbtn, SIGNAL(clicked()), this,SLOT(app_classificationbtn_clicked_slot()));
}

void LetterWidget::insert_appbtn_wid(QString desktopfn, int pos)
{
    QString appname=KylinStartMenuInterface::get_app_name(QString("/usr/share/applications/"+desktopfn));
    PushButton* btn=new PushButton(appname,1,1);
    connect(btn,SIGNAL(clicked()),
            this,SIGNAL(send_hide_mainwindow_signal()));
    connect(btn,SIGNAL(send_update_applist_signal(QString)),
            this,SIGNAL(send_update_applist_signal(QString)));
    QWidget* wid=new QWidget;
    QFlowLayout* flowlayout=new QFlowLayout(0,0,0);
    wid->setLayout(flowlayout);
    flowlayout->addWidget(btn);
    scrollareawidLayout->insertWidget(pos,wid);
}

/**
 * 应用列表字母分类按钮槽函数
 */
void LetterWidget::app_classificationbtn_clicked_slot()
{
    //加载LetterBUttonWidget界面
    is_letterbtnwid=true;
    emit send_letterbtn_list(letterbtnlist);
    mainLayout->removeWidget(applistWid);
    applistWid->setParent(nullptr);
    if(is_fullscreen)
    {
        mainLayout->removeWidget(letterlistWid);
        letterlistWid->setParent(nullptr);
        letterbtnwid->load_max_wid();
    }
    else{
        letterbtnwid->load_min_wid();
    }
    mainLayout->addWidget(letterbtnwid);
}

/**
 * 接收LetterButtonWidget界面按钮信号
 */
void LetterWidget::recv_letterbtn_signal(QString btnname)
{
    is_letterbtnwid=false;
    mainLayout->removeWidget(letterbtnwid);
    letterbtnwid->setParent(nullptr);
    mainLayout->addWidget(applistWid);

    if(is_fullscreen)
    {
        mainLayout->addWidget(letterlistWid);

        int col=0;
        if(QString::compare(btnname,"&&")==0)
            col=26;
        else if(QString::compare(btnname,"#")==0)
            col=27;
        else{
            QByteArray btnnamearr=btnname.toLocal8Bit();
            char* bn=btnnamearr.data();
            col=static_cast<int>(*bn)-65;
        }
        letterlisttableWid->selectColumn(col);
    }
    else{
        //此处需实现将字母为btnname的应用列表移动到applistWid界面最顶端
        int num=letterbtnlist.indexOf(QString(QChar(btnname.at(0))));
        if(num!=-1)
        {
            int pos=letterbtnrowlist.at(num).toInt();
            scrollarea->verticalScrollBar()->setSliderPosition(pos);

        }
    }

}

/**
 * 初始化字母列表界面
 */
void LetterWidget::init_letterlist_widget()
{
    letterlistleftSpacer=new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);
    letterlistrightSpacer=new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);

    letterlistLayout=new QHBoxLayout(letterlistWid);
    letterlistLayout->setContentsMargins(0,0,0,0);
    //    letterlistLayout->setSpacing(5);
    letterlistWid->setLayout(letterlistLayout);

    char btncolor[300];
    sprintf(btncolor,"QToolButton{background: transparent;border:0px;padding-left:0px;}\
            QToolButton:hover{background-color:%s;}\
            QToolButton:pressed{background-color:%s;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);
    QSvgRenderer* leftsvg=new QSvgRenderer(letterlistWid);
    leftsvg->load(QString(":/data/img/mainviewwidget/leftarrow.svg"));
    QPixmap* leftpixmap=new QPixmap(19,19);
    leftpixmap->fill(Qt::transparent);
    QPainter leftp(leftpixmap);
    leftsvg->render(&leftp);
    leftbtn=new QToolButton(letterlistWid);
    leftbtn->setFixedSize(30,25);
    leftbtn->setIcon(QIcon(*leftpixmap));
    leftbtn->setStyleSheet(QString::fromLocal8Bit(btncolor));

    QSvgRenderer* rightsvg=new QSvgRenderer(letterlistWid);
    rightsvg->load(QString(":/data/img/mainviewwidget/rightarrow.svg"));
    QPixmap* rightpixmap=new QPixmap(19,19);
    rightpixmap->fill(Qt::transparent);
    QPainter rightp(rightpixmap);
    rightsvg->render(&rightp);
    letterlisttableWid=new QTableWidget(letterlistWid);
    rightbtn=new QToolButton(letterlistWid);
    rightbtn->setFixedSize(30,25);
    rightbtn->setIcon(QIcon(*rightpixmap));
    rightbtn->setStyleSheet(QString::fromLocal8Bit(btncolor));

    letterlistLayout->addItem(letterlistleftSpacer);
    letterlistLayout->addWidget(leftbtn);
    letterlistLayout->addWidget(letterlisttableWid);
    letterlistLayout->addWidget(rightbtn);
    letterlistLayout->addItem(letterlistrightSpacer);
    init_letterlist_table();

    letterlisttableWid->selectColumn(0);
    QWidget* wid=letterlisttableWid->cellWidget(0,0);
    QLabel* label=qobject_cast<QLabel*>(wid);
    QFont ft;
    ft.setWeight(81);
    label->setFont(ft);
    label->setStyleSheet("color:#ffffff;font-size:14px;");

    connect(leftbtn, SIGNAL(clicked()), this, SLOT(leftbtn_clicked_slot()));
    connect(rightbtn, SIGNAL(clicked()), this, SLOT(rightbtn_clicked_slot()));
    connect(letterlisttableWid,SIGNAL(itemSelectionChanged()),this,SLOT(letterlistitem_selected_slot()));

}

/**
 * 初始化字母列表界面数据表格letterlisttableWid
 */
void LetterWidget::init_letterlist_table()
{
    letterlisttableWid->setFocusPolicy(Qt::NoFocus);
    //    letterlisttableWid->setFixedSize(28*30+28*10,25);
    letterlisttableWid->setFixedSize(840,25);//Qt5.12中QTableWidget列宽至少为40
    letterlisttableWid->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    letterlisttableWid->setColumnCount(28);
    letterlisttableWid->setRowCount(1);
    letterlisttableWid->horizontalHeader()->setFixedHeight(0);
    letterlisttableWid->verticalHeader()->setDefaultSectionSize(25);
    letterlisttableWid->setEditTriggers(QAbstractItemView::NoEditTriggers);
    letterlisttableWid->setSelectionMode(QAbstractItemView::SingleSelection);
    letterlisttableWid->setSelectionBehavior(QAbstractItemView::SelectColumns);
    letterlisttableWid->setShowGrid(false);
    letterlisttableWid->setStyleSheet("QTableWidget{border:0px;background: transparent;}"
                                      "QTableWidget::Item{background: transparent;}\
                                      QTableWidget::Item:selected{background-color:rgba(14,19,22,92%);}");
                                      //    letterlisttableWid->setStyleSheet("QTableWidget{border:0px;background: transparent;}"
                                      //                                    "QTableWidget::Item{background: transparent;padding-left:5px;}\
                                      //                                     QTableWidget::Item:selected{background-color:rgba(14,19,22,76%);}");

    QStringList header;
    for(int i=0;i<28;i++)
    {
        header.append("");
        letterlisttableWid->setColumnWidth(i,30);

    }
    letterlisttableWid->setHorizontalHeaderLabels(header);
    letterlisttableWid->verticalHeader()->setHidden(true);
    letterlisttableWid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    QStringList letterlist;
    letterlist.clear();
    for(int i=0;i<26;i++)
    {
        char letter=static_cast<char>(65+i);
        letterlist.append(QString(QChar(letter)));
    }
    letterlist.append("&");
    letterlist.append("#");

    for(int i=0;i<28;i++)
    {
        QLabel* label=new QLabel;
        label->setFixedSize(30,25);
        QFont ft;
        ft.setWeight(50);
        label->setFont(ft);
        char labelcolor[300];
        sprintf(labelcolor,"QLabel{background: transparent;color:#8b8b8b;font-size:14px;}\
                QLabel:hover{background-color:%s;color:#ffffff;font-size:14px;}\
                QLabel:pressed{background-color:%s;color:#8b8b8b;font-size:14px;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);
        label->setStyleSheet(QString::fromLocal8Bit(labelcolor));
        label->setAlignment(Qt::AlignCenter);
        label->setText(letterlist.at(i));
        letterlisttableWid->setCellWidget(0,i,label);

    }
}

/**
 * 向左按钮槽函数
 */
void LetterWidget::leftbtn_clicked_slot()
{
    int col=letterlisttableWid->currentColumn();
    if(col>0)
        letterlisttableWid->selectColumn(col-1);
}

/**
 * 向右按钮槽函数
 */
void LetterWidget::rightbtn_clicked_slot()
{
    int col=letterlisttableWid->currentColumn();
    if(col<27)
        letterlisttableWid->selectColumn(col+1);
}

/**
 * 字母列表数据项被选定槽函数
 */
void LetterWidget::letterlistitem_selected_slot()
{
    int col=letterlisttableWid->currentColumn();
    QWidget* wid=letterlisttableWid->cellWidget(0,col);
    QLabel* label=qobject_cast<QLabel*>(wid);
    label->setStyleSheet("color:#ffffff;font-size:14px;");
    QFont ft;
    ft.setWeight(81);
    label->setFont(ft);

    char labelcolor[300];
    sprintf(labelcolor,"QLabel{background: transparent;color:#8b8b8b;font-size:14px;}\
            QLabel:hover{background-color:%s;color:#ffffff;font-size:14px;}\
            QLabel:pressed{background-color:%s;color:#8b8b8b;font-size:14px;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);

    for(int i=0;i<col;i++)
    {
        QWidget* wid=letterlisttableWid->cellWidget(0,i);
        QLabel* label=qobject_cast<QLabel*>(wid);
        QFont ft;
        ft.setWeight(50);
        label->setFont(ft);
        label->setStyleSheet(QString::fromLocal8Bit(labelcolor));
    }
    for(int i=col+1;i<28;i++)
    {
        QWidget* wid=letterlisttableWid->cellWidget(0,i);
        QLabel* label=qobject_cast<QLabel*>(wid);
        QFont ft;
        ft.setWeight(50);
        label->setFont(ft);
        label->setStyleSheet(QString::fromLocal8Bit(labelcolor));
    }

    QString arg;
    if(col==26)
    {
        arg="&&";
    }
    else arg=label->text();

    //此处需实现将被选定的字母包含的应用列表移动到applistWid界面最顶端
    QString letterstr=label->text();
    int num=letterbtnlist.indexOf(letterstr);
    if(num!=-1)
    {
        int pos=letterbtnrowlist.at(num).toInt();
        scrollarea->verticalScrollBar()->setSliderPosition(pos);
    }

}
