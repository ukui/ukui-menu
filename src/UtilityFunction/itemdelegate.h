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

#ifndef ITEMDELEGATE_H
#define ITEMDELEGATE_H
#include <QStyledItemDelegate>
#include "src/Style/style.h"
#include <QPainter>
#include <QSettings>
#include "src/interface/ukuimenuinterface.h"

class ItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    /**
     * @param module为0时为常用模块，1为字母模块、2为功能模块
     */
    ItemDelegate(QObject* parent,int module);
    ~ItemDelegate();
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    int module=0;
    QSettings *setting=nullptr;
    UkuiMenuInterface* pUkuiMenuInterface=nullptr;

};

#endif // ITEMDELEGATE_H
