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

#ifndef FULLLISTMODEL_H
#define FULLLISTMODEL_H
#include <QAbstractListModel>
#include <QIcon>
#include <ukuimenuinterface.h>
#include <QEvent>
#include "src/Style/style.h"

class FullListModel : public QAbstractListModel
{
public:
    FullListModel(QObject* parent);
    ~FullListModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    void removeRow(QString desktopfp);
    void insertRow(QString desktopfp);
    QVariant data(const QModelIndex &index, int role) const;
    void setData(QStringList data);
    void updateData(QStringList data);

private:
    QStringList m_data;
    UkuiMenuInterface* pUkuiMenuInterface=nullptr;
};

#endif // FULLLISTMODEL_H
