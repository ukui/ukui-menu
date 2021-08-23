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

#include "src/Widget/mainwindow.h"
#include <QtSingleApplication>
#include <QDesktopWidget>
#include <QFile>
#include <QScreen>
#include <QTranslator>
#include <QLocale>
#include <X11/Xlib.h>
#include <syslog.h>
#include "src/UtilityFunction/proxystyle.h"
#include <KWindowEffects>
#include "src/UtilityFunction/utility.h"
#include "src/Search/file-utils.h"

#include <ukui-log4qt.h>

int main(int argc, char *argv[])
{
    initUkuiLog4qt("ukui-menu");
    qRegisterMetaType<QVector<QStringList>>("QVector<QStringList>");

#if (QT_VERSION >= QT_VERSION_CHECK(5, 12, 0))
   QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
   QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif

    QtSingleApplication app("ukui-menu", argc, argv);
    app.setQuitOnLastWindowClosed(false);


    if(app.isRunning())
    {
        app.sendMessage("raise_window_noop");
        return EXIT_SUCCESS;
    }

//    auto Style=new ProxyStyle;
//    app.setStyle(Style);

    QTranslator translator;
    if (translator.load(QLocale(), "ukui-menu", "_", QM_FILES_INSTALL_PATH))
        app.installTranslator(&translator);
    else
        qDebug() << "Load translations file" << QLocale() << "failed!";

    Zeeker::FileUtils::loadHanziTable(":/src/SearchResultWidget/pinyinWithoutTone.txt");
    MainWindow w;
    app.setActivationWindow(&w);

//    w.setProperty("useSystemStyleBlur", true);

    //测试

    if(Style::panelPosition==0)
        w.setGeometry(QRect(Style::primaryScreenX+4,Style::primaryScreenY+Style::primaryScreenHeight-Style::panelSize-Style::minh-3,
                                  Style::minw,Style::minh));
    else if(Style::panelPosition==1)
        w.setGeometry(QRect(Style::primaryScreenX+4,Style::primaryScreenY+Style::panelSize+4,Style::minw,Style::minh));
    else if(Style::panelPosition==2)
        w.setGeometry(QRect(Style::primaryScreenX+Style::panelSize+4,Style::primaryScreenY+4,Style::minw,Style::minh));
    else
        w.setGeometry(QRect(Style::primaryScreenX+Style::primaryScreenWidth-Style::panelSize-Style::minw-4,Style::primaryScreenY+4,
                                  Style::minw,Style::minh));

    w.show();
    w.raise();
    w.update();
    w.activateWindow();
    w.hide();
    //测试

    return app.exec();
}
