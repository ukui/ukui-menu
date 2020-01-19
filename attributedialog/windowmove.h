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

#ifndef WINDOWMOVE_H
#define WINDOWMOVE_H

#include <QWidget>
#include <QMouseEvent>
#include <QDesktopWidget>

namespace Ui {
class WindowMove;
}

class WindowMove : public QWidget
{
    Q_OBJECT

public:
    explicit WindowMove(QWidget *parent = 0);
    ~WindowMove();

private:
    Ui::WindowMove *ui;
    QPoint press_point;
    bool is_move;

protected:
    /*
     * 设置窗口移动事件
     */
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
};

#endif // WINDOWMOVE_H
