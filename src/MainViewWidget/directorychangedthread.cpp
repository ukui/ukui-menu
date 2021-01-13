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

#include "directorychangedthread.h"
#include <syslog.h>
#include <QIcon>

DirectoryChangedThread::DirectoryChangedThread()
{
    m_ukuiMenuInterface=new UkuiMenuInterface;
    QString path=QDir::homePath()+"/.config/ukui/ukui-menu.ini";
    setting=new QSettings(path,QSettings::IniFormat);

}

DirectoryChangedThread::~DirectoryChangedThread()
{
    delete m_ukuiMenuInterface;
}

void DirectoryChangedThread::run()
{
    QStringList desktopfpList=m_ukuiMenuInterface->getDesktopFilePath();
    if(desktopfpList.size() > UkuiMenuInterface::desktopfpVector.size())//有新的应用安装
    {
        setting->beginGroup("recentapp");
        for(int i=0;i<desktopfpList.count();i++)
        {
            if(!UkuiMenuInterface::desktopfpVector.contains(desktopfpList.at(i)))
            {
                //获取当前时间戳
                QDateTime dt=QDateTime::currentDateTime();
                int datetime=dt.toTime_t();
                QFileInfo fileInfo(desktopfpList.at(i));
                QString desktopfn=fileInfo.fileName();
                setting->setValue(desktopfn,datetime);
                setting->sync();

                QString iconstr=m_ukuiMenuInterface->getAppIcon(desktopfpList.at(i));
                syslog(LOG_LOCAL0 | LOG_DEBUG ,"%s",iconstr.toLocal8Bit().data());
                syslog(LOG_LOCAL0 | LOG_DEBUG ,"软件安装desktop文件名：%s",desktopfn.toLocal8Bit().data());
                Q_FOREACH(QString path,QIcon::themeSearchPaths())
                    syslog(LOG_LOCAL0 | LOG_DEBUG ,"%s",path.toLocal8Bit().data());
                break;
            }

        }
        setting->endGroup();
    }
    else//软件卸载
    {
        for(int i=0;i<UkuiMenuInterface::desktopfpVector.size();i++)
        {
            if(!desktopfpList.contains(UkuiMenuInterface::desktopfpVector.at(i)))
            {
                QString desktopfp=UkuiMenuInterface::appInfoVector.at(i).at(0);
                QFileInfo fileInfo(desktopfp);
                QString desktopfn=fileInfo.fileName();
                setting->beginGroup("lockapplication");
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
                setting->beginGroup("recentapp");
                setting->remove(desktopfn);
                setting->sync();
                setting->endGroup();
                syslog(LOG_LOCAL0 | LOG_DEBUG ,"软件卸载desktop文件名：%s",desktopfn.toLocal8Bit().data());
                break;
            }
        }
    }

    UkuiMenuInterface::appInfoVector.clear();
    UkuiMenuInterface::alphabeticVector.clear();
    UkuiMenuInterface::functionalVector.clear();
    UkuiMenuInterface::appInfoVector=m_ukuiMenuInterface->createAppInfoVector();
    UkuiMenuInterface::alphabeticVector=m_ukuiMenuInterface->getAlphabeticClassification();
    UkuiMenuInterface::functionalVector=m_ukuiMenuInterface->getFunctionalClassification();
    Q_EMIT requestUpdateSignal();
}
