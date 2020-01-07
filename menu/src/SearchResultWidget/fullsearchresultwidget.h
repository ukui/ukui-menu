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

#ifndef FULLSEARCHRESULTWIDGET_H
#define FULLSEARCHRESULTWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QDesktopWidget>
#include <QLabel>
#include <QToolButton>
#include <QScrollBar>
#include <ukuimenuinterface.h>
#include "src/UtilityFunction/fulllistview.h"
#include "src/Style/style.h"

namespace Ui {
class FullSearchResultWidget;
}

class FullSearchResultWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FullSearchResultWidget(QWidget *parent = nullptr);
    ~FullSearchResultWidget();
    void update_app_listview(QStringList desktopfplist);

private:
    Ui::FullSearchResultWidget *ui;

    UkuiMenuInterface* pUkuiMenuInterface=nullptr;

    QHBoxLayout* mainLayout=nullptr;
    FullListView* listview=nullptr;
    QStringList data;

protected:
    void init_widget();

private slots:
    void exec_app_name(QString appname);//执行应用程序

signals:
    void send_update_applist_signal();//向CommonUseWidget发送更新应用列表信号
    void send_hide_mainwindow_signal();//向MainViewWidget发送隐藏主窗口信号

};

#endif // FULLSEARCHRESULTWIDGET_H
