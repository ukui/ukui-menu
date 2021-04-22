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
    m_cmdProc=new QProcess;
    connect(m_cmdProc , &QProcess::readyReadStandardOutput, this , &RightClickMenu::onReadOutput);

    m_whiteList.append("kylin-screenshot.desktop");
    m_whiteList.append("ukui-notebook.desktop");
    m_whiteList.append("ukui-clock.desktop");
    m_whiteList.append("kylin-calculator.desktop");
    m_whiteList.append("kylin-recorder.desktop");
    m_whiteList.append("kylin-software-center.desktop");
    m_whiteList.append("kylin-camera.desktop");
    m_whiteList.append("biometric-manager.desktop");
    m_whiteList.append("yhkylin-backup-tools.desktop");
    m_whiteList.append("box-manager.desktop");
    m_whiteList.append("ukui-system-monitor.desktop");
    m_whiteList.append("ksc-defender.desktop");
    m_whiteList.append("logview.desktop");
    m_whiteList.append("kylin-service-support.desktop");
    m_whiteList.append("kylin-user-guide.desktop");
    m_whiteList.append("ukui-control-center.desktop");

    m_ukuiMenuInterface=new UkuiMenuInterface;
}

RightClickMenu::~RightClickMenu()
{
    delete m_cmdProc;
//    delete m_setting;
    delete m_ukuiMenuInterface;

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
    updateDataBaseTableType(desktopfn,1);
}

void RightClickMenu::unfixedFromAllActionTriggerSlot()
{
    m_actionNumber=2;
    QFileInfo fileInfo(m_desktopfp);
    QString desktopfn=fileInfo.fileName();
    updateDataBaseTableType(desktopfn,0);
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
    qDebug()<<"查询包名："<<cmd;
}

void RightClickMenu::onReadOutput()
{
    QString packagestr=QString::fromLocal8Bit(m_cmdProc->readAllStandardOutput().data());
    QString packageName=packagestr.split(":").at(0);
//    qDebug()<<packagestr<<packageName;
    char command[100];
    sprintf(command,"kylin-installer -remove %s",packageName.toLocal8Bit().data());
    bool ret=QProcess::startDetached(command);
    qDebug()<<"卸载："<<command<<ret;
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
    QFileInfo fileInfo(desktopfp);
    QString desktopfn=fileInfo.fileName();
    if(!checkIfLocked(desktopfn))
        menu.addAction(QIcon(getIconPixmap(":/data/img/mainviewwidget/fixed.svg",0)),tr("Pin to all"),
                       this,SLOT(fixToAllActionTriggerSlot()));
    else
        menu.addAction(QIcon(getIconPixmap(":/data/img/mainviewwidget/unfixed.svg",0)),tr("Unpin from all"),
                       this,SLOT(unfixedFromAllActionTriggerSlot()));
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
    //检查桌面快捷方式是否存在
    QString desktopPath=QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString path=QString(desktopPath+"/"+QFileInfo(m_desktopfp).fileName());
    if(QFile(path).exists())
        menu.actions().at(2)->setEnabled(false);//存在时禁用
    menu.addSeparator();

    if(!m_ukuiMenuInterface->getAppCategories(desktopfp).contains("Android") &&
            !m_whiteList.contains(desktopfn))
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
    menu.addAction(QIcon(getIconPixmap("system-sleep",1)),tr("Sleep"),
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
