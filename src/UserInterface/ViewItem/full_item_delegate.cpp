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

#include "full_item_delegate.h"
#include "src/UtilityFunction/utility.h"
#include <QDebug>

FullItemDelegate::FullItemDelegate(QObject *parent, int module):
    KItemDelegate(parent)
{
    this->module = module;
    pUkuiMenuInterface = new UkuiMenuInterface;
}

FullItemDelegate::~FullItemDelegate()
{
    delete pUkuiMenuInterface;
}

void FullItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid()) {
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

        if ((option.state & QStyle::State_MouseOver) || (option.state & QStyle::State_HasFocus)) {
            //            QColor color = option.palette.text().color();
            //            color.setAlphaF(0.15);
            painter->save();
            painter->setPen(QPen(Qt::NoPen));
            painter->setBrush(Qt::white);
            painter->setOpacity(0.15);
            painter->drawPath(path);
            painter->restore();
        }

        painter->save();
        painter->setOpacity(1);
        QString desktopfp = index.data(Qt::DisplayRole).value<QString>();
        QString iconstr = pUkuiMenuInterface->getAppIcon(desktopfp);
        QIcon icon;
        QFileInfo iconFileInfo(iconstr);

        if (iconFileInfo.isFile() && (iconstr.endsWith(".png") || iconstr.endsWith(".svg"))) {
            icon = QIcon(iconstr);
        } else {
            iconstr.remove(".png");
            iconstr.remove(".svg");
            icon = QIcon::fromTheme(iconstr);

            if (icon.isNull()) {
                if (QFile::exists(QString("/usr/share/icons/hicolor/scalable/apps/%1.%2").arg(iconstr).arg("svg"))) {
                    icon = QIcon(QString("/usr/share/icons/hicolor/scalable/apps/%1.%2").arg(iconstr).arg("svg"));
                } else if (QFile::exists(QString("/usr/share/icons/hicolor/scalable/apps/%1.%2").arg(iconstr).arg("png"))) {
                    icon = QIcon(QString("/usr/share/icons/hicolor/scalable/apps/%1.%2").arg(iconstr).arg("png"));
                } else if (QFile::exists(QString("/usr/share/icons/hicolor/128x128/apps/%1.%2").arg(iconstr).arg("png"))) {
                    icon = QIcon(QString("/usr/share/icons/hicolor/128x128/apps/%1.%2").arg(iconstr).arg("png"));
                } else if (QFile::exists(QString("/usr/share/icons/hicolor/128x128/apps/%1.%2").arg(iconstr).arg("svg"))) {
                    icon = QIcon(QString("/usr/share/icons/hicolor/128x128/apps/%1.%2").arg(iconstr).arg("svg"));
                } else if (QFile::exists(QString("/usr/share/icons/hicolor/96x96/apps/%1.%2").arg(iconstr).arg("png"))) {
                    icon = QIcon(QString("/usr/share/icons/hicolor/96x96/apps/%1.%2").arg(iconstr).arg("png"));
                } else if (QFile::exists(QString("/usr/share/icons/hicolor/96x96/apps/%1.%2").arg(iconstr).arg("svg"))) {
                    icon = QIcon(QString("/usr/share/icons/hicolor/96x96/apps/%1.%2").arg(iconstr).arg("svg"));
                } else if (QFile::exists(QString("/usr/share/icons/hicolor/64x64/apps/%1.%2").arg(iconstr).arg("png"))) {
                    icon = QIcon(QString("/usr/share/icons/hicolor/64x64/apps/%1.%2").arg(iconstr).arg("png"));
                } else if (QFile::exists(QString("/usr/share/icons/hicolor/64x64/apps/%1.%2").arg(iconstr).arg("svg"))) {
                    icon = QIcon(QString("/usr/share/icons/hicolor/64x64/apps/%1.%2").arg(iconstr).arg("svg"));
                } else if (QFile::exists(QString("/usr/share/icons/hicolor/48x48/apps/%1.%2").arg(iconstr).arg("png"))) {
                    icon = QIcon(QString("/usr/share/icons/hicolor/48x48/apps/%1.%2").arg(iconstr).arg("png"));
                } else if (QFile::exists(QString("/usr/share/icons/hicolor/48x48/apps/%1.%2").arg(iconstr).arg("svg"))) {
                    icon = QIcon(QString("/usr/share/icons/hicolor/48x48/apps/%1.%2").arg(iconstr).arg("svg"));
                } else if (QFile::exists(QString("/usr/share/icons/hicolor/32x32/apps/%1.%2").arg(iconstr).arg("png"))) {
                    icon = QIcon(QString("/usr/share/icons/hicolor/32x32/apps/%1.%2").arg(iconstr).arg("png"));
                } else if (QFile::exists(QString("/usr/share/icons/hicolor/32x32/apps/%1.%2").arg(iconstr).arg("svg"))) {
                    icon = QIcon(QString("/usr/share/icons/hicolor/32x32/apps/%1.%2").arg(iconstr).arg("svg"));
                } else if (QFile::exists(QString("/usr/share/pixmaps/%1.%2").arg(iconstr).arg("png"))) {
                    icon = QIcon(QString("/usr/share/pixmaps/%1.%2").arg(iconstr).arg("png"));
                } else if (QFile::exists(QString("/usr/share/pixmaps/%1.%2").arg(iconstr).arg("svg"))) {
                    icon = QIcon(QString("/usr/share/pixmaps/%1.%2").arg(iconstr).arg("svg"));
                } else {
                    icon = QIcon::fromTheme(QString("application-x-desktop"));
                }
            }
        }

        QString appname = pUkuiMenuInterface->getAppName(desktopfp);
        QRect iconRect;
        QRect textRect;
        iconRect = QRect(/*rect.x()+66*/rect.x() + (rect.width() - Style::AppListIconSize) / 2,
                                        /*rect.y()+40*/rect.y() + 40,
                                        Style::AppListIconSize,
                                        Style::AppListIconSize);
        icon.paint(painter, iconRect);
        painter->restore();
        painter->save();
        bool is_locked = false;
        QFileInfo fileInfo(desktopfp);
        QString desktopfn = fileInfo.fileName();

        //添加固定图标
        if (module == 0) {
            if (checkIfLocked(desktopfn)) {
                is_locked = true;
                QIcon icon(QString(":/data/img/mainviewwidget/lock-fs.svg"));
                icon.paint(painter, QRect(iconRect.topRight().x() - 14, iconRect.topRight().y() - 2, 16, 16));
            }
        }

        painter->restore();
        textRect = QRect(rect.x() + 5,
                         iconRect.bottom() + Style::AppSpaceBetweenIconText,
                         rect.width() - 10,
                         rect.height() - iconRect.height() - Style::AppSpaceBetweenIconText);
        painter->save();
        //添加最近安装蓝色标签
        bool is_recentapp = false;
        QFontMetrics fm = painter->fontMetrics();
        QString appnameElidedText = fm.elidedText(appname, Qt::ElideRight, rect.width() - 10, Qt::TextShowMnemonic);

        if (checkIfRecent(desktopfn) && !checkIfLocked(desktopfn)) {
            is_recentapp = true;
            appnameElidedText = fm.elidedText(appname, Qt::ElideRight, rect.width() - 23, Qt::TextShowMnemonic);
            textRect = QRect(rect.x() + 18,
                             iconRect.bottom() + Style::AppSpaceBetweenIconText,
                             rect.width() - 23,
                             rect.height() - iconRect.height() - Style::AppSpaceBetweenIconText);
            painter->setPen(QPen(Qt::NoPen));
            painter->setBrush(QColor("#4d94ff"));
            int x = 0;

            if (rect.width() < (23 + fm.boundingRect(appname).width())) {
                x = rect.x() + 9;
            } else {
                x = rect.x() + (rect.width() - 13 - fm.boundingRect(appname).width()) / 2 + 4;
            }

            painter->drawEllipse(QPoint(x,
                                        textRect.y() + (fm.boundingRect(appname).height() - 8) / 2 + 4),
                                 4,
                                 4);
        }

        painter->restore();
        painter->save();

        //        painter->setPen(QPen(option.palette.text().color()));
        if (QGSettings::isSchemaInstalled(QString("org.ukui.style").toLocal8Bit())) {
            QGSettings gsetting(QString("org.ukui.style").toLocal8Bit());

            if (gsetting.keys().contains("styleName")) {
                if (/*gsetting.get("styleName").toString() == "ukui-light"*/false) {
                    painter->setPen(QPen(Qt::black));
                } else {
                    painter->setPen(QPen(Qt::white));
                }
            }
        }

        painter->setBrush(Qt::NoBrush);
        painter->drawText(textRect, Qt::AlignHCenter | Qt::AlignTop, appnameElidedText);
        //        painter->drawText(textRect,Qt::TextWordWrap |Qt::AlignHCenter,appname);
        painter->restore();

        if ((option.state & QStyle::State_MouseOver) || (option.state & QStyle::State_HasFocus)) {
            int len = 0;

            if (!is_locked && is_recentapp) {
                len = fm.boundingRect(appname).width() + 23;
            } else {
                len = fm.boundingRect(appname).width() + 10;
            }

            if (len > rect.width()) {
                if (option.state & QStyle::State_MouseOver) {
                    QToolTip::showText(QCursor::pos(), appname);
                } else {
                    QToolTip::showText(option.widget->mapToGlobal(QPoint(rect.center().x(), rect.center().y() + 15)), appname);
                }
            } else {
                QToolTip::hideText();
            }
        }
    }
}

QSize FullItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(/*Style::AppListItemSizeWidth*/220, /*Style::AppListItemSizeWidth*/220);
}
