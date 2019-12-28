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

#ifndef FULLFUNCTIONWIDGET_H
#define FULLFUNCTIONWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QToolButton>
#include <QAbstractItemView>
#include <QSpacerItem>
#include <QLabel>
#include <QScrollBar>
#include <QPushButton>
#include <QButtonGroup>
#include <ukuimenuinterface.h>
#include "src/UtilityFunction/scrollarea.h"
#include "src/UtilityFunction/pushbutton.h"
#include "src/UtilityFunction/fulllistview.h"
#include "src/UtilityFunction/classifyscrollarea.h"
#include "src/UtilityFunction/toolbutton.h"
#include "src/UtilityFunction/functionclassifybutton.h"

namespace Ui {
class FullFunctionWidget;
}

class FullFunctionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FullFunctionWidget(QWidget *parent=nullptr);
    ~FullFunctionWidget();
    void widget_make_zero();//MainWindow隐藏时，此界面恢复至初始状态

private:
    Ui::FullFunctionWidget *ui;

    UkuiMenuInterface* pUkuiMenuInterface=nullptr;

    //主界面
    QHBoxLayout* mainLayout=nullptr;

    //应用列表界面
    QWidget* applistWid=nullptr;

    ScrollArea* scrollarea=nullptr;
    QWidget* scrollareawid=nullptr;
    QVBoxLayout* scrollareawidLayout=nullptr;
    QStringList classificationbtnlist;//存放分类按钮
    QStringList classificationbtnrowlist;//存放分类按钮所在行
    QStringList data;
//    int pos=0;

    //图标列表界面
    QStringList iconlist;
    QStringList iconlightlist;
    QStringList functionnamelist;
    QWidget* iconlistWid=nullptr;
    QHBoxLayout* iconlistLayout=nullptr;
    QSpacerItem* pIconListTopSpacer=nullptr;
    QSpacerItem* pIconListBottomSpacer=nullptr;

    ClassifyScrollArea* iconlistscrollarea=nullptr;
    QWidget* iconlistscrollareaWid=nullptr;
    QVBoxLayout* iconlistscrollareawidLayout=nullptr;
    QList<QAbstractButton*> buttonList;
    QButtonGroup* pBtnGroup=nullptr;

    int btnPos=0;
    int beforebtnPos=0;

protected:
    void init_widget();
    void init_iconlist_widget();//初始化图标列表界面
    void init_iconlist_scrollarea();//初始化图标列表界面数据表格iconlisttableWid

    void init_applist_widget();//初始化应用列表界面
    void fill_app_list();//填充应用列表
    void insert_classification_btn(QString btnname);//插入分类按钮
    void insert_app_list(QStringList appnamelist);//插入应用列表
    void resize_scrollarea_controls();//设置scrollarea填充控件大小

private slots:
    void exec_app_name(QString appname);//执行应用程序
//    void leftbtn_clicked_slot();//向左按钮槽函数
//    void rightbtn_clicked_slot();//向右按钮槽函数
//    void iconbtn_clicked_slot();//图标列表数据项被点击槽函数
//    void iconbtn_checked_slot(bool check);//图标列表按钮check状态改变
    void btngroup_clicked_slot(QAbstractButton *btn);
    void update_app_listview();//更新应用列表

signals:
    void send_classificationbtn_list(QStringList list);//向FullFunctionButtonWidget界面发送分类按钮列表
    void send_update_applist_signal();//向常用软件模块发送更新应用列表信号
    void send_hide_mainwindow_signal();//向MainViewWidget发送隐藏主窗口信号
};

#endif // FULLFUNCTIONWIDGET_H
