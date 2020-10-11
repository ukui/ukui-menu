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
#include <syslog.h>

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
        QStyleOptionViewItem viewOption(option);//用来在视图中画一个item
        QRectF rect;
        QStringList strlist1=index.model()->data(index,Qt::DisplayRole).toStringList();
        if(strlist1.at(1).toInt()==1)
        {
            rect.setX(option.rect.x());
            rect.setY(option.rect.y());
            rect.setWidth(option.rect.width()-6);
            rect.setHeight(option.rect.height()-2);
        }
        else
        {
            rect.setX(option.rect.x());
            rect.setY(option.rect.y()+2);
            rect.setWidth(option.rect.width()-6);
            rect.setHeight(option.rect.height()-6);
        }

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

        QStringList strlist=index.model()->data(index,Qt::DisplayRole).toStringList();
        painter->setRenderHint(QPainter::Antialiasing);
        if(option.state & QStyle::State_MouseOver)
        {
            painter->save();
            painter->setPen(QPen(Qt::NoPen));
            painter->setBrush(option.palette.highlight());
            painter->drawPath(path);
            painter->restore();
        }
        painter->setOpacity(1);

        if(module>0)
        {
            if(strlist.at(1).toInt()==1)
            {
                QRect iconRect=QRect(rect.x()+11,rect.y()+(rect.height()-32)/2,32,32);
                QString iconstr=pUkuiMenuInterface->getAppIcon(strlist.at(0));
                QIcon icon;
                QFileInfo iconFileInfo(iconstr);
                if(iconFileInfo.isFile() && (iconstr.endsWith(".png") || iconstr.endsWith(".svg")))
                    icon=QIcon(iconstr);
                else
                {
                    iconstr.remove(".png");
                    iconstr.remove(".svg");
                    icon=QIcon::fromTheme(iconstr);
                    if(icon.isNull())
                    {
                        if(QFile::exists(QString("/usr/share/icons/hicolor/scalable/apps/%1.%2").arg(iconstr).arg("svg")))
                            icon=QIcon(QString("/usr/share/icons/hicolor/scalable/apps/%1.%2").arg(iconstr).arg("svg"));
                        else if(QFile::exists(QString("/usr/share/icons/hicolor/scalable/apps/%1.%2").arg(iconstr).arg("png")))
                            icon=QIcon(QString("/usr/share/icons/hicolor/scalable/apps/%1.%2").arg(iconstr).arg("png"));
                        else if(QFile::exists(QString("/usr/share/icons/hicolor/32x32/apps/%1.%2").arg(iconstr).arg("svg")))
                            icon=QIcon(QString("/usr/share/icons/hicolor/32x32/apps/%1.%2").arg(iconstr).arg("svg"));
                        else if(QFile::exists(QString("/usr/share/icons/hicolor/32x32/apps/%1.%2").arg(iconstr).arg("png")))
                            icon=QIcon(QString("/usr/share/icons/hicolor/32x32/apps/%1.%2").arg(iconstr).arg("png"));
                        else
                            icon=QIcon::fromTheme(QString("application-x-desktop"));
                    }
                }
                icon.paint(painter,iconRect,Qt::AlignLeft);
//                painter->setPen(QPen(Qt::white));
                QString appname=pUkuiMenuInterface->getAppName(strlist.at(0));
                painter->drawText(QRect(iconRect.right()+15,rect.y(),
                                        rect.width()-62,rect.height()),Qt::AlignVCenter,appname);
                setting->beginGroup("recentapp");
                QFileInfo fileInfo(strlist.at(0));
                QString desktopfn=fileInfo.fileName();
                if(setting->contains(desktopfn))
                {
                    painter->setPen(QPen(Qt::NoPen));
                    painter->setBrush(QColor("#4d94ff"));
                    painter->drawEllipse(QPoint(rect.topRight().x()-22,rect.y()+(rect.height()-8)/2+4),4,4);
                }
                setting->endGroup();
            }
            else
            {
                QRect textRect=QRect(rect.x()+11,rect.y()+12,rect.width(),rect.height());
                painter->drawText(textRect,Qt::AlignLeft,strlist.at(0));
                painter->setRenderHint(QPainter::Antialiasing, true);
                painter->setPen(QPen(QColor("#FFFFFF"),1));
                painter->setOpacity(0.06);
                painter->drawLine(QPoint(5,rect.bottom()),QPoint(rect.right(),rect.bottom()));
            }

        }
        else
        {
            painter->setOpacity(1);
            QRect iconRect=QRect(rect.left()+11,rect.y()+(rect.height()-32)/2,32,32);
            QString iconstr=pUkuiMenuInterface->getAppIcon(strlist.at(0));
            QIcon icon;
            QFileInfo iconFileInfo(iconstr);
            if(iconFileInfo.isFile() && (iconstr.endsWith(".png") || iconstr.endsWith(".svg")))
                icon=QIcon(iconstr);
            else
            {
                iconstr.remove(".png");
                iconstr.remove(".svg");
                icon=QIcon::fromTheme(iconstr);
                if(icon.isNull())
                {
                    if(pUkuiMenuInterface->getAppName(strlist.at(0))=="baidunetdisk")
                        syslog(LOG_LOCAL0 | LOG_DEBUG ,"---2---");
                    if(QFile::exists(QString("/usr/share/icons/hicolor/scalable/apps/%1.%2").arg(iconstr).arg("svg")))
                        icon=QIcon(QString("/usr/share/icons/hicolor/scalable/apps/%1.%2").arg(iconstr).arg("svg"));
                    else if(QFile::exists(QString("/usr/share/icons/hicolor/scalable/apps/%1.%2").arg(iconstr).arg("png")))
                        icon=QIcon(QString("/usr/share/icons/hicolor/scalable/apps/%1.%2").arg(iconstr).arg("png"));
                    else if(QFile::exists(QString("/usr/share/icons/hicolor/32x32/apps/%1.%2").arg(iconstr).arg("png")))
                        icon=QIcon(QString("/usr/share/icons/hicolor/32x32/apps/%1.%2").arg(iconstr).arg("png"));
                    else if(QFile::exists(QString("/usr/share/icons/hicolor/32x32/apps/%1.%2").arg(iconstr).arg("svg")))
                        icon=QIcon(QString("/usr/share/icons/hicolor/32x32/apps/%1.%2").arg(iconstr).arg("svg"));
                    else
                        icon=QIcon::fromTheme(QString("application-x-desktop"));
                }
            }
            icon.paint(painter,iconRect,Qt::AlignLeft);
            QString appname=pUkuiMenuInterface->getAppName(strlist.at(0));
            QFileInfo fileInfo(strlist.at(0));
            QString desktopfn=fileInfo.fileName();
            bool is_locked=false;
            setting->beginGroup("lockapplication");
            if(setting->contains(desktopfn))
            {
                is_locked=true;
                QIcon icon(QString(":/data/img/mainviewwidget/lock.svg"));
                icon.paint(painter,QRect(rect.topRight().x()-22,rect.y()+(rect.height()-16)/2,16,16));
            }
            setting->endGroup();
            setting->beginGroup("recentapp");
            if(setting->contains(desktopfn) && !is_locked)
            {
                painter->setPen(QPen(Qt::NoPen));
                painter->setBrush(QColor("#4d94ff"));
                painter->drawEllipse(QPoint(rect.topRight().x()-22,rect.y()+(rect.height()-8)/2+4),4,4);
            }
            setting->endGroup();
            painter->setOpacity(1);
//            painter->setPen(QPen(Qt::white));
            painter->drawText(QRect(iconRect.right()+15,rect.y(),
                                    rect.width()-62,rect.height()),Qt::AlignVCenter,appname);
        }
        painter->restore();
    }

}

QSize ItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStringList strlist=index.model()->data(index,Qt::DisplayRole).toStringList();
    if(strlist.at(1).toInt()==1)
        return QSize(310,44);
    else
        return QSize(310,48);

}
