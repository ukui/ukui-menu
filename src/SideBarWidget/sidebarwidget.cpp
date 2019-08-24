#include "sidebarwidget.h"
#include "ui_sidebarwidget.h"
#include <QDebug>
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
}

/**
 * 侧边栏初始化
 */
void SideBarWidget::init_widget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setStyleSheet("border:0px;background:transparent;");
//    this->setStyleSheet("border:0px;background-color:rgba(40,49,56,92%);");

    add_sidebar_btn();
    load_min_sidebar();
}

/**
 * 侧边栏添加控件
 */
void SideBarWidget::add_sidebar_btn()
{
    mainlayout=new QVBoxLayout;
    mainlayout->setContentsMargins(0,0,0,0);
    mainlayout->setSpacing(0);
    userwid=new QWidget;
    char userwidcolor[100];
    sprintf(userwidcolor,"border:0px;background-color:%s;",SIDEBARWIDGETCOLOR);
    userwid->setStyleSheet(QString::fromLocal8Bit(userwidcolor));
    commonusebtn=new QToolButton;
    commonusebtn->setFocusPolicy(Qt::ClickFocus);
    commonusebtn->installEventFilter(this);
    set_btn_style(commonusebtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/icon-常用软件.svg"));
    letterbtn=new QToolButton;
    letterbtn->setFocusPolicy(Qt::ClickFocus);
    letterbtn->installEventFilter(this);
    set_btn_style(letterbtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/icon-按字母.svg"));
    functionbtn=new QToolButton;
    functionbtn->setFocusPolicy(Qt::ClickFocus);
    functionbtn->installEventFilter(this);
    set_btn_style(functionbtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/icon-按功能.svg"));
    computerbtn=new QToolButton;
    set_btn_style(computerbtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/icon-我的电脑.svg"));
    controlbtn=new QToolButton;
    set_btn_style(controlbtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/icon-控制面板.svg"));
    shutdownbtn=new QToolButton;
    set_btn_style(shutdownbtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/icon-关机.svg"));

    commonusebtnname=new QLabel;
    commonusebtnname->setText(tr("常用软件"));
    commonusebtnname->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
    commonusebtn->layout()->addWidget(commonusebtnname);

    letterbtnname=new QLabel;
    letterbtnname->setText(tr("字母排序"));
    letterbtnname->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
    letterbtn->layout()->addWidget(letterbtnname);

    functionbtnname=new QLabel;
    functionbtnname->setText(tr("功能分类"));
    functionbtnname->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
    functionbtn->layout()->addWidget(functionbtnname);

    computerbtnname=new QLabel;
    computerbtnname->setText(tr("我的电脑"));
    computerbtnname->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
    computerbtn->layout()->addWidget(computerbtnname);

    controlbtnname=new QLabel;
    controlbtnname->setText(tr("控制面板"));
    controlbtnname->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
    controlbtn->layout()->addWidget(controlbtnname);

    shutdownbtnname=new QLabel;
    shutdownbtnname->setText(tr("关机"));
    shutdownbtnname->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
    shutdownbtn->layout()->addWidget(shutdownbtnname);

    labelarrow=new QLabel;
    QPixmap pixmaparrow(QString(":/data/img/sidebarwidget/向右-箭头.svg"));
    labelarrow->setFixedSize(pixmaparrow.size());
    labelarrow->setPixmap(pixmaparrow);
    labelarrow->setStyleSheet("QLabel{background:transparent;}");
    shutdownbtn->layout()->addWidget(labelarrow);

//    verticalSpacer = new QSpacerItem(20,40, QSizePolicy::Fixed, QSizePolicy::Expanding);
    spaceWid=new QWidget;
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
void SideBarWidget::set_btn_style(QToolButton *btn, QString btnicon)
{
    char btncolor[300];
    sprintf(btncolor,"QToolButton{background-color:%s;border:0px;padding-left:0;}\
            QToolButton:hover{background-color:#414e59;}\
            QToolButton:pressed{background-color:#37424b;}", SIDEBARWIDGETCOLOR);
    btn->setStyleSheet(btncolor);
    btnlayout=new QHBoxLayout;
    QLabel* labelicon=new QLabel;
    QPixmap pixmapicon(btnicon);
    labelicon->setStyleSheet("QLabel{background:transparent;}");
    labelicon->setFixedSize(pixmapicon.size());
    labelicon->setPixmap(pixmapicon);
    btnlayout->addWidget(labelicon);
    btn->setLayout(btnlayout);

}

/**
 * 用户图像界面初始化
 */
void SideBarWidget::init_usericon_widget()
{
    usericonwid=new QWidget;
    usericonwid->setStyleSheet("border:0px;background:transparent;");
    usericonlayout=new QHBoxLayout;
    usericonlayout->setContentsMargins(0,0,0,0);
    usericonbtn=new QToolButton;
    usericonlayout->addWidget(usericonbtn);
    usericonwid->setLayout(usericonlayout);

    usernamewid=new QWidget;
    usernamewid->setStyleSheet("border:0px;background:transparent;");
    usernamewid->setFixedSize(160,18);
    usernamelayout=new QHBoxLayout;
    usernamelayout->setContentsMargins(0,0,0,0);
    usernamelabel=new QLabel;
    usernamelabel->setText("UKylin");
    usernamelabel->adjustSize();
    usernamelabel->setFixedSize(160,18);
    usernamelabel->setStyleSheet("background:transparent;color:#ffffff;font-size:18px;");
    usernamelabel->setAlignment(Qt::AlignCenter);
    usernamelayout->addWidget(usernamelabel);
    usernamewid->setLayout(usernamelayout);

    userLayout=new QVBoxLayout;
    userLayout->addWidget(usericonwid);
    userLayout->addWidget(usernamewid);
    userwid->setLayout(userLayout);
}

/**
 * 加载默认侧边栏
 */
void SideBarWidget::load_min_sidebar()
{
    this->setGeometry(0,QApplication::desktop()->availableGeometry().height()-532,60,532);

    //移除用户名窗口
    userLayout->removeWidget(usernamewid);
    usernamewid->setParent(NULL);

    userwid->setFixedSize(60,75);
    userLayout->setContentsMargins(0,15,0,24);
    usericonwid->setFixedSize(60,36);
    QPixmap pixmap(QString(":/data/img/sidebarwidget/用户头像-默认-36x36.png"));
    usericonbtn->setFixedSize(pixmap.size());
    usericonbtn->setStyleSheet("QToolButton{background:transparent;border:0px;image:url(:/data/img/sidebarwidget/用户头像-默认-36x36.png)}\
                               QToolButton:hover{background-color:#414e59;}\
                               QToolButton:pressed{background-color:#37424b;}");

    set_min_sidebar_btn(commonusebtn);
    set_min_sidebar_btn(letterbtn);
    set_min_sidebar_btn(functionbtn);
    set_min_sidebar_btn(computerbtn);
    set_min_sidebar_btn(controlbtn);
    set_min_sidebar_btn(shutdownbtn);

    spaceWid->setFixedSize(this->width(),this->height()-userwid->height()-6*commonusebtn->height());

    //移除关机按钮中的向右箭头
    shutdownbtn->layout()->removeWidget(labelarrow);
    labelarrow->setParent(NULL);

    disconnect(commonusebtn, SIGNAL(clicked()), this, SIGNAL(send_fullscreen_commonusebtn_signal()));
    disconnect(letterbtn,SIGNAL(clicked()), this, SIGNAL(send_fullscreen_letterbtn_signal()));
    disconnect(functionbtn,SIGNAL(clicked()), this, SIGNAL(send_fullscreen_functionbtn_signal()));
    connect(commonusebtn, SIGNAL(clicked()), this, SIGNAL(send_commonusebtn_signal()));
    connect(letterbtn,SIGNAL(clicked()), this, SIGNAL(send_letterbtn_signal()));
    connect(functionbtn,SIGNAL(clicked()), this, SIGNAL(send_functionbtn_signal()));
}

/**
 * 设置默认侧边栏按钮
 */
void SideBarWidget::set_min_sidebar_btn(QToolButton* btn)
{
    btn->setFixedSize(60,48);
    btn->layout()->setContentsMargins(0,0,0,0);
    btn->layout()->setSpacing(0);
    //移除按钮文本
    QLayoutItem *child;
    if((child = btn->layout()->takeAt(1)) != 0) {
        QWidget* childwid=child->widget();
        if(childwid!=NULL)
        {
            btn->layout()->removeWidget(childwid);
            childwid->setParent(NULL);
        }
    }
}

/**
 * 加载全屏侧边栏
 */
void SideBarWidget::load_max_sidebar()
{
    this->setGeometry(0,0,160,QApplication::desktop()->availableGeometry().height());

    userLayout->addWidget(usernamewid);

    userwid->setFixedSize(160,200);
    userLayout->setContentsMargins(0,45,0,72);
    userLayout->setSpacing(15);
    usericonwid->setFixedSize(160,50);
    QPixmap pixmap(QString(":/data/img/sidebarwidget/用户头像-全屏-50x50.png"));
    usericonbtn->setFixedSize(pixmap.size());
    usericonbtn->setStyleSheet("QToolButton{background:transparent;border:0px;image:url(:/data/img/sidebarwidget/用户头像-全屏-50x50.png)}\
                               QToolButton:hover{background-color:#414e59;}\
                               QToolButton:pressed{background-color:#37424b;}");

    set_max_sidebar_btn(commonusebtn);
    set_max_sidebar_btn(letterbtn);
    set_max_sidebar_btn(functionbtn);
    set_max_sidebar_btn(computerbtn);
    set_max_sidebar_btn(controlbtn);
    set_max_sidebar_btn(shutdownbtn);

    spaceWid->setFixedSize(this->width(),this->height()-userwid->height()-6*commonusebtn->height());

    commonusebtn->layout()->addWidget(commonusebtnname);
    letterbtn->layout()->addWidget(letterbtnname);
    functionbtn->layout()->addWidget(functionbtnname);
    computerbtn->layout()->addWidget(computerbtnname);
    controlbtn->layout()->addWidget(controlbtnname);
    shutdownbtn->layout()->setContentsMargins(25,0,15,0);
    shutdownbtn->layout()->addWidget(shutdownbtnname);
    shutdownbtn->layout()->addWidget(labelarrow);


    disconnect(commonusebtn, SIGNAL(clicked()), this, SIGNAL(send_commonusebtn_signal()));
    disconnect(letterbtn,SIGNAL(clicked()), this, SIGNAL(send_letterbtn_signal()));
    disconnect(functionbtn,SIGNAL(clicked()), this, SIGNAL(send_functionbtn_signal()));
    connect(commonusebtn, SIGNAL(clicked()), this, SIGNAL(send_fullscreen_commonusebtn_signal()));
    connect(letterbtn,SIGNAL(clicked()), this, SIGNAL(send_fullscreen_letterbtn_signal()));
    connect(functionbtn,SIGNAL(clicked()), this, SIGNAL(send_fullscreen_functionbtn_signal()));

}

/**
 * 设置全屏侧边栏按钮
 */
void SideBarWidget::set_max_sidebar_btn(QToolButton *btn)
{
    btn->setFixedSize(160,48);
    btn->layout()->setContentsMargins(25,0,0,0);
    btn->layout()->setSpacing(15);
}

/**
 * 按钮焦点事件过滤
 */
bool SideBarWidget::eventFilter(QObject *watched, QEvent *event)
{
    char style[300];
    sprintf(style,"QToolButton{background-color:%s;border:0px;padding-left:0;}\
            QToolButton:hover{background-color:#414e59;}\
            QToolButton:pressed{background-color:#37424b;}", SIDEBARWIDGETCOLOR);

    char pressstyle[200];
    sprintf(pressstyle,"QToolButton{background-color:%s;border:0px;padding-left:0;}",MAINVIEWWIDGETCOLOR);
//    QString pressstyle="QToolButton{background-color:rgba(14,19,22,92%);border:0px;padding-left:0;}";

    if(watched==commonusebtn)
    {
        if(event->type()==QEvent::FocusIn)
            commonusebtn->setStyleSheet(QString::fromLocal8Bit(pressstyle));
        else if(event->type()==QEvent::FocusOut){
            commonusebtn->setStyleSheet(QString::fromLocal8Bit(style));

        }
    }
    else if(watched==letterbtn)
    {
        if(event->type()==QEvent::FocusIn)
            letterbtn->setStyleSheet(QString::fromLocal8Bit(pressstyle));
        else if(event->type()==QEvent::FocusOut){
            letterbtn->setStyleSheet(QString::fromLocal8Bit(style));

        }
    }
    else if(watched==functionbtn)
    {
        if(event->type()==QEvent::FocusIn)
            functionbtn->setStyleSheet(QString::fromLocal8Bit(pressstyle));
        else if(event->type()==QEvent::FocusOut){
            functionbtn->setStyleSheet(QString::fromLocal8Bit(style));

        }

    }
    //屏蔽按键
    if(event->type()==QEvent::KeyPress)
        return true;

    return QWidget::eventFilter(watched,event);
}

/**
 * 设置主界面从全屏回到默认状态时侧边栏按钮焦点状态
 */
void SideBarWidget::set_sidebarbtn_state(int arg)
{
    if(arg==1)
    {
        commonusebtn->setFocus();
    }
    else if(arg==2)
    {
        letterbtn->setFocus();
    }
    else if(arg==3)
    {
        functionbtn->setFocus();
    }
}
