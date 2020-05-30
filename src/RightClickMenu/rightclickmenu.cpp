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

#include "rightclickmenu.h"
#include <QDebug>

RightClickMenu::RightClickMenu(QWidget *parent, int module):
    QWidget (parent)

{
    QString path=QDir::homePath()+"/.config/ukui/ukui-menu.ini";
    m_setting=new QSettings(path,QSettings::IniFormat);

    m_ukuiMenuInterface=new UkuiMenuInterface;
    m_cmdProc=new QProcess(this);
    connect(m_cmdProc , &QProcess::readyReadStandardOutput, this , &RightClickMenu::onReadOutput);

    sprintf(m_style, "QMenu{padding-left:2px;padding-top:6px;padding-right:2px;padding-bottom:6px;border:1px solid %s;border-radius:6px;background-color:%s;}\
            QMenu::separator{height:1px;background-color:%s;margin-top:2px;margin-bottom:2px;}",
            RightClickMenuBorder ,RightClickMenuBackground,RightClickMenuSeparator);

    switch (module) {
    case 0:
        addAppBtnAction();
        break;
    case 1:
        addShutdownAction();
        break;
    case 2:
        addOtherAction();
        break;
    default:
        break;
    }
}

RightClickMenu::~RightClickMenu()
{
    delete m_ukuiMenuInterface;

}

//普通应用按钮右键菜单
void RightClickMenu::initAppBtnAction()
{
    //普通应用按钮右键菜单
    m_appBtnMenu=new QMenu(this);
    m_appBtnMenu->setLayoutDirection(Qt::LeftToRight);
    m_appBtnMenu->setFixedSize(250+2,36*4+5+12+2);
    m_fixToAllAction=new QWidgetAction(m_appBtnMenu);
    m_fixToAllWid=new QWidget();
    m_unfixedFromAllAction=new QWidgetAction(m_appBtnMenu);
    m_unfixedFromAllWid=new QWidget();
    m_fixToTaskBarAction=new QWidgetAction(m_appBtnMenu);
    m_fixToTaskBarWid=new QWidget();
    m_unfixedFromTaskBarAction=new QWidgetAction(m_appBtnMenu);
    m_unfixedFromTaskBarWid=new QWidget();
    m_addToDesktopAction=new QWidgetAction(m_appBtnMenu);
    m_addToDesktopWid=new QWidget();
    m_uninstallAction=new QWidgetAction(m_appBtnMenu);
    m_uninstallWid=new QWidget();
    m_attributeAction=new QWidgetAction(m_appBtnMenu);
    m_attributeWid=new QWidget();

    initWidgetAction(m_fixToAllWid,":/data/img/mainviewwidget/fixed.svg",tr("Pin to all"),0);
    m_fixToAllAction->setDefaultWidget(m_fixToAllWid);
    connect(m_fixToAllAction, &QWidgetAction::triggered,this,&RightClickMenu::fixToAllActionTriggerSlot);

    initWidgetAction(m_unfixedFromAllWid,":/data/img/mainviewwidget/unfixed.svg",tr("Unpin from all"),0);
    m_unfixedFromAllAction->setDefaultWidget(m_unfixedFromAllWid);
    connect(m_unfixedFromAllAction, &QWidgetAction::triggered,this,&RightClickMenu::unfixedFromAllActionTriggerSlot);

    initWidgetAction(m_fixToTaskBarWid,":/data/img/mainviewwidget/fixed.svg",tr("Pin to taskbar"),0);
    m_fixToTaskBarAction->setDefaultWidget(m_fixToTaskBarWid);
    connect(m_fixToTaskBarAction, &QWidgetAction::triggered,this,&RightClickMenu::fixToTaskbarActionTriggerSlot);

    initWidgetAction(m_unfixedFromTaskBarWid,":/data/img/mainviewwidget/unfixed.svg",tr("Unpin from taskbar"),0);
    m_unfixedFromTaskBarAction->setDefaultWidget(m_unfixedFromTaskBarWid);
    connect(m_unfixedFromTaskBarAction, &QWidgetAction::triggered,this,&RightClickMenu::unfixedFromTaskbarActionTriggerSlot);

    initWidgetAction(m_addToDesktopWid,"",tr("Add to desktop shortcuts"),0);
    m_addToDesktopAction->setDefaultWidget(m_addToDesktopWid);
    connect(m_addToDesktopAction, &QWidgetAction::triggered,this,&RightClickMenu::addToDesktopActionTriggerSlot);

    initWidgetAction(m_uninstallWid,":/data/img/mainviewwidget/uninstall.svg",tr("Uninstall"),0);
    m_uninstallAction->setDefaultWidget(m_uninstallWid);
    connect(m_uninstallAction, &QWidgetAction::triggered,this,&RightClickMenu::uninstallActionTriggerSlot);
}

void RightClickMenu::addAppBtnAction()
{
    initAppBtnAction();
    m_appBtnMenu->addAction(m_addToDesktopAction);
    m_appBtnMenu->addSeparator();
    m_appBtnMenu->addAction(m_uninstallAction);
    m_appBtnMenu->setAttribute(Qt::WA_TranslucentBackground);
    m_appBtnMenu->setStyleSheet(m_style);
}

//关机按钮右键菜单
void RightClickMenu::addShutdownAction()
{
    //关机按钮右键菜单
    m_shutDownMenu=new QMenu(this);
    m_shutDownMenu->setLayoutDirection(Qt::LeftToRight);
    m_shutDownMenu->setFixedSize(250+2,36*5+12+2);

    m_lockScreenAction=new QWidgetAction(m_shutDownMenu);
    m_lockScreenWid=new QWidget();
    m_switchUserAction=new QWidgetAction(m_shutDownMenu);
    m_switchUserWid=new QWidget();
    m_logOutAction=new QWidgetAction(m_shutDownMenu);
    m_logOutWid=new QWidget();
    m_rebootAction=new QWidgetAction(m_shutDownMenu);
    m_rebootWid=new QWidget();
    m_shutDownAction=new QWidgetAction(m_shutDownMenu);
    m_shutDownWid=new QWidget();

    initWidgetAction(m_lockScreenWid,"system-lock-screen-symbolic",tr("Lock Screen"),1);
    m_lockScreenAction->setDefaultWidget(m_lockScreenWid);
    m_shutDownMenu->addAction(m_lockScreenAction);
    connect(m_lockScreenAction,&QWidgetAction::triggered,this,&RightClickMenu::lockScreenActionTriggerSlot);

    initWidgetAction(m_switchUserWid,"stock-people-symbolic",tr("Switch User"),1);
    m_switchUserAction->setDefaultWidget(m_switchUserWid);
    m_shutDownMenu->addAction(m_switchUserAction);
    connect(m_switchUserAction,&QWidgetAction::triggered,this,&RightClickMenu::switchUserActionTriggerSlot);

    initWidgetAction(m_logOutWid,"system-logout-symbolic",tr("Log Out"),1);
    m_logOutAction->setDefaultWidget(m_logOutWid);
    m_shutDownMenu->addAction(m_logOutAction);
    connect(m_logOutAction,&QWidgetAction::triggered,this,&RightClickMenu::logoutActionTriggerSlot);

    initWidgetAction(m_rebootWid,"system-restart-symbolic",tr("Restart"),1);
    m_rebootAction->setDefaultWidget(m_rebootWid);
    m_shutDownMenu->addAction(m_rebootAction);
    connect(m_rebootAction,&QWidgetAction::triggered,this,&RightClickMenu::rebootActionTriggerSlot);

    initWidgetAction(m_shutDownWid,"exit-symbolic",tr("Power Off"),1);
    m_shutDownAction->setDefaultWidget(m_shutDownWid);
    m_shutDownMenu->addAction(m_shutDownAction);
    connect(m_shutDownAction,&QWidgetAction::triggered,this,&RightClickMenu::shutdownActionTriggerSlot);

    m_shutDownMenu->setAttribute(Qt::WA_TranslucentBackground);
    m_shutDownMenu->setStyleSheet(m_style);
}

//其它按钮右键菜单
void RightClickMenu::addOtherAction()
{
    m_otherMenu=new QMenu(this);
    m_otherMenu->setLayoutDirection(Qt::LeftToRight);
    m_otherMenu->setFixedSize(250+2,36*2+12+2);
    m_otherFixToTaskBarAction=new QWidgetAction(m_otherMenu);
    m_otherFixToTaskBarWid=new QWidget();
    m_otherUnfixedFromTaskBarAction=new QWidgetAction(m_otherMenu);
    m_otherUnfixedFromTaskBarWid=new QWidget();
    m_otherListAction=new QWidgetAction(m_otherMenu);
    m_otherListWid=new QWidget();

    initWidgetAction(m_otherFixToTaskBarWid,":/data/img/sidebarwidget/fixed.svg",tr("Pin to taskbar"),0);
    m_otherFixToTaskBarAction->setDefaultWidget(m_otherFixToTaskBarWid);
    connect(m_otherFixToTaskBarAction,&QWidgetAction::triggered,this,&RightClickMenu::fixToTaskbarActionTriggerSlot);

    initWidgetAction(m_otherUnfixedFromTaskBarWid,":/data/img/sidebarwidget/unfixed.svg",tr("Unpin from taskbar"),0);
    m_otherUnfixedFromTaskBarAction->setDefaultWidget(m_otherUnfixedFromTaskBarWid);
    connect(m_otherUnfixedFromTaskBarAction,&QWidgetAction::triggered,this,&RightClickMenu::unfixedFromTaskbarActionTriggerSlot);

    initWidgetAction(m_otherListWid,":/data/img/sidebarwidget/setting.svg",tr("Personalize this list"),0);
    m_otherListAction->setDefaultWidget(m_otherListWid);
    connect(m_otherListAction,&QWidgetAction::triggered,this,&RightClickMenu::otherListActionTriggerSlot);

    m_otherMenu->addAction(m_otherListAction);
    m_otherMenu->setAttribute(Qt::WA_TranslucentBackground);
    m_otherMenu->setStyleSheet(m_style);
}

void RightClickMenu::initWidgetAction(QWidget *wid, QString iconstr, QString textstr,int type)
{
    char style[200];
    sprintf(style,"QWidget{background:transparent;border:0px;border-radius:2px;}\
            QWidget:hover{background-color:%s;}",
            RightClickMenuHover);

    char labelstyle[100];
    sprintf(labelstyle,"background:transparent;border:0px;color:%s;font-size:14px;",
            RightClickMenuFont);

    QHBoxLayout* layout=new QHBoxLayout(wid);
    wid->setLayout(layout);
    wid->setFixedSize(246,36);
    wid->setStyleSheet(style);
    wid->setFocusPolicy(Qt::NoFocus);

    if(!iconstr.isEmpty())
    {
        QLabel* labelicon=new QLabel(wid);
        if(type==0)
        {
            QSvgRenderer* svg=new QSvgRenderer(wid);
            svg->load(iconstr);
            QPixmap* pixmap=new QPixmap(16,16);
            pixmap->fill(Qt::transparent);
            QPainter p(pixmap);
            svg->render(&p);
            labelicon->setPixmap(*pixmap);
        }
        else
        {
            QIcon icon=QIcon::fromTheme(iconstr);
            QPixmap pixmap=icon.pixmap(QSize(16,16));
            labelicon->setPixmap(drawSymbolicColoredPixmap(pixmap));
        }
        labelicon->setFixedSize(QSize(16,16));
        labelicon->setAlignment(Qt::AlignCenter);
        labelicon->setStyleSheet(labelstyle);
        layout->addWidget(labelicon);
    }

    QLabel* labeltext=new QLabel(wid);
    labeltext->setStyleSheet(labelstyle);
    labeltext->setText(textstr);
    labeltext->adjustSize();
    layout->addWidget(labeltext);

    if(!iconstr.isEmpty())
    {
        layout->setContentsMargins(10,0,wid->width()-16-labeltext->width()-20,0);
        layout->setSpacing(10);
    }
    else {
        layout->setContentsMargins(36,0,0,0);
    }
}

QPixmap RightClickMenu::drawSymbolicColoredPixmap(const QPixmap &source)
{
    QColor gray(128,128,128);
    QColor standard (31,32,34);
    QImage img = source.toImage();
    for (int x = 0; x < img.width(); x++) {
        for (int y = 0; y < img.height(); y++) {
            auto color = img.pixelColor(x, y);
            if (color.alpha() > 0) {
                if (qAbs(color.red()-gray.red())<20 && qAbs(color.green()-gray.green())<20 && qAbs(color.blue()-gray.blue())<20) {
                    color.setRed(255);
                    color.setGreen(255);
                    color.setBlue(255);
                    img.setPixelColor(x, y, color);
                }
                else if(qAbs(color.red()-standard.red())<20 && qAbs(color.green()-standard.green())<20 && qAbs(color.blue()-standard.blue())<20)
                {
                    color.setRed(255);
                    color.setGreen(255);
                    color.setBlue(255);
                    img.setPixelColor(x, y, color);
                }
                else
                {
                    img.setPixelColor(x, y, color);
                }
            }
        }
    }
    return QPixmap::fromImage(img);
}

void RightClickMenu::fixToAllActionTriggerSlot()
{
    m_actionNumber=1;
    QFileInfo fileInfo(m_desktopfp);
    QString desktopfn=fileInfo.fileName();
    m_setting->beginGroup("lockapplication");
    m_setting->setValue(desktopfn,m_setting->allKeys().size());
    m_setting->sync();
    m_setting->endGroup();
    m_setting->beginGroup("application");
    m_setting->remove(desktopfn);
    m_setting->sync();
    m_setting->endGroup();
    m_setting->beginGroup("datetime");
    m_setting->remove(desktopfn);
    m_setting->sync();
    m_setting->endGroup();
    m_setting->beginGroup("recentapp");
    m_setting->remove(desktopfn);
    m_setting->sync();
    m_setting->endGroup();
}

void RightClickMenu::unfixedFromAllActionTriggerSlot()
{
    m_actionNumber=2;
    QFileInfo fileInfo(m_desktopfp);
    QString desktopfn=fileInfo.fileName();
    m_setting->beginGroup("lockapplication");
    Q_FOREACH(QString desktop,m_setting->allKeys())
    {
        if(m_setting->value(desktop).toInt() > m_setting->value(desktopfn).toInt())
        {
            m_setting->setValue(desktop,m_setting->value(desktop).toInt()-1);
        }
    }
    m_setting->remove(desktopfn);
    m_setting->sync();
    m_setting->endGroup();
    m_setting->beginGroup("application");
    m_setting->remove(desktopfn);
    m_setting->sync();
    m_setting->endGroup();
    m_setting->beginGroup("datetime");
    m_setting->remove(desktopfn);
    m_setting->sync();
    m_setting->endGroup();
}

void RightClickMenu::fixToTaskbarActionTriggerSlot()
{
    QDBusInterface iface("com.ukui.panel.desktop",
                         "/",
                         "com.ukui.panel.desktop",
                         QDBusConnection::sessionBus());
    QDBusReply<QVariant> ret=iface.call("AddToTaskbar",m_desktopfp);
    m_actionNumber=3;
}

void RightClickMenu::unfixedFromTaskbarActionTriggerSlot()
{
    QDBusInterface iface("com.ukui.panel.desktop",
                         "/",
                         "com.ukui.panel.desktop",
                         QDBusConnection::sessionBus());
    QDBusReply<QVariant> ret=iface.call("RemoveFromTaskbar",m_desktopfp);
    m_actionNumber=4;
}

void RightClickMenu::addToDesktopActionTriggerSlot()
{
    QString path=QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QFileInfo fileInfo(m_desktopfp);
    QString desktopfn=fileInfo.fileName();
    QFile file(m_desktopfp);
    QString newname=QString(path+"/"+desktopfn);
    bool ret=file.copy(QString(path+"/"+desktopfn));
    if(ret)
    {
        char command[200];
        sprintf(command,"chmod a+x %s",newname.toLocal8Bit().data());
        QProcess::startDetached(QString::fromLocal8Bit(command));
    }
    m_actionNumber=5;
}

void RightClickMenu::uninstallActionTriggerSlot()
{
    QString cmd=QString("dpkg -S "+m_desktopfp);
    m_cmdProc->setReadChannel(QProcess::StandardOutput);
    m_cmdProc->start("sh",QStringList()<<"-c"<<cmd);
    m_cmdProc->waitForFinished();
    m_cmdProc->waitForReadyRead();
    m_cmdProc->close();
    m_actionNumber=6;
}

void RightClickMenu::onReadOutput()
{
    QString packagestr=QString::fromLocal8Bit(m_cmdProc->readAllStandardOutput().data());
    QString packageName=packagestr.split(":").at(0);
    qDebug()<<packagestr<<packageName;
    char command[100];
    sprintf(command,"ubuntu-kylin-software-center -remove %s",packageName.toLocal8Bit().data());
    QProcess::startDetached(command);
}

void RightClickMenu::attributeActionTriggerSlot()
{
    char command[100];
    sprintf(command,"ukui-menu-attr %s",m_desktopfp.toLocal8Bit().data());
    QProcess::startDetached(command);
    m_actionNumber=7;
}

void RightClickMenu::lockScreenActionTriggerSlot()
{
    m_actionNumber=10;
}

void RightClickMenu::switchUserActionTriggerSlot()
{
    m_actionNumber=11;
}

void RightClickMenu::logoutActionTriggerSlot()
{
    m_actionNumber=12;
}

void RightClickMenu::rebootActionTriggerSlot()
{
    m_actionNumber=13;
}

void RightClickMenu::shutdownActionTriggerSlot()
{
    m_actionNumber=14;
}

void RightClickMenu::otherListActionTriggerSlot()
{
    m_actionNumber=15;
}

int RightClickMenu::showAppBtnMenu(QString desktopfp)
{
    m_actionNumber=0;
    this->m_desktopfp.clear();
    this->m_desktopfp=desktopfp;
    m_appBtnMenu->removeAction(m_fixToAllAction);
    m_appBtnMenu->removeAction(m_unfixedFromAllAction);
    m_appBtnMenu->removeAction(m_fixToTaskBarAction);
    m_appBtnMenu->removeAction(m_unfixedFromTaskBarAction);
    m_setting->beginGroup("lockapplication");
    QFileInfo fileInfo(desktopfp);
    QString desktopfn=fileInfo.fileName();
    if(!m_setting->contains(desktopfn))
        m_appBtnMenu->insertAction(m_addToDesktopAction,m_fixToAllAction);
    else
        m_appBtnMenu->insertAction(m_addToDesktopAction,m_unfixedFromAllAction);
    m_setting->endGroup();

    QDBusInterface iface("com.ukui.panel.desktop",
                         "/",
                         "com.ukui.panel.desktop",
                         QDBusConnection::sessionBus());

    QDBusReply<bool> ret=iface.call("CheckIfExist",desktopfp);
    if(!ret)
        m_appBtnMenu->insertAction(m_addToDesktopAction,m_fixToTaskBarAction);
    else
        m_appBtnMenu->insertAction(m_addToDesktopAction,m_unfixedFromTaskBarAction);
    m_appBtnMenu->exec(QCursor::pos());
    return m_actionNumber;
}

int RightClickMenu::showShutdownMenu()
{
    m_actionNumber=0;
    m_shutDownMenu->exec(QCursor::pos());
    return m_actionNumber;

}

int RightClickMenu::showOtherMenu(QString desktopfp)
{
    m_actionNumber=0;
    this->m_desktopfp.clear();
    this->m_desktopfp=desktopfp;
    m_otherMenu->removeAction(m_otherFixToTaskBarAction);
    m_otherMenu->removeAction(m_otherUnfixedFromTaskBarAction);
    QDBusInterface iface("com.ukui.panel.desktop",
                         "/",
                         "com.ukui.panel.desktop",
                         QDBusConnection::sessionBus());

    QDBusReply<bool> ret=iface.call("CheckIfExist",this->m_desktopfp);
    if(!ret)
        m_otherMenu->insertAction(m_otherListAction,m_otherFixToTaskBarAction);
    else
        m_otherMenu->insertAction(m_otherListAction,m_otherUnfixedFromTaskBarAction);
    m_otherMenu->exec(QCursor::pos());
    return m_actionNumber;
}
