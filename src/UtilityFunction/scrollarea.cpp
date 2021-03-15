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
#include <QPainter>
#include <QGSettings>
#include "utility.h"

ScrollAreaWid::ScrollAreaWid()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
}

void ScrollAreaWid::paintEvent(QPaintEvent *event)
{
    double transparency=getTransparency();
    QPainter painter(this);

    if(QGSettings::isSchemaInstalled(QString("org.ukui.control-center.personalise").toLocal8Bit()))
    {
        QGSettings gsetting(QString("org.ukui.control-center.personalise").toLocal8Bit());
        if(gsetting.keys().contains(QString("effect")))
        {
            if(gsetting.get("effect").toBool())
            {
                painter.setBrush(Qt::black);
//                painter.setBrush(Qt::red);
                painter.setPen(Qt::transparent);
                painter.setOpacity(0.25);
                painter.drawRect(this->rect());
            }
            else
            {
                painter.setBrush(this->palette().base());
                painter.setPen(Qt::transparent);
                painter.setOpacity(transparency);
                painter.drawRect(this->rect());
            }
        }
        else
        {
            painter.setBrush(this->palette().base());
            painter.setPen(Qt::transparent);
            painter.setOpacity(transparency);
            painter.drawRect(this->rect());
        }
    }
    else
    {
        painter.setBrush(this->palette().base());
        painter.setPen(Qt::transparent);
        painter.setOpacity(transparency);
        painter.drawRect(this->rect());
    }
    QWidget::paintEvent(event);
}

ScrollArea::ScrollArea()
{
    this->verticalScrollBar()->setVisible(false);
    installEventFilter(this);
    this->setFocusPolicy(Qt::NoFocus);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
    this->setFrameShape(QFrame::NoFrame);
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

void ScrollArea::scrollContentsBy(int dx, int dy)
{
    QScrollArea::scrollContentsBy(dx,dy);

    Q_EMIT requestUpdate();

//    update();
//    viewport()->update();
}
