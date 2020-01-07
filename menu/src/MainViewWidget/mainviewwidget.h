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
#include <ukuimenuinterface.h>
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
    void load_min_mainview();//加载默认主视图
    void load_max_mainview();//加载全屏主视图
    void widget_make_zero();//MainWindow隐藏时，此界面恢复至初始状态

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
    QAction *searchAction=nullptr;

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

    bool is_fullWid=false;

    int letterwid_state=0;
    int functionwid_state=0;
    QString letterbtnname;
    QString functionbtnname;

    QFileSystemWatcher* fileWatcher;//监控/usr/share/applications文件夹状态

    QSettings* setting=nullptr;

protected:
    void init_widget();//初始化界面
    void init_top_widget();
    void add_top_control();//添加顶部窗口控件
    void init_query_lineedit();//初始化查询框

    void add_search_result_widget();//添加搜索列表界面
    void fill_search_result_tablewid();//填充默认态程序搜索列表
    void fill_full_search_result_tablewid();//填充全屏态程序搜索列表

    void keyPressEvent(QKeyEvent* e);//查询框响应键盘事件

private slots:
    bool eventFilter(QObject *watched, QEvent *event);
    void load_commonuse_widget();//加载常用分类界面
    void load_letter_widget();//加载字母分类界面
    void load_function_widget();//加载功能分类界面
    void load_fullcommonuse_widget();//加载全屏常用分类界面
    void load_fullletter_widget();//加载全屏字母分类界面
    void load_fullfunction_widget();//加载全屏功能分类界面

    void search_app_slot(QString arg);//搜索程序和文件槽函数
    void ViewOpenedSlot(QDBusMessage msg);//监控进程开启
    void directoryChangedSlot();//desktop文件目录改变信号槽
    void recv_search_result(QStringList desktopfplist);//接收搜索结果

signals:
    void send_hide_mainwindow_signal();//向MainWindow发送隐藏主窗口信号
    void view_opened_signal();//向常用软件模块发送刷新信号
    void directoryChangedSignal();//desktop文件目录改变信号
    void send_search_keyword(QString);//向SearchAppThread发送搜索关键字
    void send_querylineEdit_focusin_signal();//发送搜索框获取焦点信号
};

#endif // MAINVIEWWIDGET_H
