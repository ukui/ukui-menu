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
    setting=new QSettings(path,QSettings::IniFormat);

    pUkuiMenuInterface=new UkuiMenuInterface;
    cmdProc=new QProcess(this);
    connect(cmdProc , &QProcess::readyReadStandardOutput, this , &RightClickMenu::onReadOutput);

//    sprintf(style, "QMenu{padding-left:2px;padding-top:6px;padding-right:2px;padding-bottom:6px;border:1px solid %s;border-radius:6px;background-color:%s;}\
//            QMenu::separator{height:4px;background:transparent;}",
//            RightClickMenuBorder ,RightClickMenuBackground);

    sprintf(style, "QMenu{padding-left:2px;padding-top:6px;padding-right:2px;padding-bottom:6px;border:1px solid %s;border-radius:6px;background-color:%s;}\
            QMenu::separator{height:1px;background-color:%s;margin-top:2px;margin-bottom:2px;}",
            RightClickMenuBorder ,RightClickMenuBackground,RightClickMenuSeparator);

    switch (module) {
    case 0:
        addCommonUseAppBtnAction();
        break;
    case 1:
        addAppBtnAction();
        break;
    case 2:
        addShutdownAction();
        break;
    case 3:
        addOtherAction();
        break;
    default:
        break;
    }
}

RightClickMenu::~RightClickMenu()
{
//    delete cuappbtnmenu;
//    delete CuFix2CommonUseWid;
//    delete CuUnfixed4CommonUseWid;
//    delete CuFix2TaskBarWid;
//    delete CuUnfixed4TaskBarWid;
//    delete CuAdd2DesktopWid;
//    delete CuDeleteAllWid;
//    delete CuDeleteAllWid;
//    delete CuUninstallWid;
//    delete CuAttributeWid;

//    delete appbtnmenu;
//    delete Fix2CommonUseWid;
//    delete Unfixed4CommonUseWid;
//    delete Fix2TaskBarWid;
//    delete Unfixed4TaskBarWid;
//    delete Add2DesktopWid;
//    delete UninstallWid;
//    delete AttributeWid;

//    delete shutdownmenu;
//    delete LockScreenWid;
//    delete LogOutWid;
//    delete RebootWid;
//    delete ShutDownWid;

//    delete othermenu;
//    delete OtherFix2TaskBarWid;
//    delete OtherUnfix2TaskBarWid;
//    delete OtherListWid;

    delete pUkuiMenuInterface;

}

//常用应用按钮右键菜单
void RightClickMenu::initCommonUseAppBtnAction()
{
    cuappbtnmenu=new QMenu(this);
    cuappbtnmenu->setLayoutDirection(Qt::LeftToRight);
    cuappbtnmenu->setFixedSize(250+2,36*4+5*1+12+2);
    CuFix2CommonUseAction=new QWidgetAction(cuappbtnmenu);
    CuFix2CommonUseWid=new QWidget();
    CuUnfixed4CommonUseAction=new QWidgetAction(cuappbtnmenu);
    CuUnfixed4CommonUseWid=new QWidget();
    CuFix2TaskBarAction=new QWidgetAction(cuappbtnmenu);
    CuFix2TaskBarWid=new QWidget();
    CuUnfixed4TaskBarAction=new QWidgetAction(cuappbtnmenu);
    CuUnfixed4TaskBarWid=new QWidget();
    CuAdd2DesktopAction=new QWidgetAction(cuappbtnmenu);
    CuAdd2DesktopWid=new QWidget();
    CuUninstallAction=new QWidgetAction(cuappbtnmenu);
    CuUninstallWid=new QWidget();
    CuAttributeAction=new QWidgetAction(cuappbtnmenu);
    CuAttributeWid=new QWidget();

    initWidgetAction(CuFix2CommonUseWid,":/data/img/mainviewwidget/fixed.svg",tr("Pin to all"),0);
    CuFix2CommonUseAction->setDefaultWidget(CuFix2CommonUseWid);
    connect(CuFix2CommonUseAction, &QWidgetAction::triggered,this,&RightClickMenu::fixToCommonUseActionTriggerSlot);

    initWidgetAction(CuUnfixed4CommonUseWid,":/data/img/mainviewwidget/unfixed.svg",tr("Unpin from all"),0);
    CuUnfixed4CommonUseAction->setDefaultWidget(CuUnfixed4CommonUseWid);
    connect(CuUnfixed4CommonUseAction, &QWidgetAction::triggered,this,&RightClickMenu::unfixedFromCommonUseActionTriggerSlot);

    initWidgetAction(CuFix2TaskBarWid,":/data/img/mainviewwidget/fixed.svg",tr("Pin to taskbar"),0);
    CuFix2TaskBarAction->setDefaultWidget(CuFix2TaskBarWid);
    connect(CuFix2TaskBarAction, &QWidgetAction::triggered,this,&RightClickMenu::fixToTaskbarActionTriggerSlot);

    initWidgetAction(CuUnfixed4TaskBarWid,":/data/img/mainviewwidget/unfixed.svg",tr("Unpin from taskbar"),0);
    CuUnfixed4TaskBarAction->setDefaultWidget(CuUnfixed4TaskBarWid);
    connect(CuUnfixed4TaskBarAction, &QWidgetAction::triggered,this,&RightClickMenu::unfixedFromTaskbarActionTriggerSlot);

    initWidgetAction(CuAdd2DesktopWid,"",tr("Add to desktop shortcuts"),0);
    CuAdd2DesktopAction->setDefaultWidget(CuAdd2DesktopWid);
    connect(CuAdd2DesktopAction, &QWidgetAction::triggered,this,&RightClickMenu::addToDesktopActionTriggerSlot);

    initWidgetAction(CuUninstallWid,":/data/img/mainviewwidget/uninstall.svg",tr("Uninstall"),0);
    CuUninstallAction->setDefaultWidget(CuUninstallWid);
    connect(CuUninstallAction, &QWidgetAction::triggered,this,&RightClickMenu::uninstallActionTriggerSlot);

}

void RightClickMenu::addCommonUseAppBtnAction()
{
    initCommonUseAppBtnAction();
    cuappbtnmenu->addAction(CuAdd2DesktopAction);
    cuappbtnmenu->addSeparator();
    cuappbtnmenu->addAction(CuUninstallAction);
    cuappbtnmenu->setAttribute(Qt::WA_TranslucentBackground);
    cuappbtnmenu->setStyleSheet(style);
}

//普通应用按钮右键菜单
void RightClickMenu::initAppBtnAction()
{
    //普通应用按钮右键菜单
    appbtnmenu=new QMenu(this);
    appbtnmenu->setLayoutDirection(Qt::LeftToRight);
    appbtnmenu->setFixedSize(250+2,36*4+5+12+2);
    Fix2CommonUseAction=new QWidgetAction(appbtnmenu);
    Fix2CommonUseWid=new QWidget();
    Unfixed4CommonUseAction=new QWidgetAction(appbtnmenu);
    Unfixed4CommonUseWid=new QWidget();
    Fix2TaskBarAction=new QWidgetAction(appbtnmenu);
    Fix2TaskBarWid=new QWidget();
    Unfixed4TaskBarAction=new QWidgetAction(appbtnmenu);
    Unfixed4TaskBarWid=new QWidget();
    Add2DesktopAction=new QWidgetAction(appbtnmenu);
    Add2DesktopWid=new QWidget();
    UninstallAction=new QWidgetAction(appbtnmenu);
    UninstallWid=new QWidget();
    AttributeAction=new QWidgetAction(appbtnmenu);
    AttributeWid=new QWidget();

    initWidgetAction(Fix2CommonUseWid,":/data/img/mainviewwidget/fixed.svg",tr("Pin to all"),0);
    Fix2CommonUseAction->setDefaultWidget(Fix2CommonUseWid);
    connect(Fix2CommonUseAction, &QWidgetAction::triggered,this,&RightClickMenu::fixToCommonUseActionTriggerSlot);

    initWidgetAction(Unfixed4CommonUseWid,":/data/img/mainviewwidget/unfixed.svg",tr("Unpin from all"),0);
    Unfixed4CommonUseAction->setDefaultWidget(Unfixed4CommonUseWid);
    connect(Unfixed4CommonUseAction, &QWidgetAction::triggered,this,&RightClickMenu::unfixedFromCommonUseActionTriggerSlot);

    initWidgetAction(Fix2TaskBarWid,":/data/img/mainviewwidget/fixed.svg",tr("Pin to taskbar"),0);
    Fix2TaskBarAction->setDefaultWidget(Fix2TaskBarWid);
    connect(Fix2TaskBarAction, &QWidgetAction::triggered,this,&RightClickMenu::fixToTaskbarActionTriggerSlot);

    initWidgetAction(Unfixed4TaskBarWid,":/data/img/mainviewwidget/unfixed.svg",tr("Unpin from taskbar"),0);
    Unfixed4TaskBarAction->setDefaultWidget(Unfixed4TaskBarWid);
    connect(Unfixed4TaskBarAction, &QWidgetAction::triggered,this,&RightClickMenu::unfixedFromTaskbarActionTriggerSlot);

    initWidgetAction(Add2DesktopWid,"",tr("Add to desktop shortcuts"),0);
    Add2DesktopAction->setDefaultWidget(Add2DesktopWid);
    connect(Add2DesktopAction, &QWidgetAction::triggered,this,&RightClickMenu::addToDesktopActionTriggerSlot);

    initWidgetAction(UninstallWid,":/data/img/mainviewwidget/uninstall.svg",tr("Uninstall"),0);
    UninstallAction->setDefaultWidget(UninstallWid);
    connect(UninstallAction, &QWidgetAction::triggered,this,&RightClickMenu::uninstallActionTriggerSlot);
}

void RightClickMenu::addAppBtnAction()
{
    initAppBtnAction();
    appbtnmenu->addAction(Add2DesktopAction);
    appbtnmenu->addSeparator();
    appbtnmenu->addAction(UninstallAction);
    appbtnmenu->setAttribute(Qt::WA_TranslucentBackground);
    appbtnmenu->setStyleSheet(style);
}

//关机按钮右键菜单
void RightClickMenu::addShutdownAction()
{
    //关机按钮右键菜单
    shutdownmenu=new QMenu(this);
    shutdownmenu->setLayoutDirection(Qt::LeftToRight);
    shutdownmenu->setFixedSize(250+2,36*5+12+2);
    LockScreenAction=new QWidgetAction(shutdownmenu);
    LockScreenWid=new QWidget();
    SwitchUserAction=new QWidgetAction(shutdownmenu);
    SwitchUserWid=new QWidget();
    LogOutAction=new QWidgetAction(shutdownmenu);
    LogOutWid=new QWidget();
    RebootAction=new QWidgetAction(shutdownmenu);
    RebootWid=new QWidget();
    ShutDownAction=new QWidgetAction(shutdownmenu);
    ShutDownWid=new QWidget();

    initWidgetAction(LockScreenWid,"system-lock-screen-symbolic",tr("Lock Screen"),1);
    LockScreenAction->setDefaultWidget(LockScreenWid);
    shutdownmenu->addAction(LockScreenAction);
    connect(LockScreenAction,&QWidgetAction::triggered,this,&RightClickMenu::lockScreenActionTriggerSlot);

    initWidgetAction(SwitchUserWid,"stock-people-symbolic",tr("Switch User"),1);
    SwitchUserAction->setDefaultWidget(SwitchUserWid);
    shutdownmenu->addAction(SwitchUserAction);
    connect(SwitchUserAction,&QWidgetAction::triggered,this,&RightClickMenu::switchUserActionTriggerSlot);

    initWidgetAction(LogOutWid,"system-logout-symbolic",tr("Log Out"),1);
    LogOutAction->setDefaultWidget(LogOutWid);
    shutdownmenu->addAction(LogOutAction);
    connect(LogOutAction,&QWidgetAction::triggered,this,&RightClickMenu::logoutActionTriggerSlot);

    initWidgetAction(RebootWid,"system-restart-symbolic",tr("Restart"),1);
    RebootAction->setDefaultWidget(RebootWid);
    shutdownmenu->addAction(RebootAction);
    connect(RebootAction,&QWidgetAction::triggered,this,&RightClickMenu::rebootActionTriggerSlot);

    initWidgetAction(ShutDownWid,"exit-symbolic",tr("Power Off"),1);
    ShutDownAction->setDefaultWidget(ShutDownWid);
    shutdownmenu->addAction(ShutDownAction);
    connect(ShutDownAction,&QWidgetAction::triggered,this,&RightClickMenu::shutdownActionTriggerSlot);

    shutdownmenu->setAttribute(Qt::WA_TranslucentBackground);
    shutdownmenu->setStyleSheet(style);
}

//其它按钮右键菜单
void RightClickMenu::addOtherAction()
{
    othermenu=new QMenu(this);
    othermenu->setLayoutDirection(Qt::LeftToRight);
    othermenu->setFixedSize(250+2,36*2+12+2);
    OtherFix2TaskBarAction=new QWidgetAction(othermenu);
    OtherFix2TaskBarWid=new QWidget();
    OtherUnfix2TaskBarAction=new QWidgetAction(othermenu);
    OtherUnfix2TaskBarWid=new QWidget();
    OtherListAction=new QWidgetAction(othermenu);
    OtherListWid=new QWidget();

    initWidgetAction(OtherFix2TaskBarWid,":/data/img/sidebarwidget/fixed.svg",tr("Pin to taskbar"),0);
    OtherFix2TaskBarAction->setDefaultWidget(OtherFix2TaskBarWid);
    connect(OtherFix2TaskBarAction,&QWidgetAction::triggered,this,&RightClickMenu::fixToTaskbarActionTriggerSlot);

    initWidgetAction(OtherUnfix2TaskBarWid,":/data/img/sidebarwidget/unfixed.svg",tr("Unpin from taskbar"),0);
    OtherUnfix2TaskBarAction->setDefaultWidget(OtherUnfix2TaskBarWid);
    connect(OtherUnfix2TaskBarAction,&QWidgetAction::triggered,this,&RightClickMenu::unfixedFromTaskbarActionTriggerSlot);

    initWidgetAction(OtherListWid,":/data/img/sidebarwidget/setting.svg",tr("Personalize this list"),0);
    OtherListAction->setDefaultWidget(OtherListWid);
    connect(OtherListAction,&QWidgetAction::triggered,this,&RightClickMenu::otherListActionTriggerSlot);

    othermenu->addAction(OtherListAction);
    othermenu->setAttribute(Qt::WA_TranslucentBackground);
    othermenu->setStyleSheet(style);
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
            labelicon->setPixmap(pixmap);
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

void RightClickMenu::fixToCommonUseActionTriggerSlot()
{
    action_number=1;
    QFileInfo fileInfo(desktopfp);
    QString desktopfn=fileInfo.fileName();
    setting->beginGroup("lockapplication");
    setting->setValue(desktopfn,setting->allKeys().size());
    setting->sync();
    setting->endGroup();
    setting->beginGroup("application");
    setting->remove(desktopfn);
    setting->sync();
    setting->endGroup();
    setting->beginGroup("datetime");
    setting->remove(desktopfn);
    setting->sync();
    setting->endGroup();
    setting->beginGroup("recentapp");
    setting->remove(desktopfn);
    setting->sync();
    setting->endGroup();
}

void RightClickMenu::unfixedFromCommonUseActionTriggerSlot()
{
    action_number=2;
    QFileInfo fileInfo(desktopfp);
    QString desktopfn=fileInfo.fileName();
    setting->beginGroup("lockapplication");
    Q_FOREACH(QString desktop,setting->allKeys())
    {
        if(setting->value(desktop).toInt() > setting->value(desktopfn).toInt())
        {
            setting->setValue(desktop,setting->value(desktop).toInt()-1);
        }
    }
    setting->remove(desktopfn);
    setting->sync();
    setting->endGroup();
    setting->beginGroup("application");
    setting->remove(desktopfn);
    setting->sync();
    setting->endGroup();
    setting->beginGroup("datetime");
    setting->remove(desktopfn);
    setting->sync();
    setting->endGroup();
}

void RightClickMenu::fixToTaskbarActionTriggerSlot()
{
    QDBusInterface iface("com.ukui.panel.desktop",
                         "/",
                         "com.ukui.panel.desktop",
                         QDBusConnection::sessionBus());
    QDBusReply<QVariant> ret=iface.call("AddToTaskbar",desktopfp);
    action_number=3;
}

void RightClickMenu::unfixedFromTaskbarActionTriggerSlot()
{
    QDBusInterface iface("com.ukui.panel.desktop",
                         "/",
                         "com.ukui.panel.desktop",
                         QDBusConnection::sessionBus());
    QDBusReply<QVariant> ret=iface.call("RemoveFromTaskbar",desktopfp);
    action_number=4;
}

void RightClickMenu::addToDesktopActionTriggerSlot()
{
    QString path=QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QFileInfo fileInfo(desktopfp);
    QString desktopfn=fileInfo.fileName();
    QFile file(desktopfp);
    QString newname=QString(path+"/"+desktopfn);
    bool ret=file.copy(QString(path+"/"+desktopfn));
    if(ret)
    {
        char command[200];
        sprintf(command,"chmod a+x %s",newname.toLocal8Bit().data());
        QProcess::startDetached(QString::fromLocal8Bit(command));
    }
    action_number=5;
}

void RightClickMenu::uninstallActionTriggerSlot()
{
    QString cmd=QString("dpkg -S "+desktopfp);
    cmdProc->setReadChannel(QProcess::StandardOutput);
    cmdProc->start("sh",QStringList()<<"-c"<<cmd);
    cmdProc->waitForFinished();
    cmdProc->waitForReadyRead();
    cmdProc->close();
    action_number=6;
}

void RightClickMenu::onReadOutput()
{
    QString packagestr=QString::fromLocal8Bit(cmdProc->readAllStandardOutput().data());
    QString packageName=packagestr.split(":").at(0);
    qDebug()<<packagestr<<packageName;
    char command[100];
    sprintf(command,"ubuntu-kylin-software-center -remove %s",packageName.toLocal8Bit().data());
    QProcess::startDetached(command);
}

void RightClickMenu::attributeActionTriggerSlot()
{
    char command[100];
    sprintf(command,"ukui-menu-attr %s",desktopfp.toLocal8Bit().data());
    QProcess::startDetached(command);
    action_number=7;
}

void RightClickMenu::lockScreenActionTriggerSlot()
{
    action_number=10;
}

void RightClickMenu::switchUserActionTriggerSlot()
{
    action_number=11;
}

void RightClickMenu::logoutActionTriggerSlot()
{
    action_number=12;
}

void RightClickMenu::rebootActionTriggerSlot()
{
    action_number=13;
}

void RightClickMenu::shutdownActionTriggerSlot()
{
    action_number=14;
}

void RightClickMenu::otherListActionTriggerSlot()
{
    action_number=15;
}

int RightClickMenu::showCommonUseAppBtnMenu(QString desktopfp)
{
    action_number=0;
    this->desktopfp.clear();
    this->desktopfp=desktopfp;
    cuappbtnmenu->removeAction(CuFix2CommonUseAction);
    cuappbtnmenu->removeAction(CuUnfixed4CommonUseAction);
    cuappbtnmenu->removeAction(CuFix2TaskBarAction);
    cuappbtnmenu->removeAction(CuUnfixed4TaskBarAction);
    setting->beginGroup("lockapplication");
    QFileInfo fileInfo(desktopfp);
    QString desktopfn=fileInfo.fileName();
    if(!setting->contains(desktopfn))
        cuappbtnmenu->insertAction(CuAdd2DesktopAction,CuFix2CommonUseAction);
    else
        cuappbtnmenu->insertAction(CuAdd2DesktopAction,CuUnfixed4CommonUseAction);
    setting->endGroup();

    QDBusInterface iface("com.ukui.panel.desktop",
                         "/",
                         "com.ukui.panel.desktop",
                         QDBusConnection::sessionBus());

    QDBusReply<bool> ret=iface.call("CheckIfExist",desktopfp);
    if(!ret)
        cuappbtnmenu->insertAction(CuAdd2DesktopAction,CuFix2TaskBarAction);
    else
        cuappbtnmenu->insertAction(CuAdd2DesktopAction,CuUnfixed4TaskBarAction);

    cuappbtnmenu->exec(QCursor::pos());
    return action_number;
}

int RightClickMenu::showAppBtnMenu(QString desktopfp)
{
    action_number=0;
    this->desktopfp.clear();
    this->desktopfp=desktopfp;
    appbtnmenu->removeAction(Fix2CommonUseAction);
    appbtnmenu->removeAction(Unfixed4CommonUseAction);
    appbtnmenu->removeAction(Fix2TaskBarAction);
    appbtnmenu->removeAction(Unfixed4TaskBarAction);
    setting->beginGroup("lockapplication");
    QFileInfo fileInfo(desktopfp);
    QString desktopfn=fileInfo.fileName();
    if(!setting->contains(desktopfn))
        appbtnmenu->insertAction(Add2DesktopAction,Fix2CommonUseAction);
    else
        appbtnmenu->insertAction(Add2DesktopAction,Unfixed4CommonUseAction);
    setting->endGroup();

    QDBusInterface iface("com.ukui.panel.desktop",
                         "/",
                         "com.ukui.panel.desktop",
                         QDBusConnection::sessionBus());

    QDBusReply<bool> ret=iface.call("CheckIfExist",desktopfp);
    if(!ret)
        appbtnmenu->insertAction(Add2DesktopAction,Fix2TaskBarAction);
    else
        appbtnmenu->insertAction(Add2DesktopAction,Unfixed4TaskBarAction);
    appbtnmenu->exec(QCursor::pos());
    return action_number;
}

int RightClickMenu::showShutdownMenu()
{
    action_number=0;
    shutdownmenu->exec(QCursor::pos());
    return action_number;

}

int RightClickMenu::showOtherMenu(QString desktopfp)
{
    action_number=0;
    this->desktopfp.clear();
    this->desktopfp=desktopfp;
    othermenu->removeAction(OtherFix2TaskBarAction);
    othermenu->removeAction(OtherUnfix2TaskBarAction);
    QDBusInterface iface("com.ukui.panel.desktop",
                         "/",
                         "com.ukui.panel.desktop",
                         QDBusConnection::sessionBus());

    QDBusReply<bool> ret=iface.call("CheckIfExist",this->desktopfp);
    if(!ret)
        othermenu->insertAction(OtherListAction,OtherFix2TaskBarAction);
    else
        othermenu->insertAction(OtherListAction,OtherUnfix2TaskBarAction);
    othermenu->exec(QCursor::pos());
    return action_number;
}
