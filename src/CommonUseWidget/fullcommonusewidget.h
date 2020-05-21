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
#include <gio/gdesktopappinfo.h>
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
    void updateListView();
    void repaintWidget();
    /**
     * @brief moveScrollBar移动滚动条
     * @param type为0时表示向上移动，为1时表示向下移动
     */
    void moveScrollBar(int type);

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
//    void getCommonUseAppList();//获取常用应用列表

public Q_SLOTS:
    void execApplication(QString desktopfp);//执行应用程序
    void updateListViewSlot();

Q_SIGNALS:
    void sendHideMainWindowSignal();//向MainViewWidget发送隐藏主窗口信号
};

#endif // FULLCOMMONUSEWIDGET_H
