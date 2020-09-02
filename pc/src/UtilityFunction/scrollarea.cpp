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

#include "scrollarea.h"
#include <QDebug>

ScrollArea::ScrollArea()
{
    this->verticalScrollBar()->setVisible(false);
    this->verticalScrollBar()->setStyleSheet("QScrollBar{width:6px;padding-top:300px;padding-bottom:300px;background:transparent;border-radius:6px;max-height:50px;}"
                                             "QScrollBar::handle{background-color:rgba(255,255,255,0.25); width:6px;height:10px;border-radius:1.5px;}"
                                             "QScrollBar::handle:hover{background-color:#697883;border-radius:3px;width:6px;height:10px;}"
                                             "QScrollBar::handle:pressed{background-color:#8897a3;border-radius:3px;width:6px;height:10px;}"
                                             "QScrollBar::sub-line{background-color:transparent;height:0px;width:0px;}"
                                             "QScrollBar::add-line{background-color:transparent;height:0px;width:0px;}"
                                              "QScrollBar::add-page{background-color:#000000;border-radius:3px;}"
                                             "QScrollBar::sub-page{background-color:#000000;border-radius:3px;}"
                                             );
      installEventFilter(this);
      this->setFocusPolicy(Qt::NoFocus);
      this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void ScrollArea::enterEvent(QEvent *e)
{
    Q_UNUSED(e);
    this->verticalScrollBar()->setVisible(true);
}

void ScrollArea::leaveEvent(QEvent *e)
{
    Q_UNUSED(e);
    this->verticalScrollBar()->setVisible(false);
}
