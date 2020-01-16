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
#include "ukuimenuinterface.h"

int main(int argc, char *argv[])
{
    QtSingleApplication app("ukui-start-menu",argc,argv);
    app.setQuitOnLastWindowClosed(false);

    if(app.isRunning())
    {
        app.sendMessage("raise_window_noop");
        return EXIT_SUCCESS;
    }

    QFile qss(":/data/qss/ukui-menu.qss");
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());

    char style[400];
    sprintf(style,"QToolTip{border:0px;border-radius:3px;color:#ffffff;background-color:%s}",
            ToolTipBackground);
    qApp->setStyleSheet(style);

    MainWindow w;
    w.setGeometry(QRect(0,QtSingleApplication::desktop()->availableGeometry().height()-590-2,376+2,590+2));
    app.setActivationWindow(&w);

    qDebug()<<"Single:"<<QtSingleApplication::desktop()->availableGeometry().height();
    qDebug()<<"Main:"<<Style::AppListWidHeight<<QApplication::desktop()->availableGeometry().height();
    foreach (QScreen *screen, QGuiApplication::screens())
    {
        qDebug()<<"Gui:"<< screen->availableSize().height();
        qDebug()<<"Gui:"<< screen->availableSize().height();
    }

    //注释掉，以保证自启动时不显示界面
    w.show();
    w.raise();
    w.activateWindow();
//    w.hide();

    return app.exec();
}
