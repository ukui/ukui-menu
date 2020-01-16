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

#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H
#include <QToolButton>
#include <QEvent>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QSvgRenderer>
#include <QPainter>

class ToolButton : public QToolButton
{
    Q_OBJECT
public:
    /**
     * @param iconstr正常图片
     * @param piconstr点击图片
     * @param hoverbg悬浮背景色
     * @param pressedbg点击后背景色
     * @param module为0时为常用模块，1为字母模块、2为功能模块,3为属性模块
     */
    ToolButton(int width,
               int height,
               QString iconstr,
               QString piconstr,
               QString hoverbg,
               QString pressedbg,
               int module,
               QString text);

    bool is_pressed=false;//记录功能分类模块点击状态

private:
    QString iconstr;
    QString piconstr;
    QString hoverbg;
    QString pressedbg;
    int module;
    QLabel* label;
    QHBoxLayout* layout;
    QPixmap* pixmap;
    QSvgRenderer* svgRender;
    QString text;

protected:
    void enterEvent(QEvent* e);
    void leaveEvent(QEvent* e);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);


signals:
    void buttonClicked(QAbstractButton* btn);
};

#endif // TOOLBUTTON_H
