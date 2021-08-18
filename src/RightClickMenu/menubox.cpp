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
#include "menubox.h"
#include <QDebug>
#include <QMouseEvent>
#include <QEventLoop>
#include <QTimer>

MenuBox::MenuBox(QWidget *parent):
    QMenu(parent)
{
    this->activateWindow();
}

void MenuBox::mousePressEvent(QMouseEvent *event)
{
    this->activateWindow();
    Q_EMIT sendMainWinActiveSignal(true);
    return QMenu::mousePressEvent(event);
}

//void MenuBox::mouseReleaseEvent(QMouseEvent *event)
//{
//    Q_UNUSED(event);
//    this->activateWindow();
//     Q_EMIT sendMainWinActiveSignal(true);
//    if(this->geometry().contains(QCursor::pos()))
//    {
//        return QMenu::mousePressEvent(event);
//    }
//    else
//    {
//        this->raise();
//        this->activateWindow();
//    }

//    return QMenu::mouseReleaseEvent(event);
//}
