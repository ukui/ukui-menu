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
    letterbtn->click();
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
//    this->setStyleSheet("border:0px;background:transparent;");
    char style[100];
    sprintf(style, "border:0px;background-color:%s;",DefaultBackground);
    this->setStyleSheet(QString::fromLocal8Bit(style));
    this->setFocusPolicy(Qt::NoFocus);

    add_sidebar_btn();
    load_min_sidebar();

    //全屏侧边栏悬浮动画
    pEnterAnimation=new QPropertyAnimation;
    pEnterAnimation->setTargetObject(pMainWidget);
    pEnterAnimation->setPropertyName("pos");
    pEnterAnimation->setDuration(500);
//    pAnimation->setStartValue(QRect(390,QApplication::desktop()->availableGeometry().height()-532,390,532));
//    pAnimation->setEndValue(QRect(490,QApplication::desktop()->availableGeometry().height()-532,490,532));
//    pAnimation->setStartValue(QRect(100,0,
//                              60,QApplication::desktop()->availableGeometry().height()));
//    pAnimation->setEndValue(QRect(0,0,
//                                  160,QApplication::desktop()->availableGeometry().height()));
    pEnterAnimation->setStartValue(QPoint(100,0));
    pEnterAnimation->setEndValue(QPoint(0,0));
    pEnterAnimation->setEasingCurve(QEasingCurve::Linear);
//    connect(pEnterAnimation,SIGNAL(finished()),this,SLOT(animation_finished_slot()));

    pLeaveAnimation=new QPropertyAnimation;
    pLeaveAnimation->setTargetObject(pMainWidget);
    pLeaveAnimation->setPropertyName("pos");
    pLeaveAnimation->setDuration(500);
    pLeaveAnimation->setStartValue(QPoint(0,0));
    pLeaveAnimation->setEndValue(QPoint(100,0));
    pLeaveAnimation->setEasingCurve(QEasingCurve::Linear);

    pUkuiMenuInterface=new UkuiMenuInterface;

}

/**
 * 侧边栏添加控件
 */
void SideBarWidget::add_sidebar_btn()
{
//    layout=new QHBoxLayout(this);
//    layout->setContentsMargins(0,0,0,0);
//    layout->setSpacing(0);
//    this->setLayout(layout);

    pHorizonSpacer=new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);
//    layout->addItem(pHorizonSpacer);

    pMainWidget=new QWidget(this);
    pMainWidgetLayout=new QVBoxLayout;
    pMainWidgetLayout->setContentsMargins(0,0,0,0);
    pMainWidgetLayout->setSpacing(0);
    pMainWidget->setLayout(pMainWidgetLayout);
//    layout->addWidget(pMainWidget);
    pMainWidget->setStyleSheet("background:transparent;");

    //放大缩小按钮界面
    minmaxWidget=new QWidget(pMainWidget);
    minmaxWidget->setStyleSheet("QWidget{background:transparent;border:0px;}");
    minmaxLayout=new QHBoxLayout(minmaxWidget);
    minmaxbtn=new QToolButton(minmaxWidget);
    minmaxbtn->setShortcut(QKeySequence::InsertParagraphSeparator);
//    minmaxbtn->setShortcut(Qt::Key_Return);
//    horizontalSpacer=new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);
//    minmaxLayout->addItem(horizontalSpacer);
    minmaxLayout->addWidget(minmaxbtn);
    minmaxLayout->setAlignment(minmaxbtn,Qt::AlignRight);
    minmaxWidget->setLayout(minmaxLayout);
    char btncolor[300];
    sprintf(btncolor,"QToolButton{background:transparent;border:0px;padding-left:0px;}\
            QToolButton:hover{background-color:%s;border:0px;}\
            QToolButton:pressed{background-color:%s;border:0px;}",
            MMBtnHoverBackground,MMBtnHoverBackground);
    minmaxbtn->setStyleSheet(QString::fromLocal8Bit(btncolor));

    //分类按钮
    pBtnGroup=new QButtonGroup(pMainWidget);
    commonusebtn=new QPushButton(pMainWidget);
//    commonusebtn->setFocusPolicy(Qt::NoFocus);
//    commonusebtn->installEventFilter(this);
    set_btn_style(commonusebtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/commonuse.svg"),0);
    letterbtn=new QPushButton(pMainWidget);
//    letterbtn->setFocusPolicy(Qt::NoFocus);
//    letterbtn->installEventFilter(this);
    set_btn_style(letterbtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/letter.svg"),1);
    functionbtn=new QPushButton(pMainWidget);
//    functionbtn->setFocusPolicy(Qt::NoFocus);
//    functionbtn->installEventFilter(this);
    set_btn_style(functionbtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/function.svg"),2);
    buttonList.append(commonusebtn);
    buttonList.append(letterbtn);
    buttonList.append(functionbtn);
    int id=0;
    foreach (QAbstractButton* btn, buttonList) {
        pBtnGroup->addButton(btn,id++);
    }

    QString usericon=pUkuiMenuInterface->get_user_icon();
    usericonbtn=new QPushButton(pMainWidget);
    set_btn_style(usericonbtn,usericon,3);
    computerbtn=new QPushButton(pMainWidget);
    set_btn_style(computerbtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/computer.svg"),4);
    controlbtn=new QPushButton(pMainWidget);
    set_btn_style(controlbtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/control.svg"),5);
    shutdownbtn=new QPushButton(pMainWidget);
    set_btn_style(shutdownbtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/shutdown.svg"),6);
    connect(pBtnGroup,SIGNAL(buttonClicked(QAbstractButton*)),this,SLOT(btngroup_clicked_slot(QAbstractButton*)));
    connect(computerbtn,SIGNAL(clicked()),this,SLOT(computerbtn_clicked_slot()));
    connect(controlbtn,SIGNAL(clicked()),this,SLOT(controlbtn_clicked_slot()));
    connect(shutdownbtn,SIGNAL(clicked()),this,SLOT(shutdownbtn_clicked_slot()));
    connect(usericonbtn,SIGNAL(clicked()),this,SLOT(usericonbtn_clicked_slot()));

    commonusebtnname=new QLabel;
    commonusebtnname->setText(tr("常用软件"));
    commonusebtnname->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
    commonusebtnname->adjustSize();
//    commonusebtn->layout()->addWidget(commonusebtnname);

    letterbtnname=new QLabel;
    letterbtnname->setText(tr("字母排序"));
    letterbtnname->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
    letterbtnname->adjustSize();
//    letterbtn->layout()->addWidget(letterbtnname);

    functionbtnname=new QLabel;
    functionbtnname->setText(tr("功能分类"));
    functionbtnname->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
    functionbtnname->adjustSize();
//    functionbtn->layout()->addWidget(functionbtnname);

    QString username=pUkuiMenuInterface->get_user_name();
    usericonbtnname=new QLabel;
    usericonbtnname->setText(username);
    usericonbtnname->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
    usericonbtnname->adjustSize();

    computerbtnname=new QLabel;
    computerbtnname->setText(tr("我的电脑"));
    computerbtnname->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
    computerbtnname->adjustSize();
//    computerbtn->layout()->addWidget(computerbtnname);

    controlbtnname=new QLabel;
    controlbtnname->setText(tr("控制面板"));
    controlbtnname->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
    controlbtnname->adjustSize();
//    controlbtn->layout()->addWidget(controlbtnname);

    shutdownbtnname=new QLabel;
    shutdownbtnname->setText(tr("关机"));
    shutdownbtnname->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
    shutdownbtnname->adjustSize();
//    shutdownbtn->layout()->addWidget(shutdownbtnname);


    add_right_click_menu(computerbtn);
    add_right_click_menu(controlbtn);
    shutdownbtn->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(shutdownbtn,SIGNAL(customContextMenuRequested(const QPoint&)),this,
            SLOT(shutdownbtn_right_click_slot()));

    verticalSpacer = new QSpacerItem(20,40, QSizePolicy::Fixed, QSizePolicy::Expanding);
    pMainWidgetLayout->addWidget(minmaxWidget);

    pMainWidgetLayout->addWidget(commonusebtn);
    pMainWidgetLayout->addWidget(letterbtn);
    pMainWidgetLayout->addWidget(functionbtn);

    pMainWidgetLayout->addItem(verticalSpacer);
    pMainWidgetLayout->addWidget(usericonbtn);
    pMainWidgetLayout->addWidget(computerbtn);
    pMainWidgetLayout->addWidget(controlbtn);
    pMainWidgetLayout->addWidget(shutdownbtn);
}

/**
 * 设置按钮样式
 */
void SideBarWidget::set_btn_style(QPushButton *btn, QString btnicon, int num)
{
    char btncolor[300];
    sprintf(btncolor,"QPushButton{background:transparent;border:0px;padding-left:0;}\
            QPushButton:hover{background-color:%s;border:0px;}\
            QPushButton:pressed{background-color:%s;border:0px;}",
            SBFunBtnHoverBackground,SBFunBtnHoverBackground);
    btn->setStyleSheet(QString::fromLocal8Bit(btncolor));
    btnlayout=new QHBoxLayout;
    labelicon=new QLabel;
    labelicon->setAlignment(Qt::AlignCenter);
    labelicon->setStyleSheet("QLabel{background:transparent;}");

    if(num!=3)
    {
        QSvgRenderer* svgRender = new QSvgRenderer(btn);
        svgRender->load(btnicon);
        QPixmap* pixmap = new QPixmap(19,19);
        pixmap->fill(Qt::transparent);//设置背景透明
        QPainter p(pixmap);
        svgRender->render(&p);
        labelicon->setPixmap(*pixmap);
        labelicon->setFixedSize(pixmap->size());
    }
    else {
        QIcon icon=QIcon::fromTheme(btnicon);
        labelicon->setPixmap(icon.pixmap(19,19));
        labelicon->setFixedSize(19,19);
    }

    btnlayout->addWidget(labelicon);
    btn->setLayout(btnlayout);
    btn->setFocusPolicy(Qt::NoFocus);
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
        QProcess *process=new QProcess(this);
        switch (ret) {
        case 10:
            process->startDetached(QString("ukui-screensaver-command -l"));
            break;
        case 11:
            process->startDetached(QString("ukui-session-tools --switchuser"));
            break;
        case 12:
            process->startDetached(QString("ukui-session-tools --logout"));
            break;
        case 13:
            process->startDetached(QString("ukui-session-tools --reboot"));
            break;
        case 14:
            process->startDetached(QString("ukui-session-tools --shutdown"));
            break;
        default:
            break;
        }

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

void SideBarWidget::computerbtn_clicked_slot()
{
    emit send_hide_mainwindow_signal();
    QProcess *process=new QProcess(this);
    process->startDetached(QString("peony-qt computer:///"));
}

void SideBarWidget::controlbtn_clicked_slot()
{
    emit send_hide_mainwindow_signal();
    QString execpath=pUkuiMenuInterface->get_app_exec(QString("/usr/share/applications/ukui-control-center.desktop"));
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

void SideBarWidget::shutdownbtn_clicked_slot()
{
    shutdownbtn_right_click_slot();
}

void SideBarWidget::usericonbtn_clicked_slot()
{
    emit send_hide_mainwindow_signal();
    system("ukui-control-center --u");
}

/**
 * 加载默认侧边栏
 */
void SideBarWidget::load_min_sidebar()
{
    is_fullscreen=false;
    set_max_btn();

    this->setFixedSize(55,590);
    pMainWidget->setGeometry(QRect(0,0,this->width(),this->height()));
    minmaxWidget->setFixedSize(55,70);

    set_min_sidebar_btn(commonusebtn);
    set_min_sidebar_btn(letterbtn);
    set_min_sidebar_btn(functionbtn);

    set_min_sidebar_btn(usericonbtn);
    set_min_sidebar_btn(computerbtn);
    set_min_sidebar_btn(controlbtn);
    set_min_sidebar_btn(shutdownbtn);

    disconnect(minmaxbtn,SIGNAL(clicked()),this, SIGNAL(send_defaultbtn_signal()));
    connect(minmaxbtn, SIGNAL(clicked()),this,SIGNAL(send_fullscreenbtn_signal()));
}

/**
 * 设置全屏按钮
 */
void SideBarWidget::set_max_btn()
{
    minmaxbtn->setFixedSize(30,30);
    QSvgRenderer* svgRender = new QSvgRenderer(minmaxbtn);
    svgRender->load(QString(":/data/img/mainviewwidget/max.svg"));
    QPixmap* pixmap = new QPixmap(14,14);
    pixmap->fill(Qt::transparent);//设置背景透明
    QPainter p(pixmap);
    svgRender->render(&p);
    minmaxbtn->setIcon(QIcon(*pixmap));
}

/**
 * 设置默认侧边栏按钮
 */
void SideBarWidget::set_min_sidebar_btn(QPushButton* btn)
{
    btn->setFixedSize(55,37);
    btn->layout()->setContentsMargins(0,0,0,0);
    btn->layout()->setSpacing(10);

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


//    btn->setFixedSize(160,48);
//    QLayoutItem* item=btn->layout()->itemAt(1);
//    QWidget* wid=item->widget();
//    QLabel* label=qobject_cast<QLabel*>(wid);
//    int len=label->text().length();
////    btn->layout()->setContentsMargins(25,0,btn->width()-40-labelicon->width()-letterbtnname->width(),0);
//    btn->layout()->setContentsMargins(25,0,btn->width()-40-labelicon->width()-len*14,0);
//    btn->layout()->setSpacing(15);
}

/**
 * 加载全屏侧边栏
 */
void SideBarWidget::load_max_sidebar()
{
    is_fullscreen=true;
    set_min_btn();

    if(QApplication::desktop()->width()*QApplication::desktop()->height() <= 1600*900)
    {
        this->setFixedSize(265,QApplication::desktop()->availableGeometry().height());
        pMainWidget->setGeometry(QRect(265-112-21,0,112,QApplication::desktop()->availableGeometry().height()));

        minmaxWidget->setFixedSize(112,87);

    }
    else{
        this->setFixedSize(286,QApplication::desktop()->availableGeometry().height());
        pMainWidget->setGeometry(QRect(286-112-21,0,112,QApplication::desktop()->availableGeometry().height()));

        minmaxWidget->setFixedSize(112,105);
    }

    commonusebtn->layout()->addWidget(commonusebtnname);
    letterbtn->layout()->addWidget(letterbtnname);
    functionbtn->layout()->addWidget(functionbtnname);
    usericonbtn->layout()->addWidget(usericonbtnname);
    computerbtn->layout()->addWidget(computerbtnname);
    controlbtn->layout()->addWidget(controlbtnname);
    shutdownbtn->layout()->addWidget(shutdownbtnname);

    set_max_sidebar_btn(commonusebtn);
    set_max_sidebar_btn(letterbtn);
    set_max_sidebar_btn(functionbtn);
    set_max_sidebar_btn(usericonbtn);
    set_max_sidebar_btn(computerbtn);
    set_max_sidebar_btn(controlbtn);
    set_max_sidebar_btn(shutdownbtn);

    disconnect(minmaxbtn, SIGNAL(clicked()),this,SIGNAL(send_fullscreenbtn_signal()));
    connect(minmaxbtn, SIGNAL(clicked()),this,SIGNAL(send_defaultbtn_signal()));
}

/**
 * 设置还原按钮
 */
void SideBarWidget::set_min_btn()
{
    minmaxbtn->setFixedSize(70.2,70.2);
    QSvgRenderer* svgRender = new QSvgRenderer(minmaxbtn);
    svgRender->load(QString(":/data/img/mainviewwidget/min.svg"));
    QPixmap* pixmap = new QPixmap(16,16);
    pixmap->fill(Qt::transparent);//设置背景透明
    QPainter p(pixmap);
    svgRender->render(&p);
    minmaxbtn->setIcon(QIcon(*pixmap));
}

/**
 * 设置全屏侧边栏按钮
 */
void SideBarWidget::set_max_sidebar_btn(QPushButton *btn)
{
//    btn->setFixedSize(60,48);
//    btn->layout()->setContentsMargins(0,0,0,0);
//    btn->layout()->setSpacing(10);

    btn->setFixedSize(112,50);
//    QLayoutItem* item=btn->layout()->itemAt(1);
//    QWidget* wid=item->widget();
//    QLabel* label=qobject_cast<QLabel*>(wid);
//    int len=label->text().length();
//    btn->layout()->setContentsMargins(15,0,btn->width()-40-labelicon->width()-letterbtnname->width(),0);
    btn->layout()->setContentsMargins(15,0,0,0);
    btn->layout()->setSpacing(10);

}

void SideBarWidget::enterEvent(QEvent *e)
{
    this->is_hover=true;
    Q_UNUSED(e);
    if(!is_fullscreen)
    {
        emit send_hover_signal(true);
    }
    else
    {
//        pEnterAnimation->start();
    }
}

void SideBarWidget::leaveEvent(QEvent *e)
{
    this->is_hover=false;
    Q_UNUSED(e);
    if(!is_fullscreen)
    {
        emit send_hover_signal(false);
    }
    else
    {
//        pLeaveAnimation->start();
    }
}

void SideBarWidget::animation_finished_slot()
{
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

void SideBarWidget::btngroup_clicked_slot(QAbstractButton *btn)
{
    char pressstyle[200];
    sprintf(pressstyle,"QPushButton{background-color:%s;border:0px;padding-left:0;}",SBClassifyBtnHoverBackground);

    foreach (QAbstractButton* button, buttonList) {
        if(pBtnGroup->id(btn)==buttonList.indexOf(button))
        {
            button->setStyleSheet(pressstyle);
            if(pBtnGroup->id(btn)==0)
            {
                if(is_fullscreen)
                {
                    emit send_fullscreen_commonusebtn_signal();
                }
                else
                {
                    emit send_commonusebtn_signal();
                }
            }
            else if(pBtnGroup->id(btn)==1)
            {
                if(is_fullscreen)
                    emit send_fullscreen_letterbtn_signal();
                else emit send_letterbtn_signal();
            }
            else{
                if(is_fullscreen)
                    emit send_fullscreen_functionbtn_signal();
                else emit send_functionbtn_signal();
            }
        }
        else{
            button->setStyleSheet("background:transparent;");
        }
    }
}

void SideBarWidget::widget_make_zero()
{
    letterbtn->click();
}

void SideBarWidget::recv_querylineEdit_focusin_slot()
{
    foreach (QAbstractButton* button, buttonList)
    {
        button->setStyleSheet("background:transparent;");
    }
}
