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
#include "src/Interface/ukuimenuinterface.h"
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
    void widgetMakeZero();//MainWindow隐藏时，此界面恢复至初始状态

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
    void initWidget();
    void initAppListWidget();//初始化应用列表界面
    void fillAppList();//填充应用列表
    void getCommonUseAppList();//获取常用应用列表

private Q_SLOTS:
    void updateListViewSlot();//更新应用列表槽函数
    void execApplication(QString appname);//执行应用程序

Q_SIGNALS:
    void sendUpdateAppListSignal();//向CommonUseWidget发送更新应用列表信号
    void sendHideMainWindowSignal();//向MainViewWidget发送隐藏主窗口信号
};

#endif // FULLCOMMONUSEWIDGET_H
