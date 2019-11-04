#include "sidebarwidget.h"
#include "ui_sidebarwidget.h"
#include <QDebug>
#include <QSvgRenderer>
#include <QPainter>
#include "src/color.h"

SideBarWidget::SideBarWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SideBarWidget)
{
    ui->setupUi(this);
    init_widget();
}

SideBarWidget::~SideBarWidget()
{
    delete ui;
    delete shutdownmenu;
    delete [] othermenu;
}

/**
 * 侧边栏初始化
 */
void SideBarWidget::init_widget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setStyleSheet("border:0px;background:transparent;");
    this->setFocusPolicy(Qt::NoFocus);

    add_sidebar_btn();
    load_min_sidebar();
}

/**
 * 侧边栏添加控件
 */
void SideBarWidget::add_sidebar_btn()
{
    mainlayout=new QVBoxLayout(this);
    mainlayout->setContentsMargins(0,0,0,0);
    mainlayout->setSpacing(0);
    userwid=new QWidget(this);
    char userwidcolor[100];
    sprintf(userwidcolor,"border:0px;background-color:%s;",SIDEBARWIDGETCOLOR);
    userwid->setStyleSheet(QString::fromLocal8Bit(userwidcolor));
    commonusebtn=new QPushButton(this);
//    commonusebtn->setFocusPolicy(Qt::NoFocus);
//    commonusebtn->installEventFilter(this);
    set_btn_style(commonusebtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/commonuse.svg"));
    letterbtn=new QPushButton(this);
//    letterbtn->setFocusPolicy(Qt::NoFocus);
//    letterbtn->installEventFilter(this);
    set_btn_style(letterbtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/letter.svg"));
    functionbtn=new QPushButton(this);
//    functionbtn->setFocusPolicy(Qt::NoFocus);
//    functionbtn->installEventFilter(this);
    set_btn_style(functionbtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/function.svg"));
    computerbtn=new QPushButton(this);
    set_btn_style(computerbtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/computer.svg"));
    controlbtn=new QPushButton(this);
    set_btn_style(controlbtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/control.svg"));
    shutdownbtn=new QPushButton(this);
    set_btn_style(shutdownbtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/shutdown.svg"));
    connect(commonusebtn,SIGNAL(clicked()),this,SLOT(commonusebtn_clicked_slot()));
    connect(letterbtn,SIGNAL(clicked()),this,SLOT(letterbtn_clicked_slot()));
    connect(functionbtn,SIGNAL(clicked()),this,SLOT(functionbtn_clicked_slot()));
    connect(computerbtn,SIGNAL(clicked()),this,SLOT(computerbtn_click_slot()));
    connect(controlbtn,SIGNAL(clicked()),this,SLOT(controlbtn_click_slot()));
    connect(shutdownbtn,SIGNAL(clicked()),this,SLOT(shutdownbtn_click_slot()));

    commonusebtnname=new QLabel;
    commonusebtnname->setText(tr("常用软件"));
    commonusebtnname->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
    commonusebtnname->adjustSize();
    commonusebtn->layout()->addWidget(commonusebtnname);

    letterbtnname=new QLabel;
    letterbtnname->setText(tr("字母排序"));
    letterbtnname->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
    letterbtnname->adjustSize();
    letterbtn->layout()->addWidget(letterbtnname);

    functionbtnname=new QLabel(functionbtn);
    functionbtnname->setText(tr("功能分类"));
    functionbtnname->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
    functionbtnname->adjustSize();
    functionbtn->layout()->addWidget(functionbtnname);

    computerbtnname=new QLabel(computerbtn);
    computerbtnname->setText(tr("我的电脑"));
    computerbtnname->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
    computerbtnname->adjustSize();
    computerbtn->layout()->addWidget(computerbtnname);

    controlbtnname=new QLabel(controlbtn);
    controlbtnname->setText(tr("控制面板"));
    controlbtnname->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
    controlbtnname->adjustSize();
    controlbtn->layout()->addWidget(controlbtnname);

    shutdownbtnname=new QLabel(shutdownbtn);
    shutdownbtnname->setText(tr("关机"));
    shutdownbtnname->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
    shutdownbtnname->adjustSize();
    shutdownbtn->layout()->addWidget(shutdownbtnname);


    add_right_click_menu(computerbtn);
    add_right_click_menu(controlbtn);
    shutdownbtn->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(shutdownbtn,SIGNAL(customContextMenuRequested(const QPoint&)),this,
            SLOT(shutdownbtn_right_click_slot()));

//    QSvgRenderer* svg=new QSvgRenderer(this);
//    svg->load(QString(":/data/img/sidebarwidget/rightarrow.svg"));
//    QPixmap* pixmaparrow=new QPixmap(19,19);
//    pixmaparrow->fill(Qt::transparent);
//    QPainter p(pixmaparrow);
//    svg->render(&p);
//    labelarrow=new QLabel;
//    labelarrow->setFixedSize(pixmaparrow->size());
//    labelarrow->setPixmap(*pixmaparrow);
//    labelarrow->setStyleSheet("QLabel{background:transparent;}");
//    shutdownbtn->layout()->addWidget(labelarrow);

//    verticalSpacer = new QSpacerItem(20,40, QSizePolicy::Fixed, QSizePolicy::Expanding);
    spaceWid=new QWidget(this);
    char spaceWidcolor[100];
    sprintf(spaceWidcolor,"border:0px;background-color:%s;",SIDEBARWIDGETCOLOR);
    spaceWid->setStyleSheet(QString::fromLocal8Bit(spaceWidcolor));
    mainlayout->addWidget(userwid);
    mainlayout->addWidget(commonusebtn);
    mainlayout->addWidget(letterbtn);
    mainlayout->addWidget(functionbtn);
//    mainlayout->addItem(verticalSpacer);
    mainlayout->addWidget(spaceWid);
    mainlayout->addWidget(computerbtn);
    mainlayout->addWidget(controlbtn);
    mainlayout->addWidget(shutdownbtn);
    this->setLayout(mainlayout);

    init_usericon_widget();
}

/**
 * 设置按钮样式
 */
void SideBarWidget::set_btn_style(QPushButton *btn, QString btnicon)
{
    char btncolor[300];
    sprintf(btncolor,"QPushButton{background-color:%s;border:0px;padding-left:0;}\
            QPushButton:hover{background-color:%s;}\
            QPushButton:pressed{background-color:%s;}", SIDEBARWIDGETCOLOR,SIDEBARBTNHOVER,SIDEBARBTNPRESSED);
    btn->setStyleSheet(QString::fromLocal8Bit(btncolor));
    btnlayout=new QHBoxLayout(btn);

    QSvgRenderer* svgRender = new QSvgRenderer(btn);
    svgRender->load(btnicon);
    QPixmap* pixmap = new QPixmap(19,19);
    pixmap->fill(Qt::transparent);//设置背景透明
    QPainter p(pixmap);
    svgRender->render(&p);

    labelicon=new QLabel;
    labelicon->setPixmap(*pixmap);
    labelicon->setAlignment(Qt::AlignCenter);
//    QPixmap pixmapicon(btnicon);
    labelicon->setStyleSheet("QLabel{background:transparent;}");
    labelicon->setFixedSize(pixmap->size());
    btnlayout->addWidget(labelicon);
    btn->setLayout(btnlayout);
    btn->setFocusPolicy(Qt::NoFocus);

}

/**
 * 用户图像界面初始化
 */
void SideBarWidget::init_usericon_widget()
{
    usericonwid=new QWidget(userwid);
    usericonwid->setStyleSheet("border:0px;background:transparent;");
    usericonlayout=new QHBoxLayout(usericonwid);
    usericonlayout->setContentsMargins(0,0,0,0);
    usericonbtn=new QToolButton(usericonwid);
    usericonlayout->addWidget(usericonbtn);
    usericonwid->setLayout(usericonlayout);

    usernamewid=new QWidget(userwid);
    usernamewid->setStyleSheet("border:0px;background:transparent;");
    usernamewid->setFixedSize(160,18);
    usernamelayout=new QHBoxLayout(usernamewid);
    usernamelayout->setContentsMargins(0,0,0,0);
    usernamelabel=new QLabel(usernamewid);
    usernamelabel->setText("UKylin");
    usernamelabel->adjustSize();
    usernamelabel->setFixedSize(160,18);
    usernamelabel->setStyleSheet("background:transparent;color:#ffffff;font-size:18px;");
    usernamelabel->setAlignment(Qt::AlignCenter);
    usernamelayout->addWidget(usernamelabel);
    usernamewid->setLayout(usernamelayout);

    userLayout=new QVBoxLayout(userwid);
    userLayout->addWidget(usericonwid);
    userLayout->addWidget(usernamewid);
    userwid->setLayout(userLayout);
}

/**
 * 加载关机按钮右键菜单
 */
void SideBarWidget::shutdownbtn_right_click_slot()
{
    shutdownmenu=new RightClickMenu;
    int ret=shutdownmenu->show_shutdown_menu();
    if(ret>=10 && ret<=14)
    {
        emit send_hide_mainwindow_signal();
        system("ukui-session-tools");
    }
}

void SideBarWidget::add_right_click_menu(QPushButton *btn)
{
    btn->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(btn,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(otherbtn_right_click_slot()));
}

void SideBarWidget::otherbtn_right_click_slot()
{
    othermenu=new RightClickMenu;
    othermenu->show_other_menu();
}

/**
 * 加载默认侧边栏
 */
void SideBarWidget::load_min_sidebar()
{
    is_fullscreen=false;

    this->setGeometry(0,QApplication::desktop()->availableGeometry().height()-532,60,532);

    //移除用户名窗口
    userLayout->removeWidget(usernamewid);
    usernamewid->setParent(nullptr);

    userwid->setFixedSize(60,75);
    userLayout->setContentsMargins(0,15,0,24);
    usericonwid->setFixedSize(60,36);
    QPixmap pixmap(QString(":/data/img/sidebarwidget/usericon.png"));
    usericonbtn->setFixedSize(pixmap.size());
    char btncolor[300];
    sprintf(btncolor,"QToolButton{background:transparent;border:0px;image:url(:/data/img/sidebarwidget/usericon.png)}\
            QToolButton:hover{background-color:%s;}\
            QToolButton:pressed{background-color:%s;}",SIDEBARBTNHOVER,SIDEBARBTNPRESSED);
    usericonbtn->setStyleSheet(QString::fromLocal8Bit(btncolor));

    set_min_sidebar_btn(commonusebtn);
    set_min_sidebar_btn(letterbtn);
    set_min_sidebar_btn(functionbtn);
    set_min_sidebar_btn(computerbtn);
    set_min_sidebar_btn(controlbtn);
    set_min_sidebar_btn(shutdownbtn);

    spaceWid->setFixedSize(this->width(),this->height()-userwid->height()-6*commonusebtn->height());

    //移除关机按钮中的向右箭头
//    shutdownbtn->layout()->removeWidget(labelarrow);
//    labelarrow->setParent(nullptr);

//    disconnect(commonusebtn, SIGNAL(clicked()), this, SIGNAL(send_fullscreen_commonusebtn_signal()));
//    disconnect(letterbtn,SIGNAL(clicked()), this, SIGNAL(send_fullscreen_letterbtn_signal()));
//    disconnect(functionbtn,SIGNAL(clicked()), this, SIGNAL(send_fullscreen_functionbtn_signal()));
//    connect(commonusebtn, SIGNAL(clicked()), this, SIGNAL(send_commonusebtn_signal()));
//    connect(letterbtn,SIGNAL(clicked()), this, SIGNAL(send_letterbtn_signal()));
//    connect(functionbtn,SIGNAL(clicked()), this, SIGNAL(send_functionbtn_signal()));
}

/**
 * 设置默认侧边栏按钮
 */
void SideBarWidget::set_min_sidebar_btn(QPushButton* btn)
{
    btn->setFixedSize(60,48);
    btn->layout()->setContentsMargins(0,0,0,0);
    btn->layout()->setSpacing(0);
    //移除按钮文本
    QLayoutItem *child;
    if((child = btn->layout()->takeAt(1)) != nullptr) {
        QWidget* childwid=child->widget();
        if(childwid!=nullptr)
        {
            btn->layout()->removeWidget(childwid);
            childwid->setParent(nullptr);
        }
    }
}

/**
 * 加载全屏侧边栏
 */
void SideBarWidget::load_max_sidebar()
{
    is_fullscreen=true;

    this->setGeometry(0,0,160,QApplication::desktop()->availableGeometry().height());

    userLayout->addWidget(usernamewid);

    userwid->setFixedSize(160,200);
    userLayout->setContentsMargins(0,45,0,72);
    userLayout->setSpacing(15);
    usericonwid->setFixedSize(160,50);
    QPixmap pixmap(QString(":/data/img/sidebarwidget/fullusericon.png"));
    usericonbtn->setFixedSize(pixmap.size());
    char btncolor[300];
    sprintf(btncolor,"QToolButton{background:transparent;border:0px;image:url(:/data/img/sidebarwidget/fullusericon.png)}\
            QToolButton:hover{background-color:%s;}\
            QToolButton:pressed{background-color:%s;}",SIDEBARBTNHOVER,SIDEBARBTNPRESSED);
    usericonbtn->setStyleSheet(QString::fromLocal8Bit(btncolor));

    spaceWid->setFixedSize(this->width(),this->height()-userwid->height()-6*commonusebtn->height());

    commonusebtn->layout()->addWidget(commonusebtnname);
    letterbtn->layout()->addWidget(letterbtnname);
    functionbtn->layout()->addWidget(functionbtnname);
    computerbtn->layout()->addWidget(computerbtnname);
    controlbtn->layout()->addWidget(controlbtnname);
//    shutdownbtn->layout()->setContentsMargins(25,0,15,0);
    shutdownbtn->layout()->addWidget(shutdownbtnname);
//    shutdownbtn->layout()->addWidget(labelarrow);

    set_max_sidebar_btn(commonusebtn);
    set_max_sidebar_btn(letterbtn);
    set_max_sidebar_btn(functionbtn);
    set_max_sidebar_btn(computerbtn);
    set_max_sidebar_btn(controlbtn);
    set_max_sidebar_btn(shutdownbtn);

//    disconnect(commonusebtn, SIGNAL(clicked()), this, SIGNAL(send_commonusebtn_signal()));
//    disconnect(letterbtn,SIGNAL(clicked()), this, SIGNAL(send_letterbtn_signal()));
//    disconnect(functionbtn,SIGNAL(clicked()), this, SIGNAL(send_functionbtn_signal()));
//    connect(commonusebtn, SIGNAL(clicked()), this, SIGNAL(send_fullscreen_commonusebtn_signal()));
//    connect(letterbtn,SIGNAL(clicked()), this, SIGNAL(send_fullscreen_letterbtn_signal()));
//    connect(functionbtn,SIGNAL(clicked()), this, SIGNAL(send_fullscreen_functionbtn_signal()));

}

/**
 * 设置全屏侧边栏按钮
 */
void SideBarWidget::set_max_sidebar_btn(QPushButton *btn)
{
    btn->setFixedSize(160,48);
    QLayoutItem* item=btn->layout()->itemAt(1);
    QWidget* wid=item->widget();
    QLabel* label=qobject_cast<QLabel*>(wid);
    int len=label->text().length();
//    btn->layout()->setContentsMargins(25,0,btn->width()-40-labelicon->width()-letterbtnname->width(),0);
    btn->layout()->setContentsMargins(25,0,btn->width()-40-labelicon->width()-len*14,0);
    btn->layout()->setSpacing(15);
}

/**
 * 按钮焦点事件过滤
 */
//bool SideBarWidget::eventFilter(QObject *watched, QEvent *event)
//{
//    char style[300];
//    sprintf(style,"QToolButton{background-color:%s;border:0px;padding-left:0;}\
//            QToolButton:hover{background-color:%s;}\
//            QToolButton:pressed{background-color:%s;}", SIDEBARWIDGETCOLOR,SIDEBARBTNHOVER,SIDEBARBTNPRESSED);

//    char pressstyle[200];
//    sprintf(pressstyle,"QToolButton{background-color:%s;border:0px;padding-left:0;}",MAINVIEWWIDGETCOLOR);

//    if(watched==commonusebtn)
//    {
//        if(event->type()==QEvent::FocusIn)
//        {
//            commonusebtn->setStyleSheet(QString::fromLocal8Bit(pressstyle));
//        }
//        else if(event->type()==QEvent::FocusOut){
//            commonusebtn->setStyleSheet(QString::fromLocal8Bit(style));

//        }
//    }
//    else if(watched==letterbtn)
//    {
//        if(event->type()==QEvent::FocusIn)
//        {
//            letterbtn->setStyleSheet(QString::fromLocal8Bit(pressstyle));
//        }
//        else if(event->type()==QEvent::FocusOut){
//            letterbtn->setStyleSheet(QString::fromLocal8Bit(style));

//        }
//    }
//    else if(watched==functionbtn)
//    {
//        if(event->type()==QEvent::FocusIn)
//        {
//            functionbtn->setStyleSheet(QString::fromLocal8Bit(pressstyle));
//        }
//        else if(event->type()==QEvent::FocusOut){
//            functionbtn->setStyleSheet(QString::fromLocal8Bit(style));

//        }

//    }
//    //屏蔽按键
//    if(event->type()==QEvent::KeyPress)
//        return true;

//    return QWidget::eventFilter(watched,event);
//}

//void SideBarWidget::mousePressEvent(QMouseEvent *e)
//{
//    if(e->button()==Qt::RightButton)
//    {
//        if(commonusebtn->hasFocus())
//        {
//            if(is_fullscreen)
//                emit send_fullscreen_commonusebtn_signal();
//            else emit send_commonusebtn_signal();

//        }
//        else if(letterbtn->hasFocus())
//        {
//            if(is_fullscreen)
//                emit send_fullscreen_letterbtn_signal();
//            else emit send_letterbtn_signal();
//        }
//        else if(functionbtn->hasFocus())
//        {
//            if(is_fullscreen)
//                emit send_fullscreen_functionbtn_signal();
//            else emit send_functionbtn_signal();
//        }

//    }
//}

void SideBarWidget::commonusebtn_clicked_slot()
{
    char style[300];
    sprintf(style,"QPushButton{background-color:%s;border:0px;padding-left:0;}\
            QPushButton:hover{background-color:%s;}\
            QPushButton:pressed{background-color:%s;}", SIDEBARWIDGETCOLOR,SIDEBARBTNHOVER,SIDEBARBTNPRESSED);

            char pressstyle[200];
    sprintf(pressstyle,"QPushButton{background-color:%s;border:0px;padding-left:0;}",MAINVIEWWIDGETCOLOR);

    commonusebtn->setStyleSheet(pressstyle);
    letterbtn->setStyleSheet(style);
    functionbtn->setStyleSheet(style);

//    if(is_fullscreen)
//        emit send_fullscreen_commonusebtn_signal();
    emit send_commonusebtn_signal();

}

void SideBarWidget::letterbtn_clicked_slot()
{
    char style[300];
    sprintf(style,"QPushButton{background-color:%s;border:0px;padding-left:0;}\
            QPushButton:hover{background-color:%s;}\
            QPushButton:pressed{background-color:%s;}", SIDEBARWIDGETCOLOR,SIDEBARBTNHOVER,SIDEBARBTNPRESSED);

            char pressstyle[200];
    sprintf(pressstyle,"QPushButton{background-color:%s;border:0px;padding-left:0;}",MAINVIEWWIDGETCOLOR);
    letterbtn->setStyleSheet(pressstyle);
    commonusebtn->setStyleSheet(style);
    functionbtn->setStyleSheet(style);

//    if(is_fullscreen)
//        emit send_fullscreen_letterbtn_signal();
    emit send_letterbtn_signal();

}

void SideBarWidget::functionbtn_clicked_slot()
{
    char style[300];
    sprintf(style,"QPushButton{background-color:%s;border:0px;padding-left:0;}\
            QPushButton:hover{background-color:%s;}\
            QPushButton:pressed{background-color:%s;}", SIDEBARWIDGETCOLOR,SIDEBARBTNHOVER,SIDEBARBTNPRESSED);

    char pressstyle[200];
    sprintf(pressstyle,"QPushButton{background-color:%s;border:0px;padding-left:0;}",MAINVIEWWIDGETCOLOR);
    functionbtn->setStyleSheet(pressstyle);
    commonusebtn->setStyleSheet(style);
    letterbtn->setStyleSheet(style);

//    if(is_fullscreen)
//        emit send_fullscreen_functionbtn_signal();
    emit send_functionbtn_signal();
}

void SideBarWidget::computerbtn_click_slot()
{
    emit send_hide_mainwindow_signal();
    QString execpath=KylinStartMenuInterface::get_app_exec(QString("/usr/share/applications/peony-computer.desktop"));
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

void SideBarWidget::controlbtn_click_slot()
{
    emit send_hide_mainwindow_signal();
    QString execpath=KylinStartMenuInterface::get_app_exec(QString("/usr/share/applications/ukui-control-center.desktop"));
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

void SideBarWidget::shutdownbtn_click_slot()
{
    emit send_hide_mainwindow_signal();
    system("ukui-session-tools");
}

/**
 * 设置主界面从全屏回到默认状态时侧边栏按钮焦点状态
 */
void SideBarWidget::set_sidebarbtn_state(int arg)
{
    if(arg==1)
    {
        commonusebtn_clicked_slot();
    }
    else if(arg==2)
    {
        letterbtn_clicked_slot();
    }
    else if(arg==3)
    {
        functionbtn_clicked_slot();
    }
}
