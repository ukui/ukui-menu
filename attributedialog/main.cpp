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

#include "attributedialog.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QFile qss(":/ukui-menu-attr.qss");
//    qss.open(QFile::ReadOnly);
//    qApp->setStyleSheet(qss.readAll());

    QString locale = QLocale::system().name();
    QTranslator translator;
    if (locale == "zh_CN"){
        if (translator.load(":/ukui_menu_attr_ch.qm"))
            a.installTranslator(&translator);
        else
            qDebug() << "Load translations file" << locale << "failed!";
    }

    AttributeDialog w;
    w.set_attribute_value(argv[1]);
    w.show();
    w.activateWindow();

    return a.exec();
}
