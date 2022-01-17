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
#include "utility.h"

DirectoryChangedThread::DirectoryChangedThread()
{
    m_ukuiMenuInterface = new UkuiMenuInterface;
}

DirectoryChangedThread::~DirectoryChangedThread()
{
    delete m_ukuiMenuInterface;
}

void DirectoryChangedThread::run()
{
    closeDataBase("DirectoryChangedThread");
    openDataBase("DirectoryChangedThread");
    QStringList desktopfpList = m_ukuiMenuInterface->getDesktopFilePath();

    if (desktopfpList.size() > UkuiMenuInterface::desktopfpVector.size()) { //有新的应用安装
        for (int i = 0; i < desktopfpList.count(); i++) {
            if (!UkuiMenuInterface::desktopfpVector.contains(desktopfpList.at(i))) {
                QFileInfo fileInfo(desktopfpList.at(i));
                QString desktopfn = fileInfo.fileName();
                updateDataBaseTableRecent(desktopfn);
                break;
            }
        }
    } else { //软件卸载
        for (int i = 0; i < UkuiMenuInterface::desktopfpVector.size(); i++) {
            if (!desktopfpList.contains(UkuiMenuInterface::desktopfpVector.at(i))) {
                QString desktopfp = UkuiMenuInterface::appInfoVector.at(i).at(0);
                QFileInfo fileInfo(desktopfp);
                QString desktopfn = fileInfo.fileName();
                deleteAppRecord(desktopfn);
                break;
            }
        }
    }

    myDebug() << "DirectoryChangedThread::run()";
    UkuiMenuInterface::appInfoVector.clear();
    UkuiMenuInterface::alphabeticVector.clear();
    UkuiMenuInterface::functionalVector.clear();
    UkuiMenuInterface::appInfoVector = m_ukuiMenuInterface->createAppInfoVector();
    UkuiMenuInterface::alphabeticVector = m_ukuiMenuInterface->getAlphabeticClassification();
    UkuiMenuInterface::functionalVector = m_ukuiMenuInterface->getFunctionalClassification();
    Q_EMIT requestUpdateSignal();
}
