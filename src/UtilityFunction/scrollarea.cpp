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
    this->verticalScrollBar()->setStyleSheet("QScrollBar{width:3px;padding-top:0px;padding-bottom:0px;background:transparent;border-radius:6px;}"
                                             "QScrollBar::handle{background-color:rgba(255,255,255,0.25); width:3px;border-radius:1.5px;}"
                                             "QScrollBar::handle:hover{background-color:#697883;border-radius:1.5px;}"
                                             "QScrollBar::handle:pressed{background-color:#8897a3;border-radius:1.5px;}"
                                             "QScrollBar::sub-line{background-color:transparent;height:0px;width:0px;}"
                                             "QScrollBar::add-line{background-color:transparent;height:0px;width:0px;}"
                                             );
      installEventFilter(this);
      this->setFocusPolicy(Qt::NoFocus);

      m_scrollAnimation=new QPropertyAnimation(this->verticalScrollBar(), "value");
      m_scrollAnimation->setEasingCurve(QEasingCurve::OutQuint);
      m_scrollAnimation->setDuration(800);
      connect(m_scrollAnimation, &QPropertyAnimation::valueChanged, this, &ScrollArea::handleScrollValueChanged);
      connect(m_scrollAnimation, &QPropertyAnimation::finished, this, &ScrollArea::handleScrollFinished);

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

void ScrollArea::wheelEvent(QWheelEvent *e)
{
    int offset = -e->angleDelta().y();
    m_scrollAnimation->stop();
    m_scrollAnimation->setStartValue(verticalScrollBar()->value());
    m_scrollAnimation->setEndValue(verticalScrollBar()->value() + offset * m_speedTime);
    m_scrollAnimation->start();
}

void ScrollArea::handleScrollValueChanged()
{
    QScrollBar *vscroll = verticalScrollBar();

    if (vscroll->value() == vscroll->maximum() ||
        vscroll->value() == vscroll->minimum()) {
        blockSignals(false);
    } else {
        blockSignals(true);
    }
}

void ScrollArea::handleScrollFinished()
{
    blockSignals(false);

//    QPoint pos = mapFromGlobal(QCursor::pos());
//    emit entered(indexAt(pos));
}
