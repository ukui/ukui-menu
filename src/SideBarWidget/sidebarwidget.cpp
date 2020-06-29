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
#include <QDebug>
#include <QSvgRenderer>
#include <QPainter>
#include <QPainterPath>
#include <syslog.h>
#include "src/Style/style.h"
#include "src/UtilityFunction/utility.h"

SideBarWidget::SideBarWidget(QWidget *parent) :
    QWidget(parent)
{
    initUi();
    m_allBtn->click();
}

SideBarWidget::~SideBarWidget()
{
    delete m_ukuiMenuInterface;
    delete m_shutDownMenu;
    delete m_otherMenu;
}

/**
 * 侧边栏初始化
 */
void SideBarWidget::initUi()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setStyleSheet("border:0px;background:transparent;");
    this->setFocusPolicy(Qt::NoFocus);

    addSidebarBtn();
    loadMinSidebar();

    m_ukuiMenuInterface=new UkuiMenuInterface;
    m_shutDownMenu=new RightClickMenu;
    m_otherMenu=new RightClickMenu;
}

/**
 * 侧边栏添加控件
 */
void SideBarWidget::addSidebarBtn()
{
    m_mainWidget=new QWidget;
    m_mainWidget->setParent(this);
    m_mainWidgetLayout=new QVBoxLayout;
    m_mainWidgetLayout->setContentsMargins(0,0,0,0);
    m_mainWidgetLayout->setSpacing(10);
    m_mainWidget->setLayout(m_mainWidgetLayout);
    m_mainWidget->setStyleSheet("background:transparent;");

    //放大缩小按钮界面
    m_minMaxWidget=new QWidget;
    m_minMaxWidget->setStyleSheet("QWidget{background:transparent;border:0px;}");
    m_minMaxLayout=new QHBoxLayout;
    m_minMaxBtn=new QToolButton;
//    m_minMaxBtn->setShortcut(QKeySequence::InsertParagraphSeparator);
//    m_minMaxBtn->setShortcut(Qt::Key_Enter);
    m_minMaxBtn->setShortcut(Qt::Key_Return);
    m_minMaxLayout->addWidget(m_minMaxBtn);
    m_minMaxWidget->setLayout(m_minMaxLayout);
    char btncolor[300];
    sprintf(btncolor,"QToolButton{background:transparent;border:0px;padding-left:0px;border-radius:4px;}\
            QToolButton:hover{background-color:%s;border:0px;border-radius:4px;}\
            QToolButton:pressed{background-color:%s;border:0px;border-radius:4px;}",
            MMBtnHoverBackground,MMBtnHoverBackground);
    m_minMaxBtn->setStyleSheet(QString::fromLocal8Bit(btncolor));

    //分类按钮
    m_buttonList.clear();
    m_buttonTextList.clear();
    m_btnGroup=new QButtonGroup(m_mainWidget);
    m_allBtn=new QPushButton;
    initBtn(m_allBtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/commonuse.svg"),tr("All"),0);
    m_letterBtn=new QPushButton;
    initBtn(m_letterBtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/letter.svg"),tr("Letter"),1);
    m_functionBtn=new QPushButton;
    initBtn(m_functionBtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/function.svg"),tr("Function"),2);
    int id=0;
    Q_FOREACH (QAbstractButton* btn, m_buttonList) {
        m_btnGroup->addButton(btn,id++);
    }

    QString usericon=m_ukuiMenuInterface->getUserIcon();
    QString username=m_ukuiMenuInterface->getUserName();
    m_userIconBtn=new QPushButton;
    initBtn(m_userIconBtn,usericon,username,3);
    m_personalBtn=new QPushButton;
    initBtn(m_personalBtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/personal.svg"),tr("Personal"),5);
    m_trashBtn=new QPushButton;
    initBtn(m_trashBtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/trash.svg"),tr("Recycle Bin"),7);
    m_computerBtn=new QPushButton;
    initBtn(m_computerBtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/computer.svg"),tr("Computer"),4);
    m_controlBtn=new QPushButton;
    initBtn(m_controlBtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/control.svg"),tr("Settings"),6);
    m_shutDownBtn=new QPushButton;
    initBtn(m_shutDownBtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/shutdown.svg"),tr("Power"),8);
    connect(m_btnGroup,static_cast<void(QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked),this,&SideBarWidget::btnGroupClickedSlot);
    connect(m_computerBtn,&QPushButton::clicked,this,&SideBarWidget::computerBtnClickedSlot);
    connect(m_personalBtn,&QPushButton::clicked,this,&SideBarWidget::personalBtnClickedSlot);
    connect(m_controlBtn,&QPushButton::clicked,this,&SideBarWidget::controlBtnClickedSlot);
    connect(m_trashBtn,&QPushButton::clicked,this,&SideBarWidget::trashBtnClickedSlot);
    connect(m_shutDownBtn,&QPushButton::clicked,this,&SideBarWidget::shutdownBtnClickedSlot);
    connect(m_userIconBtn,&QPushButton::clicked,this,&SideBarWidget::userIconBtnClickedSlot);

    Q_FOREACH(QAbstractButton* button,m_buttonList)
    {
        QPushButton* btn=qobject_cast<QPushButton*>(button);
        if(m_buttonList.indexOf(button)>3 && m_buttonList.indexOf(button)<m_buttonList.count()-1)
            addRightClickMenu(btn);
    }
    m_shutDownBtn->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_shutDownBtn,&QPushButton::customContextMenuRequested,this,
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

    m_mainWidgetLayout->addWidget(m_minMaxWidget);
    m_mainWidgetLayout->addWidget(m_buttonList.at(0));
    m_mainWidgetLayout->addWidget(m_buttonList.at(1));
    m_mainWidgetLayout->addWidget(m_buttonList.at(2));
    m_verticalSpacer = new QSpacerItem(20,40, QSizePolicy::Fixed, QSizePolicy::Expanding);
    m_mainWidgetLayout->addItem(m_verticalSpacer);
    m_mainWidgetLayout->addWidget(m_buttonList.at(3));
    if(QGSettings::isSchemaInstalled(QString("org.ukui.control-center.desktop").toLocal8Bit()))
    {
        m_gsetting=new QGSettings(QString("org.ukui.control-center.desktop").toLocal8Bit());
        if(m_gsetting->keys().contains(QString("personalIconLocking")) && m_gsetting->get("personal-icon-locking").toBool())
            m_mainWidgetLayout->addWidget(m_buttonList.at(4));
        if(m_gsetting->keys().contains(QString("trashIconLocking")) && m_gsetting->get("trash-icon-locking").toBool())
            m_mainWidgetLayout->addWidget(m_buttonList.at(5));
        if(m_gsetting->keys().contains(QString("computerIconLocking")) && m_gsetting->get("computer-icon-locking").toBool())
            m_mainWidgetLayout->addWidget(m_buttonList.at(6));
        if(m_gsetting->keys().contains(QString("settingsIconLocking")) && m_gsetting->get("settings-icon-locking").toBool())
            m_mainWidgetLayout->addWidget(m_buttonList.at(7));
        connect(m_gsetting,&QGSettings::changed,
                this,&SideBarWidget::resetSidebarBtnSlot);
    }
    m_mainWidgetLayout->addWidget(m_buttonList.at(8));

    m_animation = new QPropertyAnimation(m_mainWidget, "geometry");
}

void SideBarWidget::resetSidebarBtnSlot()
{
    QLayoutItem *child;
    while ((child = m_mainWidgetLayout->takeAt(6)) != 0) {
        QWidget* wid=child->widget();
        m_mainWidgetLayout->removeWidget(wid);
        wid->setParent(nullptr);
        delete child;
    }

    if(QGSettings::isSchemaInstalled(QString("org.ukui.control-center.desktop").toLocal8Bit()))
    {
        if(m_gsetting->keys().contains(QString("personalIconLocking")) && m_gsetting->get("personal-icon-locking").toBool())
            m_mainWidgetLayout->addWidget(m_buttonList.at(4));
        if(m_gsetting->keys().contains(QString("trashIconLocking")) && m_gsetting->get("trash-icon-locking").toBool())
            m_mainWidgetLayout->addWidget(m_buttonList.at(5));
        if(m_gsetting->keys().contains(QString("computerIconLocking")) && m_gsetting->get("computer-icon-locking").toBool())
            m_mainWidgetLayout->addWidget(m_buttonList.at(6));
        if(m_gsetting->keys().contains(QString("settingsIconLocking")) && m_gsetting->get("settings-icon-locking").toBool())
            m_mainWidgetLayout->addWidget(m_buttonList.at(7));
    }
    m_mainWidgetLayout->addWidget(m_buttonList.at(8));
}

/**
 * 设置按钮样式
 */
void SideBarWidget::initBtn(QPushButton *btn, QString btnicon, QString text, int num)
{
    char btncolor[300];
    sprintf(btncolor,"QPushButton{background:transparent;border:0px;padding-left:0;border-radius:4px;}\
            QPushButton:hover{background-color:%s;border:0px;border-radius:4px;}\
            QPushButton:pressed{background-color:%s;border:0px;border-radius:4px;}",
            SBFunBtnHoverBackground,SBFunBtnHoverBackground);
    btn->setStyleSheet(QString::fromLocal8Bit(btncolor));
    QHBoxLayout* btnLayout=new QHBoxLayout;
    QLabel* labelicon=new QLabel;
    labelicon->setAlignment(Qt::AlignCenter);
    labelicon->setStyleSheet("background:transparent;border:0px;");
    const auto ratio=devicePixelRatioF();
    if(num!=3)
    {
        QPixmap pixmap=loadSvg(btnicon,Style::SideBarIconSize*ratio);
        pixmap.setDevicePixelRatio(qApp->devicePixelRatio());
        labelicon->setFixedSize(Style::SideBarIconSize,Style::SideBarIconSize);
        labelicon->setPixmap(pixmap);
    }
    else {
        if(!QFile::exists(btnicon))
            btnicon=QString("/usr/share/ukui/faces/default.png");
        labelicon->setObjectName(QStringLiteral("faceLabel"));
        labelicon->setFocusPolicy(Qt::NoFocus);
//        const QString SheetStyle = QString("border-radius: %1px;  border:0px solid white;").arg(12);
//        labelicon->setStyleSheet(SheetStyle);
        labelicon->setAlignment(Qt::AlignCenter);
        labelicon->setFixedSize(Style::SideBarIconSize+4,Style::SideBarIconSize+4);

        QPixmap facePixmap(btnicon);
        facePixmap = facePixmap.scaled((Style::SideBarIconSize+4)*ratio,(Style::SideBarIconSize+4)*ratio, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        facePixmap = PixmapToRound(facePixmap, (Style::SideBarIconSize+4)*ratio/2);
        facePixmap.setDevicePixelRatio(qApp->devicePixelRatio());
        labelicon->setPixmap(facePixmap);


//        QPixmap pixmapa;
//        QFileInfo fileInfo(btnicon);
//        if(fileInfo.isFile())
//            pixmapa=QPixmap(btnicon);
//        else
//            pixmapa=QPixmap(":/data/img/sidebarwidget/usericon-darkcolor.svg");

//        QPixmap p(btnicon);
//        QPixmap pixmapa(p.scaled(Style::SideBarIconSize+4,Style::SideBarIconSize+4, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
////        QPixmap pixmapa(p);
//        QPixmap pixmap(Style::SideBarIconSize+4,Style::SideBarIconSize+4);
//        pixmap.fill(Qt::transparent);
//        QPainter painter(&pixmap);
//        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
//        QPainterPath path;
//        path.addEllipse(0, 0, Style::SideBarIconSize+4,Style::SideBarIconSize+4);    //绘制椭圆
//        painter.setClipPath(path);
//        painter.drawPixmap(0, 0, Style::SideBarIconSize+4,Style::SideBarIconSize+4, pixmapa);

//        labelicon->setScaledContents(true);
//        labelicon->setPixmap(pixmap.scaled(Style::SideBarIconSize+4,Style::SideBarIconSize+4));
//        labelicon->setFixedSize(Style::SideBarIconSize+4,Style::SideBarIconSize+4);
    }

    QFont ft;
    QFontMetrics fm(ft);
    QString text_1 = fm.elidedText(text, Qt::ElideRight, Style::SideBarBtnWidth-44);
    QLabel* labeltext=new QLabel;
    labeltext->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    labeltext->setText(text_1);
    labeltext->setFixedSize(Style::SideBarBtnWidth-44,Style::SideBarBtnHeight);
    labeltext->setStyleSheet(QString("QLabel{background:transparent;color:#ffffff;}"));
//    labeltext->adjustSize();

    btnLayout->setSpacing(0);
    btnLayout->addWidget(labelicon);
    btnLayout->addWidget(labeltext);
    btnLayout->addStretch();
    btn->setLayout(btnLayout);
    btn->setFocusPolicy(Qt::NoFocus);

    m_buttonList.append(btn);
    m_buttonTextList.append(labeltext);
}

QPixmap SideBarWidget::PixmapToRound(const QPixmap &src, int radius)
{
    if (src.isNull()) {
        return QPixmap();
    }

    QPixmap pixmapa(src);
    QPixmap pixmap(radius*2,radius*2);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QPainterPath path;
    path.addEllipse(0, 0, radius*2, radius*2);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, radius*2, radius*2, pixmapa);
    return pixmap;
}


/**
 * 加载关机按钮右键菜单
 */
void SideBarWidget::shutdownBtnRightClickSlot()
{
    int ret=m_shutDownMenu->showShutdownMenu();
    if(ret>=10 && ret<=14)
    {
        this->parentWidget()->parentWidget()->hide();
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
//    disconnect(btn,&QPushButton::customContextMenuRequested,this,&SideBarWidget::otherBtnRightClickSlot);
    connect(btn,&QPushButton::customContextMenuRequested,this,&SideBarWidget::otherBtnRightClickSlot);
}

void SideBarWidget::otherBtnRightClickSlot()
{
    QPushButton* btn=dynamic_cast<QPushButton*>(QObject::sender());
    int index=m_buttonList.indexOf(btn);
    QString desktopfp;
    switch (index) {
    case 4:
        desktopfp=QString("/usr/share/applications/peony-home.desktop");
        break;
    case 5:
        desktopfp=QString("/usr/share/applications/peony-trash.desktop");
        break;
    case 6:
        desktopfp=QString("/usr/share/applications/peony-computer.desktop");
        break;
    case 7:
        desktopfp=QString("/usr/share/applications/ukui-control-center.desktop");
        break;
    default:
        break;
    }
    int ret=m_otherMenu->showOtherMenu(desktopfp);
    if(ret==15)
    {
        this->parentWidget()->parentWidget()->hide();
        QProcess::startDetached(QString("ukui-control-center -d"));
    }
}

void SideBarWidget::computerBtnClickedSlot()
{
    this->parentWidget()->parentWidget()->hide();
    QProcess::startDetached(QString("/usr/bin/peony computer:///"));
}

void SideBarWidget::personalBtnClickedSlot()
{
    this->parentWidget()->parentWidget()->hide();
    QProcess::startDetached(QString("/usr/bin/peony"));
}

void SideBarWidget::controlBtnClickedSlot()
{
    this->parentWidget()->parentWidget()->hide();
    QString execpath=m_ukuiMenuInterface->getAppExec(QString("/usr/share/applications/ukui-control-center.desktop"));
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
    this->parentWidget()->parentWidget()->hide();
    QProcess::startDetached(QString("/usr/bin/peony trash:///"));
}

void SideBarWidget::shutdownBtnClickedSlot()
{
    this->parentWidget()->parentWidget()->hide();
    QProcess::startDetached(QString("ukui-session-tools"));
}

void SideBarWidget::userIconBtnClickedSlot()
{
    this->parentWidget()->parentWidget()->hide();
    QProcess::startDetached(QString("ukui-control-center -u"));
}

void SideBarWidget::userAccountsChanged()
{
    const auto ratio=devicePixelRatioF();
    QString usericon=m_ukuiMenuInterface->getUserIcon();
    if(!QFile::exists(usericon))
        usericon=QString("/usr/share/ukui/faces/default.png");
    QPixmap facePixmap(usericon);
    facePixmap = facePixmap.scaled((Style::SideBarIconSize+4)*ratio,(Style::SideBarIconSize+4)*ratio, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    facePixmap = PixmapToRound(facePixmap, (Style::SideBarIconSize+4)*ratio/2);

//    QLayoutItem* item=m_userIconBtn->layout()->itemAt(0);
//    QLabel* labelicon=qobject_cast<QLabel*>(item->widget());
//    labelicon->setScaledContents(true);
    QLabel* labelicon=m_userIconBtn->findChild<QLabel*>("faceLabel");
    facePixmap.setDevicePixelRatio(qApp->devicePixelRatio());
    labelicon->setPixmap(facePixmap);
}

/**
 * 加载默认侧边栏
 */
void SideBarWidget::loadMinSidebar()
{
    m_isFullScreen=false;
    setMaxBtn();

    this->setFixedSize(Style::defaultSideBarWidWidth,Style::minh);
    m_mainWidget->setGeometry(QRect(0,0,this->width(),this->height()));
    m_mainWidget->show();
    m_minMaxWidget->setFixedSize(37,70);
    m_minMaxLayout->setContentsMargins(0,0,0,0);

    m_mainWidgetLayout->setContentsMargins(8,0,10,0);

    Q_FOREACH(QAbstractButton* button,m_buttonList)
    {
        QPushButton* btn=qobject_cast<QPushButton*>(button);
        setMinSidebarBtn(btn);
    }

    disconnect(m_minMaxBtn,&QToolButton::clicked,this, &SideBarWidget::sendDefaultBtnSignal);
    connect(m_minMaxBtn, &QToolButton::clicked,this,&SideBarWidget::sendFullScreenBtnSignal);
}

/**
 * 设置全屏按钮
 */
void SideBarWidget::setMaxBtn()
{
    m_minMaxBtn->setFixedSize(37,37);
    QSvgRenderer* svgRender = new QSvgRenderer(m_minMaxBtn);
    svgRender->load(QString(":/data/img/sidebarwidget/max.svg"));
    QPixmap* pixmap = new QPixmap(14,14);
    pixmap->fill(Qt::transparent);//设置背景透明
    QPainter p(pixmap);
    svgRender->render(&p);
    m_minMaxBtn->setIcon(QIcon(*pixmap));
}

/**
 * 设置默认侧边栏按钮
 */
void SideBarWidget::setMinSidebarBtn(QPushButton* btn)
{
    const auto ratio=devicePixelRatioF();
    btn->setFixedSize(37,37);
    if(m_buttonList.indexOf(btn)==3)
        btn->layout()->setContentsMargins(7,0,17,0);
    else
        btn->layout()->setContentsMargins(9,0,17,0);
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
void SideBarWidget::loadMaxSidebar()
{
    m_isFullScreen=true;
    setMinBtn();

    this->setFixedSize(Style::SideBarWidWidth,Style::heightavailable);
//    m_mainWidget->setGeometry(QRect(this->width()-Style::SideBarBtnWidth-Style::SideBarMargin,0,
//                                   Style::SideBarBtnWidth,this->height()));

    m_minMaxWidget->setFixedSize(Style::MinMaxWidWidth,Style::MinMaxWidHeight);
    m_minMaxLayout->setContentsMargins(m_minMaxWidget->width()-m_minMaxBtn->width(),0,0,0);

    m_mainWidgetLayout->setContentsMargins(0,0,0,0);

    Q_FOREACH(QAbstractButton* button,m_buttonList)
    {
        QPushButton* btn=qobject_cast<QPushButton*>(button);
        setMaxSidebarBtn(btn);
    }

    disconnect(m_minMaxBtn, &QToolButton::clicked,this,&SideBarWidget::sendFullScreenBtnSignal);
    connect(m_minMaxBtn, &QToolButton::clicked,this,&SideBarWidget::sendDefaultBtnSignal);
}

/**
 * 设置还原按钮
 */
void SideBarWidget::setMinBtn()
{
    m_minMaxBtn->setFixedSize(Style::MinMaxBtnWidth,Style::MinMaxBtnWidth);
    QSvgRenderer* svgRender = new QSvgRenderer(m_minMaxBtn);
    svgRender->load(QString(":/data/img/sidebarwidget/min.svg"));
    QPixmap* pixmap = new QPixmap(Style::MinMaxIconSize,Style::MinMaxIconSize);
    pixmap->fill(Qt::transparent);//设置背景透明
    QPainter p(pixmap);
    svgRender->render(&p);
    m_minMaxBtn->setIcon(QIcon(*pixmap));
}

/**
 * 设置全屏侧边栏按钮
 */
void SideBarWidget::setMaxSidebarBtn(QPushButton *btn)
{
    btn->setFixedSize(Style::SideBarBtnWidth,Style::SideBarBtnHeight);
    QHBoxLayout* layout=qobject_cast<QHBoxLayout*>(btn->layout());
    if(m_buttonList.indexOf(btn)==3)
    {
        layout->setContentsMargins(13,0,0,0);
        layout->setSpacing(8);
    }
    else
    {
        layout->setContentsMargins(15,0,0,0);
        layout->setSpacing(10);
    }
    layout->insertWidget(1,m_buttonTextList.at(m_buttonList.indexOf(btn)));
//    btn->setToolTip("");
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

    Q_FOREACH (QAbstractButton* button, m_buttonList) {
        if(m_btnGroup->id(btn)==m_buttonList.indexOf(button))
        {
            button->setStyleSheet(pressstyle);
            if(m_btnGroup->id(btn)==0)
            {
                if(m_isFullScreen)
                    Q_EMIT sendFullScreenCommonUseBtnSignal();
                else
                    Q_EMIT sendCommonUseBtnSignal();
            }
            else if(m_btnGroup->id(btn)==1)
            {
                if(m_isFullScreen)
                    Q_EMIT sendFullScreenLetterBtnSignal();
                else Q_EMIT sendLetterBtnSignal();
            }
            else{
                if(m_isFullScreen)
                    Q_EMIT sendFullScreenFunctionBtnSignal();
                else Q_EMIT sendFunctionBtnSignal();
            }
        }
        else{
            button->setStyleSheet(btncolor);
        }
    }
}

void SideBarWidget::enterAnimation()
{
    m_animation->setDuration(200);//动画总时间
    m_animation->setStartValue(QRect(this->width(),0,
                                    0,this->height()));
    m_animation->setEndValue(QRect(this->width()-Style::SideBarBtnWidth-Style::SideBarMargin,0,
                                  Style::SideBarBtnWidth,this->height()));
    m_animation->setEasingCurve(QEasingCurve::InQuart);
    m_animation->start();
    m_mainWidget->show();
}

void SideBarWidget::setSideBarBtnGeometry()
{
    m_mainWidget->setGeometry(QRect(this->width()-Style::SideBarBtnWidth-Style::SideBarMargin,0,
                                    Style::SideBarBtnWidth,this->height()));
    m_mainWidget->show();
}

void SideBarWidget::widgetMakeZero()
{
    char pressstyle[200];
    sprintf(pressstyle,"QPushButton{background-color:%s;border:0px;padding-left:0;border-radius:4px;}",SBClassifyBtnSelectedBackground);
    m_allBtn->setStyleSheet(pressstyle);
    char btncolor[300];
    sprintf(btncolor,"QPushButton{background:transparent;border:0px;padding-left:0;border-radius:4px;}\
            QPushButton:hover{background-color:%s;border:0px;border-radius:4px;}\
            QPushButton:pressed{background-color:%s;border:0px;border-radius:4px;}",
            SBFunBtnHoverBackground,SBFunBtnHoverBackground);
    m_letterBtn->setStyleSheet(btncolor);
    m_functionBtn->setStyleSheet(btncolor);
}

//void SideBarWidget::mousePressEvent(QMouseEvent *event)
//{
//    if(m_isFullScreen && event->button()==Qt::LeftButton)
//    {
//        int x=event->x();
//        int y=event->y();
//        QRect rect_1(0,0,this->width()-Style::SideBarBtnWidth-Style::SideBarMargin,this->height());
//        QRect rect_2(this->width()-Style::SideBarBtnWidth-Style::SideBarMargin,0,Style::MinMaxWidWidth,Style::MinMaxWidHeight);
//        QRect rect_3(rect_1.width()+m_functionBtn->x(),m_functionBtn->y()+m_functionBtn->height(),Style::SideBarBtnWidth,m_otherButtonList.at(0)->y()-m_functionBtn->y()-m_functionBtn->height());
//        syslog(LOG_LOCAL0 | LOG_DEBUG ,"坐标：%d:%d:%d",x,rect_1.x(),rect_3.x());
//        if((x>=rect_1.x() && x<=rect_1.x()+rect_1.width() && y>=rect_1.y() && y<=rect_1.y()+rect_1.height()) ||
//           (x>=rect_2.x() && x<=rect_2.x()+rect_2.width() && y>=rect_2.y() && y<=rect_2.y()+rect_2.height()) ||
//           (x>=rect_3.x() && x<=rect_3.x()+rect_3.width() && y>=rect_3.y() && y<=rect_3.y()+rect_3.height()))
//                 this->parentWidget()->parentWidget()->hide();
//    }
//}
