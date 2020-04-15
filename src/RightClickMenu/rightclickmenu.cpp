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

RightClickMenu::RightClickMenu(QWidget *parent):
    QWidget (parent)

{
    QString path=QDir::homePath()+"/.config/ukui/ukui-menu.ini";
    setting=new QSettings(path,QSettings::IniFormat);

    //其它按钮右键菜单项
    othermenu=new QMenu(this);
    othermenu->setLayoutDirection(Qt::LeftToRight);
    othermenu->setFixedSize(250+2,36*2+12+2);
    OtherFix2TaskBarAction=new QWidgetAction(othermenu);
    OtherFix2TaskBarWid=new QWidget();
    OtherUnfix2TaskBarAction=new QWidgetAction(othermenu);
    OtherUnfix2TaskBarWid=new QWidget();
    OtherListAction=new QWidgetAction(othermenu);
    OtherListWid=new QWidget();

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

    //常用应用按钮右键菜单
    cuappbtnmenu=new QMenu(this);
    cuappbtnmenu->setLayoutDirection(Qt::LeftToRight);
    cuappbtnmenu->setFixedSize(250+2,36*7+4*3+9*3+12+2);
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
    CuDeleteAction=new QWidgetAction(cuappbtnmenu);
    CuDeleteWid=new QWidget();
    CuDeleteAllAction=new QWidgetAction(cuappbtnmenu);
    CuDeleteAllWid=new QWidget();
    CuUninstallAction=new QWidgetAction(cuappbtnmenu);
    CuUninstallWid=new QWidget();
    CuAttributeAction=new QWidgetAction(cuappbtnmenu);
    CuAttributeWid=new QWidget();

    //普通应用按钮右键菜单
    appbtnmenu=new QMenu(this);
    appbtnmenu->setLayoutDirection(Qt::LeftToRight);
    appbtnmenu->setFixedSize(250+2,36*5+4*2+9*2+12+2);
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

    //间隔线
    char lineStyle[100];
    sprintf(lineStyle,"QLabel{background-color:%s;}",
            RightClickMenuSeparator);
    separatorLabel=new QLabel[5];
    separatorWid=new QWidget[5];
    for(int i=0;i<5;i++)
    {
        separatorLabel[i].setStyleSheet(lineStyle);
        separatorWid[i].setStyleSheet("background:transparent;");
        QHBoxLayout* layout=new QHBoxLayout(&separatorWid[i]);
        separatorWid[i].setLayout(layout);
        layout->setContentsMargins(8,0,8,0);
        separatorWid[i].setFixedSize(246,9);
        separatorWid[i].setFocusPolicy(Qt::NoFocus);
        separatorLabel[i].setFixedSize(separatorWid->width()-16,1);
        layout->addWidget(&separatorLabel[i]);
        layout->setAlignment(&separatorLabel[i],Qt::AlignCenter);
    }

    separatorAction_1=new QWidgetAction(cuappbtnmenu);
    separatorAction_2=new QWidgetAction(cuappbtnmenu);
    separatorAction_3=new QWidgetAction(cuappbtnmenu);
    separatorAction_4=new QWidgetAction(appbtnmenu);
    separatorAction_5=new QWidgetAction(appbtnmenu);
    separatorAction_1->setDefaultWidget(&separatorWid[0]);
    separatorAction_2->setDefaultWidget(&separatorWid[1]);
    separatorAction_3->setDefaultWidget(&separatorWid[2]);
    separatorAction_4->setDefaultWidget(&separatorWid[3]);
    separatorAction_5->setDefaultWidget(&separatorWid[4]);
    separatorAction_1->setDisabled(true);
    separatorAction_2->setDisabled(true);
    separatorAction_3->setDisabled(true);
    separatorAction_4->setDisabled(true);
    separatorAction_5->setDisabled(true);

    pUkuiMenuInterface=new UkuiMenuInterface;
    cmdProc=new QProcess(this);
    connect(cmdProc , SIGNAL(readyReadStandardOutput()) , this , SLOT(onReadOutput()));

    sprintf(style, "QMenu{padding-left:2px;padding-top:6px;padding-right:2px;padding-bottom:6px;border:1px solid %s;border-radius:6px;background-color:%s;}\
            QMenu::separator{height:4px;background:transparent;}",
            RightClickMenuBorder ,RightClickMenuBackground);

    addShutdownAction();
//    addOtherAction();
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
void RightClickMenu::addCommonUseAppBtnAction()
{
    setting->beginGroup("lockapplication");
    QFileInfo fileInfo(desktopfp);
    QString desktopfn=fileInfo.fileName();
    if(!setting->contains(desktopfn))
    {
        initWidgetAction(CuFix2CommonUseWid,":/data/img/mainviewwidget/fixed.svg",tr("Pin to common"));
        CuFix2CommonUseAction->setDefaultWidget(CuFix2CommonUseWid);
        cuappbtnmenu->addAction(CuFix2CommonUseAction);
        connect(CuFix2CommonUseAction, SIGNAL(triggered()),this,SLOT(fixToCommonUseActionTriggerSlot()));
    }
    else
    {
        initWidgetAction(CuUnfixed4CommonUseWid,":/data/img/mainviewwidget/unfixed.svg",tr("Unpin from common"));
        CuUnfixed4CommonUseAction->setDefaultWidget(CuUnfixed4CommonUseWid);
        cuappbtnmenu->addAction(CuUnfixed4CommonUseAction);
        connect(CuUnfixed4CommonUseAction, SIGNAL(triggered()),this,SLOT(unfixedFromCommonUseActionTriggerSlot()));
    }
    setting->endGroup();

    cuappbtnmenu->addSeparator();

//    QString desktopfp=pUkuiMenuInterface->getDesktopPathByAppName(appname);
    QDBusInterface iface("com.ukui.panel.desktop",
                         "/",
                         "com.ukui.panel.desktop",
                         QDBusConnection::sessionBus());

    QDBusReply<bool> ret=iface.call("CheckIfExist",desktopfp);
    if(!ret)
    {
        initWidgetAction(CuFix2TaskBarWid,":/data/img/mainviewwidget/fixed.svg",tr("Pin to taskbar"));
        CuFix2TaskBarAction->setDefaultWidget(CuFix2TaskBarWid);
        cuappbtnmenu->addAction(CuFix2TaskBarAction);
        connect(CuFix2TaskBarAction, SIGNAL(triggered()),this,SLOT(fixToTaskbarActionTriggerSlot()));
    }
    else {
        initWidgetAction(CuUnfixed4TaskBarWid,":/data/img/mainviewwidget/unfixed.svg",tr("Unpin from taskbar"));
        CuUnfixed4TaskBarAction->setDefaultWidget(CuUnfixed4TaskBarWid);
        cuappbtnmenu->addAction(CuUnfixed4TaskBarAction);
        connect(CuUnfixed4TaskBarAction, SIGNAL(triggered()),this,SLOT(unfixedFromTaskbarActionTriggerSlot()));
    }

    cuappbtnmenu->addSeparator();

    initWidgetAction(CuAdd2DesktopWid,"",tr("Add to desktop shortcuts"));
    CuAdd2DesktopAction->setDefaultWidget(CuAdd2DesktopWid);
    cuappbtnmenu->addAction(CuAdd2DesktopAction);
    connect(CuAdd2DesktopAction, SIGNAL(triggered()),this,SLOT(addToDesktopActionTriggerSlot()));

//    cuappbtnmenu->addSeparator();
    cuappbtnmenu->addAction(separatorAction_1);
//    cuappbtnmenu->addSeparator();


    initWidgetAction(CuDeleteWid,"",tr("Remove from list"));
    CuDeleteAction->setDefaultWidget(CuDeleteWid);
    cuappbtnmenu->addAction(CuDeleteAction);
    connect(CuDeleteAction,SIGNAL(triggered()),this,SLOT(commonUseDeleteActionTriggerSlot()));
    setting->beginGroup("lockapplication");
    if(setting->contains(desktopfn))
    {
        QLayoutItem* item=CuDeleteWid->layout()->itemAt(0);
        QWidget* wid=item->widget();
        QLabel* label=qobject_cast<QLabel*>(wid);
        label->setStyleSheet("QLabel{background:transparent;border:0px;color:#4Dffffff;font-size:14px;}");
        CuDeleteAction->setDisabled(true);
    }
    setting->endGroup();

    cuappbtnmenu->addSeparator();

    initWidgetAction(CuDeleteAllWid,"",tr("Remove all"));
    CuDeleteAllAction->setDefaultWidget(CuDeleteAllWid);
    cuappbtnmenu->addAction(CuDeleteAllAction);
    connect(CuDeleteAllAction,SIGNAL(triggered()),this,SLOT(commonUseDeleteAllActionTriggerSlot()));
    setting->beginGroup("application");
    if(setting->allKeys().size()==0)
    {
        QLayoutItem* item=CuDeleteAllWid->layout()->itemAt(0);
        QWidget* wid=item->widget();
        QLabel* label=qobject_cast<QLabel*>(wid);
        label->setStyleSheet("QLabel{background:transparent;border:0px;color:#4Dffffff;font-size:14px;}");
        CuDeleteAllAction->setDisabled(true);
    }
    setting->endGroup();

    cuappbtnmenu->addAction(separatorAction_2);

    initWidgetAction(CuUninstallWid,":/data/img/mainviewwidget/uninstall.svg",tr("Uninstall"));
    CuUninstallAction->setDefaultWidget(CuUninstallWid);
    cuappbtnmenu->addAction(CuUninstallAction);
    connect(CuUninstallAction, SIGNAL(triggered()),this,SLOT(uninstallActionTriggerSlot()));

    cuappbtnmenu->addAction(separatorAction_3);

    initWidgetAction(CuAttributeWid,":/data/img/mainviewwidget/attributeaction.svg",tr("Attribute"));
    CuAttributeAction->setDefaultWidget(CuAttributeWid);
    cuappbtnmenu->addAction(CuAttributeAction);
    connect(CuAttributeAction, SIGNAL(triggered()),this,SLOT(attributeActionTriggerSlot()));

    cuappbtnmenu->setAttribute(Qt::WA_TranslucentBackground);
//    cuappbtnmenu->setWindowOpacity(RightClickMenuOpacity);
    cuappbtnmenu->setStyleSheet(style);
}

//普通应用按钮右键菜单
void RightClickMenu::addAppBtnAction()
{
    setting->beginGroup("lockapplication");
    QFileInfo fileInfo(desktopfp);
    QString desktopfn=fileInfo.fileName();
    if(!setting->contains(desktopfn))
    {
        initWidgetAction(Fix2CommonUseWid,":/data/img/mainviewwidget/fixed.svg",tr("Pin to common"));
        Fix2CommonUseAction->setDefaultWidget(Fix2CommonUseWid);
        appbtnmenu->addAction(Fix2CommonUseAction);
        connect(Fix2CommonUseAction, SIGNAL(triggered()),this,SLOT(fixToCommonUseActionTriggerSlot()));
    }
    else
    {
        initWidgetAction(Unfixed4CommonUseWid,":/data/img/mainviewwidget/unfixed.svg",tr("Unpin from common"));
        Unfixed4CommonUseAction->setDefaultWidget(Unfixed4CommonUseWid);
        appbtnmenu->addAction(Unfixed4CommonUseAction);
        connect(Unfixed4CommonUseAction, SIGNAL(triggered()),this,SLOT(unfixedFromCommonUseActionTriggerSlot()));
    }
    setting->endGroup();

    appbtnmenu->addSeparator();

    QDBusInterface iface("com.ukui.panel.desktop",
                         "/",
                         "com.ukui.panel.desktop",
                         QDBusConnection::sessionBus());

    QDBusReply<bool> ret=iface.call("CheckIfExist",desktopfp);
    if(!ret)
    {
        initWidgetAction(Fix2TaskBarWid,":/data/img/mainviewwidget/fixed.svg",tr("Pin to taskbar"));
        Fix2TaskBarAction->setDefaultWidget(Fix2TaskBarWid);
        appbtnmenu->addAction(Fix2TaskBarAction);
        connect(Fix2TaskBarAction, SIGNAL(triggered()),this,SLOT(fixToTaskbarActionTriggerSlot()));

    }
    else{
        initWidgetAction(Unfixed4TaskBarWid,":/data/img/mainviewwidget/unfixed.svg",tr("Unpin from taskbar"));
        Unfixed4TaskBarAction->setDefaultWidget(Unfixed4TaskBarWid);
        appbtnmenu->addAction(Unfixed4TaskBarAction);
        connect(Unfixed4TaskBarAction, SIGNAL(triggered()),this,SLOT(unfixedFromTaskbarActionTriggerSlot()));
    }

    appbtnmenu->addSeparator();

    initWidgetAction(Add2DesktopWid,"",tr("Add to desktop shortcuts"));
    Add2DesktopAction->setDefaultWidget(Add2DesktopWid);
    appbtnmenu->addAction(Add2DesktopAction);
    connect(Add2DesktopAction, SIGNAL(triggered()),this,SLOT(addToDesktopActionTriggerSlot()));


    appbtnmenu->addAction(separatorAction_4);

    initWidgetAction(UninstallWid,":/data/img/mainviewwidget/uninstall.svg",tr("Uninstall"));
    UninstallAction->setDefaultWidget(UninstallWid);
    appbtnmenu->addAction(UninstallAction);
    connect(UninstallAction, SIGNAL(triggered()),this,SLOT(uninstallActionTriggerSlot()));

    appbtnmenu->addAction(separatorAction_5);

    initWidgetAction(AttributeWid,":/data/img/mainviewwidget/attributeaction.svg",tr("Attribute"));
    AttributeAction->setDefaultWidget(AttributeWid);
    appbtnmenu->addAction(AttributeAction);
    connect(AttributeAction, SIGNAL(triggered()),this,SLOT(attributeActionTriggerSlot()));

    appbtnmenu->setAttribute(Qt::WA_TranslucentBackground);
//    appbtnmenu->setWindowOpacity(RightClickMenuOpacity);
    appbtnmenu->setStyleSheet(style);
}

//关机按钮右键菜单
void RightClickMenu::addShutdownAction()
{
    initWidgetAction(LockScreenWid,":/data/img/sidebarwidget/lock.svg",tr("Lock Screen"));
    LockScreenAction->setDefaultWidget(LockScreenWid);
    shutdownmenu->addAction(LockScreenAction);
    connect(LockScreenAction,SIGNAL(triggered()),this,SLOT(lockScreenActionTriggerSlot()));

    initWidgetAction(SwitchUserWid,"",tr("Switch User"));
    SwitchUserAction->setDefaultWidget(SwitchUserWid);
    shutdownmenu->addAction(SwitchUserAction);
    connect(SwitchUserAction,SIGNAL(triggered()),this,SLOT(switchUserActionTriggerSlot()));

    initWidgetAction(LogOutWid,"",tr("Log Out"));
    LogOutAction->setDefaultWidget(LogOutWid);
    shutdownmenu->addAction(LogOutAction);
    connect(LogOutAction,SIGNAL(triggered()),this,SLOT(logoutActionTriggerSlot()));

    initWidgetAction(RebootWid,"",tr("Restart"));
    RebootAction->setDefaultWidget(RebootWid);
    shutdownmenu->addAction(RebootAction);
    connect(RebootAction,SIGNAL(triggered()),this,SLOT(rebootActionTriggerSlot()));

    initWidgetAction(ShutDownWid,":/data/img/sidebarwidget/shutdown.svg",tr("Power Off"));
    ShutDownAction->setDefaultWidget(ShutDownWid);
    shutdownmenu->addAction(ShutDownAction);
    connect(ShutDownAction,SIGNAL(triggered()),this,SLOT(shutdownActionTriggerSlot()));

    shutdownmenu->setAttribute(Qt::WA_TranslucentBackground);
//    shutdownmenu->setWindowOpacity(RightClickMenuOpacity);
    shutdownmenu->setStyleSheet(style);
}

//其它按钮右键菜单
void RightClickMenu::addOtherAction()
{
    QDBusInterface iface("com.ukui.panel.desktop",
                         "/",
                         "com.ukui.panel.desktop",
                         QDBusConnection::sessionBus());

    QDBusReply<bool> ret=iface.call("CheckIfExist",this->desktopfp);
    if(!ret)
    {
        initWidgetAction(OtherFix2TaskBarWid,":/data/img/sidebarwidget/fixed.svg",tr("Pin to taskbar"));
        OtherFix2TaskBarAction->setDefaultWidget(OtherFix2TaskBarWid);
        othermenu->addAction(OtherFix2TaskBarAction);
        connect(OtherFix2TaskBarAction,SIGNAL(triggered()),this,SLOT(fixToTaskbarActionTriggerSlot()));
    }
    else {
        initWidgetAction(OtherUnfix2TaskBarWid,":/data/img/sidebarwidget/unfixed.svg",tr("Unpin from taskbar"));
        OtherUnfix2TaskBarAction->setDefaultWidget(OtherUnfix2TaskBarWid);
        othermenu->addAction(OtherUnfix2TaskBarAction);
        connect(OtherUnfix2TaskBarAction,SIGNAL(triggered()),this,SLOT(unfixedFromTaskbarActionTriggerSlot()));
    }

    initWidgetAction(OtherListWid,":/data/img/sidebarwidget/setting.svg",tr("Personalize this list"));
    OtherListAction->setDefaultWidget(OtherListWid);
    othermenu->addAction(OtherListAction);
    connect(OtherListAction,SIGNAL(triggered()),this,SLOT(otherListActionTriggerSlot()));

    othermenu->setAttribute(Qt::WA_TranslucentBackground);
//    othermenu->setWindowOpacity(RightClickMenuOpacity);
    othermenu->setStyleSheet(style);
}

void RightClickMenu::initWidgetAction(QWidget *wid, QString iconstr, QString textstr)
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
        QSvgRenderer* svg=new QSvgRenderer(wid);
        svg->load(iconstr);
        QPixmap* pixmap=new QPixmap(16,16);
        pixmap->fill(Qt::transparent);
        QPainter p(pixmap);
        svg->render(&p);
        labelicon->setPixmap(*pixmap);
        labelicon->setFixedSize(pixmap->size());
        labelicon->setAlignment(Qt::AlignCenter);
        labelicon->setStyleSheet(labelstyle);
        layout->addWidget(labelicon);

    }

    QLabel* labeltext=new QLabel(wid);
    labeltext->setStyleSheet(labelstyle);
//    QByteArray textbyte=textstr.toLocal8Bit();
//    char* text=textbyte.data();
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
    int maxValue=1;
    Q_FOREACH(QString desktopfn,setting->allKeys())
    {
        if(setting->value(desktopfn).toInt()>maxValue)
            maxValue=setting->value(desktopfn).toInt();
    }
    setting->setValue(desktopfn,maxValue+1);
    setting->sync();
    setting->endGroup();
    setting->beginGroup("datetime");
    QDateTime dt=QDateTime::currentDateTime();
    int dateTime=dt.toTime_t();
    setting->setValue(desktopfn,dateTime);
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
    //    qDebug()<<desktopfp<<ret.value().toBool();
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
//        system(command);
        QProcess::startDetached(QString::fromLocal8Bit(command));
    }
    action_number=5;
}

void RightClickMenu::uninstallActionTriggerSlot()
{
//    QString exec=pUkuiMenuInterface->getAppExec(pUkuiMenuInterface->getDesktopPathByAppName(appname));
//    QFileInfo fileInfo(exec.split(" ").at(0));
//    bool ret=fileInfo.isAbsolute();

    QString cmd=QString("dpkg -S "+desktopfp);
    cmdProc->setReadChannel(QProcess::StandardOutput);
    cmdProc->start("sh",QStringList()<<"-c"<<cmd);
    cmdProc->waitForFinished();
    cmdProc->waitForReadyRead();
    cmdProc->close();

//    if(ret)
//    {
//        QString cmd=QString("dpkg -S "+exec.split(" ").at(0));
//        cmdProc->setReadChannel(QProcess::StandardOutput);
//        cmdProc->start("sh",QStringList()<<"-c"<<cmd);
//        cmdProc->waitForFinished();
//        cmdProc->waitForReadyRead();
//        cmdProc->close();
//    }
//    else {
//        QString cmd=QString("dpkg -S "+exec.split(" ").at(0)+"| grep /usr/bin");
//        cmdProc->setReadChannel(QProcess::StandardOutput);
//        cmdProc->start("sh",QStringList()<<"-c"<<cmd);
//        cmdProc->waitForFinished();
//        cmdProc->waitForReadyRead();
//        cmdProc->close();
//    }
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

void RightClickMenu::commonUseDeleteActionTriggerSlot()
{
    action_number=8;
    QFileInfo fileInfo(desktopfp);
    QString desktopfn=fileInfo.fileName();
    setting->beginGroup("application");
    setting->remove(desktopfn);
    setting->sync();
    setting->endGroup();
    setting->beginGroup("datetime");
    setting->remove(desktopfn);
    setting->sync();
    setting->endGroup();
}

void RightClickMenu::commonUseDeleteAllActionTriggerSlot()
{
    action_number=9;
    QStringList deleteKeys;
    deleteKeys.clear();
    setting->beginGroup("application");
    QStringList keys=setting->allKeys();
    deleteKeys=keys;
    setting->remove("");
    setting->sync();
    setting->endGroup();

    setting->beginGroup("datetime");
    for(int i=0;i<deleteKeys.count();i++)
        setting->remove(deleteKeys.at(i));
    setting->sync();
    setting->endGroup();

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
    qDebug()<<"---2---";
}

int RightClickMenu::showCommonUseAppBtnMenu(QString desktopfp)
{
    this->desktopfp.clear();
    this->desktopfp=desktopfp;
    addCommonUseAppBtnAction();
    cuappbtnmenu->exec(QCursor::pos());
    return action_number;
}

int RightClickMenu::showAppBtnMenu(QString desktopfp)
{
    this->desktopfp.clear();
    this->desktopfp=desktopfp;
    addAppBtnAction();
    appbtnmenu->exec(QCursor::pos());
    return action_number;
}

int RightClickMenu::showShutdownMenu()
{
    shutdownmenu->exec(QCursor::pos());
    return action_number;

}

void RightClickMenu::showOtherMenu(QString desktopfp)
{
    this->desktopfp.clear();
    this->desktopfp=desktopfp;
    addOtherAction();
    othermenu->exec(QCursor::pos());
}
