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
#include "src/Interface/ukuimenuinterface.h"
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
    void updateAppListView(QStringList desktopfplist);

private:
    Ui::FullSearchResultWidget *ui;

    UkuiMenuInterface* pUkuiMenuInterface=nullptr;

    QHBoxLayout* mainLayout=nullptr;
    FullListView* listview=nullptr;
    QStringList data;

protected:
    void initWidget();

private Q_SLOTS:
    void execApplication(QString appname);//执行应用程序

Q_SIGNALS:
    void sendUpdateAppListSignal();//向CommonUseWidget发送更新应用列表信号
    void sendHideMainWindowSignal();//向MainViewWidget发送隐藏主窗口信号

};

#endif // FULLSEARCHRESULTWIDGET_H
