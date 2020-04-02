/*
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/&gt;.
 *
 */

#include "sidebarwidget.h"
#include "ui_sidebarwidget.h"
#include <QDebug>
#include <QSvgRenderer>
#include <QPainter>
#include "src/Style/style.h"

SideBarWidget::SideBarWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SideBarWidget)
{
    ui->setupUi(this);
    initWidget();
    letterbtn->click();
}

SideBarWidget::~SideBarWidget()
{
    delete ui;
    delete pUkuiMenuInterface;
}

/**
 * 侧边栏初始化
 */
void SideBarWidget::initWidget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setStyleSheet("border:0px;background:transparent;");
//    char style[100];
//    sprintf(style, "border:0px;background-color:%s;",DefaultBackground);
//    this->setStyleSheet(QString::fromLocal8Bit(style));
    this->setFocusPolicy(Qt::NoFocus);

    addSidebarBtn();
    loadMinSidebar();

    pUkuiMenuInterface=new UkuiMenuInterface;

}

/**
 * 侧边栏添加控件
 */
void SideBarWidget::addSidebarBtn()
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
    pMainWidgetLayout->setSpacing(10);
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
//    minmaxLayout->setAlignment(minmaxbtn,Qt::AlignCenter);
    minmaxWidget->setLayout(minmaxLayout);
    char btncolor[300];
    sprintf(btncolor,"QToolButton{background:transparent;border:0px;padding-left:0px;border-radius:2px;}\
            QToolButton:hover{background-color:%s;border:0px;border-radius:2px;}\
            QToolButton:pressed{background-color:%s;border:0px;border-radius:2px;}",
            MMBtnHoverBackground,MMBtnHoverBackground);
    minmaxbtn->setStyleSheet(QString::fromLocal8Bit(btncolor));

    //分类按钮
    pBtnGroup=new QButtonGroup(pMainWidget);
    commonusebtn=new QPushButton(pMainWidget);
//    commonusebtn->setFocusPolicy(Qt::NoFocus);
//    commonusebtn->installEventFilter(this);
    setBtnStyle(commonusebtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/commonuse.svg"),0);
    letterbtn=new QPushButton(pMainWidget);
//    letterbtn->setFocusPolicy(Qt::NoFocus);
//    letterbtn->installEventFilter(this);
    setBtnStyle(letterbtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/letter.svg"),1);
    functionbtn=new QPushButton(pMainWidget);
//    functionbtn->setFocusPolicy(Qt::NoFocus);
//    functionbtn->installEventFilter(this);
    setBtnStyle(functionbtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/function.svg"),2);
    buttonList.append(commonusebtn);
    buttonList.append(letterbtn);
    buttonList.append(functionbtn);
    int id=0;
    Q_FOREACH (QAbstractButton* btn, buttonList) {
        pBtnGroup->addButton(btn,id++);
    }

    QString usericon=pUkuiMenuInterface->getUserIcon();
    usericonbtn=new QPushButton(pMainWidget);
    setBtnStyle(usericonbtn,usericon,3);
    computerbtn=new QPushButton(pMainWidget);
    setBtnStyle(computerbtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/computer.svg"),4);
    controlbtn=new QPushButton(pMainWidget);
    setBtnStyle(controlbtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/control.svg"),5);
    shutdownbtn=new QPushButton(pMainWidget);
    setBtnStyle(shutdownbtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/shutdown.svg"),6);
    connect(pBtnGroup,SIGNAL(buttonClicked(QAbstractButton*)),this,SLOT(btnGroupClickedSlot(QAbstractButton*)));
    connect(computerbtn,SIGNAL(clicked()),this,SLOT(computerBtnClickedSlot()));
    connect(controlbtn,SIGNAL(clicked()),this,SLOT(controlBtnClickedSlot()));
    connect(shutdownbtn,SIGNAL(clicked()),this,SLOT(shutdownBtnClickedSlot()));
    connect(usericonbtn,SIGNAL(clicked()),this,SLOT(userIconBtnClickedSlot()));
    otherButtonList.append(usericonbtn);
    otherButtonList.append(computerbtn);
    otherButtonList.append(controlbtn);
    otherButtonList.append(shutdownbtn);

    QString fontsizestr=QString::number(Style::SideBarFontSize)+"px";
    char textstyle[100];
    sprintf(textstyle,"QLabel{background:transparent;color:#ffffff;font-size:%s;}",
            fontsizestr.toLocal8Bit().data());
    commonusebtnname=new QLabel;
    commonusebtnname->setText(tr("Common"));
    commonusebtnname->setStyleSheet(textstyle);
    commonusebtnname->adjustSize();
//    commonusebtn->layout()->addWidget(commonusebtnname);

    letterbtnname=new QLabel;
    letterbtnname->setText(tr("Letter"));
    letterbtnname->setStyleSheet(textstyle);
    letterbtnname->adjustSize();
//    letterbtn->layout()->addWidget(letterbtnname);

    functionbtnname=new QLabel;
    functionbtnname->setText(tr("Function"));
    functionbtnname->setStyleSheet(textstyle);
    functionbtnname->adjustSize();
//    functionbtn->layout()->addWidget(functionbtnname);

    QString username=pUkuiMenuInterface->getUserName();
    usericonbtnname=new QLabel;
    usericonbtnname->setText(username);
    usericonbtnname->setStyleSheet(textstyle);
    usericonbtnname->adjustSize();

    computerbtnname=new QLabel;
    computerbtnname->setText(tr("Computer"));
    computerbtnname->setStyleSheet(textstyle);
    computerbtnname->adjustSize();
//    computerbtn->layout()->addWidget(computerbtnname);

    controlbtnname=new QLabel;
    controlbtnname->setText(tr("Settings"));
    controlbtnname->setStyleSheet(textstyle);
    controlbtnname->adjustSize();
//    controlbtn->layout()->addWidget(controlbtnname);

    shutdownbtnname=new QLabel;
    shutdownbtnname->setText(tr("Power"));
    shutdownbtnname->setStyleSheet(textstyle);
    shutdownbtnname->adjustSize();
//    shutdownbtn->layout()->addWidget(shutdownbtnname);


    addRightClickMenu(computerbtn);
    addRightClickMenu(controlbtn);
    shutdownbtn->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(shutdownbtn,SIGNAL(customContextMenuRequested(const QPoint&)),this,
            SLOT(shutdownBtnRightClickSlot()));

    //监控用户账户信息更改
    qint64 uid=static_cast<qint64>(getuid());
    QDBusInterface iface("org.freedesktop.Accounts",
                         "/org/freedesktop/Accounts",
                         "org.freedesktop.Accounts",
                         QDBusConnection::systemBus());
    QDBusReply<QDBusObjectPath>objPath=iface.call("FindUserById",uid);
    QDBusConnection::systemBus().connect("org.freedesktop.Accounts",
                                          objPath.value().path(),
                                          "org.freedesktop.Accounts.User",
                                         QString("Changed"),this,SLOT(userAccountsChanged()));

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
void SideBarWidget::setBtnStyle(QPushButton *btn, QString btnicon, int num)
{
    char btncolor[300];
    sprintf(btncolor,"QPushButton{background:transparent;border:0px;padding-left:0;border-radius:2px;}\
            QPushButton:hover{background-color:%s;border:0px;border-radius:2px;}\
            QPushButton:pressed{background-color:%s;border:0px;border-radius:2px;}",
            SBFunBtnHoverBackground,SBFunBtnHoverBackground);
    btn->setStyleSheet(QString::fromLocal8Bit(btncolor));
    btnlayout=new QHBoxLayout;
    labelicon=new QLabel;
    labelicon->setAlignment(Qt::AlignCenter);
    labelicon->setStyleSheet("background:transparent;");

    if(num!=3)
    {
        QSvgRenderer* svgRender = new QSvgRenderer(btn);
        svgRender->load(btnicon);
        QPixmap* pixmap = new QPixmap(Style::SideBarIconSize,Style::SideBarIconSize);
        pixmap->fill(Qt::transparent);//设置背景透明
        QPainter p(pixmap);
        svgRender->render(&p);
        labelicon->setPixmap(*pixmap);
        labelicon->setFixedSize(pixmap->size());
    }
    else {
        QPixmap pixmapa;
        QFileInfo fileInfo(btnicon);
        if(fileInfo.isFile())
            pixmapa=QPixmap(btnicon);
        else
            pixmapa=QPixmap(":/data/img/sidebarwidget/usericon-darkcolor.svg");
        QPixmap pixmap(Style::SideBarIconSize+4,Style::SideBarIconSize+4);
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

        QPainterPath path;
        path.addEllipse(0, 0, Style::SideBarIconSize+4,Style::SideBarIconSize+4);

        //画背景
//        QColor color;
//        color.setNamedColor(UserIconBackground);
//        painter.setOpacity(UserIconOpacity);
//        painter.setPen(Qt::NoPen);
//        painter.setBrush(QBrush(color));
//        painter.drawPath(path);

        //填充图片
        painter.setOpacity(1);
        painter.setClipPath(path);
        painter.drawPixmap(0, 0, Style::SideBarIconSize+4,Style::SideBarIconSize+4, pixmapa);

        //画圈圈
//        path.addEllipse(0, 0, Style::SideBarIconSize+4,Style::SideBarIconSize+4);
//        painter.setOpacity(1);
//        color.setNamedColor("#d5d5d5");
//        painter.setPen(QPen(color,2));
//        painter.setBrush(Qt::NoBrush);
//        painter.drawPath(path);

        labelicon->setPixmap(pixmap);
        labelicon->setFixedSize(Style::SideBarIconSize+4,Style::SideBarIconSize+4);
    }

    btnlayout->addWidget(labelicon);
    btn->setLayout(btnlayout);
    btn->setFocusPolicy(Qt::NoFocus);
}

/**
 * 加载关机按钮右键菜单
 */
void SideBarWidget::shutdownBtnRightClickSlot()
{
    shutdownmenu=new RightClickMenu(this);
    int ret=shutdownmenu->showShutdownMenu();
    if(ret>=10 && ret<=14)
    {
        Q_EMIT sendHideMainWindowSignal();
        switch (ret) {
        case 10:
            QProcess::startDetached(QString("ukui-screensaver-command -l"));
            break;
        case 11:
            QProcess::startDetached(QString("ukui-session-tools --switchuser"));
            break;
        case 12:
            QProcess::startDetached(QString("ukui-session-tools --logout"));
            break;
        case 13:
            QProcess::startDetached(QString("ukui-session-tools --reboot"));
            break;
        case 14:
            QProcess::startDetached(QString("ukui-session-tools --shutdown"));
            break;
        default:
            break;
        }

    }
}

void SideBarWidget::addRightClickMenu(QPushButton *btn)
{
    btn->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(btn,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(otherBtnRightClickSlot()));
}

void SideBarWidget::otherBtnRightClickSlot()
{
    othermenu=new RightClickMenu(this);
    QPushButton* btn=dynamic_cast<QPushButton*>(QObject::sender());
    int index=otherButtonList.indexOf(btn);
    QString desktopfp;
    if(index==1)
        desktopfp=QString("/usr/share/applications/peony-computer.desktop");
    if(index==2)
        desktopfp=QString("/usr/share/applications/ukui-control-center.desktop");
    othermenu->showOtherMenu(desktopfp);
}

void SideBarWidget::computerBtnClickedSlot()
{
    Q_EMIT sendHideMainWindowSignal();
    QProcess::startDetached(QString("/usr/bin/peony computer:///"));
}

void SideBarWidget::controlBtnClickedSlot()
{
    Q_EMIT sendHideMainWindowSignal();
    QString execpath=pUkuiMenuInterface->getAppExec(QString("/usr/share/applications/ukui-control-center.desktop"));
    //移除启动参数%u或者%U
    if(execpath.contains("%"))
    {
        int index=execpath.indexOf(QString("%").at(0));
        execpath.remove(index-1,3);
    }
    QProcess::startDetached(execpath);

}

void SideBarWidget::shutdownBtnClickedSlot()
{
    shutdownBtnRightClickSlot();
}

void SideBarWidget::userIconBtnClickedSlot()
{
    Q_EMIT sendHideMainWindowSignal();
    QProcess::startDetached(QString("ukui-control-center -u"));
}

void SideBarWidget::userAccountsChanged()
{
    QString usericon=pUkuiMenuInterface->getUserIcon();
    QPixmap pixmapa;
    QFileInfo fileInfo(usericon);
    if(fileInfo.isFile())
        pixmapa=QPixmap(usericon);
    else
        pixmapa=QPixmap(":/data/img/sidebarwidget/usericon-darkcolor.svg");
    QPixmap pixmap(Style::SideBarIconSize+4,Style::SideBarIconSize+4);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QPainterPath path;
    path.addEllipse(0, 0, Style::SideBarIconSize+4,Style::SideBarIconSize+4);

    //画背景
//    QColor color;
//    color.setNamedColor(UserIconBackground);
//    painter.setOpacity(UserIconOpacity);
//    painter.setPen(Qt::NoPen);
//    painter.setBrush(QBrush(color));
//    painter.drawPath(path);

    //填充图片
    painter.setOpacity(1);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, Style::SideBarIconSize+4,Style::SideBarIconSize+4, pixmapa);

    //画圈圈
//    path.addEllipse(0, 0, Style::SideBarIconSize+4,Style::SideBarIconSize+4);
//    painter.setOpacity(1);
//    color.setNamedColor("#d5d5d5");
//    painter.setPen(QPen(color,2));
//    painter.setBrush(Qt::NoBrush);
//    painter.drawPath(path);

    QLayoutItem* item=usericonbtn->layout()->itemAt(0);
    QLabel* labelicon=qobject_cast<QLabel*>(item->widget());
    labelicon->setPixmap(pixmap);
    labelicon->setFixedSize(Style::SideBarIconSize+4,Style::SideBarIconSize+4);
}

/**
 * 加载默认侧边栏
 */
void SideBarWidget::loadMinSidebar()
{
    is_fullscreen=false;
    setMaxBtn();

    this->setFixedSize(55,590);
    pMainWidget->setGeometry(QRect(0,0,this->width(),this->height()));
    minmaxWidget->setFixedSize(37,70);
    minmaxLayout->setContentsMargins(0,0,0,0);

    pMainWidgetLayout->setContentsMargins(8,0,10,0);

    setMinSidebarBtn(commonusebtn);
    setMinSidebarBtn(letterbtn);
    setMinSidebarBtn(functionbtn);

    setMinSidebarBtn(usericonbtn);
    setMinSidebarBtn(computerbtn);
    setMinSidebarBtn(controlbtn);
    setMinSidebarBtn(shutdownbtn);
    commonusebtn->setToolTip(tr("Common"));
    letterbtn->setToolTip(tr("Letter"));
    functionbtn->setToolTip(tr("Function"));
    QString username=pUkuiMenuInterface->getUserName();
    usericonbtn->setToolTip(username);
    computerbtn->setToolTip(tr("Computer"));
    controlbtn->setToolTip(tr("Settings"));
    shutdownbtn->setToolTip(tr("Power"));

    disconnect(minmaxbtn,SIGNAL(clicked()),this, SIGNAL(sendDefaultBtnSignal()));
    connect(minmaxbtn, SIGNAL(clicked()),this,SIGNAL(sendFullScreenBtnSignal()));
}

/**
 * 设置全屏按钮
 */
void SideBarWidget::setMaxBtn()
{
    minmaxbtn->setFixedSize(37,37);
    QSvgRenderer* svgRender = new QSvgRenderer(minmaxbtn);
    svgRender->load(QString(":/data/img/sidebarwidget/max.svg"));
    QPixmap* pixmap = new QPixmap(14,14);
    pixmap->fill(Qt::transparent);//设置背景透明
    QPainter p(pixmap);
    svgRender->render(&p);
    minmaxbtn->setIcon(QIcon(*pixmap));
}

/**
 * 设置默认侧边栏按钮
 */
void SideBarWidget::setMinSidebarBtn(QPushButton* btn)
{
    btn->setFixedSize(37,37);
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
}

/**
 * 加载全屏侧边栏
 */
void SideBarWidget::loadMaxSidebar()
{
    is_fullscreen=true;
    setMinBtn();

    this->setFixedSize(Style::SideBarWidWidth,Style::heightavailable);
    pMainWidget->setGeometry(QRect(this->width()-Style::SideBarBtnWidth-Style::SideBarMargin,0,
                                   this->width(),this->height()));

    minmaxWidget->setFixedSize(Style::MinMaxWidWidth,Style::MinMaxWidHeight);
    minmaxLayout->setContentsMargins(minmaxWidget->width()-minmaxbtn->width(),0,0,0);

    pMainWidgetLayout->setContentsMargins(0,0,0,0);

    commonusebtn->layout()->addWidget(commonusebtnname);
    letterbtn->layout()->addWidget(letterbtnname);
    functionbtn->layout()->addWidget(functionbtnname);
    usericonbtn->layout()->addWidget(usericonbtnname);
    computerbtn->layout()->addWidget(computerbtnname);
    controlbtn->layout()->addWidget(controlbtnname);
    shutdownbtn->layout()->addWidget(shutdownbtnname);

    setMaxSidebarBtn(commonusebtn);
    setMaxSidebarBtn(letterbtn);
    setMaxSidebarBtn(functionbtn);
    setMaxSidebarBtn(usericonbtn);
    setMaxSidebarBtn(computerbtn);
    setMaxSidebarBtn(controlbtn);
    setMaxSidebarBtn(shutdownbtn);
    commonusebtn->setToolTip("");
    letterbtn->setToolTip("");
    functionbtn->setToolTip("");
    usericonbtn->setToolTip("");
    computerbtn->setToolTip("");
    controlbtn->setToolTip("");
    shutdownbtn->setToolTip("");

    disconnect(minmaxbtn, SIGNAL(clicked()),this,SIGNAL(sendFullScreenBtnSignal()));
    connect(minmaxbtn, SIGNAL(clicked()),this,SIGNAL(sendDefaultBtnSignal()));
}

/**
 * 设置还原按钮
 */
void SideBarWidget::setMinBtn()
{
    minmaxbtn->setFixedSize(Style::MinMaxBtnWidth,Style::MinMaxBtnWidth);
    QSvgRenderer* svgRender = new QSvgRenderer(minmaxbtn);
    svgRender->load(QString(":/data/img/sidebarwidget/min.svg"));
    QPixmap* pixmap = new QPixmap(Style::MinMaxIconSize,Style::MinMaxIconSize);
    pixmap->fill(Qt::transparent);//设置背景透明
    QPainter p(pixmap);
    svgRender->render(&p);
    minmaxbtn->setIcon(QIcon(*pixmap));
}

/**
 * 设置全屏侧边栏按钮
 */
void SideBarWidget::setMaxSidebarBtn(QPushButton *btn)
{
    btn->setFixedSize(Style::SideBarBtnWidth,Style::SideBarBtnHeight);
    btn->layout()->setContentsMargins(Style::SideBarSpaceIconLeft,
                                      0,
                                      0,
                                      0);
    btn->layout()->setSpacing(Style::SideBarSpaceIconText);
}

void SideBarWidget::btnGroupClickedSlot(QAbstractButton *btn)
{
    char btncolor[300];
    sprintf(btncolor,"QPushButton{background:transparent;border:0px;padding-left:0;border-radius:2px;}\
            QPushButton:hover{background-color:%s;border:0px;border-radius:2px;}\
            QPushButton:pressed{background-color:%s;border:0px;border-radius:2px;}",
            SBFunBtnHoverBackground,SBFunBtnHoverBackground);

    char pressstyle[200];
    sprintf(pressstyle,"QPushButton{background-color:%s;border:0px;padding-left:0;border-radius:2px;}",SBClassifyBtnSelectedBackground);

    Q_FOREACH (QAbstractButton* button, buttonList) {
        if(pBtnGroup->id(btn)==buttonList.indexOf(button))
        {
            button->setStyleSheet(pressstyle);
            if(pBtnGroup->id(btn)==0)
            {
                if(is_fullscreen)
                {
                    Q_EMIT sendFullScreenCommonUseBtnSignal();
                }
                else
                {
                    Q_EMIT sendCommonUseBtnSignal();
                }
            }
            else if(pBtnGroup->id(btn)==1)
            {
                if(is_fullscreen)
                    Q_EMIT sendFullScreenLetterBtnSignal();
                else Q_EMIT sendLetterBtnSignal();
            }
            else{
                if(is_fullscreen)
                    Q_EMIT sendFullScreenFunctionBtnSignal();
                else Q_EMIT sendFunctionBtnSignal();
            }
        }
        else{
            button->setStyleSheet("background:transparent;");
        }
    }
}

void SideBarWidget::widgetMakeZero()
{
    letterbtn->click();
}
