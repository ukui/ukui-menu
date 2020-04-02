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

#ifndef COMMONUSEWIDGET_H
#define COMMONUSEWIDGET_H

#include <QWidget>
#include <QDBusMessage>
#include <QSettings>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QScrollBar>
#include <QLabel>
#include <QPushButton>
#include <QProcess>
#include <QSvgRenderer>
#include <QHeaderView>
#include "src/Interface/ukuimenuinterface.h"
#include "src/RightClickMenu/rightclickmenu.h"
#include "src/UtilityFunction/listview.h"
#include "src/UtilityFunction/itemdelegate.h"

namespace Ui {
class CommonUseWidget;
}

class CommonUseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CommonUseWidget(QWidget *parent = nullptr);
    ~CommonUseWidget();
    void widgetMakeZero();//MainWindow隐藏时，此界面恢复至初始状态

private:
    Ui::CommonUseWidget *ui;
    QSettings *setting=nullptr;

    QHBoxLayout* mainLayout=nullptr;

    UkuiMenuInterface* pUkuiMenuInterface=nullptr;
    //应用列表界面
    ListView* listview=nullptr;
    QVector<QStringList> data;

    RightClickMenu* menu=nullptr;

protected:
    void initWidget();
    void initAppListWidget();//初始化应用列表界面
    void fillAppList();//填充应用列表
    void getCommonUseAppList();//获取常用应用列表

private Q_SLOTS:
    void updateListViewSlot();//更新应用列表槽函数
    void execApplication(QStringList arg);//执行应用程序

Q_SIGNALS:
    void sendHideMainWindowSignal();//向MainViewWidget发送隐藏主窗口信号
    void sendUpdateAppListSignal();//向FullCommonUseWidget发送更新应用列表信号
};

#endif // COMMONUSEWIDGET_H
