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

#include "tablet_directorychanged_thread.h"
#include <syslog.h>
#include <QIcon>
#include <QtDBus>
#include "tabletlistview.h"
#include <QDebug>
#include <QEventLoop>

TabletDirectoryChangedThread::TabletDirectoryChangedThread()
{
    m_ukuiMenuInterface = new UkuiMenuInterface;
    QString path = QDir::homePath() + "/.config/ukui/ukui-menu.ini";
    setting = new QSettings(path, QSettings::IniFormat);
}

TabletDirectoryChangedThread::~TabletDirectoryChangedThread()
{
    if (m_ukuiMenuInterface) {
        delete m_ukuiMenuInterface;
    }

    if (setting) {
        delete setting;
    }

    m_ukuiMenuInterface = nullptr;
    setting = nullptr;
}


void TabletDirectoryChangedThread::run()
{
    QStringList desktopfpList = m_ukuiMenuInterface->getDesktopFilePath();
    myDebug() << "应用列表desktopfpList" << desktopfpList.size() << desktopfpList;
    myDebug() << "应用列表desktopfpVector" << UkuiMenuInterface::desktopfpVector.size() << UkuiMenuInterface::desktopfpVector;
    QString m_desktopfp;

    for (int i = 0; i < desktopfpList.count(); i++) {
        myDebug() << desktopfpList.at(i) << "是否存在于已安装应用列表中" <<
                  UkuiMenuInterface::desktopfpVector.contains(QString(desktopfpList.at(i)));

        if (!UkuiMenuInterface::desktopfpVector.contains(QString(desktopfpList.at(i)))) {
            m_desktopfp = desktopfpList.at(i);
            myDebug() << "安装应用" << m_desktopfp;
            //获取当前时间戳
            QDateTime dt = QDateTime::currentDateTime();
            int datetime = dt.toTime_t();
            QString str = desktopfpList.at(i).section(' ', 0, 0);
            QStringList list = str.split('/');
            str = list[list.size() - 1];
            QString desktopfn = str;
            setting->beginGroup("recentapp");
            setting->setValue(desktopfn, datetime);
            setting->sync();
            setting->endGroup();
            //wgx
            setting->beginGroup("application");
            QStringList applist = setting->allKeys();
            int appnum = setting->allKeys().count();
            int maxindex = 0;

            for (int i = 0; i < appnum; i++) {
                if (setting->value(applist.at(i)).toInt() > maxindex) {
                    maxindex = setting->value(applist.at(i)).toInt();
                }
            }

            setting->setValue(desktopfn, maxindex + 1); //setting->allKeys().count()
            setting->sync();
            setting->endGroup();
            //
            QString iconstr = m_ukuiMenuInterface->getAppIcon(desktopfpList.at(i));
            syslog(LOG_LOCAL0 | LOG_DEBUG, "%s", iconstr.toLocal8Bit().data());
            syslog(LOG_LOCAL0 | LOG_DEBUG, "软件安装desktop文件名：%s", desktopfn.toLocal8Bit().data());

            Q_FOREACH (QString path, QIcon::themeSearchPaths()) {
                syslog(LOG_LOCAL0 | LOG_DEBUG, "%s", path.toLocal8Bit().data());
            }
        }
    }

    for (int i = 0; i < UkuiMenuInterface::desktopfpVector.count(); i++) {
        if (!desktopfpList.contains(UkuiMenuInterface::desktopfpVector.at(i))) {
            QString desktopfp = UkuiMenuInterface::desktopfpVector.at(i);
            QFileInfo fileInfo(desktopfp);
            QString desktopfn = fileInfo.fileName();
            myDebug() << "卸载" << desktopfn;
            setting->beginGroup("lockapplication");
            setting->remove(desktopfn);
            setting->sync();
            setting->endGroup();
            setting->beginGroup("application");

            if (!setting->contains(desktopfn)) {
                setting->sync();
                setting->endGroup();
                continue;
            }

            int val = setting->value(desktopfn).toInt();
            myDebug() << "卸载val" << val;
            setting->remove(desktopfn);
            QStringList desktopfnList = setting->allKeys();

            for (int i = 0; i < desktopfnList.count(); i++) {
                if (setting->value(desktopfnList.at(i)).toInt() > val) {
                    setting->setValue(desktopfnList.at(i), setting->value(desktopfnList.at(i)).toInt() - 1);
                }
            }

            setting->sync();
            setting->endGroup();
            setting->beginGroup("recentapp");
            setting->remove(desktopfn);
            setting->sync();
            setting->endGroup();
            syslog(LOG_LOCAL0 | LOG_DEBUG, "软件卸载desktop文件名：%s", desktopfn.toLocal8Bit().data());
            QDBusInterface iface("com.ukui.panel.desktop",
                                 "/",
                                 "com.ukui.panel.desktop",
                                 QDBusConnection::sessionBus());
            iface.call("RemoveFromTaskbar", desktopfp);
        }
    }

    UkuiMenuInterface::appInfoVector.clear();
    UkuiMenuInterface::appInfoVector = m_ukuiMenuInterface->createAppInfoVector();
    Q_EMIT requestUpdateSignal(m_desktopfp);
}
