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
#include "fullitemdelegate.h"
#include "src/Style/style.h"

class FullListView : public QListView
{
    Q_OBJECT
public:
    FullListView(QWidget *parent,int module);
    ~FullListView();

    void addData(QStringList data);//字母排序模块添加数据
    void updateData(QStringList data);

protected:
    void initWidget();
    void enterEvent(QEvent* e) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent* e) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *e) override;
//    void mousePressEvent(QMouseEvent *event);

private:
    FullItemDelegate* m_delegate=nullptr;
    QStandardItemModel* listmodel=nullptr;
    int module=0;

    RightClickMenu* menu=nullptr;//右键菜单
    QStringList data;

    UkuiMenuInterface* pUkuiMenuInterface=nullptr;

private Q_SLOTS:
    void onClicked(QModelIndex index);//点击item
    void rightClickedSlot(const QPoint &pos);//右键菜单

Q_SIGNALS:
    void sendItemClickedSignal(QString arg);//发送item点击信号
    void sendHideMainWindowSignal();
    void sendUpdateAppListSignal();
};

#endif // FULLLISTVIEW_H
