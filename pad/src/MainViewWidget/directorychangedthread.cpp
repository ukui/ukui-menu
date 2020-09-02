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
#include <QtDBus>
#include "src/UtilityFunction/fulllistview.h"
#include "src/UtilityFunction/fullcommonusewidget.h"
#include <QDebug>

DirectoryChangedThread::DirectoryChangedThread()
{
    m_ukuiMenuInterface=new UkuiMenuInterface;
    QString path=QDir::homePath()+"/.config/ukui/ukui-menu-panel.ini";
    setting=new QSettings(path,QSettings::IniFormat);

}

DirectoryChangedThread::~DirectoryChangedThread()
{
    delete m_ukuiMenuInterface;
}

void DirectoryChangedThread::run()
{
//    if(this->m_path=="/usr/share/applications")
//    {

       QStringList desktopfpList=m_ukuiMenuInterface->getDesktopFilePath();
        if(desktopfpList.size() > UkuiMenuInterface::desktopfpVector.size())//有新的应用安装
        {
             QString m_desktopfp;

            for(int i=0;i<desktopfpList.count();i++)
            {
                if(!UkuiMenuInterface::desktopfpVector.contains(desktopfpList.at(i)))
                {
                    index++;
                    m_desktopfp=desktopfpList.at(i);
                    QFileInfo fileInfo(desktopfpList.at(i));
                    QString desktopfn=fileInfo.fileName();

                    //获取当前最后应用序号
//                    int pos=desktopfpList.size()-1;

                    setting->beginGroup("position");
                    setting->setValue(desktopfn,setting->childKeys().count());
                    setting->sync();
                    setting->endGroup();

//                    QString iconstr=m_ukuiMenuInterface->getAppIcon(desktopfpList.at(i));
//                    syslog(LOG_LOCAL0 | LOG_DEBUG ,"%s",iconstr.toLocal8Bit().data());
//                    syslog(LOG_LOCAL0 | LOG_DEBUG ,"软件安装desktop文件名：%s",desktopfn.toLocal8Bit().data());

//                    Q_FOREACH(QString path,QIcon::themeSearchPaths())
//                        syslog(LOG_LOCAL0 | LOG_DEBUG ,"%s",path.toLocal8Bit().data());

                    break;
                }

            }

            QFileInfo fileInfo(m_desktopfp);
            if(fileInfo.isFile() && fileInfo.exists())
            {
                UkuiMenuInterface::appInfoVector.clear();
                UkuiMenuInterface::appInfoVector=m_ukuiMenuInterface->createAppInfoVector();
                Q_EMIT requestUpdateSignal(m_desktopfp);
            }
        }
        else//软件卸载
        {
          //
        for(int i=0;i<UkuiMenuInterface::desktopfpVector.size();i++)
            {                

                if(!desktopfpList.contains(UkuiMenuInterface::desktopfpVector.at(i)))
                {
                    QString desktopfp=UkuiMenuInterface::appInfoVector.at(i).at(0);
                    QFileInfo fileInfo(desktopfp);
                    QString desktopfn=fileInfo.fileName();
                    setting->beginGroup("position");
                    int app_position=setting->value(desktopfn).toInt();
                    setting->remove(desktopfn);
                    setting->sync();
                    setting->endGroup();

                    syslog(LOG_LOCAL0 | LOG_DEBUG ,"软件卸载desktop文件名：%s",desktopfn.toLocal8Bit().data());

                    setting->beginGroup("position");

                    //qDebug() << app_position;

                    for (int index = 0; index < desktopfpList.size(); index++) {

                        QFileInfo fileInfos(desktopfpList.at(index));
                        QString app_name=fileInfos.fileName();

                        int po=setting->value(app_name).toInt();
                      //  qDebug() << app_name << po;
                        if(po>app_position)
                            {

                             setting->setValue(app_name,--po);
                            // qDebug() << app_name << setting->value(desktopfn).toInt();
                            // qDebug() << "achieve";
                            }
                        }
                    setting->sync();
                    setting->endGroup();
                    QDBusInterface iface("com.ukui.panel.desktop",
                                         "/",
                                         "com.ukui.panel.desktop",
                                         QDBusConnection::sessionBus());
                    iface.call("RemoveFromTaskbar",desktopfp);
                    break;

                }
            }

            UkuiMenuInterface::appInfoVector.clear();
            UkuiMenuInterface::appInfoVector=m_ukuiMenuInterface->createAppInfoVector();
            Q_EMIT deleteAppSignal();
        }
//    }

}
void DirectoryChangedThread::recvDirectoryPath(QString arg)
{
    this->m_path.clear();
    this->m_path=arg;
}
