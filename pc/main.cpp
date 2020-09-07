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
#include <KWindowEffects>
#include <X11/Xlib.h>
#include <syslog.h>

#include <QApplication>
#include <QDebug>
#include <QObject>

#include <QMessageBox>
//

int getScreenWidth() {
    Display *disp = XOpenDisplay(NULL);
    Screen *scrn = DefaultScreenOfDisplay(disp);
    if (NULL == scrn) {
        return 0;
    }
    int width = scrn->width;

    if (NULL != disp) {
        XCloseDisplay(disp);
    }
    return width;
}

int main(int argc, char *argv[])
{

    qRegisterMetaType<QVector<QStringList>>("QVector<QStringList>");

    if (getScreenWidth() > 2560) {
        QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    }

    QtSingleApplication app("ukui-menu", argc, argv);
    app.setQuitOnLastWindowClosed(false);


  

    if(app.isRunning())
    {
        app.sendMessage("raise_window_noop");
        return EXIT_SUCCESS;
    }

    QTranslator translator;
    if (translator.load(QLocale(), "ukui-menu", "_", QM_FILES_INSTALL_PATH))
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


    KWindowEffects::enableBlurBehind(w.winId(),true);
    
    
    

////
//    QGSettings* tabletMode= new QGSettings(TABLED_SCHEMA);
//    bool res = tabletMode->get("tablet-mode").toBool();
//    qDebug()<<res;
//    if(res)
//    {
//        w.hide();
//    }


//    else
//    {
//        w.show();
//        w.raise();
//        w.activateWindow();
//    }
//    w.show();
    
    

    return app.exec();
}
