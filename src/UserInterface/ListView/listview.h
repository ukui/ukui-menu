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

#ifndef LISTVIEW_H
#define LISTVIEW_H
#include <QListView>
#include <QStandardItem>
#include <QEvent>
#include <QScrollBar>
#include <QStandardItemModel>
#include "style.h"
#include "rightclickmenu.h"
#include <QSettings>
#include <QEvent>
#include <QKeyEvent>
#include "ukuimenuinterface.h"
#include "itemdelegate.h"
#include "klistview.h"
#include <KWindowEffects>

class ListView : public KListView
{
    Q_OBJECT

public:
    ListView(QWidget *parent/*, int width, int height, int module*/);
    ~ListView();

    void addData(QVector<QStringList> data, int module);
    void updateData(QVector<QStringList> data);

protected:
    void initWidget();
    void enterEvent(QEvent *e) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *e) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *e) override;
    void keyPressEvent(QKeyEvent *e);

private:
    int w = 0;
    int h = 0;
    int m_preRowCount;


private Q_SLOTS:
    void onClicked(QModelIndex index);//点击item
//    void rightClickedSlot(const QPoint &pos);//右键菜单

Q_SIGNALS:
    void sendMainWinActiveSignal(bool flag);
    void sendAppClassificationBtnClicked();
};

#endif // LISTVIEW_H
