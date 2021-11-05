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

#ifndef UTILITY_H
#define UTILITY_H

#include <QtCore>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <gio/gdesktopappinfo.h>
#include "src/Interface/ukuimenuinterface.h"

#define DBUS_NAME       "org.ukui.panel"
#define DBUS_PATH       "/panel/position"
#define DBUS_INTERFACE  "org.ukui.panel"

#define myDebug() qDebug() << QString("%1 %2 行 %3").arg(__FILE__).arg(__FUNCTION__).arg(__LINE__)

static int collectCount = 0;

const QPixmap loadSvg(const QString &fileName, const int size);

//图片反白
QPixmap drawSymbolicColoredPixmap(const QPixmap &source);

//图片反黑
QPixmap drawSymbolicBlackColoredPixmap(const QPixmap &source);

//调试日志
void debugLog(QString strMsg);
QRect getScreenAvailableGeometry();
QVariantList getScreenGeometry();
double getTransparency();
QString getEnvOverriddenDesktopFile(int pid);
//数据库操作
void openDataBase(QString connectionName);
void closeDataBase(QString connectionName);
void initDatabase();
bool updateDataBaseTableTimes(QString desktopfn);//更新打开次数
bool updateDataBaseTableType(QString desktopfn, int type);//更新是否锁定
bool updateDataBaseTableRecent(QString desktopfn);//更新最近添加
bool checkIfLocked(QString desktopfn);//检查是否被锁定
bool checkIfRecent(QString desktopfn);//检查是否最近安装
bool checkIfCollected(QString desktopfn);
QStringList getCollectAppList();
void changeCollectSort(QString dragDesktopfn, QString dropDesktopfn);
bool updateDataBaseCollect(QString desktopfn, int type);
QStringList getLockAppList();//获取被锁定应用
QStringList getUnlockAllList();//获取未被锁定应用
void cleanTimeoutApp();//清理超时应用
bool deleteAppRecord(QString desktopfn);//删除应用记录

//启动应用
void execApp(QString desktopfp);
bool checkOsRelease();//区分社区办与商业版


//获取用户图像
QString getUserIcon();
//获取用户姓名
QString getUserName();

extern QString projectCodeName;


#endif // UTILITY_H
