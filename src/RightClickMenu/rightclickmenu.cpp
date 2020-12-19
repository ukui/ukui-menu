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
#include "src/UtilityFunction/utility.h"
#include <QDebug>

RightClickMenu::RightClickMenu(QWidget *parent):
    QWidget(parent)
{
    QString path=QDir::homePath()+"/.config/ukui/ukui-menu.ini";
    m_setting=new QSettings(path,QSettings::IniFormat);

    m_cmdProc=new QProcess;
    connect(m_cmdProc , &QProcess::readyReadStandardOutput, this , &RightClickMenu::onReadOutput);
}

RightClickMenu::~RightClickMenu()
{
    delete m_cmdProc;
    delete m_setting;

}

QPixmap RightClickMenu::getIconPixmap(QString iconstr, int type)
{
    const auto ratio=devicePixelRatioF();
    QPixmap pixmap;

    if(type==0)
    {
        pixmap=loadSvg(iconstr,16*ratio);
        pixmap.setDevicePixelRatio(qApp->devicePixelRatio());
    }
    else
    {
        QIcon icon=QIcon::fromTheme(iconstr);
        QPixmap pixmap_1=icon.pixmap(QSize(16*ratio,16*ratio));
        pixmap=drawSymbolicColoredPixmap(pixmap_1);
    }
    return pixmap;
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
//    qDebug()<<packagestr<<packageName;
    char command[100];
    sprintf(command,"kylin-installer -remove %s",packageName.toLocal8Bit().data());
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

void RightClickMenu::hibernateActionTriggerSlot()
{
    m_actionNumber=16;
}

void RightClickMenu::sleepActionTriggerSlot()
{
    m_actionNumber=17;
}

int RightClickMenu::showAppBtnMenu(QString desktopfp)
{
    m_actionNumber=0;
    this->m_desktopfp.clear();
    this->m_desktopfp=desktopfp;
    QMenu menu;
    //添加菜单项，指定图标、名称、响应函数
    m_setting->beginGroup("lockapplication");
    QFileInfo fileInfo(desktopfp);
    QString desktopfn=fileInfo.fileName();
    if(!m_setting->contains(desktopfn))
        menu.addAction(QIcon(getIconPixmap(":/data/img/mainviewwidget/fixed.svg",0)),tr("Pin to all"),
                       this,SLOT(fixToAllActionTriggerSlot()));
    else
        menu.addAction(QIcon(getIconPixmap(":/data/img/mainviewwidget/unfixed.svg",0)),tr("Unpin from all"),
                       this,SLOT(unfixedFromAllActionTriggerSlot()));
    m_setting->endGroup();
    QDBusInterface iface("com.ukui.panel.desktop",
                         "/",
                         "com.ukui.panel.desktop",
                         QDBusConnection::sessionBus());

    QDBusReply<bool> ret=iface.call("CheckIfExist",desktopfp);
    if(!ret)
        menu.addAction(QIcon(getIconPixmap(":/data/img/mainviewwidget/fixed.svg",0)),tr("Pin to taskbar"),
                       this,SLOT(fixToTaskbarActionTriggerSlot()));
    else
        menu.addAction(QIcon(getIconPixmap(":/data/img/mainviewwidget/unfixed.svg",0)),tr("Unpin from taskbar"),
                       this,SLOT(unfixedFromTaskbarActionTriggerSlot()));

    menu.addAction(tr("Add to desktop shortcuts"),
                   this,SLOT(addToDesktopActionTriggerSlot()));
    menu.addSeparator();
    if(!UkuiMenuInterface::androidDesktopfnList.contains(desktopfn))
        menu.addAction(QIcon(getIconPixmap(":/data/img/mainviewwidget/uninstall.svg",0)),tr("Uninstall"),
                       this,SLOT(uninstallActionTriggerSlot()));

    menu.setAttribute(Qt::WA_TranslucentBackground);
    menu.exec(QCursor::pos());
    return m_actionNumber;
}

int RightClickMenu::showShutdownMenu()
{
    m_actionNumber=0;
    QMenu menu;
    menu.addAction(QIcon(getIconPixmap("kylin-sleep-symbolic",1)),tr("Sleep"),
                   this,SLOT(hibernateActionTriggerSlot()));//休眠睡眠相同
    menu.addAction(QIcon(getIconPixmap("system-logout-symbolic",1)),tr("Log Out"),
                   this,SLOT(logoutActionTriggerSlot()));
    menu.addAction(QIcon(getIconPixmap("system-restart-symbolic",1)),tr("Restart"),
                   this,SLOT(rebootActionTriggerSlot()));
    menu.addAction(QIcon(getIconPixmap("exit-symbolic",1)),tr("Power Off"),
                   this,SLOT(shutdownActionTriggerSlot()));
    menu.setAttribute(Qt::WA_TranslucentBackground);
    menu.exec(QCursor::pos());
    return m_actionNumber;
}

int RightClickMenu::showOtherMenu(QString desktopfp)
{
    m_actionNumber=0;
    this->m_desktopfp.clear();
    this->m_desktopfp=desktopfp;

    QMenu menu;
//    QDBusInterface iface("com.ukui.panel.desktop",
//                         "/",
//                         "com.ukui.panel.desktop",
//                         QDBusConnection::sessionBus());

//    QDBusReply<bool> ret=iface.call("CheckIfExist",this->m_desktopfp);
//    if(!ret)
//        menu.addAction(QIcon(getIconPixmap(":/data/img/sidebarwidget/fixed.svg",0)),tr("Pin to taskbar"),
//                       this,SLOT(fixToTaskbarActionTriggerSlot()));
//    else
//        menu.addAction(QIcon(getIconPixmap(":/data/img/sidebarwidget/unfixed.svg",0)),tr("Unpin from taskbar"),
//                       this,SLOT(unfixedFromTaskbarActionTriggerSlot()));
    menu.addAction(QIcon(getIconPixmap(":/data/img/sidebarwidget/setting.svg",0)),tr("Personalize this list"),
                   this,SLOT(otherListActionTriggerSlot()));
    menu.setAttribute(Qt::WA_TranslucentBackground);
    menu.exec(QCursor::pos());
    return m_actionNumber;
}
