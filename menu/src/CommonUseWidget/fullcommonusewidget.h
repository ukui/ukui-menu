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

#ifndef FULLCOMMONUSEWIDGET_H
#define FULLCOMMONUSEWIDGET_H

#include <QWidget>
#include <QSettings>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QScrollBar>
#include <QLabel>
#include <QPushButton>
#include <QDir>
#include <QProcess>
#include <QSvgRenderer>
#include <QHeaderView>
#include <ukuimenuinterface.h>
#include "src/RightClickMenu/rightclickmenu.h"
#include "src/UtilityFunction/fulllistview.h"
#include "src/Style/style.h"

namespace Ui {
class FullCommonUseWidget;
}

class FullCommonUseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FullCommonUseWidget(QWidget *parent = nullptr);
    ~FullCommonUseWidget();
    void widget_make_zero();//MainWindow隐藏时，此界面恢复至初始状态

private:
    Ui::FullCommonUseWidget *ui;

    UkuiMenuInterface* pUkuiMenuInterface=nullptr;

    QSettings *setting=nullptr;
    //主界面
    QVBoxLayout* mainLayout=nullptr;

    FullListView* listview=nullptr;
    QStringList data;

    QFileSystemWatcher* fileWatcher=nullptr;//监控文件夹状态
    RightClickMenu* menu=nullptr;

protected:
    void init_widget();
    void init_applist_widget();//初始化应用列表界面
    void fill_app_tablewidget();//填充应用列表

private slots:
    void update_listview_slot();//更新应用列表槽函数
    void exec_app_name(QString appname);//执行应用程序

signals:
    void send_update_applist_signal();//向CommonUseWidget发送更新应用列表信号
    void send_hide_mainwindow_signal();//向MainViewWidget发送隐藏主窗口信号
};

#endif // FULLCOMMONUSEWIDGET_H
