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

#include "src/MainWindow/mainwindow.h"
#include <QtSingleApplication>
#include <QDesktopWidget>
#include <QFile>
#include <QScreen>
#include <QTranslator>
#include <QLocale>

int main(int argc, char *argv[])
{
    qRegisterMetaType<QVector<QStringList>>("QVector<QStringList>");
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QtSingleApplication app("ukui-menu", argc, argv);
    app.setQuitOnLastWindowClosed(false);

    if(app.isRunning())
    {
        app.sendMessage("raise_window_noop");
        return EXIT_SUCCESS;
    }

    QTranslator translator;
//    if (translator.load(QLocale(), "ukui-menu", "_", QM_FILES_INSTALL_PATH))
    if (translator.load("/usr/share/ukui-menu/translations/ukui-menu_"+QLocale::system().name()+".qm"))
        app.installTranslator(&translator);
    else
        qDebug() << "Load translations file" << QLocale() << "failed!";

    MainWindow w;
    int position=0;
    int panelSize=0;
    if(QGSettings::isSchemaInstalled(QString("org.ukui.panel.settings").toLocal8Bit()))
    {
        QGSettings* gsetting=new QGSettings(QString("org.ukui.panel.settings").toLocal8Bit());
        if(gsetting->keys().contains(QString("panelposition")))
            position=gsetting->get("panelposition").toInt();
        else
            position=0;
        if(gsetting->keys().contains(QString("panelsize")))
            panelSize=gsetting->get("panelsize").toInt();
        else
            panelSize=46;
    }
    else
    {
        position=0;
        panelSize=46;
    }

    int x=QApplication::primaryScreen()->geometry().x();
    int y=QApplication::primaryScreen()->geometry().y();

    if(position==0)
        w.setGeometry(QRect(x,y+QApplication::primaryScreen()->geometry().height()-panelSize-Style::minh,Style::minw,Style::minh));
    else if(position==1)
        w.setGeometry(QRect(x,y+panelSize,Style::minw,Style::minh));
    else if(position==2)
        w.setGeometry(QRect(x+panelSize,y,Style::minw,Style::minh));
    else
        w.setGeometry(QRect(x+QApplication::primaryScreen()->geometry().width()-panelSize-Style::minw,y,Style::minw,Style::minh));
    app.setActivationWindow(&w);

//    QRectF rect;
//    rect.setX(w.rect().x()+1);
//    rect.setY(w.rect().y()+1);
//    rect.setWidth(w.rect().width()-2);
//    rect.setHeight(w.rect().height()-2);

//    //QPainterPath画圆角矩形
//    const qreal radius = 6;
//    QPainterPath path;
//    path.moveTo(rect.topRight() - QPointF(radius, 0));
//    path.lineTo(rect.topLeft() + QPointF(radius, 0));
//    path.quadTo(rect.topLeft(), rect.topLeft() + QPointF(0, radius));
//    path.lineTo(rect.bottomLeft() + QPointF(0, -radius));
//    path.quadTo(rect.bottomLeft(), rect.bottomLeft() + QPointF(radius, 0));
//    path.lineTo(rect.bottomRight() - QPointF(radius, 0));
//    path.quadTo(rect.bottomRight(), rect.bottomRight() + QPointF(0, -radius));
//    path.lineTo(rect.topRight() + QPointF(0, radius));
//    path.quadTo(rect.topRight(), rect.topRight() + QPointF(-radius, -0));

    //右上角
//    path.moveTo(rect.topRight() - QPointF(radius, 0));
//    path.lineTo(rect.topLeft());
//    path.lineTo(rect.bottomLeft());
//    path.lineTo(rect.bottomRight());
//    path.lineTo(rect.topRight() + QPointF(0, radius));
//    path.quadTo(rect.topRight(), rect.topRight() + QPointF(-radius, -0));
    //右下角
//    path.moveTo(rect.topRight());
//    path.lineTo(rect.topLeft());
//    path.lineTo(rect.bottomLeft());
//    path.lineTo(rect.bottomRight() - QPointF(radius, 0));
//    path.quadTo(rect.bottomRight(), rect.bottomRight() + QPointF(0, -radius));
//    path.lineTo(rect.topRight());
    //左下角
//    path.moveTo(rect.topRight());
//    path.lineTo(rect.topLeft());
//    path.lineTo(rect.bottomLeft() + QPointF(0, -radius));
//    path.quadTo(rect.bottomLeft(), rect.bottomLeft() + QPointF(radius, 0));
//    path.lineTo(rect.bottomRight());
//    path.lineTo(rect.topRight());
    w.setFrameStyle();
//    w.setProperty("blurRegion", QRegion(path.toFillPolygon().toPolygon()));

    //注释掉，以保证自启动时不显示界面
    w.show();
    w.raise();
    w.activateWindow();
    w.hide();

    return app.exec();
}
