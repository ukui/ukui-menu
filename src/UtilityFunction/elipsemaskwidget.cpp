/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#include "elipsemaskwidget.h"

#include <QPainter>

ElipseMaskWidget::ElipseMaskWidget(QWidget *parent) :
    QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    pWidth = parent->width();
    pHeigh = parent->height();
    pRadius = 6;
    pColor = QString("#ffffff");
    pBorder = 0;
}

ElipseMaskWidget::~ElipseMaskWidget()
{
}

void ElipseMaskWidget::setBgColor(QString color){
    pColor = color;
}

void ElipseMaskWidget::paintEvent(QPaintEvent *e){
    Q_UNUSED(e)

    QPainterPath cPath;
    cPath.addRect(0, 0, pWidth, pHeigh);
    cPath.addEllipse(0, 0, pWidth, pHeigh);

    QPainter painter(this);
    painter.setRenderHint(QPainter:: Antialiasing, true);  //设置渲染,启动反锯齿
    painter.setPen(Qt::NoPen);
    painter.setBrush(palette().color(QPalette::Base));
    painter.setOpacity(1);
    painter.drawPath(cPath);
    painter.setBrush(palette().color(QPalette::Button));
    painter.setOpacity(0.45);
    painter.drawPath(cPath);
}
