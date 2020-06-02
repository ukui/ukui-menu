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

#ifndef SCROLLAREA_H
#define SCROLLAREA_H
#include <QScrollArea>
#include <QEvent>
#include <QScrollBar>
#include <QMouseEvent>
#include <QMouseEvent>
#include <QPropertyAnimation>

class ScrollArea : public QScrollArea
{
public:
    ScrollArea();

protected:
    void enterEvent(QEvent* e) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent* e) Q_DECL_OVERRIDE;
//    void wheelEvent(QWheelEvent *e);

private:
//    QPropertyAnimation *m_scrollAnimation;
    double m_speedTime = 3;

private Q_SLOTS:
//    void animationFinishSlot();//动画停止监控
//    void animationValueChangedSlot(const QVariant &value);//动画当前值变化监控
};

#endif // SCROLLAREA_H
