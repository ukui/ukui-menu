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

#include "itemdelegate.h"
#include <QPushButton>

ItemDelegate::ItemDelegate(QObject* parent, int module):
    QStyledItemDelegate(parent)
{
    this->module=module;
    QString path=QDir::homePath()+"/.config/ukui/ukui-menu.ini";
    setting=new QSettings(path,QSettings::IniFormat);
    pUkuiMenuInterface=new UkuiMenuInterface;

}

ItemDelegate::~ItemDelegate()
{
    delete pUkuiMenuInterface;
}

void ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.isValid())
    {
        painter->save();

        QRectF rect;
        rect.setX(option.rect.x());
        rect.setY(option.rect.y());
        rect.setWidth(option.rect.width());
        rect.setHeight(option.rect.height()-10);
        //QPainterPath画圆角矩形
        const qreal radius = 2;
        QPainterPath path;
        path.moveTo(rect.topRight() - QPointF(radius, 0));
        path.lineTo(rect.topLeft() + QPointF(radius, 0));
        path.quadTo(rect.topLeft(), rect.topLeft() + QPointF(0, radius));
        path.lineTo(rect.bottomLeft() + QPointF(0, -radius));
        path.quadTo(rect.bottomLeft(), rect.bottomLeft() + QPointF(radius, 0));
        path.lineTo(rect.bottomRight() - QPointF(radius, 0));
        path.quadTo(rect.bottomRight(), rect.bottomRight() + QPointF(0, -radius));
        path.lineTo(rect.topRight() + QPointF(0, radius));
        path.quadTo(rect.topRight(), rect.topRight() + QPointF(-radius, -0));


        QFont font;
//        font.setFamily("Microsoft YaHei");

        font.setPixelSize(14);
        painter->setFont(font);

        QIcon icon=index.model()->data(index,Qt::DecorationRole).value<QIcon>();
        QStringList strlist=index.model()->data(index,Qt::DisplayRole).toStringList();
//        if(!icon.isNull())
//            painter->fillRect(QRect(rect.x(),rect.y(),40,40),QColor(Qt::blue));//图片加背景

        painter->setRenderHint(QPainter::Antialiasing);
        if(option.state & QStyle::State_MouseOver)
        {
            painter->setPen(QPen(Qt::NoPen));
            QColor color;
            color.setNamedColor(QString::fromLocal8Bit(AppBtnHover));
            painter->setBrush(color);
            painter->setOpacity(0.14);
            painter->drawPath(path);

        }
        painter->setOpacity(1);

        if(module>0)
        {
            if(strlist.at(1).toInt()==1)
            {
                QRect iconRect=QRect(rect.x()+11,rect.y()+(rect.height()-32)/2,32,32);
                icon.paint(painter,iconRect);
                painter->setPen(QPen(Qt::white));
                QString appname=pUkuiMenuInterface->getAppName(strlist.at(0));
                painter->drawText(QRect(iconRect.right()+15,rect.y(),
                                        rect.width()-62,rect.height()),Qt::AlignVCenter,appname);

            }
            else
            {
                painter->setPen(QPen(Qt::white));
                QRect textRect=QRect(rect.x()+11,rect.y()+(rect.height()-14)/2,strlist.at(0).size()*14,14);
                painter->drawText(textRect,Qt::AlignVCenter,strlist.at(0));
                painter->setRenderHint(QPainter::Antialiasing, true);
                painter->setPen(QPen(QColor("#FFFFFF"),1));
                painter->setOpacity(0.06);
                painter->drawLine(QPoint(5,rect.bottom()),QPoint(rect.right(),rect.bottom()));
            }

        }
        else
        {
            setting->beginGroup("application");
            QRect iconRect=QRect(rect.left()+11,rect.y()+(rect.height()-32)/2,32,32);
            icon.paint(painter,iconRect);

            QString appname=pUkuiMenuInterface->getAppName(strlist.at(0));
            QFileInfo fileInfo(strlist.at(0));
            QString desktopfn=fileInfo.fileName();
            if(setting->value(desktopfn).toInt()==0)
            {
                QIcon icon(QString(":/data/img/mainviewwidget/lock.svg"));
                icon.paint(painter,QRect(iconRect.topRight().x()-7,iconRect.topRight().y()-2,12,12));
            }
            painter->setOpacity(1);
            painter->setPen(QPen(Qt::white));
            painter->drawText(QRect(iconRect.right()+15,rect.y(),
                                    rect.width()-62,rect.height()),Qt::AlignVCenter,appname);

            setting->endGroup();
        }
        painter->restore();
    }

}
