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

#ifndef FULLLISTVIEW_H
#define FULLLISTVIEW_H
#include <QListView>
#include <QSettings>
#include "src/RightClickMenu/rightclickmenu.h"
#include "src/Interface/ukuimenuinterface.h"
#include <QEvent>
#include <QScrollBar>
#include <QToolTip>
#include <QStandardItemModel>
#include "src/ViewItem/fullitemdelegate.h"
#include "src/Style/style.h"
#include "klistview.h"

class FullListView : public KListView
{
    Q_OBJECT
public:
    explicit FullListView(QWidget *parent,int module);
    ~FullListView();

protected:
    void initWidget();
    void keyPressEvent(QKeyEvent* e);

private:
    RightClickMenu* menu=nullptr;//右键菜单
    QStringList data;
    QGSettings* gsetting;

public Q_SLOTS:
    void selectFirstItem();

Q_SIGNALS:
    void sendSetslidebar(int value);
};

#endif // FULLLISTVIEW_H
