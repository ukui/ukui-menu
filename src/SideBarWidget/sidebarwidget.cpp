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
#include <syslog.h>
#include "src/Style/style.h"

SideBarWidget::SideBarWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SideBarWidget)
{
    ui->setupUi(this);
    initWidget();
    commonusebtn->click();
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
//    minmaxbtn->setShortcut(QKeySequence::InsertParagraphSeparator);
//    minmaxbtn->setShortcut(Qt::Key_Enter);
    minmaxbtn->setShortcut(Qt::Key_Return);
//    horizontalSpacer=new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);
//    minmaxLayout->addItem(horizontalSpacer);
    minmaxLayout->addWidget(minmaxbtn);
//    minmaxLayout->setAlignment(minmaxbtn,Qt::AlignCenter);
    minmaxWidget->setLayout(minmaxLayout);
    char btncolor[300];
    sprintf(btncolor,"QToolButton{background:transparent;border:0px;padding-left:0px;border-radius:4px;}\
            QToolButton:hover{background-color:%s;border:0px;border-radius:4px;}\
            QToolButton:pressed{background-color:%s;border:0px;border-radius:4px;}",
            MMBtnHoverBackground,MMBtnHoverBackground);
    minmaxbtn->setStyleSheet(QString::fromLocal8Bit(btncolor));

    commonusebtnname=new QLabel;
    letterbtnname=new QLabel;
    functionbtnname=new QLabel;
    //分类按钮
    pBtnGroup=new QButtonGroup(pMainWidget);
    commonusebtn=new QPushButton();
//    commonusebtn->setFocusPolicy(Qt::NoFocus);
//    commonusebtn->installEventFilter(this);
    initBtn(commonusebtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/commonuse.svg"),commonusebtnname,tr("All"),0);
    letterbtn=new QPushButton();
//    letterbtn->setFocusPolicy(Qt::NoFocus);
//    letterbtn->installEventFilter(this);
    initBtn(letterbtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/letter.svg"),letterbtnname,tr("Letter"),1);
    functionbtn=new QPushButton();
//    functionbtn->setFocusPolicy(Qt::NoFocus);
//    functionbtn->installEventFilter(this);
    initBtn(functionbtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/function.svg"),functionbtnname,tr("Function"),2);
    buttonList.append(commonusebtn);
    buttonList.append(letterbtn);
    buttonList.append(functionbtn);
    int id=0;
    Q_FOREACH (QAbstractButton* btn, buttonList) {
        pBtnGroup->addButton(btn,id++);
    }

    QString usericon=pUkuiMenuInterface->getUserIcon();
    QString username=pUkuiMenuInterface->getUserName();
    usericonbtnname=new QLabel;
    computerbtnname=new QLabel;
    personalbtnname=new QLabel;
    controlbtnname=new QLabel;
    trashbtnname=new QLabel;
    shutdownbtnname=new QLabel;
    usericonbtn=new QPushButton();
    initBtn(usericonbtn,usericon,usericonbtnname,username,3);
    computerbtn=new QPushButton();
    initBtn(computerbtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/computer.svg"),computerbtnname,tr("Computer"),4);
    personalbtn=new QPushButton();
    initBtn(personalbtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/personal.svg"),personalbtnname,tr("Personal"),5);
    controlbtn=new QPushButton();
    initBtn(controlbtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/control.svg"),controlbtnname,tr("Settings"),6);
    trashbtn=new QPushButton();
    initBtn(trashbtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/trash.svg"),trashbtnname,tr("Recycle Bin"),7);
    shutdownbtn=new QPushButton();
    initBtn(shutdownbtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/shutdown.svg"),shutdownbtnname,tr("Power"),8);
    connect(pBtnGroup,static_cast<void(QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked),this,&SideBarWidget::btnGroupClickedSlot);
    connect(computerbtn,&QPushButton::clicked,this,&SideBarWidget::computerBtnClickedSlot);
    connect(personalbtn,&QPushButton::clicked,this,&SideBarWidget::personalBtnClickedSlot);
    connect(controlbtn,&QPushButton::clicked,this,&SideBarWidget::controlBtnClickedSlot);
    connect(trashbtn,&QPushButton::clicked,this,&SideBarWidget::trashBtnClickedSlot);
    connect(shutdownbtn,&QPushButton::clicked,this,&SideBarWidget::shutdownBtnClickedSlot);
    connect(usericonbtn,&QPushButton::clicked,this,&SideBarWidget::userIconBtnClickedSlot);

    otherButtonList.clear();
    otherButtonList.append(usericonbtn);
    if(QGSettings::isSchemaInstalled(QString("org.ukui.control-center.desktop").toLocal8Bit()))
    {
        gsetting=new QGSettings(QString("org.ukui.control-center.desktop").toLocal8Bit());
        if(gsetting->keys().contains(QString("personalIconLocking")) && gsetting->get("personal-icon-locking").toBool())
        {
                otherButtonList.append(personalbtn);
                otherButtonListIndex.append(1);
        }
        if(gsetting->keys().contains(QString("trashIconLocking")) && gsetting->get("trash-icon-locking").toBool())
        {
                otherButtonList.append(trashbtn);
                otherButtonListIndex.append(2);
        }
        if(gsetting->keys().contains(QString("computerIconLocking")) && gsetting->get("computer-icon-locking").toBool())
        {
                otherButtonList.append(computerbtn);
                otherButtonListIndex.append(3);
        }
        if(gsetting->keys().contains(QString("settingsIconLocking")) && gsetting->get("settings-icon-locking").toBool())
        {
                otherButtonList.append(controlbtn);
                otherButtonListIndex.append(4);
        }
        connect(gsetting,&QGSettings::changed,
                this,&SideBarWidget::resetSidebarBtnSlot);
    }
    otherButtonList.append(shutdownbtn);

    Q_FOREACH(QAbstractButton* button,otherButtonList)
    {
        QPushButton* btn=qobject_cast<QPushButton*>(button);
        if(otherButtonList.indexOf(button)!=0 && otherButtonList.indexOf(button)!=otherButtonList.count()-1)
            addRightClickMenu(btn);
    }
    shutdownbtn->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(shutdownbtn,&QPushButton::customContextMenuRequested,this,
            &SideBarWidget::shutdownBtnRightClickSlot);

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

    pMainWidgetLayout->addWidget(minmaxWidget);
    Q_FOREACH(QAbstractButton* button,buttonList)
        pMainWidgetLayout->addWidget(button);
    verticalSpacer = new QSpacerItem(20,40, QSizePolicy::Fixed, QSizePolicy::Expanding);
    pMainWidgetLayout->addItem(verticalSpacer);
    Q_FOREACH(QAbstractButton* button,otherButtonList)
        pMainWidgetLayout->addWidget(button);

    pAnimation = new QPropertyAnimation(pMainWidget, "geometry");
}

void SideBarWidget::resetSidebarBtnSlot()
{
    QLayoutItem *child;
    while ((child = pMainWidgetLayout->takeAt(5)) != 0) {
        QWidget* wid=child->widget();
        pMainWidgetLayout->removeWidget(wid);
        wid->setParent(nullptr);
        delete child;
    }
    otherButtonList.clear();
    otherButtonListIndex.clear();
    otherButtonList.append(usericonbtn);
    if(QGSettings::isSchemaInstalled(QString("org.ukui.control-center.desktop").toLocal8Bit()))
    {
        gsetting=new QGSettings(QString("org.ukui.control-center.desktop").toLocal8Bit());
        if(gsetting->keys().contains(QString("personalIconLocking")) && gsetting->get("personal-icon-locking").toBool())
        {
                otherButtonList.append(personalbtn);
                otherButtonListIndex.append(1);
        }
        if(gsetting->keys().contains(QString("trashIconLocking")) && gsetting->get("trash-icon-locking").toBool())
        {
                otherButtonList.append(trashbtn);
                otherButtonListIndex.append(2);
        }
        if(gsetting->keys().contains(QString("computerIconLocking")) && gsetting->get("computer-icon-locking").toBool())
        {
                otherButtonList.append(computerbtn);
                otherButtonListIndex.append(3);
        }
        if(gsetting->keys().contains(QString("settingsIconLocking")) && gsetting->get("settings-icon-locking").toBool())
        {
                otherButtonList.append(controlbtn);
                otherButtonListIndex.append(4);
        }
        connect(gsetting,&QGSettings::changed,
                this,&SideBarWidget::resetSidebarBtnSlot);
    }
    otherButtonList.append(shutdownbtn);
    Q_FOREACH(QAbstractButton* button,otherButtonList)
        pMainWidgetLayout->addWidget(button);

    Q_FOREACH(QAbstractButton* button,otherButtonList)
    {
        QPushButton* btn=qobject_cast<QPushButton*>(button);
        if(otherButtonList.indexOf(button)!=0 && otherButtonList.indexOf(button)!=otherButtonList.count()-1)
            addRightClickMenu(btn);
    }

    if(!is_fullscreen)
        loadMinSidebar();
    else
        loadMaxSidebar();
}

/**
 * 设置按钮样式
 */
void SideBarWidget::initBtn(QPushButton *btn, QString btnicon,QLabel* label,QString text,int num)
{
    char btncolor[300];
    sprintf(btncolor,"QPushButton{background:transparent;border:0px;padding-left:0;border-radius:4px;}\
            QPushButton:hover{background-color:%s;border:0px;border-radius:4px;}\
            QPushButton:pressed{background-color:%s;border:0px;border-radius:4px;}",
            SBFunBtnHoverBackground,SBFunBtnHoverBackground);
    btn->setStyleSheet(QString::fromLocal8Bit(btncolor));
    btnlayout=new QHBoxLayout;
    labelicon=new QLabel;
    labelicon->setAlignment(Qt::AlignCenter);
    labelicon->setStyleSheet("background:transparent;border:0px;");

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

    label->setAlignment(Qt::AlignVCenter);
//    label->setFixedHeight(labelicon->height());
    label->setText(text);
    label->setStyleSheet(QString("QLabel{background:transparent;color:#ffffff;}"));
    label->adjustSize();

    btnlayout->addWidget(labelicon);
    btnlayout->addWidget(label);
    btnlayout->setAlignment(labelicon,Qt::AlignVCenter);
    btnlayout->setAlignment(label,Qt::AlignVCenter);
    btn->setLayout(btnlayout);
    btn->setFocusPolicy(Qt::NoFocus);
}

/**
 * 加载关机按钮右键菜单
 */
void SideBarWidget::shutdownBtnRightClickSlot()
{
    shutdownmenu=new RightClickMenu(this,2);
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
    disconnect(btn,&QPushButton::customContextMenuRequested,this,&SideBarWidget::otherBtnRightClickSlot);
    connect(btn,&QPushButton::customContextMenuRequested,this,&SideBarWidget::otherBtnRightClickSlot);
}

void SideBarWidget::otherBtnRightClickSlot()
{
    othermenu=new RightClickMenu(this,3);
    QPushButton* btn=dynamic_cast<QPushButton*>(QObject::sender());
    int index=otherButtonListIndex.at(otherButtonList.indexOf(btn)-1);
    QString desktopfp;
    switch (index) {
    case 1:
        desktopfp=QString("/usr/share/applications/peony-home.desktop");
        break;
    case 2:
        desktopfp=QString("/usr/share/applications/peony-trash.desktop");
        break;
    case 3:
        desktopfp=QString("/usr/share/applications/peony-computer.desktop");
        break;
    case 4:
        desktopfp=QString("/usr/share/applications/ukui-control-center.desktop");
        break;
    default:
        break;
    }
    int ret=othermenu->showOtherMenu(desktopfp);
    if(ret==15)
    {
        Q_EMIT sendHideMainWindowSignal();
        QProcess::startDetached(QString("ukui-control-center -d"));
    }
}

void SideBarWidget::computerBtnClickedSlot()
{
    Q_EMIT sendHideMainWindowSignal();
    QProcess::startDetached(QString("/usr/bin/peony computer:///"));
}

void SideBarWidget::personalBtnClickedSlot()
{
    Q_EMIT sendHideMainWindowSignal();
    QProcess::startDetached(QString("/usr/bin/peony"));
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

void SideBarWidget::trashBtnClickedSlot()
{
    Q_EMIT sendHideMainWindowSignal();
    QProcess::startDetached(QString("/usr/bin/peony trash:///"));
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

    Q_FOREACH(QAbstractButton* button,buttonList)
    {
        QPushButton* btn=qobject_cast<QPushButton*>(button);
        setMinSidebarBtn(btn);
        int index=buttonList.indexOf(button);
        switch (index) {
        case 0:
            btn->setToolTip(tr("All"));
            break;
        case 1:
            btn->setToolTip(tr("Letter"));
            break;
        case 2:
            btn->setToolTip(tr("Function"));
            break;
        default:
            break;
        }
    }

    Q_FOREACH(QAbstractButton* button,otherButtonList)
    {
        QPushButton* btn=qobject_cast<QPushButton*>(button);
        setMinSidebarBtn(btn);
    }
    Q_FOREACH(int index,otherButtonListIndex)
    {
        switch (index) {
        case 1:
            personalbtn->setToolTip(tr("Personal"));
            break;
        case 2:
            trashbtn->setToolTip(tr("Recycle Bin"));
            break;
        case 3:
            computerbtn->setToolTip(tr("Computer"));
            break;
        case 4:
            controlbtn->setToolTip(tr("Settings"));
            break;
        default:
            break;
        }
    }
    QString username=pUkuiMenuInterface->getUserName();
    usericonbtn->setToolTip(username);
    shutdownbtn->setToolTip(tr("Power"));

    disconnect(minmaxbtn,&QToolButton::clicked,this, &SideBarWidget::sendDefaultBtnSignal);
    connect(minmaxbtn, &QToolButton::clicked,this,&SideBarWidget::sendFullScreenBtnSignal);
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
//    pMainWidget->setGeometry(QRect(this->width()-Style::SideBarBtnWidth-Style::SideBarMargin,0,
//                                   Style::SideBarBtnWidth,this->height()));

    minmaxWidget->setFixedSize(Style::MinMaxWidWidth,Style::MinMaxWidHeight);
    minmaxLayout->setContentsMargins(minmaxWidget->width()-minmaxbtn->width(),0,0,0);

    pMainWidgetLayout->setContentsMargins(0,0,0,0);

    commonusebtn->layout()->addWidget(commonusebtnname);
    commonusebtn->layout()->setAlignment(commonusebtnname,Qt::AlignVCenter);
    letterbtn->layout()->addWidget(letterbtnname);
    functionbtn->layout()->addWidget(functionbtnname);
    setMaxSidebarBtn(commonusebtn);
    setMaxSidebarBtn(letterbtn);
    setMaxSidebarBtn(functionbtn);
    usericonbtn->layout()->addWidget(usericonbtnname);
    shutdownbtn->layout()->addWidget(shutdownbtnname);
    setMaxSidebarBtn(usericonbtn);
    setMaxSidebarBtn(shutdownbtn);

    Q_FOREACH(int index,otherButtonListIndex)
    {
        switch (index) {
        case 1:
            personalbtn->layout()->addWidget(personalbtnname);
            setMaxSidebarBtn(personalbtn);
            break;
        case 2:
            trashbtn->layout()->addWidget(trashbtnname);
            setMaxSidebarBtn(trashbtn);
            break;
        case 3:
            computerbtn->layout()->addWidget(computerbtnname);
            setMaxSidebarBtn(computerbtn);
            break;
       case 4:
            controlbtn->layout()->addWidget(controlbtnname);
            setMaxSidebarBtn(controlbtn);
            break;
        default:
            break;
        }
    }

    disconnect(minmaxbtn, &QToolButton::clicked,this,&SideBarWidget::sendFullScreenBtnSignal);
    connect(minmaxbtn, &QToolButton::clicked,this,&SideBarWidget::sendDefaultBtnSignal);
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
    btn->setToolTip("");
}

void SideBarWidget::btnGroupClickedSlot(QAbstractButton *btn)
{
    char btncolor[300];
    sprintf(btncolor,"QPushButton{background:transparent;border:0px;padding-left:0;border-radius:4px;}\
            QPushButton:hover{background-color:%s;border:0px;border-radius:4px;}\
            QPushButton:pressed{background-color:%s;border:0px;border-radius:4px;}",
            SBFunBtnHoverBackground,SBFunBtnHoverBackground);

    char pressstyle[200];
    sprintf(pressstyle,"QPushButton{background-color:%s;border:0px;padding-left:0;border-radius:4px;}",SBClassifyBtnSelectedBackground);

    Q_FOREACH (QAbstractButton* button, buttonList) {
        if(pBtnGroup->id(btn)==buttonList.indexOf(button))
        {
            button->setStyleSheet(pressstyle);
            if(pBtnGroup->id(btn)==0)
            {
                if(is_fullscreen)
                    Q_EMIT sendFullScreenCommonUseBtnSignal();
                else
                    Q_EMIT sendCommonUseBtnSignal();
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
//            button->setStyleSheet("background:transparent;");
            button->setStyleSheet(btncolor);
        }
    }
}

void SideBarWidget::enterAnimation()
{
    pAnimation->setDuration(200);//动画总时间
    pAnimation->setStartValue(QRect(this->width(),0,
                                    0,this->height()));
    pAnimation->setEndValue(QRect(this->width()-Style::SideBarBtnWidth-Style::SideBarMargin,0,
                                  Style::SideBarBtnWidth,this->height()));
    pAnimation->setEasingCurve(QEasingCurve::InQuart);
//    pAnimation->setEasingCurve(QEasingCurve::Linear);

    pAnimation->start();

}

void SideBarWidget::widgetMakeZero()
{
    char pressstyle[200];
    sprintf(pressstyle,"QPushButton{background-color:%s;border:0px;padding-left:0;border-radius:2px;}",SBClassifyBtnSelectedBackground);
    commonusebtn->setStyleSheet(pressstyle);
    char btncolor[300];
    sprintf(btncolor,"QPushButton{background:transparent;border:0px;padding-left:0;border-radius:4px;}\
            QPushButton:hover{background-color:%s;border:0px;border-radius:4px;}\
            QPushButton:pressed{background-color:%s;border:0px;border-radius:4px;}",
            SBFunBtnHoverBackground,SBFunBtnHoverBackground);
    letterbtn->setStyleSheet(btncolor);
    functionbtn->setStyleSheet(btncolor);
}

//void SideBarWidget::mousePressEvent(QMouseEvent *event)
//{
//    if(is_fullscreen && event->button()==Qt::LeftButton)
//    {
//        int x=event->x();
//        int y=event->y();
//        QRect rect_1(0,0,this->width()-Style::SideBarBtnWidth-Style::SideBarMargin,this->height());
//        QRect rect_2(this->width()-Style::SideBarBtnWidth-Style::SideBarMargin,0,Style::MinMaxWidWidth,Style::MinMaxWidHeight);
//        QRect rect_3(rect_1.width()+functionbtn->x(),functionbtn->y()+functionbtn->height(),Style::SideBarBtnWidth,otherButtonList.at(0)->y()-functionbtn->y()-functionbtn->height());
//        syslog(LOG_LOCAL0 | LOG_DEBUG ,"坐标：%d:%d:%d",x,rect_1.x(),rect_3.x());
//        if((x>=rect_1.x() && x<=rect_1.x()+rect_1.width() && y>=rect_1.y() && y<=rect_1.y()+rect_1.height()) ||
//           (x>=rect_2.x() && x<=rect_2.x()+rect_2.width() && y>=rect_2.y() && y<=rect_2.y()+rect_2.height()) ||
//           (x>=rect_3.x() && x<=rect_3.x()+rect_3.width() && y>=rect_3.y() && y<=rect_3.y()+rect_3.height()))
//            Q_EMIT sendHideMainWindowSignal();
//    }
//}
