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

#ifndef FUNCTIONWIDGET_H
#define FUNCTIONWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QScrollBar>
#include <QPushButton>
#include <ukuimenuinterface.h>
#include "functionbuttonwidget.h"
#include "src/RightClickMenu/rightclickmenu.h"
#include "src/UtilityFunction/listview.h"
#include "src/UtilityFunction/itemdelegate.h"

namespace Ui {
class FunctionWidget;
}

class FunctionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FunctionWidget(QWidget *parent=nullptr);
    ~FunctionWidget();
    void widget_make_zero();//MainWindow隐藏时，此界面恢复至初始状态

private:
    Ui::FunctionWidget *ui;

    UkuiMenuInterface* pUkuiMenuInterface=nullptr;

    //主界面
    QHBoxLayout* mainLayout=nullptr;
    FunctionButtonWidget* functionbtnwid=nullptr;//分类列表界面

    ListView* applistview=nullptr;
    QVector<QStringList> data;
    ItemDelegate* m_delegate=nullptr;

    QStringList classificationbtnlist;//存放分类按钮
    QStringList classificationbtnrowlist;//存放分类按钮所在行
    int row=0;

    RightClickMenu* menu;//右键菜单

protected:
    void init_widget();
    void init_applist_widget();//初始化应用列表界面
    void fill_app_listview();//填充应用列表
    void insert_classification_btn(QString btnname);//插入分类按钮
    void insert_app_list(QStringList appnamelist);//插入应用列表

private slots:
    void app_classificationbtn_clicked_slot();//应用列表功能分类按钮槽函数
    void recv_functionbtn_signal(QString btnname);//接收FunctionButtonWidget界面按钮信号
    void exec_app_name(QString exec);//执行应用程序
    void update_app_listview();//更新应用列表

    void recvItemClickedSlot(QStringList arg);

signals:
    void send_classificationbtn_list(QStringList list);//向FunctionButtonWidget界面发送分类按钮列表
    void send_update_applist_signal();//向常用软件模块发送更新应用列表信号
    void send_hide_mainwindow_signal();//向MainViewWidget发送隐藏主窗口信号


};

#endif // FUNCTIONWIDGET_H
