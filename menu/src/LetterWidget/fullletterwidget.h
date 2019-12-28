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

#ifndef FULLLETTERWIDGET_H
#define FULLLETTERWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QToolButton>
#include <QAbstractItemView>
#include <QSpacerItem>
#include <QLabel>
#include <QDesktopWidget>
#include <QHeaderView>
#include "src/color.h"
#include <QScrollBar>
#include "ukuimenuinterface.h"
#include <QPushButton>
#include <QTableWidgetItem>
#include <QFrame>
#include <QSvgRenderer>
#include <QPainter>
#include <ukuimenuinterface.h>
#include <QButtonGroup>
#include "src/UtilityFunction/scrollarea.h"
#include "src/UtilityFunction/pushbutton.h"
#include "src/UtilityFunction/fulllistview.h"
#include "src/UtilityFunction/classifyscrollarea.h"
#include "src/UtilityFunction/toolbutton.h"

namespace Ui {
class FullLetterWidget;
}

class FullLetterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FullLetterWidget(QWidget *parent=nullptr);
    ~FullLetterWidget();
    void widget_make_zero();//MainWindow隐藏时，此界面恢复至初始状态

private:
    Ui::FullLetterWidget *ui;

    UkuiMenuInterface* pUkuiMenuInterface=nullptr;

    //主界面
    QHBoxLayout* mainLayout=nullptr;

    //应用列表界面
    QWidget* applistWid=nullptr;

    ScrollArea* scrollarea=nullptr;
    QWidget* scrollareawid=nullptr;
    QVBoxLayout* scrollareawidLayout=nullptr;
    QStringList letterbtnlist;//存放字母按钮
    QStringList letterbtnrowlist;//存放字母按钮所在行
    QStringList letterposlist;//存放分类字符位置列表
    QStringList appsortlist;//存放应用排序列表
    QStringList data;

    //字母列表界面
    QWidget* letterlistWid=nullptr;
    QHBoxLayout* letterlistLayout=nullptr;
    QSpacerItem* letterlistleftSpacer=nullptr;
    QSpacerItem* letterlistrightSpacer=nullptr;

    ClassifyScrollArea* letterlistscrollarea=nullptr;
    QWidget* letterlistscrollareaWid=nullptr;
    QVBoxLayout* letterlistscrollareawidLayout=nullptr;
    QList<QAbstractButton*> buttonList;
    QButtonGroup* pBtnGroup=nullptr;

protected:
    void init_widget();
    void fill_app_list();//填充应用列表
    void init_letterlist_widget();//初始化字母列表界面
    void init_letterlist_scrollarea();//初始化字母列表
    void init_applist_widget();//初始化应用列表界面
    void resize_scrollarea_controls();//设置scrollarea填充控件大小

private slots:
//    void leftbtn_clicked_slot();//向左按钮槽函数
//    void rightbtn_clicked_slot();//向右按钮槽函数
//    void letterbtn_clicked_slot();//字母列表数据项被选定槽函数
    void btngroup_clicked_slot(QAbstractButton *btn);
    void exec_app_name(QString appname);//执行应用程序
    void update_app_listview();//更新应用列表

signals:
    void send_letterbtn_list(QStringList list);//向FullLetterButtonWidget发送字母按钮列表
    void send_update_applist_signal();//向常用软件模块发送更新应用列表信号
    void send_hide_mainwindow_signal();//向MainViewWidget发送隐藏主窗口信号
};

#endif // FULLLETTERWIDGET_H
