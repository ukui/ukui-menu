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
#include <QToolTip>
#include "utility.h"

ItemDelegate::ItemDelegate(QObject *parent, int module):
    KItemDelegate(parent)
{
    this->module = module;
    pUkuiMenuInterface = new UkuiMenuInterface;
}

ItemDelegate::~ItemDelegate()
{
    delete pUkuiMenuInterface;
}

void ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid()) {
        QRectF rect;
        rect.setX(option.rect.x());
        rect.setY(option.rect.y());
        rect.setWidth(option.rect.width());
        rect.setHeight(option.rect.height() - 2);
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
        QStringList strlist = index.model()->data(index, Qt::DisplayRole).toStringList();
        painter->setRenderHint(QPainter::Antialiasing);

        if ((option.state & QStyle::State_MouseOver) || (option.state & QStyle::State_HasFocus)) {
            QColor color;

            if (option.state & QStyle::State_Selected) {
                if (g_curStyle == "ukui-dark") {
                    color.setNamedColor("#33FFFFFF");
                } else {
                    color.setNamedColor("#D1FFFFFF");
                }
            } else {
                if (g_curStyle == "ukui-dark") {
                    color.setNamedColor("#1AFFFFFF");
                } else {
                    color.setNamedColor("#8CFFFFFF");
                }
            }

            painter->save();
            painter->setPen(QPen(Qt::NoPen));
            painter->setBrush(color);
            painter->drawPath(path);
            painter->restore();
        }

        painter->setOpacity(1);

        if (module > 0) {
            if (strlist.at(1).toInt() == 1) {
                QRect iconRect = QRect(rect.x() + 11, rect.y() + (rect.height() - 32) / 2, 32, 32);
                QString iconstr = pUkuiMenuInterface->getAppIcon(strlist.at(0));
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
                        } else if (QFile::exists(QString("/usr/share/icons/hicolor/32x32/apps/%1.%2").arg(iconstr).arg("svg"))) {
                            icon = QIcon(QString("/usr/share/icons/hicolor/32x32/apps/%1.%2").arg(iconstr).arg("svg"));
                        } else if (QFile::exists(QString("/usr/share/icons/hicolor/32x32/apps/%1.%2").arg(iconstr).arg("png"))) {
                            icon = QIcon(QString("/usr/share/icons/hicolor/32x32/apps/%1.%2").arg(iconstr).arg("png"));
                        } else if (QFile::exists(QString("/usr/share/icons/hicolor/48x48/apps/%1.%2").arg(iconstr).arg("svg"))) {
                            icon = QIcon(QString("/usr/share/icons/hicolor/48x48/apps/%1.%2").arg(iconstr).arg("svg"));
                        } else if (QFile::exists(QString("/usr/share/icons/hicolor/48x48/apps/%1.%2").arg(iconstr).arg("png"))) {
                            icon = QIcon(QString("/usr/share/icons/hicolor/48x48/apps/%1.%2").arg(iconstr).arg("png"));
                        } else if (QFile::exists(QString("/usr/share/icons/hicolor/64x64/apps/%1.%2").arg(iconstr).arg("svg"))) {
                            icon = QIcon(QString("/usr/share/icons/hicolor/64x64/apps/%1.%2").arg(iconstr).arg("svg"));
                        } else if (QFile::exists(QString("/usr/share/icons/hicolor/64x64/apps/%1.%2").arg(iconstr).arg("png"))) {
                            icon = QIcon(QString("/usr/share/icons/hicolor/64x64/apps/%1.%2").arg(iconstr).arg("png"));
                        } else if (QFile::exists(QString("/usr/share/pixmaps/%1.%2").arg(iconstr).arg("png"))) {
                            icon = QIcon(QString("/usr/share/pixmaps/%1.%2").arg(iconstr).arg("png"));
                        } else if (QFile::exists(QString("/usr/share/pixmaps/%1.%2").arg(iconstr).arg("svg"))) {
                            icon = QIcon(QString("/usr/share/pixmaps/%1.%2").arg(iconstr).arg("svg"));
                        } else if (QFile::exists(QString("/usr/share/pixmaps/%1.%2").arg(iconstr).arg("xpm"))) {
                            icon = QIcon(QString("/usr/share/pixmaps/%1.%2").arg(iconstr).arg("xpm"));
                        } else if (QFile::exists(QString("/usr/share/icons/%1.%2").arg(iconstr).arg("png"))) {
                            icon = QIcon(QString("/usr/share/icons/%1.%2").arg(iconstr).arg("png"));
                        } else {
                            icon = QIcon::fromTheme(QString("application-x-desktop"));
                        }
                    }
                }

                painter->save();
                icon.paint(painter, iconRect, Qt::AlignCenter);
                QString appname = pUkuiMenuInterface->getAppName(strlist.at(0));
                QFontMetrics fm = painter->fontMetrics();
                QString appnameElidedText = fm.elidedText(appname, Qt::ElideRight, rect.width() - 62, Qt::TextShowMnemonic);
                painter->drawText(QRect(iconRect.right() + 15, rect.y(),
                                        rect.width() - 62, rect.height()), Qt::AlignVCenter, appnameElidedText);
                painter->restore();
                painter->save();
                QFileInfo fileInfo(strlist.at(0));
                QString desktopfn = fileInfo.fileName();

                if (checkIfRecent(desktopfn)) {
                    painter->setPen(QPen(Qt::NoPen));
                    painter->setBrush(QColor("#4d94ff"));
                    painter->drawEllipse(QPoint(rect.topRight().x() - 22, rect.y() + (rect.height() - 8) / 2 + 4), 4, 4);
                }

                painter->restore();

                if ((option.state & QStyle::State_MouseOver) || (option.state & QStyle::State_HasFocus)) {
                    int len = fm.boundingRect(appname).width();

                    if (len > rect.width() - 62) {
                        if (option.state & QStyle::State_MouseOver) {
                            QToolTip::showText(QCursor::pos(), appname);
                        } else {
                            QToolTip::showText(option.widget->mapToGlobal(QPoint(rect.bottomRight().x(), rect.center().y())), appname);
                        }
                    } else {
                        QToolTip::hideText();
                    }
                }
            } else {
                //绘制分割线
                painter->save();
                //                QRect textRect=QRect(rect.x()+11,rect.y()+12,rect.width(),rect.height());
                QRect textRect = QRect(rect.x() + 11, rect.y(), rect.width(), rect.height());
                painter->drawText(textRect, Qt::AlignVCenter, strlist.at(0));
                painter->setRenderHints(QPainter::SmoothPixmapTransform, true);
                QPixmap pixmap = getCurIcon(":/data/img/mainviewwidget/DM-icon-option.svg", true).pixmap(QSize(16, 16));

                if ((option.state & QStyle::State_MouseOver) || (option.state & QStyle::State_HasFocus)) {
                    painter->drawPixmap(rect.width() - 25, rect.y() + 15, pixmap);
                }

                painter->restore();
            }
        } else if (module == -1) {
            QRect iconRect = QRect(rect.left() + 11, rect.y() + (rect.height() - 32) / 2, 32, 32);
            QString iconstr = strlist.at(3);
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
                    } else if (QFile::exists(QString("/usr/share/icons/hicolor/32x32/apps/%1.%2").arg(iconstr).arg("svg"))) {
                        icon = QIcon(QString("/usr/share/icons/hicolor/32x32/apps/%1.%2").arg(iconstr).arg("svg"));
                    } else if (QFile::exists(QString("/usr/share/icons/hicolor/32x32/apps/%1.%2").arg(iconstr).arg("png"))) {
                        icon = QIcon(QString("/usr/share/icons/hicolor/32x32/apps/%1.%2").arg(iconstr).arg("png"));
                    } else if (QFile::exists(QString("/usr/share/icons/hicolor/48x48/apps/%1.%2").arg(iconstr).arg("svg"))) {
                        icon = QIcon(QString("/usr/share/icons/hicolor/48x48/apps/%1.%2").arg(iconstr).arg("svg"));
                    } else if (QFile::exists(QString("/usr/share/icons/hicolor/48x48/apps/%1.%2").arg(iconstr).arg("png"))) {
                        icon = QIcon(QString("/usr/share/icons/hicolor/48x48/apps/%1.%2").arg(iconstr).arg("png"));
                    } else if (QFile::exists(QString("/usr/share/icons/hicolor/64x64/apps/%1.%2").arg(iconstr).arg("svg"))) {
                        icon = QIcon(QString("/usr/share/icons/hicolor/64x64/apps/%1.%2").arg(iconstr).arg("svg"));
                    } else if (QFile::exists(QString("/usr/share/icons/hicolor/64x64/apps/%1.%2").arg(iconstr).arg("png"))) {
                        icon = QIcon(QString("/usr/share/icons/hicolor/64x64/apps/%1.%2").arg(iconstr).arg("png"));
                    } else if (QFile::exists(QString("/usr/share/pixmaps/%1.%2").arg(iconstr).arg("png"))) {
                        icon = QIcon(QString("/usr/share/pixmaps/%1.%2").arg(iconstr).arg("png"));
                    } else if (QFile::exists(QString("/usr/share/pixmaps/%1.%2").arg(iconstr).arg("svg"))) {
                        icon = QIcon(QString("/usr/share/pixmaps/%1.%2").arg(iconstr).arg("svg"));
                    } else if (QFile::exists(QString("/usr/share/pixmaps/%1.%2").arg(iconstr).arg("xpm"))) {
                        icon = QIcon(QString("/usr/share/pixmaps/%1.%2").arg(iconstr).arg("xpm"));
                    } else if (QFile::exists(QString("/usr/share/icons/%1.%2").arg(iconstr).arg("png"))) {
                        icon = QIcon(QString("/usr/share/icons/%1.%2").arg(iconstr).arg("png"));
                    } else {
                        icon = QIcon::fromTheme(QString("application-x-desktop"));
                    }
                }
            }

            icon.paint(painter, iconRect, Qt::AlignCenter);
            QString appname = strlist.at(1);
            painter->setOpacity(1);
            painter->save();
            painter->setPen(QPen(option.palette.text().color()));
            painter->setBrush(Qt::NoBrush);
            painter->setOpacity(1);
            QFontMetrics fm = painter->fontMetrics();
            QString appnameElidedText = fm.elidedText(appname, Qt::ElideRight, rect.width() - 62, Qt::TextShowMnemonic);
            painter->drawText(QRect(iconRect.right() + 15, rect.y(),
                                    rect.width() - 62, rect.height()), Qt::AlignVCenter, appnameElidedText);
            painter->restore();

            if ((option.state & QStyle::State_MouseOver) || (option.state & QStyle::State_HasFocus)) {
                int len = fm.boundingRect(appname).width();

                if (len > rect.width() - 64) {
                    if (option.state & QStyle::State_MouseOver) {
                        QToolTip::showText(QCursor::pos(), appname);
                    } else {
                        QToolTip::showText(option.widget->mapToGlobal(QPoint(rect.bottomRight().x(), rect.center().y())), appname);
                    }
                } else {
                    QToolTip::hideText();
                }
            }
        } else {
            QRect iconRect = QRect(rect.left() + 11, rect.y() + (rect.height() - 32) / 2, 32, 32);
            QString iconstr = pUkuiMenuInterface->getAppIcon(strlist.at(0));
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
                    } else if (QFile::exists(QString("/usr/share/icons/hicolor/32x32/apps/%1.%2").arg(iconstr).arg("svg"))) {
                        icon = QIcon(QString("/usr/share/icons/hicolor/32x32/apps/%1.%2").arg(iconstr).arg("svg"));
                    } else if (QFile::exists(QString("/usr/share/icons/hicolor/32x32/apps/%1.%2").arg(iconstr).arg("png"))) {
                        icon = QIcon(QString("/usr/share/icons/hicolor/32x32/apps/%1.%2").arg(iconstr).arg("png"));
                    } else if (QFile::exists(QString("/usr/share/icons/hicolor/48x48/apps/%1.%2").arg(iconstr).arg("svg"))) {
                        icon = QIcon(QString("/usr/share/icons/hicolor/48x48/apps/%1.%2").arg(iconstr).arg("svg"));
                    } else if (QFile::exists(QString("/usr/share/icons/hicolor/48x48/apps/%1.%2").arg(iconstr).arg("png"))) {
                        icon = QIcon(QString("/usr/share/icons/hicolor/48x48/apps/%1.%2").arg(iconstr).arg("png"));
                    } else if (QFile::exists(QString("/usr/share/icons/hicolor/64x64/apps/%1.%2").arg(iconstr).arg("svg"))) {
                        icon = QIcon(QString("/usr/share/icons/hicolor/64x64/apps/%1.%2").arg(iconstr).arg("svg"));
                    } else if (QFile::exists(QString("/usr/share/icons/hicolor/64x64/apps/%1.%2").arg(iconstr).arg("png"))) {
                        icon = QIcon(QString("/usr/share/icons/hicolor/64x64/apps/%1.%2").arg(iconstr).arg("png"));
                    } else if (QFile::exists(QString("/usr/share/pixmaps/%1.%2").arg(iconstr).arg("png"))) {
                        icon = QIcon(QString("/usr/share/pixmaps/%1.%2").arg(iconstr).arg("png"));
                    } else if (QFile::exists(QString("/usr/share/pixmaps/%1.%2").arg(iconstr).arg("svg"))) {
                        icon = QIcon(QString("/usr/share/pixmaps/%1.%2").arg(iconstr).arg("svg"));
                    } else if (QFile::exists(QString("/usr/share/pixmaps/%1.%2").arg(iconstr).arg("xpm"))) {
                        icon = QIcon(QString("/usr/share/pixmaps/%1.%2").arg(iconstr).arg("xpm"));
                    } else if (QFile::exists(QString("/usr/share/icons/%1.%2").arg(iconstr).arg("png"))) {
                        icon = QIcon(QString("/usr/share/icons/%1.%2").arg(iconstr).arg("png"));
                    } else {
                        icon = QIcon::fromTheme(QString("application-x-desktop"));
                    }
                }
            }

            icon.paint(painter, iconRect, Qt::AlignCenter);
            QString appname = pUkuiMenuInterface->getAppName(strlist.at(0));
            QFileInfo fileInfo(strlist.at(0));
            QString desktopfn = fileInfo.fileName();

            if (checkIfLocked(desktopfn)) {
                QIcon icon = getCurIcon(":/data/img/mainviewwidget/lock.svg", true);
                icon.paint(painter, QRect(rect.topRight().x() - 22, rect.y() + (rect.height() - 16) / 2, 16, 16));
            }

            painter->setOpacity(1);
            painter->save();

            if (checkIfRecent(desktopfn) && !checkIfLocked(desktopfn)) {
                painter->setPen(QPen(Qt::NoPen));
                painter->setBrush(QColor("#4d94ff"));
                painter->drawEllipse(QPoint(rect.topRight().x() - 22, rect.y() + (rect.height() - 8) / 2 + 4), 4, 4);
            }

            painter->restore();
            painter->save();
            painter->setPen(QPen(option.palette.text().color()));
            painter->setBrush(Qt::NoBrush);
            painter->setOpacity(1);
            QFontMetrics fm = painter->fontMetrics();
            QString appnameElidedText = fm.elidedText(appname, Qt::ElideRight, rect.width() - 62, Qt::TextShowMnemonic);
            painter->drawText(QRect(iconRect.right() + 15, rect.y(),
                                    rect.width() - 62, rect.height()), Qt::AlignVCenter, appnameElidedText);
            painter->restore();

            if ((option.state & QStyle::State_MouseOver) || (option.state & QStyle::State_HasFocus)) {
                int len = fm.boundingRect(appname).width();

                if (len > rect.width() - 62) {
                    if (option.state & QStyle::State_MouseOver) {
                        QToolTip::showText(QCursor::pos(), appname);
                    } else {
                        QToolTip::showText(option.widget->mapToGlobal(QPoint(rect.bottomRight().x(), rect.center().y())), appname);
                    }
                } else {
                    QToolTip::hideText();
                }
            }
        }
    }
}

QSize ItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //    QStringList strlist=index.model()->data(index,Qt::DisplayRole).toStringList();
    //    if(strlist.at(1).toInt()==1)
    return QSize(310, 47);
    //    else
    //        return QSize(310,48);
}
