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

#include "windowmove.h"
#include "ui_windowmove.h"

WindowMove::WindowMove(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WindowMove)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

//    this->resize(930-110,36);

}

WindowMove::~WindowMove()
{
    delete ui;
}

//设置窗口移动
void WindowMove::mousePressEvent(QMouseEvent *e)
{

    press_point = e->pos();
    is_move = true;
}

void WindowMove::mouseMoveEvent(QMouseEvent *e)
{
    if((e->buttons() == Qt::LeftButton) && is_move)
    {
        //        static QWidget* parent_widget = this->parentWidget();
        QPoint point = this->parentWidget()->pos();
        point.setX(point.x() + e->x() - press_point.x());
        point.setY(point.y() + e->y() - press_point.y());


        //设置临界值以阻止父窗口移出屏幕外
        int x=point.x() + e->x() - press_point.x();
        int y=point.y() + e->y() - press_point.y();
        if(x<0)
        {
            point.setX(0);
        }

        if(y<0)
        {
            point.setY(0);
        }

        QDesktopWidget *desktopWidget=QApplication::desktop();
        QRect screenRect = desktopWidget->screenGeometry();
        //        qDebug()<<screenRect.width()<<":"<<screenRect.height();
        int width=screenRect.width()-this->parentWidget()->width();
        int height=screenRect.height()-this->parentWidget()->height();

        //        qDebug()<<"屏幕分辨率："<<screenRect.width()<<":"<<screenRect.height();
        //        qDebug()<<"窗口移动"<<x<<":"<<y;
        //        qDebug()<<"窗口大小"<<this->width()<<":"<<this->height();
        //        qDebug()<<"屏幕减去父窗口大小"<<width<<":"<<height;
        if(x>width)
        {
            point.setX(width);
        }

        if(y>height)
        {
            point.setY(height);
        }

        this->parentWidget()->move(point);
    }

}

void WindowMove::mouseReleaseEvent(QMouseEvent *)
{
    if(is_move)
    {
        is_move = false;
    }
}
