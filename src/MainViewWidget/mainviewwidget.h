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

#ifndef MAINVIEWWIDGET_H
#define MAINVIEWWIDGET_H

#include <QWidget>
#include <QSpacerItem>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QScrollBar>
#include <QFileSystemWatcher>
#include <QSettings>
#include "src/Interface/ukuimenuinterface.h"
#include "src/CommonUseWidget/commonusewidget.h"
#include "src/LetterWidget/letterwidget.h"
#include "src/FunctionWidget/functionwidget.h"
#include "src/CommonUseWidget/fullcommonusewidget.h"
#include "src/FunctionWidget/fullfunctionwidget.h"
#include "src/LetterWidget/fullletterwidget.h"
#include "src/SearchResultWidget/fullsearchresultwidget.h"
#include "src/SearchResultWidget/searchresultwidget.h"
#include "src/SearchResultWidget/searchappthread.h"
#include "src/Style/style.h"

namespace Ui {
class MainViewWidget;
}

class MainViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainViewWidget(QWidget *parent = nullptr);
    ~MainViewWidget();
    void loadMinMainView();//加载默认主视图
    void loadMaxMainView();//加载全屏主视图
    void widgetMakeZero();//MainWindow隐藏时，此界面恢复至初始状态

private:
    Ui::MainViewWidget *ui;

    UkuiMenuInterface* pUkuiMenuInterface=nullptr;

    QVBoxLayout* mainLayout=nullptr;
    QWidget* topWidget=nullptr;
    QHBoxLayout* topLayout=nullptr;

    QSpacerItem* verticalSpacer=nullptr;

    QLineEdit* querylineEdit=nullptr;
    QHBoxLayout* queryLayout=nullptr;
    QWidget* pIconTextWid=nullptr;
    QHBoxLayout* pIconTextWidLayout=nullptr;
    QLabel* pQueryIcon=nullptr;
    QLabel* pQueryText=nullptr;
//    QWidgetAction *searchAction=nullptr;

    FullSearchResultWidget* fullsearchresultwid=nullptr;
    SearchResultWidget* searchresultwid=nullptr;
    SearchAppThread* searchappthread=nullptr;

    CommonUseWidget* commonusewid=nullptr;
    LetterWidget* letterwid=nullptr;
    FunctionWidget* functionwid=nullptr;
    FullCommonUseWidget* fullcommonusewid=nullptr;
    FullLetterWidget* fullletterwid=nullptr;
    FullFunctionWidget* fullfunctionwid=nullptr;

    int widgetState=-1;//分类窗口编号
    int saveCurrentWidState=-1;//存放当前分类窗口编号

    bool is_fullWid=false;

    int letterwid_state=0;
    int functionwid_state=0;
    QString letterbtnname;
    QString functionbtnname;

    QFileSystemWatcher* fileWatcher;//监控/usr/share/applications文件夹状态

    QSettings* setting=nullptr;

protected:
    void initWidget();//初始化界面
    void addTopControl();//添加顶部窗口控件
    void initQueryLineEdit();//初始化查询框
    void keyPressEvent(QKeyEvent* e);//查询框响应键盘事件

private Q_SLOTS:
    bool eventFilter(QObject *watched, QEvent *event);
    void loadCommonUseWidget();//加载常用分类界面
    void loadLetterWidget();//加载字母分类界面
    void loadFunctionWidget();//加载功能分类界面
    void loadFullCommonUseWidget();//加载全屏常用分类界面
    void loadFullLetterWidget();//加载全屏字母分类界面
    void loadFullFunctionWidget();//加载全屏功能分类界面

    void searchAppSlot(QString arg);//搜索程序和文件槽函数
    void ViewOpenedSlot(QDBusMessage msg);//监控进程开启
    void directoryChangedSlot();//desktop文件目录改变信号槽
    void recvSearchResult(QStringList desktopfplist);//接收搜索结果

Q_SIGNALS:
    void sendHideMainWindowSignal();//向MainWindow发送隐藏主窗口信号
    void viewOpenedSignal();//向常用软件模块发送刷新信号
    void directoryChangedSignal();//desktop文件目录改变信号
    void sendSearchKeyword(QString);//向SearchAppThread发送搜索关键字
};

#endif // MAINVIEWWIDGET_H
