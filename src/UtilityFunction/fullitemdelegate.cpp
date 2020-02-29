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

#include "fullitemdelegate.h"
#include <QDebug>

FullItemDelegate::FullItemDelegate(QObject *parent, int module):
    QStyledItemDelegate(parent)
{
    this->module=module;
    QString path=QDir::homePath()+"/.config/ukui/ukui-menu.ini";
    setting=new QSettings(path,QSettings::IniFormat);
    pUkuiMenuInterface=new UkuiMenuInterface;
}

FullItemDelegate::~FullItemDelegate()
{
    delete pUkuiMenuInterface;
}

void FullItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.isValid())
    {
        painter->save();
        QRectF rect;
        rect.setX(option.rect.x());
        rect.setY(option.rect.y());
        rect.setWidth(option.rect.width());
        rect.setHeight(option.rect.height());

        //QPainterPath画圆角矩形
        const qreal radius = 4;
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

//        if(option.state.testFlag(QStyle::State_Selected))
//        {
//            painter->setPen(QPen(Qt::blue));
//            painter->setBrush(QColor(229, 241, 255));
//            painter->drawPath(path);
//        }
//        else if(option.state.testFlag(QStyle::State_MouseOver))
//        {
//            painter->setPen(QPen(Qt::green));
//            painter->setBrush(Qt::NoBrush);
//            painter->drawPath(path);
//        }
//        else{
//            painter->setPen(QPen(Qt::gray));
//            painter->setBrush(Qt::NoBrush);
//            painter->drawPath(path);
//        }

//        painter->setRenderHint(QPainter::Antialiasing);
//        if(option.state & QStyle::State_MouseOver)
//        {
//            painter->setPen(QPen(Qt::NoPen));
//            QColor color;
//            color.setNamedColor(QString::fromLocal8Bit(AppBtnHover));
//            painter->setBrush(QBrush(color));

//            painter->setOpacity(0.14);
//            painter->drawPath(path);

//            QFontMetrics fm=painter->fontMetrics();
//            if(fm.boundingRect(appname).width()>rect.width())
//                QToolTip::showText(QCursor::pos(),appname);
//        }

        painter->setOpacity(1);
        QIcon icon=index.data(Qt::DecorationRole).value<QIcon>();
        QString desktopfp=index.data(Qt::DisplayRole).value<QString>();
        QString appname=pUkuiMenuInterface->getAppName(desktopfp);

        QFont font;
        QRect iconRect;
        font.setPixelSize(Style::AppListFontSize);
        iconRect=QRect(rect.x()+Style::AppLeftSpace ,
                       rect.y()+Style::AppTopSpace,
                       Style::AppListIconSize,
                       Style::AppListIconSize);
        painter->setFont(font);
        icon.paint(painter,iconRect);
        if(module==0)
        {
            setting->beginGroup("application");
            QFileInfo fileInfo(desktopfp);
            QString desktopfn=fileInfo.fileName();
            if(setting->value(desktopfn).toInt()==0)
            {
                QIcon icon(QString(":/data/img/mainviewwidget/lock-fs.svg"));
                icon.paint(painter,QRect(iconRect.topRight().x()-14,iconRect.topRight().y()-2,16,16));
//                painter->drawImage(QRect(iconRect.topRight().x()-8,iconRect.topRight().y(),16,16),
//                                   icon.pixmap(icon.actualSize(QSize(16, 16))).toImage());
            }
            setting->endGroup();

        }

        painter->setPen(QPen(Qt::white));
        QRect textRect;

        textRect=QRect(rect.x(),
                       iconRect.bottom()+Style::AppSpaceBetweenIconText,
                       rect.width(),
                       rect.height()-iconRect.height()-Style::AppSpaceBetweenIconText);
        QFontMetrics fm=painter->fontMetrics();
        QString appnameElidedText=fm.elidedText(appname,Qt::ElideRight,rect.width(),Qt::TextShowMnemonic);
//        painter->drawText(textRect,Qt::TextWordWrap |Qt::AlignHCenter | Qt::AlignTop,appname);
        painter->drawText(textRect,Qt::AlignHCenter |Qt::AlignTop,appnameElidedText);

        painter->setRenderHint(QPainter::Antialiasing);
        if(option.state & QStyle::State_MouseOver)
        {
            painter->setPen(QPen(Qt::NoPen));
            QColor color;
            color.setNamedColor(QString::fromLocal8Bit(AppBtnHover));
            painter->setBrush(QBrush(color));

            painter->setOpacity(0.14);
            painter->drawPath(path);

            if(fm.boundingRect(appname).width()>rect.width())
                QToolTip::showText(QCursor::pos(),appname);

        }
        painter->restore();

    }
}

//bool FullItemDelegate::helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index)
//{
//    if(event->type()==QEvent::ToolTip)
//    {
//    }
//    else if(event->type()==QEvent::Leave)
//    {
//    }
//    return true;
//}
