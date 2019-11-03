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
    delete menu;
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
    apptablewid=new QTableWidget(applistWid);
//    apptablewid->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
//    apptablewid->setMinimumSize(applistWid->width()-4,applistWid->height());
    layout->addWidget(apptablewid);
    applistWid->setLayout(layout);
    init_app_tablewidget();
    fill_app_tablewidget();
    load_min_wid();

}

/**
 * 初始化应用列表
 */
void LetterWidget::init_app_tablewidget()
{
    QStringList header;
    header.append("");
    apptablewid->setHorizontalHeaderLabels(header);
    apptablewid->setColumnCount(1);
    apptablewid->verticalHeader()->setDefaultSectionSize(42);
    apptablewid->verticalHeader()->setHidden(true);
    apptablewid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    apptablewid->setFocusPolicy(Qt::NoFocus);
    apptablewid->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    apptablewid->horizontalHeader()->setFixedHeight(0);
    apptablewid->setEditTriggers(QAbstractItemView::NoEditTriggers);
    apptablewid->setSelectionMode(QAbstractItemView::NoSelection);
    apptablewid->setShowGrid(false);
    apptablewid->setStyleSheet("QTableWidget{border:0px;background:transparent;}");
    apptablewid->verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
    apptablewid->verticalScrollBar()->setStyleSheet(
                "QScrollBar{width:12px;padding-top:15px;padding-bottom:15px;background-color:#283138;}"
                "QScrollBar::handle{background-color:#414e59; width:12px;}"
                "QScrollBar::handle:hover{background-color:#697883; }"
                "QScrollBar::handle:pressed{background-color:#8897a3;}"
                "QScrollBar::sub-line{background-color:#283138;height:15px;width:12px;image:url(:/data/img/mainviewwidget/uparrow.svg);subcontrol-position:top;}"
                "QScrollBar::sub-line:hover{background-color:#697883;}"
                "QScrollBar::sub-line:pressed{background-color:#8897a3;image:url(:/data/img/mainviewwidget/uparrow-pressed.svg);}"
                "QScrollBar::add-line{background-color:#283138;height:15px;width:12px;image:url(:/data/img/mainviewwidget/downarrow.svg);subcontrol-position:bottom;}"
                "QScrollBar::add-line:hover{background-color:#697883;}"
                "QScrollBar::add-line:pressed{background-color:#8897a3;image:url(:/data/img/mainviewwidget/downarrow-pressed.svg);}"
                );

}

/**
 * 填充应用列表
 */
void LetterWidget::fill_app_tablewidget()
{
    char btnstyle[300];
    sprintf(btnstyle,"QPushButton{background:transparent;border:0px;color:#ffffff;font-size:14px;padding-left:0px;text-align: left center;}\
            QPushButton:hover{background-color:%s;}\
            QPushButton:pressed{background-color:%s;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);

//    QStringList applist=KylinStartMenuInterface::sort_app_name();
//    apptablewid->setRowCount(applist.count());
//    for(int i=0;i<applist.count();i++)
//    {
//        QPushButton* btn=new QPushButton;
//        btn->setFixedSize(apptablewid->width()-14,42);
//        btn->setStyleSheet(QString::fromLocal8Bit(btncolor));
//        QHBoxLayout* layout=new QHBoxLayout(btn);
//        layout->setContentsMargins(15,0,0,0);
//        layout->setSpacing(15);

//        QString iconstr=KylinStartMenuInterface::get_app_icon(KylinStartMenuInterface::get_desktop_path_by_app_name(applist.at(i)));
//        QIcon::setThemeName("ukui-icon-theme");
//        QIcon icon=QIcon::fromTheme(iconstr);
//        QLabel* labelicon=new QLabel(btn);
//        labelicon->setFixedSize(32,32);
//        QPixmap pixmapicon(icon.pixmap(QSize(32,32)));
//        labelicon->setPixmap(pixmapicon);
//        labelicon->setStyleSheet("QLabel{background:transparent;}");
//        QLabel* labeltext=new QLabel(btn);
//        labeltext->setText(applist.at(i));
//        labeltext->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
//        layout->addWidget(labelicon);
//        layout->addWidget(labeltext);
//        btn->setLayout(layout);
//        btn->setFocusPolicy(Qt::NoFocus);

//        apptablewid->setCellWidget(i,0,btn);

//        connect(btn,SIGNAL(clicked()),this,SLOT(exec_app_name()));
////        connect(apptablewid,SIGNAL(cellClicked(int,int)),this,SLOT(exec_app_name(int,int)));
//        btn->setContextMenuPolicy(Qt::CustomContextMenu);
//        connect(btn,SIGNAL(customContextMenuRequested(const QPoint&)),this,
//                SLOT(right_click_slot()));
//    }

//    insert_letter_btn();


//    QStringList letterlist;
//    letterlist.clear();
//    for(int i=0;i<26;i++)
//    {
//        char letter=static_cast<char>(65+i);
//        letterlist.append(QString(QChar(letter)));
//    }
//    letterlist.append("&");
//    letterlist.append("#");

    letterposlist=KylinStartMenuInterface::get_app_diff_first_letter_pos();
    appsortlist=KylinStartMenuInterface::sort_app_name();
    for(int i=0;i<letterposlist.count();i++)
    {
//        QStringList applist=KylinStartMenuInterface::get_app_diff_first_letter_pos();
        QString startstr=letterposlist.at(i);
        int start=startstr.toInt();
        int end;
        if(i==letterposlist.count()-1)
            end=appsortlist.count()-1;
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
            apptablewid->insertRow(apptablewid->rowCount());//用于插入字母分类按钮
            apptablewid->insertRow(apptablewid->rowCount());//用于插入应用按钮界面
            letterbtnlist.append(QString(letter));//存储分类字符
            letterbtnrowlist.append(QString::number(apptablewid->rowCount()-2));//存储分类字符所在行
//            applistnum.append(QString::number(applist.count()));

            QPushButton* letterbtn=new QPushButton;
            letterbtn->setStyleSheet(btnstyle);
//            letterbtn->setFixedSize(apptablewid->width()-14,20);
            letterbtn->setFocusPolicy(Qt::NoFocus);
            QHBoxLayout* letterlayout=new QHBoxLayout(letterbtn);
            letterlayout->setContentsMargins(15,0,0,0);
            letterlayout->setSpacing(6);
            QFrame* line=new QFrame(letterbtn);
            line->setFrameShape(QFrame::HLine);
//            line->setFixedSize(letterbtn->width()-41,1);
            line->setStyleSheet("background-color:#626c6e");
            QLabel* letterlabel=new QLabel(letterbtn);
            letterlabel->setFixedSize(20,20);
            letterlabel->setAlignment(Qt::AlignCenter);
            letterlabel->setStyleSheet("color:#ffffff;font-size:14px;");
            letterlabel->setText(QString(letter));
            letterlayout->addWidget(line);
            letterlayout->addWidget(letterlabel);
            letterbtn->setLayout(letterlayout);
//            apptablewid->setRowHeight(apptablewid->rowCount()-2,20);
            apptablewid->setCellWidget(apptablewid->rowCount()-2,0,letterbtn);
            connect(letterbtn, SIGNAL(clicked()), this,SLOT(app_classificationbtn_clicked_slot()));

            QFlowLayout* flowlayout=new QFlowLayout(0,0,0);
            QWidget* wid=new QWidget;
//            wid->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
            wid->setLayout(flowlayout);
//            apptablewid->setRowHeight(apptablewid->rowCount()-1,applist.count()*42);
//            wid->resize(apptablewid->width()-14,applist.count()*42);
            apptablewid->setCellWidget(apptablewid->rowCount()-1,0,wid);
            for(int i=0;i<applist.count();i++)
            {
                QPushButton* btn=new QPushButton;
//                btn->setFixedSize(apptablewid->width()-14,42);
                btn->setFixedSize(330-4-14,42);
                btn->setStyleSheet(QString::fromLocal8Bit(btnstyle));
                QHBoxLayout* layout=new QHBoxLayout(btn);
                layout->setContentsMargins(15,0,0,0);
                layout->setSpacing(15);

                QString iconstr=KylinStartMenuInterface::get_app_icon(KylinStartMenuInterface::get_desktop_path_by_app_name(applist.at(i)));
//                QIcon::setThemeName("ukui-icon-theme");
                QIcon icon=QIcon::fromTheme(iconstr);
                QLabel* labelicon=new QLabel(btn);
                labelicon->setFixedSize(32,32);
                QPixmap pixmapicon(icon.pixmap(QSize(32,32)));
                labelicon->setPixmap(pixmapicon);
                labelicon->setStyleSheet("QLabel{background:transparent;}");
                QLabel* labeltext=new QLabel(btn);
                labeltext->setText(applist.at(i));
                labeltext->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
                layout->addWidget(labelicon);
                layout->addWidget(labeltext);
                btn->setLayout(layout);
                btn->setFocusPolicy(Qt::NoFocus);
                flowlayout->addWidget(btn);

                connect(btn,SIGNAL(clicked()),this,SLOT(exec_app_name()));
                btn->setContextMenuPolicy(Qt::CustomContextMenu);
                connect(btn,SIGNAL(customContextMenuRequested(const QPoint&)),this,
                        SLOT(right_click_slot()));

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
    is_fullscreen=false;
    this->setGeometry(QRect(60,QApplication::desktop()->availableGeometry().height()-462,
                                 330,462));
    applistWid->setFixedSize(this->width(),this->height());
    applistWid->layout()->setContentsMargins(2,0,2,0);
    apptablewid->setFixedSize(applistWid->width()-4,applistWid->height());
    mainLayout->removeWidget(letterlistWid);
    letterlistWid->setParent(NULL);
    if(is_letterbtnwid)
        letterbtnwid->load_min_wid();
    int row=0;
    while(row<apptablewid->rowCount()/2)
    {
        //分类按钮
        QWidget* letterbtnwid=apptablewid->cellWidget(row*2,0);
        QPushButton* letterbtn=qobject_cast<QPushButton*>(letterbtnwid);
        letterbtn->setFixedSize(apptablewid->width()-14,20);
        letterbtn->layout()->setContentsMargins(15,0,0,0);
        letterbtn->layout()->setSpacing(6);
        apptablewid->setRowHeight(row*2,20);
        QLayoutItem* item=letterbtn->layout()->itemAt(0);
        QWidget* linewid=item->widget();
        QFrame* line=qobject_cast<QFrame*>(linewid);
        line->setFixedSize(letterbtn->width()-41,1);
        //应用按钮界面
        QWidget* wid=apptablewid->cellWidget(row*2+1,0);
//        QString numstr=applistnum.at(row);
        int num=wid->layout()->count();
        wid->setFixedSize(apptablewid->width()-14,num*42);
        apptablewid->setRowHeight(row*2+1,num*42);
        row++;

    }
}

void LetterWidget::load_max_wid()
{
    is_fullscreen=true;
    this->setGeometry(QRect(160,
                            70,
                            QApplication::desktop()->availableGeometry().width()-160,
                            QApplication::desktop()->availableGeometry().height()-70));

    applistWid->setFixedSize(this->width(),this->height()-70);
    applistWid->layout()->setContentsMargins(30,0,2,0);
    apptablewid->setFixedSize(applistWid->width()-32-12,applistWid->height());
    letterlistWid->setFixedSize(this->width(),70);
    if(!is_letterbtnwid)
        mainLayout->addWidget(letterlistWid);
    else{
        letterbtnwid->load_max_wid();
    }
    int row=0;
    while(row<apptablewid->rowCount()/2)
    {
        //分类按钮
        QWidget* letterbtnwid=apptablewid->cellWidget(row*2,0);
        QPushButton* letterbtn=qobject_cast<QPushButton*>(letterbtnwid);
        letterbtn->setFixedSize(apptablewid->width()-32,20);
        letterbtn->layout()->setContentsMargins(0,0,0,0);
        letterbtn->layout()->setSpacing(12);
        apptablewid->setRowHeight(row*2,20);
        QLayoutItem* item=letterbtn->layout()->itemAt(0);
        QWidget* linewid=item->widget();
        QFrame* line=qobject_cast<QFrame*>(linewid);
        line->setFixedSize(letterbtn->width()-26,1);
        //应用按钮界面
        int dividend=apptablewid->width()/312;
        QWidget* wid=apptablewid->cellWidget(row*2+1,0);
//        QString numstr=applistnum.at(row);
        int num=wid->layout()->count();
        if(num%dividend>0)
            num=num/dividend+1;
        else num=num/dividend;
        wid->setFixedSize(apptablewid->width()-14,num*42);
        apptablewid->setRowHeight(row*2+1,num*42);
        row++;

    }

}


/**
 * 加载右键菜单
 */
void LetterWidget::right_click_slot()
{
    char style[200];
    sprintf(style,"QPushButton{background-color:%s;border:0px;color:#ffffff;font-size:14px;padding-left:15px;text-align: left center;}", MAINVIEWBTNHOVER);

    char btnstyle[300];
    sprintf(btnstyle,"QPushButton{background:transparent;border:0px;color:#ffffff;font-size:14px;padding-left:15px;text-align: left center;}\
            QPushButton:hover{background-color:%s;}\
            QPushButton:pressed{background-color:%s;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);

    QPushButton* btn=dynamic_cast<QPushButton*>(QObject::sender());
    QLayoutItem* item=btn->layout()->itemAt(1);
    QWidget* wid=item->widget();
    QLabel* label=qobject_cast<QLabel*>(wid);
    QString appname=label->text();
    btn->setStyleSheet(style);
    menu=new RightClickMenu;
    int ret=menu->show_appbtn_menu(appname);
    if(ret==1 || ret==2)
        emit send_update_applist_signal(appname);
    btn->setStyleSheet(btnstyle);
}

/**
 * 执行应用程序
 */
void LetterWidget::exec_app_name()
{
    emit send_hide_mainwindow_signal();
        QPushButton* btn=dynamic_cast<QPushButton*>(QObject::sender());
//        QModelIndex index=apptablewid->indexAt(btn->pos());
//    QWidget* btn=apptablewid->cellWidget(row,col);
        QLayoutItem* child=btn->layout()->itemAt(1);
        QWidget* wid=child->widget();
        QLabel* appnamelabel=qobject_cast<QLabel*>(wid);
    //    qDebug()<<appname->text();
//    QPushButton* btn=dynamic_cast<QPushButton*>(QObject::sender());
    QString appname=appnamelabel->text();
//    qDebug()<<appname;
    QString execpath=KylinStartMenuInterface::get_app_exec(KylinStartMenuInterface::get_desktop_path_by_app_name(appname));
    //    qDebug()<<execpath;
    //移除启动参数%u或者%U
    for(int i=0;i<execpath.length();i++)
    {
        if(execpath.at(i)=='%')
        {
            execpath.remove(i,2);
        }
    }
    QProcess *process=new QProcess(this);
    process->startDetached(execpath);
}

/**
 * 更新应用列表
 */
void LetterWidget::update_app_tablewidget(QString name, int arg)
{
    qDebug()<<"---dsdsadas---"<<name;
    if(arg==0)//有新软件安装
    {
        QString appname=KylinStartMenuInterface::get_app_name(QString("/usr/share/applications/"+name));
        QString lettersrt=KylinStartMenuInterface::get_app_name_pinyin(appname);
        QChar letter=lettersrt.at(0);
        if(letter<48 || (letter>57 && letter<65) || letter>90)
        {
            letter='&';
            if(letterbtnlist.indexOf(QString(letter))==-1)
            {
                int index=0;
                for(index=0;index<letterbtnlist.count();index++)
                {
                    QString letterstr=letterbtnlist.at(index);
                    QChar letter=letterstr.at(0);
                    if(letter>=65 && letter<=90)
                        continue;
                    else
                        break;
                }
                int row=0;
                if(index==letterbtnlist.count())
                {
                    row=apptablewid->rowCount();
                }
                else{
                    QString rowstr=letterbtnrowlist.at(index);
                    row=rowstr.toInt();
                }

                letterbtnlist.insert(index,QString(letter));
                letterbtnrowlist.insert(index,QString::number(row));
                apptablewid->insertRow(row);
                insert_letter_btn(QString(letter),row);
                apptablewid->insertRow(row+1);
                QPushButton* btn=new QPushButton;
                QWidget* wid=new QWidget;
                insert_appbtn_wid(wid,btn,name,row+1);
                apptablewid->setCellWidget(row+1,0,wid);
            }
            else{
                QPushButton* btn=new QPushButton;
                create_app_btn(btn,name);
                int index=letterbtnlist.indexOf(QString(letter));
                QString rowstr=letterbtnrowlist.at(index);
                int row=rowstr.toInt();
                QWidget* wid=apptablewid->cellWidget(row+1,0);
                wid->layout()->addWidget(btn);
                int num=wid->layout()->count();
                if(!is_fullscreen)
                {
                    wid->setFixedSize(apptablewid->width()-14,num*42);
                    apptablewid->setRowHeight(row+1,num*42);
                }
                else{
                    int dividend=apptablewid->width()/312;
                    if(num%dividend>0)
                        num=num/dividend+1;
                    else num=num/dividend;
                    wid->setFixedSize(apptablewid->width()-14,num*42);
                    apptablewid->setRowHeight(row+1,num*42);
                }
            }
        }
        else if(letter>=48 && letter<=57)
        {
            letter='#';
            if(letterbtnlist.indexOf(QString(letter))==-1)
            {
                int row=apptablewid->rowCount();
                letterbtnlist.append(QString(letter));
                letterbtnrowlist.append(QString::number(row));
                apptablewid->insertRow(row);
                insert_letter_btn(QString(letter),row);
                apptablewid->insertRow(row+1);
                QPushButton* btn=new QPushButton;
                QWidget* wid=new QWidget;
                insert_appbtn_wid(wid,btn,name,row+1);
                apptablewid->setCellWidget(row+1,0,wid);
            }
            else{
                QPushButton* btn=new QPushButton;
                create_app_btn(btn,name);
                int index=letterbtnlist.indexOf(QString(letter));
                QString rowstr=letterbtnrowlist.at(index);
                int row=rowstr.toInt();
                QWidget* wid=apptablewid->cellWidget(row+1,0);
                wid->layout()->addWidget(btn);
                int num=wid->layout()->count();
                if(!is_fullscreen)
                {
                    wid->setFixedSize(apptablewid->width()-14,num*42);
                    apptablewid->setRowHeight(row+1,num*42);
                }
                else{
                    int dividend=apptablewid->width()/312;
                    if(num%dividend>0)
                        num=num/dividend+1;
                    else num=num/dividend;
                    wid->setFixedSize(apptablewid->width()-14,num*42);
                    apptablewid->setRowHeight(row+1,num*42);
                }
            }
        }
        else{
            letter=lettersrt.at(0);
            if(letterbtnlist.indexOf(QString(letter))==-1)
            {
                int index=0;
                for(index=0;index<letterbtnlist.count();index++)
                {
                    QString cmpstr=letterbtnlist.at(index);
                    QChar cmp=cmpstr.at(0);
                    if(cmp=='&' || cmp=='#')
                        break;
                    else{
                        if(cmp>letter)
                            break;
                    }
                }
                int row=0;
                if(index==letterbtnlist.count())
                {
                    row=apptablewid->rowCount();
                }
                else
                {
                    QString rowstr=letterbtnrowlist.at(index);
                    row=rowstr.toInt();
                }

                letterbtnlist.insert(index,QString(letter));
                letterbtnrowlist.insert(index,QString::number(row));
                apptablewid->insertRow(row);
                insert_letter_btn(QString(letter),row);
                apptablewid->insertRow(row+1);
                QPushButton* btn=new QPushButton;
                QWidget* wid=new QWidget;
                insert_appbtn_wid(wid,btn,name,row+1);
                apptablewid->setCellWidget(row+1,0,wid);
            }
            else{
                QPushButton* btn=new QPushButton;
                create_app_btn(btn,name);
                int index=letterbtnlist.indexOf(QString(letter));
                QString rowstr=letterbtnrowlist.at(index);
                int row=rowstr.toInt();
                QWidget* wid=apptablewid->cellWidget(row+1,0);
                wid->layout()->addWidget(btn);
                int num=wid->layout()->count();
                if(!is_fullscreen)
                {
                    wid->setFixedSize(apptablewid->width()-14,num*42);
                    apptablewid->setRowHeight(row+1,num*42);
                }
                else{
                    int dividend=apptablewid->width()/312;
                    if(num%dividend>0)
                        num=num/dividend+1;
                    else num=num/dividend;
                    wid->setFixedSize(apptablewid->width()-14,num*42);
                    apptablewid->setRowHeight(row+1,num*42);
                }
            }
        }
    }
    else{
        QString appname=name;
        QString letterstr=KylinStartMenuInterface::get_app_name_pinyin(appname);
        QChar letter=letterstr.at(0);
        if(letter<48 || (letter>57 && letter<65) || letter>90)
            letter='&';
        else if(letter>=48 && letter<=57)
            letter='#';
        else letter=letterstr.at(0);
        int index=letterbtnlist.indexOf(QString(letter));
        QString rowstr=letterbtnrowlist.at(index);
        int row=rowstr.toInt();
        QWidget* wid=apptablewid->cellWidget(row+1,0);
        int count=wid->layout()->count();
        for(int i=0;i<count;i++)
        {
            QLayoutItem* item=wid->layout()->itemAt(i);
            QWidget* wid=item->widget();
            QPushButton* btn=qobject_cast<QPushButton*>(wid);
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
        if(!is_fullscreen)
        {
            wid->setFixedSize(apptablewid->width()-14,(count-1)*42);
            apptablewid->setRowHeight(row+1,(count-1)*42);
        }
        else{
            int dividend=apptablewid->width()/312;
            int num=0;
            if((count-1)%dividend>0)
                num=(count-1)/dividend+1;
            else num=(count-1)/dividend;
            wid->setFixedSize(apptablewid->width()-14,num*42);
            apptablewid->setRowHeight(row+1,num*42);
        }
    }
}

void LetterWidget::insert_letter_btn(QString letter, int row)
{
    char btnstyle[300];
    sprintf(btnstyle,"QPushButton{background:transparent;border:0px;color:#ffffff;font-size:14px;padding-left:0px;text-align: left center;}\
            QPushButton:hover{background-color:%s;}\
            QPushButton:pressed{background-color:%s;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);
    QPushButton* letterbtn=new QPushButton;
    letterbtn->setStyleSheet(btnstyle);
    letterbtn->setFocusPolicy(Qt::NoFocus);
    QHBoxLayout* letterlayout=new QHBoxLayout(letterbtn);
    letterlayout->setContentsMargins(15,0,0,0);
    letterlayout->setSpacing(6);
    QFrame* line=new QFrame(letterbtn);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background-color:#626c6e");
    QLabel* letterlabel=new QLabel(letterbtn);
    letterlabel->setFixedSize(20,20);
    letterlabel->setAlignment(Qt::AlignCenter);
    letterlabel->setStyleSheet("color:#ffffff;font-size:14px;");
    letterlabel->setText(QString(letter));
    letterlayout->addWidget(line);
    letterlayout->addWidget(letterlabel);
    letterbtn->setLayout(letterlayout);
    apptablewid->setCellWidget(row,0,letterbtn);
    connect(letterbtn, SIGNAL(clicked()), this,SLOT(app_classificationbtn_clicked_slot()));
    if(!is_fullscreen)
    {
        letterbtn->setFixedSize(apptablewid->width()-14,20);
        letterbtn->layout()->setContentsMargins(15,0,0,0);
        letterbtn->layout()->setSpacing(6);
        apptablewid->setRowHeight(row,20);
        line->setFixedSize(letterbtn->width()-41,1);
    }
    else{
        letterbtn->setFixedSize(apptablewid->width()-32,20);
        letterbtn->layout()->setContentsMargins(0,0,0,0);
        letterbtn->layout()->setSpacing(12);
        apptablewid->setRowHeight(row,20);
        line->setFixedSize(letterbtn->width()-26,1);
    }
}

void LetterWidget::insert_appbtn_wid(QWidget *wid, QPushButton *btn, QString desktopfn, int row)
{
    create_app_btn(btn,desktopfn);
    QFlowLayout* flowlayout=new QFlowLayout(0,0,0);
    wid->setLayout(flowlayout);
    flowlayout->addWidget(btn);
    wid->setFixedSize(apptablewid->width()-14,42);
    apptablewid->setRowHeight(row+1,42);
}

void LetterWidget::create_app_btn(QPushButton *btn, QString desktopfn)
{
    char btnstyle[300];
    sprintf(btnstyle,"QPushButton{background:transparent;border:0px;color:#ffffff;font-size:14px;padding-left:0px;text-align: left center;}\
            QPushButton:hover{background-color:%s;}\
            QPushButton:pressed{background-color:%s;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);
    btn->setFixedSize(330-4-14,42);
    btn->setStyleSheet(QString::fromLocal8Bit(btnstyle));
    QHBoxLayout* layout=new QHBoxLayout(btn);
    layout->setContentsMargins(15,0,0,0);
    layout->setSpacing(15);

    QString iconstr=KylinStartMenuInterface::get_app_icon(QString("/usr/share/applications/"+desktopfn));
    QString appname=KylinStartMenuInterface::get_app_name(QString("/usr/share/applications/"+desktopfn));
//                QIcon::setThemeName("ukui-icon-theme");
    QIcon icon=QIcon::fromTheme(iconstr);
    QLabel* labelicon=new QLabel(btn);
    labelicon->setFixedSize(32,32);
    QPixmap pixmapicon(icon.pixmap(QSize(32,32)));
    labelicon->setPixmap(pixmapicon);
    labelicon->setStyleSheet("QLabel{background:transparent;}");
    QLabel* labeltext=new QLabel(btn);
    labeltext->setText(appname);
    labeltext->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
    layout->addWidget(labelicon);
    layout->addWidget(labeltext);
    btn->setLayout(layout);
    btn->setFocusPolicy(Qt::NoFocus);

    connect(btn,SIGNAL(clicked()),this,SLOT(exec_app_name()));
    btn->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(btn,SIGNAL(customContextMenuRequested(const QPoint&)),this,
            SLOT(right_click_slot()));
}

/**
 * 应用列表字母分类按钮槽函数
 */
void LetterWidget::app_classificationbtn_clicked_slot()
{
    //加载LetterBUttonWidget界面
    is_letterbtnwid=true;
//    letterbtnwid=new LetterButtonWidget(this);
//    connect(this,SIGNAL(send_letterbtn_list(QStringList)),letterbtnwid,SLOT(recv_letterbtn_list(QStringList)));
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
//    connect(letterbtnwid, SIGNAL(send_letterbtn_signal(QString)),this,SLOT(recv_letterbtn_signal(QString)));
}

/**
 * 接收LetterButtonWidget界面按钮信号
 */
void LetterWidget::recv_letterbtn_signal(QString btnname)
{
    is_letterbtnwid=false;
    mainLayout->removeWidget(letterbtnwid);
    letterbtnwid->setParent(nullptr);
//    if(letterbtnwid!=nullptr)
//    {
//        delete letterbtnwid;
//        letterbtnwid=nullptr;
//    }
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
            int row=letterbtnrowlist.at(num).toInt();
            apptablewid->verticalScrollBar()->setValue(row);

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
    letterlisttableWid->setFixedSize(28*40,25);//Qt5.12中QTableWidget列宽至少为40
    letterlisttableWid->setColumnCount(28);
    letterlisttableWid->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
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
        letterlisttableWid->setColumnWidth(i,40);

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
        label->setFixedSize(40,25);
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
        int row=letterbtnrowlist.at(num).toInt();
        apptablewid->verticalScrollBar()->setValue(row);
    }

}
