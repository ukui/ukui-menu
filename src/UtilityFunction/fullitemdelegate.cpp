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
        QStyleOptionViewItem viewOption(option);//用来在视图中画一个item
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

        painter->setRenderHint(QPainter::Antialiasing);
        if(option.state & QStyle::State_MouseOver)
        {
            painter->setPen(QPen(Qt::NoPen));
            QColor color;
            color.setNamedColor(QString::fromLocal8Bit(AppBtnHover));
            painter->setBrush(QBrush(color));

            painter->setOpacity(0.14);
            painter->drawPath(path);
        }

        painter->setOpacity(1);
        QString desktopfp=index.data(Qt::DisplayRole).value<QString>();
        QString iconstr=pUkuiMenuInterface->getAppIcon(desktopfp);
        QIcon icon;
        if(QFile::exists(iconstr))
            icon=QIcon(iconstr);
        else
        {
            iconstr.remove(".png");
            iconstr.remove(".svg");
            icon=QIcon::fromTheme(iconstr);
            if(icon.isNull())
                icon=QIcon::fromTheme(QString("application-x-desktop"));
        }

        QString appname=pUkuiMenuInterface->getAppName(desktopfp);

        QRect iconRect;
        QRect textRect;
        iconRect=QRect(rect.x()+Style::AppLeftSpace ,
                       rect.y()+Style::AppTopSpace,
                       Style::AppListIconSize,
                       Style::AppListIconSize);
        icon.paint(painter,iconRect);

        textRect=QRect(rect.x()+5,
                       iconRect.bottom()+Style::AppSpaceBetweenIconText,
                       rect.width()-10,
                       rect.height()-iconRect.height()-Style::AppSpaceBetweenIconText);

        bool is_locked=false;
        bool is_recentapp=false;
        QFileInfo fileInfo(desktopfp);
        QString desktopfn=fileInfo.fileName();
        QFontMetrics fm=painter->fontMetrics();
        QString appnameElidedText=fm.elidedText(appname,Qt::ElideRight,rect.width()-10,Qt::TextShowMnemonic);

        if(module==0)
        {
            setting->beginGroup("lockapplication");
            if(setting->contains(desktopfn))
            {
                is_locked=true;
                QIcon icon(QString(":/data/img/mainviewwidget/lock-fs.svg"));
                icon.paint(painter,QRect(iconRect.topRight().x()-14,iconRect.topRight().y()-2,16,16));
            }
            setting->endGroup();
        }
        setting->beginGroup("recentapp");
        if(setting->contains(desktopfn) && !is_locked)
        {
            is_recentapp=true;
            appnameElidedText=fm.elidedText(appname,Qt::ElideRight,rect.width()-23,Qt::TextShowMnemonic);
            textRect=QRect(rect.x()+18,
                           iconRect.bottom()+Style::AppSpaceBetweenIconText,
                           rect.width()-23,
                           rect.height()-iconRect.height()-Style::AppSpaceBetweenIconText);

            painter->setPen(QPen(Qt::NoPen));
            painter->setBrush(QColor("#4d94ff"));
            int x=0;
            if(rect.width()<(23+fm.boundingRect(appname).width()))
                x=rect.x()+9;
            else
                x=rect.x()+(rect.width()-13-fm.boundingRect(appname).width())/2+4;

            painter->drawEllipse(QPoint(x,
                                        textRect.y()+(fm.boundingRect(appname).height()-8)/2+4),
                                 4,
                                 4);

        }
        setting->endGroup();

        painter->setPen(QPen(Qt::white));
        painter->drawText(textRect,Qt::AlignHCenter |Qt::AlignTop,appnameElidedText);

        if(option.state & QStyle::State_MouseOver)
        {
            int len=0;
            if(!is_locked && is_recentapp)
                len=fm.boundingRect(appname).width()+23;
            else
                len=fm.boundingRect(appname).width()+10;
            if(len>rect.width())
            {
                QToolTip::showText(QCursor::pos(),appname);
            }
        }
        else {
            QToolTip::hideText();
        }
        painter->restore();

    }
}

QSize FullItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(Style::AppListItemSizeWidth, Style::AppListItemSizeWidth);
}
