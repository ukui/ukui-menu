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
#include "fullitemdelegate.h"
#include "fulllistmodel.h"
#include "src/Style/style.h"

class FullListView : public QListView
{
    Q_OBJECT
public:
    FullListView(QWidget *parent,int module);
    ~FullListView();

    void addData(QStringList data);//字母排序模块添加数据
    void updateData(QStringList data);
    void removeRow(QString desktopfp);
    void insertRow(QString desktopfp);

protected:
    void initWidget();
    void enterEvent(QEvent* e) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent* e) Q_DECL_OVERRIDE;

private:
    FullItemDelegate* m_delegate=nullptr;
    FullListModel* listmodel=nullptr;
    int module=0;

    RightClickMenu* menu;//右键菜单
    QSettings *setting=nullptr;
    QStringList data;

    UkuiMenuInterface* pUkuiMenuInterface=nullptr;

private Q_SLOTS:
    void onClicked(QModelIndex index);//点击item
    void rightClickedSlot();//右键菜单

Q_SIGNALS:
    void sendItemClickedSignal(QString arg);//向字母排序或功能分类模块发送item点击信号
    void sendFixedOrUnfixedSignal();//向字母排序、功能分类发送固定或者取消固定信号
    void sendUpdateAppListSignal();//向FullCommonUseWidget发送更新应用列表信号
    void sendHideMainWindowSignal();
};

#endif // FULLLISTVIEW_H
