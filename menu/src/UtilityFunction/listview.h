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
#include "listmodel.h"
#include "src/color.h"
#include "src/RightClickMenu/rightclickmenu.h"
#include <QSettings>
#include <ukuimenuinterface.h>

class ListView : public QListView
{
    Q_OBJECT
public:
    ListView(QWidget *parent, int width, int height, int module);
    ~ListView();

//    void addData(QVector<QStringList> data,QStringList rowlist);//字母排序模块添加数据
    void addData(QVector<QStringList> data);
    void updateData(QVector<QStringList> data);

protected:
    void init_widget();
    void enterEvent(QEvent* e) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent* e) Q_DECL_OVERRIDE;

private:
    ListModel* model=nullptr;
    int w=0;
    int h=0;
    int module=0;
    UkuiMenuInterface* pUkuiMenuInterface=nullptr;
//    QStringList rowlist;//存放分类字母所在行

    RightClickMenu* menu;//右键菜单
    QSettings *setting=nullptr;
    QVector<QStringList> data;

private slots:
    void onClicked(QModelIndex index);//点击item
    void rightClickedSlot();//右键菜单

signals:
    void sendItemClickedSignal(QStringList arg);//向字母排序或功能分类模块发送item点击信号
    void sendFixedOrUnfixedSignal();//向字母排序、功能分类发送固定或者取消固定信号
    void send_update_applist_signal();//向CommonUseWidget发送更新应用列表信号
};

#endif // LISTVIEW_H
