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

#include "fulllistmodel.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

FullListModel::FullListModel(QObject* parent):
    QAbstractListModel(parent)
{
    pUkuiMenuInterface=new UkuiMenuInterface;
}

FullListModel::~FullListModel()
{
    delete pUkuiMenuInterface;
}

void FullListModel::setData(QStringList data)
{
    m_data=data;
}

void FullListModel::updateData(QStringList data)
{
//    int width=QApplication::desktop()->availableGeometry().width()-160-32-12;
//    int columncount=width/120;
//    QStringList max=(m_data.size() > data.size())? m_data:data;
//    int row=0;
//    int column=0;
//    if(max.size()%columncount>0)
//    {
//        column=(max.size()/columncount+1)*columncount-m_data.size();
//        row=max.size()/columncount+1;
//    }
//    else
//    {
//        column=columncount;
//        row=max.size()/columncount+1;
//    }



    m_data.clear();
    m_data=data;
//    emit dataChanged(createIndex(0,0), createIndex(4,5));

}

int FullListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.size();
}

int FullListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
//    if(m_data.size()>0)
//    {
//        int width=QApplication::desktop()->availableGeometry().width()-160-32-12;
//        int columncount=width/120;
//        return columncount;
//    }
//    else return 0;

    return 1;
}

void FullListModel::removeRow(QString desktopfp)
{
    m_data.removeAll(desktopfp);
    Q_EMIT dataChanged(createIndex(0,0), createIndex(0,0));
}

void FullListModel::insertRow(QString desktopfp)
{
    m_data.insert(m_data.count()-1,desktopfp);
    Q_EMIT dataChanged(createIndex(0,0), createIndex(0,0));
}

QVariant FullListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    int row=index.row();
    QString val=m_data.at(row);
    switch (role){
    case Qt::DecorationRole:
    {
        QString iconstr=pUkuiMenuInterface->get_app_icon(val);
        iconstr.remove(".png");
        iconstr.remove(".svg");
        QIcon icon=QIcon::fromTheme(iconstr);
        if(icon.isNull())
            icon=QIcon::fromTheme(QString("application-x-desktop"));
        return icon;
    }
    case Qt::DisplayRole:
    {

        QString appname=pUkuiMenuInterface->get_app_name(val);
        return appname;
    }
    case Qt::SizeHintRole:
    {
        return QSize(Style::AppListItemSizeWidth,Style::AppListItemSizeWidth);
    }
    default:
        return QVariant();
    }


     return QVariant();
}
