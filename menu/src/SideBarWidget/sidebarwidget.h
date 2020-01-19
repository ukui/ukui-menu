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

#ifndef SIDEBARWIDGET_H
#define SIDEBARWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QMouseEvent>
#include <QEvent>
#include <QPropertyAnimation>
#include <QButtonGroup>
#include <ukuimenuinterface.h>
#include <QBitmap>
#include "src/Style/style.h"
#include "src/MainViewWidget/mainviewwidget.h"
#include "src/RightClickMenu/rightclickmenu.h"

namespace Ui {
class SideBarWidget;
}

class SideBarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SideBarWidget(QWidget *parent = nullptr);
    ~SideBarWidget();
    void load_min_sidebar();//加载默认侧边栏
    void load_max_sidebar();//加载全屏侧边栏
    void widget_make_zero();//MainWindow隐藏时，此界面恢复至初始状态

private:
    Ui::SideBarWidget *ui;
    UkuiMenuInterface* pUkuiMenuInterface=nullptr;

    QHBoxLayout* layout=nullptr;
    QSpacerItem* pHorizonSpacer=nullptr;
    QWidget* pMainWidget=nullptr;
    QVBoxLayout* pMainWidgetLayout=nullptr;

    //放大缩小界面
    QWidget* minmaxWidget=nullptr;
    QHBoxLayout* minmaxLayout=nullptr;
    QToolButton* minmaxbtn=nullptr;
    QSpacerItem* horizontalSpacer=nullptr;

    QButtonGroup* pBtnGroup=nullptr;
    QList<QAbstractButton*> buttonList;
    QHBoxLayout* btnlayout=nullptr;
    QLabel* labelicon;
    QPushButton* commonusebtn=nullptr;
    QLabel* commonusebtnname=nullptr;
    QPushButton* letterbtn=nullptr;
    QLabel* letterbtnname=nullptr;
    QPushButton* functionbtn=nullptr;
    QLabel* functionbtnname=nullptr;

    QPushButton* usericonbtn=nullptr;
    QLabel* usericonbtnname=nullptr;
    QPushButton* computerbtn=nullptr;
    QLabel* computerbtnname=nullptr;
    QPushButton* controlbtn=nullptr;
    QLabel* controlbtnname=nullptr;
    QPushButton* shutdownbtn=nullptr;
    QLabel* shutdownbtnname=nullptr;
    QSpacerItem* verticalSpacer=nullptr;
    QList<QAbstractButton*> otherButtonList;

    MainViewWidget* mainviewwid=nullptr;

    bool is_fullscreen=false;

    RightClickMenu* shutdownmenu;
    RightClickMenu* othermenu;

//    QPropertyAnimation* pAnimation=nullptr;
//    QPropertyAnimation* pEnterAnimation=nullptr;
//    QPropertyAnimation* pLeaveAnimation=nullptr;
    bool is_hover=false;

protected:
    void init_widget();//初始化主界面
    void add_sidebar_btn();//添加侧边栏按钮
    void set_btn_style(QPushButton *btn, QString btnicon, int num);//设置按钮样式

    void set_min_sidebar_btn(QPushButton *btn);//设置默认侧边栏按钮
    void set_max_sidebar_btn(QPushButton *btn);//设置全屏侧边栏按钮

    void add_right_click_menu(QPushButton* btn);

    void set_min_btn();//设置还原按钮
    void set_max_btn();//设置全屏按钮

//    void enterEvent(QEvent* e);
//    void leaveEvent(QEvent* e);

//    void mousePressEvent(QMouseEvent* e);

private slots:
//    bool eventFilter(QObject *watched, QEvent *event);
    void btngroup_clicked_slot(QAbstractButton *btn);
    void shutdownbtn_right_click_slot();//加载关机按钮右键菜单
    void otherbtn_right_click_slot();//加载载右键菜单
//    void animation_finished_slot();
    void computerbtn_clicked_slot();
    void controlbtn_clicked_slot();
    void shutdownbtn_clicked_slot();
    void usericonbtn_clicked_slot();
    void recv_querylineEdit_focusin_slot();//接收搜索框获取焦点槽函数
    void user_accounts_changed();

signals:
    void send_commonusebtn_signal();//发送常用分类按钮信号
    void send_letterbtn_signal();//发送字母分类按钮信号
    void send_functionbtn_signal();//发送功能分类按钮信号
    void send_fullscreen_commonusebtn_signal();//发送常用分类按钮信号
    void send_fullscreen_letterbtn_signal();//发送字母分类按钮信号
    void send_fullscreen_functionbtn_signal();//发送功能分类按钮信号
    void send_hide_mainwindow_signal();//向MainViewWidget发送隐藏主窗口信号
    void send_fullscreenbtn_signal();//发送全屏按钮点击信号
    void send_defaultbtn_signal();//发送默认(还原)按钮点击信号
    void send_hover_signal(bool is_hover);
};

#endif // SIDEBARWIDGET_H
