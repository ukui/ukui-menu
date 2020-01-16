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

#ifndef LISTMODEL_H
#define LISTMODEL_H
#include <QAbstractListModel>
#include <QIcon>
#include <ukuimenuinterface.h>

class ListModel : public QAbstractListModel
{
public:
    ListModel(QObject* parent);
    ~ListModel();
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    QVariant data(const QModelIndex &index, int role) const;
    void setData(const QVector<QStringList>& data);
    void updateData(const QVector<QStringList>& data);

private:
    QVector<QStringList> m_data;
    UkuiMenuInterface* pUkuiMenuInterface=nullptr;
};

#endif // LISTMODEL_H
