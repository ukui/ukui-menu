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

#include "classifybtnscrollarea.h"
#include "utility.h"
#include <QGSettings>
#include <QVariant>
#include <QKeyEvent>

ClassifyBtnScrollAreaWid::ClassifyBtnScrollAreaWid()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
}

void ClassifyBtnScrollAreaWid::paintEvent(QPaintEvent *event)
{
    double transparency=getTransparency();
    QPainter painter(this);

//    if(QGSettings::isSchemaInstalled(QString("org.ukui.control-center.personalise").toLocal8Bit()))
//    {
//        QGSettings gsetting(QString("org.ukui.control-center.personalise").toLocal8Bit());
//        if(gsetting.keys().contains(QString("effect")))
//        {
//            if(gsetting.get("effect").toBool())
//            {
//                painter.setBrush(Qt::black);
//                painter.setPen(Qt::transparent);
//                painter.setOpacity(0.25);
//                painter.drawRect(this->rect());
//            }
//            else
//            {
//                painter.setBrush(this->palette().base());
//                painter.setPen(Qt::transparent);
//                painter.setOpacity(transparency);
//                painter.drawRect(this->rect());
//            }
//        }
//        else
//        {
//            painter.setBrush(this->palette().base());
//            painter.setPen(Qt::transparent);
//            painter.setOpacity(transparency);
//            painter.drawRect(this->rect());
//        }
//    }
//    else{
        painter.setBrush(this->palette().base());
        painter.setPen(Qt::transparent);
        painter.setOpacity(transparency);
        painter.drawRect(this->rect());
//    }
    QWidget::paintEvent(event);
}

//void ClassifyBtnScrollAreaWid::keyPressEvent(QKeyEvent* e)
//{
//    if(e->key() == QEvent::KeyPress)
//    {
//        qDebug() << "ClassifyBtnScrollAreaWid::keyPressEvent" << e->key();
//        switch(e->key())
//        {
//        case Qt::Key_Up:
//        {
//            focusPreviousChild();
//        }
//            break;
//        case Qt::Key_Down:
//        {
//            focusNextChild();
//        }
//            break;
//        default:
//            return QWidget::keyPressEvent(e);
//            break;
//        }
//    }
//}


ClassifyBtnScrollArea::ClassifyBtnScrollArea(QWidget *parent):
    QScrollArea(parent)
{
    initWid();
}

void ClassifyBtnScrollArea::initWid()
{
    this->verticalScrollBar()->setHidden(true);
    this->horizontalScrollBar()->setHidden(true);
    this->setWidgetResizable(true);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setFrameShape(QFrame::NoFrame);
}
