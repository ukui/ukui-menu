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

#ifndef LETTERWIDGET_H
#define LETTERWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QTableWidget>
#include <ukuimenuinterface.h>
#include "letterbuttonwidget.h"
#include "src/UtilityFunction/listview.h"
#include "src/UtilityFunction/itemdelegate.h"

namespace Ui {
class LetterWidget;
}

class LetterWidget : public QWidget
{
    Q_OBJECT

public:
     explicit LetterWidget(QWidget *parent=nullptr);
    ~LetterWidget();
    void widget_make_zero();//MainWindow隐藏时，此界面恢复至初始状态

private:
    Ui::LetterWidget *ui;

    UkuiMenuInterface* pUkuiMenuInterface=nullptr;

    QVBoxLayout* mainLayout=nullptr;

    LetterButtonWidget* letterbtnwid=nullptr;//分类按钮界面

    QLabel* line=nullptr;
    ListView* applistview=nullptr;
    QVector<QStringList> data;
    ItemDelegate* m_delegate=nullptr;

    QStringList letterbtnlist;//存放字母按钮
    QStringList letterbtnrowlist;//存放字母按钮所在行
    QStringList letterposlist;//存放分类字符位置列表
    QStringList appsortlist;//存放应用排序列表

protected:
    void init_widget();
    void init_applist_widget();//初始化应用列表界面
    void fill_app_listview();//填充应用列表

private slots:
    void app_classificationbtn_clicked_slot();//应用列表字母分类按钮槽函数
    void recv_letterbtn_signal(QString btnname);//接收LetterButtonWidget界面按钮信号
    void exec_app_name(QString exec);//执行应用程序
    void update_app_listview();//更新应用列表

    void recvItemClickedSlot(QStringList arg);


signals:
    void send_letterbtn_list(QStringList list);//向LetterButtonWidget发送字母按钮列表
    void send_update_applist_signal();//向常用软件模块发送更新应用列表信号
    void send_hide_mainwindow_signal();//向MainViewWidget发送隐藏主窗口信号


};

#endif // LETTERWIDGET_H
