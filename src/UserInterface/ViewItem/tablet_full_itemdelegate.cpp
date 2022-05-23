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

#include "tablet_full_itemdelegate.h"
#include <QDebug>
#include "ukuimenuinterface.h"
#include "style.h"
#include <QGraphicsDropShadowEffect>
#include <QLabel>

TabletFullItemDelegate::TabletFullItemDelegate(QObject *parent, int module):
    QStyledItemDelegate(parent)
{
    this->module = module;
    pUkuiMenuInterface = new UkuiMenuInterface;
    //打开ini文件
    QString pathini = QDir::homePath() + "/.cache/ukui-menu/ukui-menu.ini";
    settt = new QSettings(pathini, QSettings::IniFormat);
    QString path = QDir::homePath() + "/.config/ukui/ukui-menu.ini";
    setting = new QSettings(path, QSettings::IniFormat);
}

TabletFullItemDelegate::~TabletFullItemDelegate()
{
    if (pUkuiMenuInterface) {
        delete pUkuiMenuInterface;
    }

    if (settt) {
        delete settt;
    }

    if (setting) {
        delete setting;
    }

    pUkuiMenuInterface = nullptr;
    settt = nullptr;
    setting = nullptr;
}

void TabletFullItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid()) {
        painter->save();
        QStyleOptionViewItem viewOption(option);//用来在视图中画一个item
        QRectF rect;
        rect.setX(option.rect.x());
        rect.setY(option.rect.y());
        rect.setWidth(option.rect.width());
        rect.setHeight(option.rect.height());
        painter->setOpacity(1);
        QString desktopfp = index.model()->data(index, Qt::DisplayRole).value<QString>();
        QString iconstr = pUkuiMenuInterface->getAppIcon(desktopfp);
        QIcon icon;
        QFileInfo iconFileInfo(iconstr);
        QString appname = pUkuiMenuInterface->getAppName(desktopfp);
        QRect iconRect;
        QRect textRect;

        if (iconFileInfo.isFile() && (iconstr.endsWith(".png") || iconstr.endsWith(".svg"))) {
            icon = QIcon(iconstr);
            //qDebug()<<"str-----------"<<iconstr;
        } else {
            //qDebug()<<"else"<<iconstr;
            iconstr.remove(".png");
            iconstr.remove(".svg");
            icon = QIcon::fromTheme(iconstr);

            //qDebug()<<"icon"<<icon;
            if (icon.isNull()) {
                //                qDebug()<<"isnottheme";
                if (QFile::exists(QString("/usr/share/icons/hicolor/scalable/apps/%1.%2").arg(iconstr).arg("svg"))) {
                    icon = QIcon(QString("/usr/share/icons/hicolor/scalable/apps/%1.%2").arg(iconstr).arg("svg"));
                } else if (QFile::exists(QString("/usr/share/icons/hicolor/scalable/apps/%1.%2").arg(iconstr).arg("png"))) {
                    icon = QIcon(QString("/usr/share/icons/hicolor/scalable/apps/%1.%2").arg(iconstr).arg("png"));
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
                } else {
                    icon = QIcon::fromTheme(QString("application-x-desktop"));
                }
            }
        }

        bool bigIcon = index.data(Qt::UserRole + 2).toBool();
        int iconTopSpace = (rect.height() - Style::AppListIconSize) / 2;
        int iconLeftSpace = (rect.width() - Style::AppListIconSize) / 2;

        if (bigIcon) {
            iconRect = QRect(rect.x() + iconLeftSpace - 6,
                             rect.y() + iconTopSpace - 6,
                             Style::AppListIconSize + 12,
                             Style::AppListIconSize + 12);
            textRect = QRect(rect.x(),
                             rect.bottom() - iconTopSpace,
                             rect.width(),
                             iconTopSpace);
        } else {
            iconRect = QRect(rect.x() + iconLeftSpace,
                             rect.y() + iconTopSpace,
                             Style::AppListIconSize,
                             Style::AppListIconSize);
            textRect = QRect(rect.x(),
                             rect.bottom() - iconTopSpace,
                             rect.width(),
                             iconTopSpace);
        }

        QString str;
        //打开文件.desktop
        GError **error = nullptr;
        GKeyFileFlags flags = G_KEY_FILE_NONE;
        GKeyFile *keyfile = g_key_file_new();
        QByteArray fpbyte = desktopfp.toLocal8Bit();
        char *filepath = fpbyte.data();
        g_key_file_load_from_file(keyfile, filepath, flags, error);
        char *name = g_key_file_get_locale_string(keyfile, "Desktop Entry", "Exec", nullptr, nullptr);
        //取出value值
        QString execnamestr = QString::fromLocal8Bit(name);
        //处理value值
        str = execnamestr;
        //        str = execnamestr.section(' ', 0, 0);
        //        QStringList list = str.split('/');
        //        str = list[list.size()-1];
        //关闭文件
        g_key_file_free(keyfile);
        QString desktopfp1 = str; //不带desktop
        settt->beginGroup("application");
        //判断禁用
        settt->sync();
        bool bo = settt->contains(desktopfp1.toLocal8Bit().data()); // iskey
        bool bo1 = settt->QSettings::value(desktopfp1.toLocal8Bit().data()).toBool(); //isvalue
        settt->endGroup();

        if (bo && bo1 == false) { //都存在//存在并且为false
            QPixmap pixmap;

            if (bigIcon) {
                pixmap = icon.pixmap((Style::AppListBigIconSize, Style::AppListBigIconSize), QIcon::Disabled, QIcon::Off); //wgx
                pixmap = pixmap.scaled(Style::AppListBigIconSize, Style::AppListBigIconSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            } else {
                pixmap = icon.pixmap((Style::AppListIconSize, Style::AppListIconSize), QIcon::Disabled, QIcon::Off); //wgx
            }

            icon = QIcon(pixmap);
        } else {
            QPixmap mPixmap;

            if (bigIcon) {
                mPixmap = icon.pixmap((Style::AppListBigIconSize, Style::AppListBigIconSize)); //wgx
                mPixmap = mPixmap.scaled(Style::AppListBigIconSize, Style::AppListBigIconSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            } else {
                mPixmap = icon.pixmap((Style::AppListIconSize, Style::AppListIconSize)); //wgx
            }

            icon = QIcon(mPixmap);
        }

        //        qDebug()<<"iconRect"<<iconRect;
        icon.paint(painter, iconRect);
        //文本换行
        QColor shadow = Qt::black;
        shadow.setAlpha(127);
        painter->setPen(shadow);
        QRect textLineRect;
        textLineRect.setLeft(textRect.left() + 1);
        textLineRect.setRight(textRect.right() + 1);
        textLineRect.setTop(textRect.top() + 1);
        textLineRect.setBottom(textRect.bottom() + 1);
        painter->drawText(textLineRect, Qt::TextWordWrap | Qt::AlignCenter, appname);
        painter->setPen(QPen(Qt::white));
        painter->drawText(textRect, Qt::TextWordWrap | Qt::AlignCenter, appname);
        painter->restore();
    }
}

QSize TabletFullItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //    if(index.row()>=Style::appNum)
    //    {
    //        return QSize(0,0);
    //    }
    return QSize(Style::TabletItemSizeWidthFirst, Style::AppListItemSizeHeight);
}

