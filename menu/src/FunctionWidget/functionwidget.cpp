#include "functionwidget.h"
#include "ui_functionwidget.h"
#include "src/color.h"
#include "kylin-start-menu-interface.h"
#include <QDebug>

FunctionWidget::FunctionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FunctionWidget)
{
    ui->setupUi(this);

    init_widget();
}

FunctionWidget::~FunctionWidget()
{
    delete ui;
    delete menu;
    delete functionbtnwid;
}

/**
 * 主界面初始化
 */
void FunctionWidget::init_widget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    char widgetcolor[100];
    sprintf(widgetcolor, "border:0px;background-color:%s;",MAINVIEWWIDGETCOLOR);
    this->setStyleSheet(QString::fromLocal8Bit(widgetcolor));
//    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
//    this->setFixedSize(330,462);

    mainLayout=new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    applistWid=new QWidget(this);
    applistWid->setStyleSheet("border:0px;background: transparent;");
//    applistWid->setFixedSize(this->width(),this->height());
    iconlistWid=new QWidget(this);
    iconlistWid->setStyleSheet("border:0px;background:transparent");
    mainLayout->addWidget(applistWid);
    mainLayout->addWidget(iconlistWid);
    this->setLayout(mainLayout);

    iconlist.clear();
    QString recentstr=":/data/img/mainviewwidget/recent-gray.svg";
    QString netstr=":/data/img/mainviewwidget/net-gray.svg";
    QString socialstr=":/data/img/mainviewwidget/social-gray.svg";
    QString avstr=":/data/img/mainviewwidget/video-gray.svg";
    QString developstr=":/data/img/mainviewwidget/develop-gray.svg";
    QString imgstr=":/data/img/mainviewwidget/img-gray.svg";
    QString gamestr=":/data/img/mainviewwidget/game-gray.svg";
    QString officestr=":/data/img/mainviewwidget/office-gray.svg";
    QString readingstr=":/data/img/mainviewwidget/reading-gray.svg";
    QString systemstr=":/data/img/mainviewwidget/system-gray.svg";
    QString otherstr=":/data/img/mainviewwidget/other-gray.svg";
    iconlist.append(recentstr);
    iconlist.append(netstr);
    iconlist.append(socialstr);
    iconlist.append(avstr);
    iconlist.append(developstr);
    iconlist.append(imgstr);
    iconlist.append(gamestr);
    iconlist.append(officestr);
    iconlist.append(readingstr);
    iconlist.append(systemstr);
    iconlist.append(otherstr);

    iconlightlist.clear();
    QString recentlightstr=":/data/img/mainviewwidget/recent.svg";
    QString netlightstr=":/data/img/mainviewwidget/net.svg";
    QString sociallightstr=":/data/img/mainviewwidget/social.svg";
    QString avlightstr=":/data/img/mainviewwidget/video.svg";
    QString developlightstr=":/data/img/mainviewwidget/develop.svg";
    QString imglightstr=":/data/img/mainviewwidget/img.svg";
    QString gamelightstr=":/data/img/mainviewwidget/game.svg";
    QString officelightstr=":/data/img/mainviewwidget/office.svg";
    QString readinglightstr=":/data/img/mainviewwidget/reading.svg";
    QString systemlightstr=":/data/img/mainviewwidget/system.svg";
    QString otherlightstr=":/data/img/mainviewwidget/other.svg";
    iconlightlist.append(recentlightstr);
    iconlightlist.append(netlightstr);
    iconlightlist.append(sociallightstr);
    iconlightlist.append(avlightstr);
    iconlightlist.append(developlightstr);
    iconlightlist.append(imglightstr);
    iconlightlist.append(gamelightstr);
    iconlightlist.append(officelightstr);
    iconlightlist.append(readinglightstr);
    iconlightlist.append(systemlightstr);
    iconlightlist.append(otherlightstr);

    functionnamelist.clear();
    functionnamelist.append("最近添加");
    functionnamelist.append("网络应用");
    functionnamelist.append("社交沟通");
    functionnamelist.append("影音播放");
    functionnamelist.append("开发编程");
    functionnamelist.append("图形图像");
    functionnamelist.append("游戏娱乐");
    functionnamelist.append("办公学习");
    functionnamelist.append("阅读翻译");
    functionnamelist.append("系统管理");
    functionnamelist.append("其他应用");

    init_applist_widget();
    init_iconlist_widget();

    functionbtnwid=new FunctionButtonWidget();
    connect(this,SIGNAL(send_classificationbtn_list(QStringList)),functionbtnwid,SLOT(recv_classificationbtn_list(QStringList)));
    connect(functionbtnwid, SIGNAL(send_functionbtn_signal(QString)),this,SLOT(recv_functionbtn_signal(QString)));

}

/**
 * 初始化应用列表界面
 */
void FunctionWidget::init_applist_widget()
{
    QHBoxLayout* layout=new QHBoxLayout(applistWid);
    layout->setContentsMargins(2,0,2,0);
    apptablewid=new QTableWidget(applistWid);
//    apptablewid->setFixedSize(applistWid->width()-4,applistWid->height());
    layout->addWidget(apptablewid);
    applistWid->setLayout(layout);
    init_app_tablewidget();
    fill_app_tablewidget();
    load_min_wid();

}

/**
 * 初始化应用列表
 */
void FunctionWidget::init_app_tablewidget()
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
                "QScrollBar::sub-line{background-color:#283138;height:15px;width:12px;border-image:url(:/data/img/mainviewwidget/uparrow.svg);subcontrol-position:top;}"
                "QScrollBar::sub-line:hover{background-color:#697883;}"
                "QScrollBar::sub-line:pressed{background-color:#8897a3;border-image:url(:/data/img/mainviewwidget/uparrow-pressed.svg);}"
                "QScrollBar::add-line{background-color:#283138;height:15px;width:12px;border-image:url(:/data/img/mainviewwidget/downarrow.svg);subcontrol-position:bottom;}"
                "QScrollBar::add-line:hover{background-color:#697883;}"
                "QScrollBar::add-line:pressed{background-color:#8897a3;border-image:url(:/data/img/mainviewwidget/downarrow-pressed.svg);}"
                );

}

/**
 * 填充应用列表
 */
void FunctionWidget::fill_app_tablewidget()
{
    classificationbtnlist.clear();
    classificationbtnrowlist.clear();

    QStringList recentlist=KylinStartMenuInterface::get_recent_app_list();
    if(!recentlist.isEmpty())
    {
        QStringList newrecentlist;
        newrecentlist.clear();
        insert_classification_btn("最近添加");
        for(int i=0;i<10;i++)
        {
            newrecentlist.append(recentlist.at(i));
        }
        insert_app_list(newrecentlist);
    }

    QStringList netlist=KylinStartMenuInterface::get_network_app_list();
    if(!netlist.isEmpty())
    {
        insert_classification_btn("网络应用");
        insert_app_list(netlist);
    }
    QStringList sociallist=KylinStartMenuInterface::get_social_app_list();
    if(!sociallist.isEmpty())
    {
        insert_classification_btn("社交沟通");
        insert_app_list(sociallist);
    }
    QStringList avlist=KylinStartMenuInterface::get_av_app_list();
    if(!avlist.isEmpty())
    {
        insert_classification_btn("影音播放");
        insert_app_list(avlist);
    }
    QStringList developlist=KylinStartMenuInterface::get_develop_app_list();
    if(!developlist.isEmpty())
    {
        insert_classification_btn("开发编程");
        insert_app_list(developlist);
    }
    QStringList graphicslist=KylinStartMenuInterface::get_graphics_app_list();
    if(!graphicslist.isEmpty())
    {
        insert_classification_btn("图形图像");
        insert_app_list(graphicslist);
    }
    QStringList gamelist=KylinStartMenuInterface::get_game_app_list();
    if(!gamelist.isEmpty())
    {
        insert_classification_btn("游戏娱乐");
        insert_app_list(gamelist);
    }
    QStringList officelist=KylinStartMenuInterface::get_office_app_list();
    if(!officelist.isEmpty())
    {
        insert_classification_btn("办公学习");
        insert_app_list(officelist);
    }
    QStringList systemadminlist=KylinStartMenuInterface::get_systemadmin_app_list();
    if(!systemadminlist.isEmpty())
    {
        insert_classification_btn("系统管理");
        insert_app_list(systemadminlist);
    }
    QStringList otherlist=KylinStartMenuInterface::get_other_app_list();
    if(!otherlist.isEmpty())
    {
        insert_classification_btn("其他应用");
        insert_app_list(otherlist);
    }
}

void FunctionWidget::insert_classification_btn(QString btnname)
{
    char btncolor[300];
    sprintf(btncolor,"QPushButton{background:transparent;border:0px;color:#ffffff;font-size:14px;}\
            QPushButton:hover{background-color:%s;}\
            QPushButton:pressed{background-color:%s;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);

    QPushButton* classificationbtn=new QPushButton;
    classificationbtn->setStyleSheet(btncolor);
//    classificationbtn->setFixedSize(apptablewid->width()-14,20);
    classificationbtn->setFocusPolicy(Qt::NoFocus);
    QHBoxLayout* classificationlayout=new QHBoxLayout(classificationbtn);
    classificationlayout->setContentsMargins(15,0,0,0);
    classificationlayout->setSpacing(6);
    QFrame* line=new QFrame(classificationbtn);
    line->setFrameShape(QFrame::HLine);
//    line->setFixedSize(classificationbtn->width()-77,1);
    line->setStyleSheet("background-color:#626c6e");
    QLabel* classificationlabel=new QLabel(classificationbtn);
    classificationlabel->setFixedSize(56,20);
    classificationlabel->setAlignment(Qt::AlignCenter);
    classificationlabel->setStyleSheet("color:#ffffff;font-size:14px;");
    classificationlabel->setText(btnname);
    classificationlayout->addWidget(line);
    classificationlayout->addWidget(classificationlabel);
    classificationbtn->setLayout(classificationlayout);

    apptablewid->insertRow(apptablewid->rowCount());
//    apptablewid->setRowHeight(apptablewid->rowCount()-1,20);
    apptablewid->setCellWidget(apptablewid->rowCount()-1,0,classificationbtn);

    classificationbtnlist.append(btnname);
    classificationbtnrowlist.append(QString::number(apptablewid->rowCount()-1));

    connect(classificationbtn, SIGNAL(clicked()), this,SLOT(app_classificationbtn_clicked_slot()));

}

void FunctionWidget::insert_app_list(QStringList appnamelist)
{
    char btncolor[300];
    sprintf(btncolor,"QPushButton{background:transparent;border:0px;color:#ffffff;font-size:14px;padding-left:0px;text-align: left center;}\
            QPushButton:hover{background-color:%s;}\
            QPushButton:pressed{background-color:%s;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);

    apptablewid->insertRow(apptablewid->rowCount());
    applistnum.append(QString::number(appnamelist.count()));
    QFlowLayout* flowlayout=new QFlowLayout(0,0,0);
    QWidget* wid=new QWidget;
    wid->setLayout(flowlayout);
    apptablewid->setCellWidget(apptablewid->rowCount()-1,0,wid);
    for(int i=0;i<appnamelist.count();i++)
    {
//        apptablewid->insertRow(apptablewid->rowCount());
        QPushButton* btn=new QPushButton;
        btn->setFixedSize(330-14,42);
        btn->setStyleSheet(QString::fromLocal8Bit(btncolor));
        QHBoxLayout* layout=new QHBoxLayout(btn);
        layout->setContentsMargins(15,0,0,0);
        layout->setSpacing(15);

        QString iconstr=KylinStartMenuInterface::get_app_icon(KylinStartMenuInterface::get_desktop_path_by_app_name(appnamelist.at(i)));
//        QIcon::setThemeName("ukui-icon-theme");
        QIcon icon=QIcon::fromTheme(iconstr);
        QLabel* labelicon=new QLabel(btn);
        labelicon->setFixedSize(32,32);
        QPixmap pixmapicon(icon.pixmap(QSize(32,32)));
        labelicon->setPixmap(pixmapicon);
        labelicon->setStyleSheet("QLabel{background:transparent;}");
        QLabel* labeltext=new QLabel(btn);
        labeltext->setText(appnamelist.at(i));
        labeltext->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
        layout->addWidget(labelicon);
        layout->addWidget(labeltext);
        btn->setLayout(layout);
        btn->setFocusPolicy(Qt::NoFocus);
        flowlayout->addWidget(btn);

//        apptablewid->setCellWidget(apptablewid->rowCount()-1,0,btn);

        connect(btn,SIGNAL(clicked()),this,SLOT(exec_app_name()));
        btn->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(btn,SIGNAL(customContextMenuRequested(const QPoint&)),this,
                SLOT(right_click_slot()));
    }

}

void FunctionWidget::load_min_wid()
{
    is_fullscreen=false;
    this->setGeometry(QRect(60,QApplication::desktop()->availableGeometry().height()-462,
                                 330,462));
    applistWid->setFixedSize(this->width(),this->height());
    applistWid->layout()->setContentsMargins(2,0,2,0);
    apptablewid->setFixedSize(applistWid->width()-4,applistWid->height());
    mainLayout->removeWidget(iconlistWid);
    iconlistWid->setParent(NULL);
    if(is_functionbtnwid)
        functionbtnwid->load_min_wid();
    int row=0;
    while(row<apptablewid->rowCount()/2)
    {
        //分类按钮界面
        QWidget* classificationbtnwid=apptablewid->cellWidget(row*2,0);
        QPushButton* classificationbtn=qobject_cast<QPushButton*>(classificationbtnwid);
        classificationbtn->setFixedSize(apptablewid->width()-14,20);
        classificationbtn->layout()->setContentsMargins(15,0,0,0);
        classificationbtn->layout()->setSpacing(6);
        apptablewid->setRowHeight(row*2,20);
        QLayoutItem* item=classificationbtn->layout()->itemAt(0);
        QWidget* linewid=item->widget();
        QFrame* line=qobject_cast<QFrame*>(linewid);
        line->setFixedSize(classificationbtn->width()-77,1);
        //应用按钮界面
        QWidget* wid=apptablewid->cellWidget(row*2+1,0);
        QString numstr=applistnum.at(row);
        int num=numstr.toInt();
        wid->setFixedSize(apptablewid->width()-14,num*42);
        apptablewid->setRowHeight(row*2+1,num*42);
        row++;

    }
}

void FunctionWidget::load_max_wid()
{
    is_fullscreen=true;
    this->setGeometry(QRect(160,
                            70,
                            QApplication::desktop()->availableGeometry().width()-160,
                            QApplication::desktop()->availableGeometry().height()-70));

    applistWid->setFixedSize(this->width(),this->height()-70);
    applistWid->layout()->setContentsMargins(30,0,2,0);
    apptablewid->setFixedSize(applistWid->width()-32-12,applistWid->height());
    iconlistWid->setFixedSize(this->width(),70);
    if(!is_functionbtnwid)
        mainLayout->addWidget(iconlistWid);
    else
        functionbtnwid->load_max_wid();
    int row=0;
    while(row<apptablewid->rowCount()/2)
    {
        //分类按钮
        QWidget* classificationbtnlayoutwid=apptablewid->cellWidget(row*2,0);
        QPushButton* classificationbtn=qobject_cast<QPushButton*>(classificationbtnlayoutwid);
        classificationbtn->setFixedSize(apptablewid->width()-32,20);
        classificationbtn->layout()->setContentsMargins(0,0,0,0);
        classificationbtn->layout()->setSpacing(12);
        apptablewid->setRowHeight(row*2,20);
        QLayoutItem* item=classificationbtn->layout()->itemAt(0);
        QWidget* linewid=item->widget();
        QFrame* line=qobject_cast<QFrame*>(linewid);
        line->setFixedSize(classificationbtn->width()-62,1);
        //应用按钮界面
        int dividend=apptablewid->width()/312;
        QWidget* wid=apptablewid->cellWidget(row*2+1,0);
        QString numstr=applistnum.at(row);
        int num=numstr.toInt();
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
void FunctionWidget::right_click_slot()
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
void FunctionWidget::exec_app_name()
{
    emit send_hide_mainwindow_signal();
    QPushButton* btn=dynamic_cast<QPushButton*>(QObject::sender());
    QLayoutItem* item=btn->layout()->itemAt(1);
    QWidget* wid=item->widget();
    QLabel* label=qobject_cast<QLabel*>(wid);
    QString appname=label->text();
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
void FunctionWidget::update_app_tablewidget(QString name, int arg)
{
    //放到最近添加模块
    QWidget* wid=apptablewid->cellWidget(1,0);
    if(arg==0)
    {
//        QWidget* wid=apptablewid->cellWidget(1,0);
        QPushButton* btn=new QPushButton;
        create_app_btn(btn,name);
        wid->layout()->addWidget(btn);
    }
    else {
//        QWidget* wid=apptablewid->cellWidget(1,0);
        int count=wid->layout()->count();
        int index=0;
        for(index=0;index<count;index++)
        {
            QLayoutItem* item=wid->layout()->itemAt(index);
            QWidget* wid=item->widget();
            QPushButton* btn=qobject_cast<QPushButton*>(wid);
            QLayoutItem* btnitem=btn->layout()->itemAt(1);
            QWidget* labelwid=btnitem->widget();
            QLabel* label=qobject_cast<QLabel*>(labelwid);
            QString appname=label->text();
            if(QString::compare(appname,name)==0)
            {
                wid->layout()->removeWidget(btn);
                btn->setParent(nullptr);
                delete btn;
                break;
            }
        }
    }

    int count=wid->layout()->count();
    if(!is_fullscreen)
    {
        wid->setFixedSize(apptablewid->width()-14,count*42);
        apptablewid->setRowHeight(1,count*42);
    }
    else{
        int dividend=apptablewid->width()/312;
        int num=0;
        if(count%dividend>0)
            num=count/dividend+1;
        else num=count/dividend;
        wid->setFixedSize(apptablewid->width()-14,num*42);
        apptablewid->setRowHeight(1,num*42);
    }

//    if(arg==0)//有新软件安装
//    {
//        QString appname=KylinStartMenuInterface::get_app_name(QString("/usr/share/applications/"+name));
//        QStringList networkapplist=KylinStartMenuInterface::get_network_app_list();
//        QStringList socilaapplist=KylinStartMenuInterface::get_social_app_list();
//        QStringList avapplist=KylinStartMenuInterface::get_av_app_list();
//        QStringList devapplist=KylinStartMenuInterface::get_develop_app_list();
//        QStringList graphicsapplist=KylinStartMenuInterface::get_graphics_app_list();
//        QStringList gameapplist=KylinStartMenuInterface::get_game_app_list();
//        QStringList officeapplist=KylinStartMenuInterface::get_office_app_list();
//        QStringList educationapplist=KylinStartMenuInterface::get_education_app_list();
//        QStringList systemadminlist=KylinStartMenuInterface::get_systemadmin_app_list();
////        QStringList otherapplist=get_other_app_list();
//        QString classify;
//        if(networkapplist.contains(appname))
//            classify="网络应用";
//        else if(socilaapplist.contains(appname))
//            classify="社交沟通";
//        else if(avapplist.contains(appname))
//            classify="影音播放";
//        else if(devapplist.contains(appname))
//            classify="开发编程";
//        else if(graphicsapplist.contains(appname))
//            classify="图形图像";
//        else if(gameapplist.contains(appname))
//            classify="游戏娱乐";
//        else if(officeapplist.contains(appname))
//            classify="办公学习";
//        else if(educationapplist.contains(appname))
//            classify="阅读翻译";
//        else if(systemadminlist.contains(appname))
//            classify="系统管理";
//        else
//            classify="其他应用";

//        if(classificationbtnlist.indexOf(classify)==-1)
//        {
//            int index=functionnamelist.indexOf(classify);
//            int row=0;
//            if(index==functionnamelist.count()-1)
//            {
//                row=apptablewid->rowCount();

//            }
//            else{
//                while(index>0)
//                {
//                    index--;
//                    QString classify=functionnamelist.at(index);
//                    int pos=classificationbtnlist.indexOf(classify);
//                    if(pos==-1)
//                        continue;
//                    else{
//                        QString rowstr=classificationbtnrowlist.at(pos);
//                        row=rowstr.toInt()+2;
//                        break;
//                    }
//                }
//                if(index<0)
//                    row=2;
//            }
//            insert_classification_btn(classify,row);
//            QWidget* wid=new QWidget;
//            QPushButton* btn=new QPushButton;
//            insert_appbtn_wid(wid,btn,name,row);
//        }
//        else{
//            int pos=classificationbtnlist.indexOf(classify);
//            QString rowstr=classificationbtnrowlist.at(pos);
//            int row=rowstr.toInt();
//            QPushButton* btn=new QPushButton;
//            create_app_btn(btn,name);
//            QWidget* wid=apptablewid->cellWidget(row+1,0);
//            wid->layout()->addWidget(btn);
//            int num=wid->layout()->count();
//            if(!is_fullscreen)
//            {
//                wid->setFixedSize(apptablewid->width()-14,num*42);
//                apptablewid->setRowHeight(row+1,num*42);
//            }
//            else{
//                int dividend=apptablewid->width()/312;
//                if(num%dividend>0)
//                    num=num/dividend+1;
//                else num=num/dividend;
//                wid->setFixedSize(apptablewid->width()-14,num*42);
//                apptablewid->setRowHeight(row+1,num*42);
//            }

//        }
//    }
//    else{
//        int row=0;
//        while(row<apptablewid->rowCount()/2)
//        {
//            QWidget* wid=apptablewid->cellWidget(row*2+1,0);
//            int count=wid->layout()->count();
//            for(int i=0;i<count;i++)
//            {
//                QLayoutItem* item=wid->layout()->itemAt(i);
//                QWidget* wid=item->widget();
//                QPushButton* btn=qobject_cast<QPushButton*>(wid);
//                QLayoutItem* labelitem=btn->layout()->itemAt(1);
//                QWidget* labelwid=labelitem->widget();
//                QLabel* label=qobject_cast<QLabel*>(labelwid);
//                QString cmpname=label->text();
//                if(QString::compare(name,cmpname)==0)
//                {
//                    wid->layout()->removeWidget(btn);
//                    btn->setParent(nullptr);
//                    delete btn;
//                    if(!is_fullscreen)
//                    {
//                        wid->setFixedSize(apptablewid->width()-14,(count-1)*42);
//                        apptablewid->setRowHeight(row+1,(count-1)*42);
//                    }
//                    else{
//                        int dividend=apptablewid->width()/312;
//                        int num=0;
//                        if((count-1)%dividend>0)
//                            num=(count-1)/dividend+1;
//                        else num=(count-1)/dividend;
//                        wid->setFixedSize(apptablewid->width()-14,num*42);
//                        apptablewid->setRowHeight(row+1,num*42);
//                    }
//                    break;
//                }
//            }
//        }
//    }
}

//void FunctionWidget::insert_classification_btn(QString btnname, int row)
//{
//    char btncolor[300];
//    sprintf(btncolor,"QPushButton{background:transparent;border:0px;color:#ffffff;font-size:14px;}\
//            QPushButton:hover{background-color:%s;}\
//            QPushButton:pressed{background-color:%s;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);

//    QPushButton* classificationbtn=new QPushButton;
//    classificationbtn->setStyleSheet(btncolor);
//    classificationbtn->setFocusPolicy(Qt::NoFocus);
//    QHBoxLayout* classificationlayout=new QHBoxLayout(classificationbtn);
//    classificationlayout->setContentsMargins(15,0,0,0);
//    classificationlayout->setSpacing(6);
//    QFrame* line=new QFrame(classificationbtn);
//    line->setFrameShape(QFrame::HLine);
//    line->setStyleSheet("background-color:#626c6e");
//    QLabel* classificationlabel=new QLabel(classificationbtn);
//    classificationlabel->setFixedSize(56,20);
//    classificationlabel->setAlignment(Qt::AlignCenter);
//    classificationlabel->setStyleSheet("color:#ffffff;font-size:14px;");
//    classificationlabel->setText(btnname);
//    classificationlayout->addWidget(line);
//    classificationlayout->addWidget(classificationlabel);
//    classificationbtn->setLayout(classificationlayout);

//    apptablewid->insertRow(row);
//    apptablewid->setCellWidget(row,0,classificationbtn);

//    classificationbtnlist.append(btnname);
//    classificationbtnrowlist.append(QString::number(row));

//    connect(classificationbtn, SIGNAL(clicked()), this,SLOT(app_classificationbtn_clicked_slot()));

//    if(!is_fullscreen)
//    {
//        classificationbtn->setFixedSize(apptablewid->width()-14,20);
//        classificationbtn->layout()->setContentsMargins(15,0,0,0);
//        classificationbtn->layout()->setSpacing(6);
//        apptablewid->setRowHeight(row,20);
//        line->setFixedSize(classificationbtn->width()-77,1);
//    }
//    else{
//        classificationbtn->setFixedSize(apptablewid->width()-32,20);
//        classificationbtn->layout()->setContentsMargins(0,0,0,0);
//        classificationbtn->layout()->setSpacing(12);
//        apptablewid->setRowHeight(row,20);
//        line->setFixedSize(classificationbtn->width()-62,1);
//    }
//}

void FunctionWidget::insert_appbtn_wid(QWidget *wid, QPushButton *btn, QString desktopfn, int row)
{
    create_app_btn(btn,desktopfn);
    apptablewid->insertRow(row);
    QFlowLayout* flowlayout=new QFlowLayout(0,0,0);
    wid->setLayout(flowlayout);
    flowlayout->addWidget(btn);
    apptablewid->insertRow(row);
    apptablewid->setCellWidget(row,0,wid);
    wid->setFixedSize(apptablewid->width()-14,42);
    apptablewid->setRowHeight(row,42);

}

void FunctionWidget::create_app_btn(QPushButton *btn, QString desktopfn)
{
    char btncolor[300];
    sprintf(btncolor,"QPushButton{background:transparent;border:0px;color:#ffffff;font-size:14px;padding-left:0px;text-align: left center;}\
            QPushButton:hover{background-color:%s;}\
            QPushButton:pressed{background-color:%s;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);

    btn->setFixedSize(330-14,42);
    btn->setStyleSheet(QString::fromLocal8Bit(btncolor));
    QHBoxLayout* layout=new QHBoxLayout(btn);
    layout->setContentsMargins(15,0,0,0);
    layout->setSpacing(15);

    QString iconstr=KylinStartMenuInterface::get_app_icon(QString("/usr/share/applications/"+desktopfn));
    QString appname=KylinStartMenuInterface::get_app_name(QString("/usr/share/applications/"+desktopfn));
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
 * 应用列表功能分类按钮槽函数
 */
void FunctionWidget::app_classificationbtn_clicked_slot()
{
    //加载FunctionButtonWidget界面
    is_functionbtnwid=true;
//    functionbtnwid=new FunctionButtonWidget(this);
//    connect(this,SIGNAL(send_classificationbtn_list(QStringList)),functionbtnwid,SLOT(recv_classificationbtn_list(QStringList)));
    emit send_classificationbtn_list(classificationbtnlist);
    mainLayout->removeWidget(applistWid);
    applistWid->setParent(nullptr);
    if(is_fullscreen)
    {
        mainLayout->removeWidget(iconlistWid);
        iconlistWid->setParent(nullptr);
        functionbtnwid->load_max_wid();
    }
    else
        functionbtnwid->load_min_wid();
    mainLayout->addWidget(functionbtnwid);

//    connect(functionbtnwid, SIGNAL(send_functionbtn_signal(QString)),this,SLOT(recv_functionbtn_signal(QString)));
}

/**
 * 接收FunctionButtonWidget界面按钮信号
 */
void FunctionWidget::recv_functionbtn_signal(QString btnname)
{
    is_functionbtnwid=false;
    mainLayout->removeWidget(functionbtnwid);
    functionbtnwid->setParent(nullptr);
//    if(functionbtnwid!=nullptr)
//    {

//        delete functionbtnwid;
//        functionbtnwid=nullptr;
//    }
    mainLayout->addWidget(applistWid);
    if(is_fullscreen)
    {
        mainLayout->addWidget(iconlistWid);
        int col=functionnamelist.indexOf(btnname);
        iconlisttableWid->selectColumn(col);
    }
    else{
        //此处需实现将功能为btnname的应用列表移动到applistWid界面最顶端
        int index=classificationbtnlist.indexOf(btnname);
        if(index!=-1)
        {
            int row=classificationbtnrowlist.at(index).toInt();
            apptablewid->verticalScrollBar()->setValue(row);
        }
    }
}

/**
 * 初始化图标列表界面
 */
void FunctionWidget::init_iconlist_widget()
{
    iconlistleftSpacer=new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);
    iconlistrightSpacer=new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);

    iconlistLayout=new QHBoxLayout(iconlistWid);
    iconlistLayout->setContentsMargins(0,0,0,0);
//    iconlistLayout->setSpacing(10);
    iconlistWid->setLayout(iconlistLayout);

    char btncolor[300];
    sprintf(btncolor,"QToolButton{background: transparent;border:0px;padding-left:0px;}\
            QToolButton:hover{background-color:%s;}\
            QToolButton:pressed{background-color:%s;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);
    leftbtn=new QToolButton(iconlistWid);
    leftbtn->setFixedSize(40,30);
    QSvgRenderer* leftsvgRender = new QSvgRenderer(leftbtn);
    leftsvgRender->load(QString(":/data/img/mainviewwidget/leftarrow.svg"));
    QPixmap* leftpixmap = new QPixmap(19,19);
    leftpixmap->fill(Qt::transparent);//设置背景透明
    QPainter leftp(leftpixmap);
    leftsvgRender->render(&leftp);
//    QPixmap leftpixmap(":/data/img/mainviewwidget/向左-箭头.svg");
//    leftbtn->setIconSize(leftpixmap.size());
    leftbtn->setIcon(QIcon(*leftpixmap));
    leftbtn->setStyleSheet(QString::fromLocal8Bit(btncolor));

    iconlisttableWid=new QTableWidget(iconlistWid);

    rightbtn=new QToolButton(iconlistWid);
    rightbtn->setFixedSize(40,30);
    QSvgRenderer* rightsvgRender = new QSvgRenderer(rightbtn);
    rightsvgRender->load(QString(":/data/img/mainviewwidget/rightarrow.svg"));
    QPixmap* rightpixmap = new QPixmap(19,19);
    rightpixmap->fill(Qt::transparent);//设置背景透明
    QPainter rightp(rightpixmap);
    rightsvgRender->render(&rightp);
//    QPixmap rightpixmap(":/data/img/mainviewwidget/向右-箭头.svg");
//    rightbtn->setIconSize(rightpixmap.size());
    rightbtn->setIcon(QIcon(*rightpixmap));
    rightbtn->setStyleSheet(QString::fromLocal8Bit(btncolor));

    iconlistLayout->addItem(iconlistleftSpacer);
    iconlistLayout->addWidget(leftbtn);
    iconlistLayout->addWidget(iconlisttableWid);
    iconlistLayout->addWidget(rightbtn);
    iconlistLayout->addItem(iconlistrightSpacer);
    init_iconlist_table();

    QWidget* wid=iconlisttableWid->cellWidget(0,0);
    QLabel* label=qobject_cast<QLabel*>(wid);
    iconlisttableWid->selectColumn(0);
    QPixmap pixmap(iconlightlist.at(0));
    label->setPixmap(pixmap);

    connect(leftbtn, SIGNAL(clicked()), this, SLOT(leftbtn_clicked_slot()));
    connect(rightbtn, SIGNAL(clicked()), this, SLOT(rightbtn_clicked_slot()));
    connect(iconlisttableWid,SIGNAL(itemSelectionChanged()),this,SLOT(iconlistitem_selected_slot()));

}

/**
 * 初始化图标列表界面数据表格iconlisttableWid
 */
void FunctionWidget::init_iconlist_table()
{
    iconlisttableWid->setFocusPolicy(Qt::NoFocus);
//    iconlisttableWid->setFixedSize(11*40+11*20,30);
    iconlisttableWid->setFixedSize(11*40,30);
    iconlisttableWid->setColumnCount(11);
    iconlisttableWid->setRowCount(1);
    iconlisttableWid->horizontalHeader()->setFixedHeight(0);
    iconlisttableWid->verticalHeader()->setDefaultSectionSize(30);
    iconlisttableWid->setEditTriggers(QAbstractItemView::NoEditTriggers);
    iconlisttableWid->setSelectionMode(QAbstractItemView::SingleSelection);
    iconlisttableWid->setSelectionBehavior(QAbstractItemView::SelectColumns);
    iconlisttableWid->setShowGrid(false);
    iconlisttableWid->setStyleSheet("QTableWidget{border:0px;background: transparent;}"
                                    "QTableWidget::Item{background: transparent;}\
                                     QTableWidget::Item:selected{background-color:rgba(14,19,22,92%);}");
//    iconlisttableWid->setStyleSheet("QTableWidget{border:0px;background: transparent;}"
//                                    "QTableWidget::Item{background: transparent;padding-left:10px;}\
//                                     QTableWidget::Item:selected{background-color:rgba(14,19,22,92%);}");
    QStringList header;
    for(int i=0;i<11;i++)
    {
        header.append("");
//        iconlisttableWid->setColumnWidth(i,60);
        iconlisttableWid->setColumnWidth(i,40);

    }
    iconlisttableWid->setHorizontalHeaderLabels(header);
    iconlisttableWid->verticalHeader()->setHidden(true);
    iconlisttableWid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    for(int i=0;i<11;i++)
    {
        QLabel* label=new QLabel;
        label->setFixedSize(40,30);
        char labelcolor[300];
        sprintf(labelcolor,"QLabel{background: transparent;}\
                QLabel:hover{background-color:%s;}\
                QLabel:pressed{background-color:%s;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);
        label->setStyleSheet(QString::fromLocal8Bit(labelcolor));
//        QPixmap pixmap(iconlist.at(i));
        QSvgRenderer* svgRender = new QSvgRenderer(label);
        svgRender->load(iconlist.at(i));
        QPixmap* pixmap = new QPixmap(19,19);
        pixmap->fill(Qt::transparent);//设置背景透明
        QPainter p(pixmap);
        svgRender->render(&p);
        label->setPixmap(*pixmap);
        label->setAlignment(Qt::AlignCenter);
        iconlisttableWid->setCellWidget(0,i,label);
    }

}

/**
 * 向左按钮槽函数
 */
void FunctionWidget::leftbtn_clicked_slot()
{
    int col=iconlisttableWid->currentColumn();
    if(col>0)
        iconlisttableWid->selectColumn(col-1);
}

/**
 * 向右按钮槽函数
 */
void FunctionWidget::rightbtn_clicked_slot()
{
    int col=iconlisttableWid->currentColumn();
    if(col<27)
        iconlisttableWid->selectColumn(col+1);
}

/**
 * 图标列表数据项被选定槽函数
 */
void FunctionWidget::iconlistitem_selected_slot()
{
    int col=iconlisttableWid->currentColumn();
    QWidget* wid=iconlisttableWid->cellWidget(0,col);
    QLabel* label=qobject_cast<QLabel*>(wid);
//    QPixmap pixmap(iconlightlist.at(col));
    QSvgRenderer* svgRender = new QSvgRenderer(label);
    svgRender->load(iconlightlist.at(col));
    QPixmap* pixmap = new QPixmap(19,19);
    pixmap->fill(Qt::transparent);//设置背景透明
    QPainter p(pixmap);
    svgRender->render(&p);
    label->setPixmap(*pixmap);
    label->setAlignment(Qt::AlignCenter);

    for(int i=0;i<col;i++)
    {
        QWidget* wid=iconlisttableWid->cellWidget(0,i);
        QLabel* label=qobject_cast<QLabel*>(wid);
//        QPixmap pixmap(iconlist.at(i));
        QSvgRenderer* svgRender = new QSvgRenderer(label);
        svgRender->load(iconlist.at(i));
        QPixmap* pixmap = new QPixmap(19,19);
        pixmap->fill(Qt::transparent);//设置背景透明
        QPainter p(pixmap);
        svgRender->render(&p);
        label->setPixmap(*pixmap);
        label->setAlignment(Qt::AlignCenter);
    }
    for(int i=col+1;i<11;i++)
    {
        QWidget* wid=iconlisttableWid->cellWidget(0,i);
        QLabel* label=qobject_cast<QLabel*>(wid);
//        QPixmap pixmap(iconlist.at(i));
        QSvgRenderer* svgRender = new QSvgRenderer(label);
        svgRender->load(iconlist.at(i));
        QPixmap* pixmap = new QPixmap(19,19);
        pixmap->fill(Qt::transparent);//设置背景透明
        QPainter p(pixmap);
        svgRender->render(&p);
        label->setPixmap(*pixmap);
        label->setAlignment(Qt::AlignCenter);
    }

    //此处需实现将被选定的功能图标所包含的应用列表移动到applistWid界面最顶端
    int index=classificationbtnlist.indexOf(functionnamelist.at(col));
    if(index!=-1)
    {
        int row=classificationbtnrowlist.at(index).toInt();
        apptablewid->verticalScrollBar()->setValue(row);
    }

}
