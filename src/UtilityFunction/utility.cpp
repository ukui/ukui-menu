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

#include "utility.h"
#include <QSvgRenderer>
#include <QPainter>
#include <QPixmap>
#include <QDBusMessage>
#include <QDBusConnection>
#include <QGSettings>

#define DBUS_NAME       "org.ukui.SettingsDaemon"
#define DBUS_PATH       "/org/ukui/SettingsDaemon/wayland"
#define DBUS_INTERFACE  "org.ukui.SettingsDaemon.wayland"

const QPixmap loadSvg(const QString &fileName, const int size)
{
    QPixmap pixmap(size, size);
    QSvgRenderer renderer(fileName);
    pixmap.fill(Qt::transparent);

    QPainter painter;
    painter.begin(&pixmap);
    renderer.render(&painter);
    painter.end();

    return pixmap;
}

QPixmap drawSymbolicColoredPixmap(const QPixmap &source)
{
    QColor gray(128,128,128);
    QColor standard (31,32,34);
    QImage img = source.toImage();
    for (int x = 0; x < img.width(); x++) {
        for (int y = 0; y < img.height(); y++) {
            auto color = img.pixelColor(x, y);
            if (color.alpha() > 0) {
                if (qAbs(color.red()-gray.red())<20 && qAbs(color.green()-gray.green())<20 && qAbs(color.blue()-gray.blue())<20) {
                    color.setRed(255);
                    color.setGreen(255);
                    color.setBlue(255);
                    img.setPixelColor(x, y, color);
                }
                else if(qAbs(color.red()-standard.red())<20 && qAbs(color.green()-standard.green())<20 && qAbs(color.blue()-standard.blue())<20)
                {
                    color.setRed(255);
                    color.setGreen(255);
                    color.setBlue(255);
                    img.setPixelColor(x, y, color);
                }
                else
                {
                    img.setPixelColor(x, y, color);
                }
            }
        }
    }
    return QPixmap::fromImage(img);
}

QPixmap drawSymbolicBlackColoredPixmap(const QPixmap &source)
{
    QImage img = source.toImage();
    for (int x = 0; x < img.width(); x++) {
        for (int y = 0; y < img.height(); y++) {
            auto color = img.pixelColor(x, y);
            if (color.alpha() > 0) {
                if (qAbs(color.red())>=200 && qAbs(color.green())>=200 && qAbs(color.blue())>=200) {
                    color.setRed(56);
                    color.setGreen(56);
                    color.setBlue(56);
                    img.setPixelColor(x, y, color);
                }
            }
        }
    }
    return QPixmap::fromImage(img);
}

int getScreenGeometry(QString methodName)
{
    int res = 0;
    QString xdg_session_type=qgetenv("XDG_SESSION_TYPE");
    if(xdg_session_type=="wayland")
    {
        QDBusMessage message = QDBusMessage::createMethodCall(DBUS_NAME,
                                   DBUS_PATH,
                                   DBUS_INTERFACE,
                                   methodName);
        QDBusMessage response = QDBusConnection::sessionBus().call(message);
        if (response.type() == QDBusMessage::ReplyMessage)
        {
            if(response.arguments().isEmpty() == false) {
                int value = response.arguments().takeFirst().toInt();
                res = value;
                qDebug() << value;
            }
        } else {
            qDebug()<<methodName<<"called failed";
        }
    }
    return res;
}

double getTransparency()
{
    double transparency=0.0;
    if(QGSettings::isSchemaInstalled(QString("org.ukui.control-center.personalise").toLocal8Bit()))
    {
        QGSettings gsetting(QString("org.ukui.control-center.personalise").toLocal8Bit());
        if(gsetting.keys().contains(QString("transparency")))
            transparency=gsetting.get("transparency").toDouble();
    }
    return transparency;
}

void debugLog(QString strMsg)
{
    QString path=QDir::homePath()+"/.config/ukui/ukui-menu.log";
    QFile confFile(path);
    if(confFile.open(QIODevice::Text | QIODevice::ReadWrite | QIODevice::Append))
    {
        QString text=QString("%1 %2")
                .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                .arg(strMsg);
        QTextStream textStream(&confFile);
        textStream<<text<<endl;
        textStream.flush();
        confFile.close();
    }
}
