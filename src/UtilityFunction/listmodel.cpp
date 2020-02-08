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

#include "listmodel.h"
#include <QDebug>

ListModel::ListModel(QObject *parent):
    QAbstractListModel(parent)
{
    pUkuiMenuInterface=new UkuiMenuInterface;
}

ListModel::~ListModel()
{
    delete pUkuiMenuInterface;
}

void ListModel::setData(const QVector<QStringList> &data)
{
    m_data=data;
}

void ListModel::updateData(const QVector<QStringList> &data)
{
    QVector<QStringList> max = (m_data.size() > data.size())? m_data:data;
    int row=max.size();
    m_data.clear();
    m_data=data;
    Q_EMIT dataChanged(createIndex(0,0), createIndex(row-1,0));
}

int ListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.size();
}

int ListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

bool ListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    for(int i=row+count-1;i>=row;i--)
    {
        m_data.removeAt(i);
    }
}

bool ListModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    m_data.removeAt(row);
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    switch (role){
    case Qt::DecorationRole:
    {
        int row=index.row();
        QStringList val=m_data.at(row);
        if(val.at(1).toInt()==0)
            return QVariant();
        else
        {
            QString iconstr=pUkuiMenuInterface->get_app_icon(val.at(0));
            iconstr.remove(".png");
            iconstr.remove(".svg");
            QIcon icon=QIcon::fromTheme(iconstr);
            if(icon.isNull())
                icon=QIcon::fromTheme(QString("application-x-desktop"));
            return icon;
        }
    }
    case Qt::DisplayRole:
    {
        int row=index.row();
        QStringList val=m_data.at(row);
        return val;
    }
    case Qt::SizeHintRole:
    {
        return QSize(296,48);

    }
    default:
        return QVariant();
    }


     return QVariant();
}
