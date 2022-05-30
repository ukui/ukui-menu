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

#include "main_view_widget.h"
#include "style.h"
#include <QSvgRenderer>
#include <QPainter>
#include <syslog.h>
#include <QDebug>
#include <thread>
#include <KWindowInfo>
#include <KWindowSystem>
#include <KWindowEffects>
#include <QCheckBox>

MainViewWidget::MainViewWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setAutoFillBackground(false);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFocusPolicy(Qt::StrongFocus);
}

MainViewWidget::~MainViewWidget()
{
}

void MainViewWidget::styleChangeSlot(const QString &style)
{
    m_curStyle = style;

    if (style == "ukui-dark") {
        m_backColor.setNamedColor("#800D0D0D");
    } else {
        m_backColor.setNamedColor("#94FFFFFF");
    }
}

void MainViewWidget::paintEvent(QPaintEvent *event)
{
//    double transparency = getTransparency();
    QRect rect = this->rect();
    QPainterPath path;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    painter.setPen(Qt::transparent);
    qreal radius = 10;
    path.moveTo(rect.topRight() - QPointF(radius, 0));
    path.lineTo(rect.topLeft() + QPointF(radius, 0));
    path.quadTo(rect.topLeft(), rect.topLeft() + QPointF(0, radius));
    path.lineTo(rect.bottomLeft() + QPointF(0, -radius));
    path.quadTo(rect.bottomLeft(), rect.bottomLeft() + QPointF(radius, 0));
    path.lineTo(rect.bottomRight() - QPointF(radius, 0));
    path.quadTo(rect.bottomRight(), rect.bottomRight() + QPointF(0, -radius));
    path.lineTo(rect.topRight() + QPointF(0, radius));
    path.quadTo(rect.topRight(), rect.topRight() + QPointF(-radius, -0));
    painter.setPen(Qt::transparent);
    painter.setOpacity(0.58);
    painter.fillPath(path, m_backColor);
    QWidget::paintEvent(event);
}
