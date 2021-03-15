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
#include <QShortcut>
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
    m_mainWidgetLayout->setContentsMargins(0,0,0,6);
    m_mainWidgetLayout->setSpacing(10);
    m_mainWidget->setLayout(m_mainWidgetLayout);

    //放大缩小按钮界面
    m_minMaxWidget=new QWidget;
    m_minMaxLayout=new QHBoxLayout;
    m_minMaxBtn=new QPushButton;
    m_minMaxBtn->setFlat(true);
    m_minMaxBtn->setProperty("doNotAnimate",true);
//    m_minMaxBtn->setShortcut(QKeySequence::InsertParagraphSeparator);
//    m_minMaxBtn->setShortcut(Qt::Key_Enter);
//    m_minMaxBtn->setShortcut(Qt::Key_Return);
    QShortcut *key_1 = new QShortcut(QKeySequence(Qt::Key_Enter),this);
    connect(key_1,&QShortcut::activated,m_minMaxBtn,&QPushButton::click);
    QShortcut *key_2 = new QShortcut(QKeySequence(Qt::Key_Return),this);
    connect(key_2,&QShortcut::activated,m_minMaxBtn,&QPushButton::click);
    m_minMaxLayout->addWidget(m_minMaxBtn);
    m_minMaxWidget->setLayout(m_minMaxLayout);

    //分类按钮
    m_buttonList.clear();
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
        btn->setCheckable(true);
    }

    QString usericon=m_ukuiMenuInterface->getUserIcon();
    QString username=m_ukuiMenuInterface->getUserName();
    m_userIconBtn=new QPushButton;
    initBtn(m_userIconBtn,usericon,username,3);
    m_personalBtn=new QPushButton;
    initBtn(m_personalBtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/personal.svg"),tr("Personal"),4);
    m_trashBtn=new QPushButton;
    initBtn(m_trashBtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/trash.svg"),tr("Trash"),5);
    m_computerBtn=new QPushButton;
    initBtn(m_computerBtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/computer.svg"),tr("Computer"),6);
    m_controlBtn=new QPushButton;
    initBtn(m_controlBtn,QString::fromLocal8Bit(":/data/img/sidebarwidget/control.svg"),tr("Settings"),7);
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
        {
            button->setCheckable(false);
            addRightClickMenu(btn);
        }
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

    if(QGSettings::isSchemaInstalled(QString("org.ukui.style").toLocal8Bit()))
    {
        QGSettings* gsetting=new QGSettings(QString("org.ukui.style").toLocal8Bit());
        connect(gsetting,&QGSettings::changed,this,&SideBarWidget::themeModeChangeSlot);
    }

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
    btn->setFlat(true);
    btn->setProperty("doNotAnimate",true);
    QHBoxLayout* btnLayout=new QHBoxLayout;
    QLabel* labelicon=new QLabel;
    labelicon->setAlignment(Qt::AlignCenter);
    const auto ratio=devicePixelRatioF();
    QGSettings gsetting(QString("org.ukui.style").toLocal8Bit());
    if(num!=3)
    {
        QPixmap pixmap=loadSvg(btnicon,Style::SideBarIconSize*ratio);
        if(gsetting.get("style-name").toString()=="ukui-light")//反黑
            pixmap=drawSymbolicBlackColoredPixmap(pixmap);
        else
            pixmap=drawSymbolicColoredPixmap(pixmap);//反白
        pixmap.setDevicePixelRatio(qApp->devicePixelRatio());
        labelicon->setFixedSize(Style::SideBarIconSize,Style::SideBarIconSize);
        labelicon->setPixmap(pixmap);
    }
    else {
        if(!QFile::exists(btnicon))
            btnicon=QString("/usr/share/ukui/faces/default.png");
        labelicon->setObjectName(QStringLiteral("faceLabel"));
        labelicon->setFocusPolicy(Qt::NoFocus);
        labelicon->setAlignment(Qt::AlignCenter);
        labelicon->setFixedSize(Style::SideBarIconSize+4,Style::SideBarIconSize+4);

        QPixmap facePixmap(btnicon);
        facePixmap = facePixmap.scaled((Style::SideBarIconSize+4)*ratio,(Style::SideBarIconSize+4)*ratio, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        facePixmap = PixmapToRound(facePixmap, (Style::SideBarIconSize+4)*ratio/2);
        facePixmap.setDevicePixelRatio(qApp->devicePixelRatio());
        labelicon->setPixmap(facePixmap);
    }

    btnLayout->setSpacing(0);
    btnLayout->addWidget(labelicon);
    btnLayout->addStretch();
    btn->setLayout(btnLayout);
    btn->setFocusPolicy(Qt::NoFocus);

    m_buttonList.append(btn);
    m_textList.append(text);
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
    if(ret>=10 && ret<=17)
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
        case 16:
            QProcess::startDetached(QString("ukui-session-tools --suspend"));
            break;
        case 17:
            QProcess::startDetached(QString("ukui-session-tools --sleep"));
            break;
        default:
            break;
        }

    }
}

void SideBarWidget::addRightClickMenu(QPushButton *btn)
{
    btn->setContextMenuPolicy(Qt::CustomContextMenu);
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
    Q_EMIT sendHideMainWindowSignal();
    QProcess::startDetached(QString("/usr/bin/peony trash:///"));
}

void SideBarWidget::shutdownBtnClickedSlot()
{
    Q_EMIT sendHideMainWindowSignal();
    QProcess::startDetached(QString("ukui-session-tools"));
}

void SideBarWidget::userIconBtnClickedSlot()
{
    Q_EMIT sendHideMainWindowSignal();
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

    m_mainWidgetLayout->setContentsMargins(8,0,10,6);

    Q_FOREACH(QAbstractButton* button,m_buttonList)
    {
        QPushButton* btn=qobject_cast<QPushButton*>(button);
        setMinSidebarBtn(btn);
        btn->setToolTip(m_textList.at(m_buttonList.indexOf(button)));
    }

    changeIconColor(false);

    disconnect(m_minMaxBtn,&QToolButton::clicked,this, &SideBarWidget::sendDefaultBtnSignal);
    connect(m_minMaxBtn, &QToolButton::clicked,this,&SideBarWidget::sendFullScreenBtnSignal);
}

/**
 * 设置全屏按钮
 */
void SideBarWidget::setMaxBtn()
{
    const auto ratio=devicePixelRatioF();
    QGSettings gsetting(QString("org.ukui.style").toLocal8Bit());
    QPixmap pixmap=loadSvg(QString(":/data/img/sidebarwidget/max.svg"),14*ratio);
    if(gsetting.get("style-name").toString()=="ukui-light")//反黑
        pixmap=drawSymbolicBlackColoredPixmap(pixmap);
    else
        pixmap=drawSymbolicColoredPixmap(pixmap);//反白
    pixmap.setDevicePixelRatio(qApp->devicePixelRatio());

    m_minMaxBtn->setFixedSize(37,37);
    m_minMaxBtn->setIcon(QIcon(pixmap));
}

/**
 * 设置默认侧边栏按钮
 */
void SideBarWidget::setMinSidebarBtn(QPushButton* btn)
{
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
            delete childwid;
            delete child;
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

    m_minMaxWidget->setFixedSize(Style::MinMaxWidWidth,Style::MinMaxWidHeight);
    m_minMaxLayout->setContentsMargins(m_minMaxWidget->width()-m_minMaxBtn->width(),0,0,0);

    m_mainWidgetLayout->setContentsMargins(0,0,0,6);

    Q_FOREACH(QAbstractButton* button,m_buttonList)
    {
        QPushButton* btn=qobject_cast<QPushButton*>(button);
        setMaxSidebarBtn(btn);
    }

    Q_FOREACH(QAbstractButton* button,m_buttonList)
    {
        QPushButton* btn=qobject_cast<QPushButton*>(button);
        btn->setFixedSize(m_btnWidth,Style::SideBarBtnHeight);
    }

    changeIconColor(true);

    disconnect(m_minMaxBtn, &QToolButton::clicked,this,&SideBarWidget::sendFullScreenBtnSignal);
    connect(m_minMaxBtn, &QToolButton::clicked,this,&SideBarWidget::sendDefaultBtnSignal);
}

/**
 * 设置还原按钮
 */
void SideBarWidget::setMinBtn()
{
    const auto ratio=devicePixelRatioF();
    QPixmap pixmap=loadSvg(QString(":/data/img/sidebarwidget/min.svg"),14*ratio);
    pixmap=drawSymbolicColoredPixmap(pixmap);//反白
    pixmap.setDevicePixelRatio(qApp->devicePixelRatio());

    m_minMaxBtn->setFixedSize(Style::MinMaxBtnWidth,Style::MinMaxBtnWidth);
    m_minMaxBtn->setIcon(QIcon(pixmap));
}

/**
 * 设置全屏侧边栏按钮
 */
void SideBarWidget::setMaxSidebarBtn(QPushButton *btn)
{
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

    //修复修改字体大小时获取文本Label大小无效
    //移除按钮文本
    QLayoutItem *child;
    if((child = btn->layout()->takeAt(1)) != nullptr) {
        QWidget* childwid=child->widget();
        if(childwid!=nullptr)
        {
            btn->layout()->removeWidget(childwid);
            childwid->setParent(nullptr);
            delete childwid;
            delete child;
        }
    }
    //添加文本
    QLabel* labeltext=new QLabel;
    if(m_buttonList.indexOf(btn)<=2)
    {
        labeltext->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        labeltext->setText(m_textList.at(m_buttonList.indexOf(btn)));
        labeltext->adjustSize();
        m_btnWidth=44+labeltext->width()+10;
        btn->setToolTip("");
    }
    else
    {
            QFont ft;
            QFontMetrics fm(ft);
            QString text = fm.elidedText(m_textList.at(m_buttonList.indexOf(btn)), Qt::ElideRight, m_btnWidth-44);
            labeltext->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            labeltext->setText(text);
            labeltext->setFixedSize(m_btnWidth-44,Style::SideBarBtnHeight);
            labeltext->adjustSize();
            if(fm.boundingRect(m_textList.at(m_buttonList.indexOf(btn))).width()>labeltext->width())
                btn->setToolTip(m_textList.at(m_buttonList.indexOf(btn)));
            else
                btn->setToolTip("");
    }
    QPalette pe = labeltext->palette();
    pe.setColor(QPalette::ButtonText,QColor(Qt::white));
    labeltext->setPalette(pe);
    btn->layout()->addWidget(labeltext);
}

void SideBarWidget::btnGroupClickedSlot(QAbstractButton *btn)
{
    Q_FOREACH (QAbstractButton* button, m_buttonList) {
        if(m_btnGroup->id(btn)==m_buttonList.indexOf(button))
        {
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
    }
}

void SideBarWidget::themeModeChangeSlot(QString styleName)
{
    if(styleName=="styleName")
    {
        QGSettings* gsetting=new QGSettings(QString("org.ukui.style").toLocal8Bit());
        Q_FOREACH(QAbstractButton *button,m_buttonList)
        {
            QPushButton *btn=qobject_cast<QPushButton*>(button);
            QLayoutItem *item=btn->layout()->itemAt(0);
            QLabel *label=qobject_cast<QLabel*>(item->widget());
            QString iconStr;
            switch (m_buttonList.indexOf(button)) {
            case 0:
                iconStr=QString(":/data/img/sidebarwidget/commonuse.svg");
                break;
            case 1:
                iconStr=QString(":/data/img/sidebarwidget/letter.svg");
                break;
            case 2:
                iconStr=QString(":/data/img/sidebarwidget/function.svg");
                break;
            case 3:
                userAccountsChanged();
                break;
            case 4:
                iconStr=QString(":/data/img/sidebarwidget/personal.svg");
                break;
            case 5:
                iconStr=QString(":/data/img/sidebarwidget/trash.svg");
                break;
            case 6:
                iconStr=QString(":/data/img/sidebarwidget/computer.svg");
                break;
            case 7:
                iconStr=QString(":/data/img/sidebarwidget/setting.svg");
                break;
            case 8:
                iconStr=QString(":/data/img/sidebarwidget/shutdown.svg");
                break;
            default:
                break;
            }
            if(m_buttonList.indexOf(button)!=3)
            {
                const auto ratio=devicePixelRatioF();
                QPixmap pixmap=loadSvg(iconStr,19*ratio);
                if(gsetting->get("style-name").toString()=="ukui-light")//反黑
                    pixmap=drawSymbolicBlackColoredPixmap(pixmap);
                else
                    pixmap=drawSymbolicColoredPixmap(pixmap);//反白
                pixmap.setDevicePixelRatio(qApp->devicePixelRatio());
                label->setPixmap(pixmap);
            }
        }
    }
}

void SideBarWidget::changeIconColor(bool isFullScreen)
{
    QGSettings* gsetting=new QGSettings(QString("org.ukui.style").toLocal8Bit());
    Q_FOREACH(QAbstractButton *button,m_buttonList)
    {
        QPushButton *btn=qobject_cast<QPushButton*>(button);
        QLayoutItem *item=btn->layout()->itemAt(0);
        QLabel *label=qobject_cast<QLabel*>(item->widget());
        QString iconStr;
        switch (m_buttonList.indexOf(button)) {
        case 0:
            iconStr=QString(":/data/img/sidebarwidget/commonuse.svg");
            break;
        case 1:
            iconStr=QString(":/data/img/sidebarwidget/letter.svg");
            break;
        case 2:
            iconStr=QString(":/data/img/sidebarwidget/function.svg");
            break;
        case 3:
            userAccountsChanged();
            break;
        case 4:
            iconStr=QString(":/data/img/sidebarwidget/personal.svg");
            break;
        case 5:
            iconStr=QString(":/data/img/sidebarwidget/trash.svg");
            break;
        case 6:
            iconStr=QString(":/data/img/sidebarwidget/computer.svg");
            break;
        case 7:
            iconStr=QString(":/data/img/sidebarwidget/setting.svg");
            break;
        case 8:
            iconStr=QString(":/data/img/sidebarwidget/shutdown.svg");
            break;
        default:
            break;
        }
        if(m_buttonList.indexOf(button)!=3)
        {
            const auto ratio=devicePixelRatioF();
            QPixmap pixmap=loadSvg(iconStr,19*ratio);
            if(gsetting->get("style-name").toString()=="ukui-light" && !isFullScreen)//反黑
                pixmap=drawSymbolicBlackColoredPixmap(pixmap);
            else
                pixmap=drawSymbolicColoredPixmap(pixmap);//反白
            pixmap.setDevicePixelRatio(qApp->devicePixelRatio());
            label->setPixmap(pixmap);
        }
    }
}

void SideBarWidget::enterAnimation()
{
    m_animation->setDuration(200);//动画总时间
    m_animation->setStartValue(QRect(this->width(),0,
                                    0,this->height()));
    m_animation->setEndValue(QRect(this->width()-m_btnWidth-Style::SideBarMargin,0,
                                   m_btnWidth,this->height()));
    m_animation->setEasingCurve(QEasingCurve::InQuart);
    m_animation->start();
    m_mainWidget->show();
}

void SideBarWidget::setSideBarBtnGeometry()
{
    m_mainWidget->setGeometry(QRect(this->width()-m_btnWidth-Style::SideBarMargin,0,
                                    m_btnWidth,this->height()));
    m_mainWidget->show();
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
