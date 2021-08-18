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
#ifndef INTERNALSTYLE_H
#define INTERNALSTYLE_H
#include <QProxyStyle>
#include <qgsettings.h>
#include <QVariant>
#include <QPainter>
#include <QPalette>
#include <QStyleOption>

class ProxyStyle : public QProxyStyle
{
    Q_OBJECT
public:
    ProxyStyle();

//    void drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const;
//    void drawControl(QStyle::ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const;
//    void drawComplexControl(QStyle::ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget = nullptr) const;
    void polish(QPalette &pal);
//    void polish(QWidget *widget);
    QPalette standardPalette() const;

private:
    double transparency;

};

#endif // INTERNALSTYLE_H
