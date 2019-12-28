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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QPaintEvent>
#include <QPainter>
#include <QtMath>
#include <QEvent>
#include <QSpacerItem>
#include <ukuimenuinterface.h>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include "src/SideBarWidget/sidebarwidget.h"
#include "src/MainViewWidget/mainviewwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    bool is_fullscreen=false;
    UkuiMenuInterface* pUkuiMenuInterface=nullptr;
    QFrame* line=nullptr;//垂直分割线
    //主窗口样式
    QWidget* mainwidget=nullptr;
    QHBoxLayout* mainlayout=nullptr;
    //侧边栏
    SideBarWidget* sidebarwid=nullptr;
    //主视图
    MainViewWidget* mainviewwid=nullptr;
    QPropertyAnimation* pEnterAnimation=nullptr;
    QPropertyAnimation* pLeaveAnimation=nullptr;

protected:
    void init_mainwindow();
    bool event(QEvent *event);//鼠标点击窗口外部事件
    void paintEvent(QPaintEvent*);//添加阴影

private slots:
    void show_fullscreen_widget();//加载全屏窗口
    void show_default_widget();//显示默认窗口
    void recv_hide_mainwindow_slot();//接收隐藏主窗口信号
    void recv_hover_signal_slot(bool is_hover);
};

#endif // MAINWINDOW_H
