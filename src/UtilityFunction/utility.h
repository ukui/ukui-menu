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
#define DBUS_NAME       "org.ukui.panel"
#define DBUS_PATH       "/panel/position"
#define DBUS_INTERFACE  "org.ukui.panel"


const QPixmap loadSvg(const QString &fileName, const int size);

//图片反白
QPixmap drawSymbolicColoredPixmap(const QPixmap &source);

//图片反黑
QPixmap drawSymbolicBlackColoredPixmap(const QPixmap &source);

//调试日志
void debugLog(QString strMsg);

int getScreenGeometry(QString methodName);
double getTransparency();


#endif // UTILITY_H
