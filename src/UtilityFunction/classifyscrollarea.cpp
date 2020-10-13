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

#include "classifyscrollarea.h"
#include <QGSettings>
#include <QVariant>

ClassifyScrollAreaWid::ClassifyScrollAreaWid()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

}

void ClassifyScrollAreaWid::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QGSettings* gsetting=new QGSettings(QString("org.ukui.control-center.personalise").toLocal8Bit());
    double transparency=gsetting->get("transparency").toDouble();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setOpacity(transparency);
    painter.setBrush(this->palette().base());
    painter.setPen(Qt::NoPen);
    QRect rect = this->rect();
    rect.setWidth(rect.width());
    rect.setHeight(rect.height());
    painter.drawRect(rect);
}

ClassifyScrollArea::ClassifyScrollArea(QWidget *parent):
    QScrollArea(parent)
{
    initWid();
}

void ClassifyScrollArea::initWid()
{
    this->verticalScrollBar()->setHidden(true);
    this->horizontalScrollBar()->setHidden(true);
    this->setWidgetResizable(true);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setFrameShape(QFrame::NoFrame);
}
