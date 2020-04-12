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
#include "src/Interface/ukuimenuinterface.h"
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
    void loadMinSidebar();//加载默认侧边栏
    void loadMaxSidebar();//加载全屏侧边栏
    void widgetMakeZero();//MainWindow隐藏时，此界面恢复至初始状态

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

    bool is_hover=false;

protected:
    void initWidget();//初始化主界面
    void addSidebarBtn();//添加侧边栏按钮
    void setBtnStyle(QPushButton *btn, QString btnicon, int num);//设置按钮样式

    void setMinSidebarBtn(QPushButton *btn);//设置默认侧边栏按钮
    void setMaxSidebarBtn(QPushButton *btn);//设置全屏侧边栏按钮

    void addRightClickMenu(QPushButton* btn);

    void setMinBtn();//设置还原按钮
    void setMaxBtn();//设置全屏按钮

//    void enterEvent(QEvent* e);
//    void leaveEvent(QEvent* e);

//    void mousePressEvent(QMouseEvent* e);

private Q_SLOTS:
//    bool eventFilter(QObject *watched, QEvent *event);
    void btnGroupClickedSlot(QAbstractButton *btn);
    void shutdownBtnRightClickSlot();//加载关机按钮右键菜单
    void otherBtnRightClickSlot();//加载载右键菜单
//    void animation_finished_slot();
    void computerBtnClickedSlot();
    void controlBtnClickedSlot();
    void shutdownBtnClickedSlot();
    void userIconBtnClickedSlot();
    void userAccountsChanged();

Q_SIGNALS:
    void sendCommonUseBtnSignal();//发送常用分类按钮信号
    void sendLetterBtnSignal();//发送字母分类按钮信号
    void sendFunctionBtnSignal();//发送功能分类按钮信号
    void sendFullScreenCommonUseBtnSignal();//发送常用分类按钮信号
    void sendFullScreenLetterBtnSignal();//发送字母分类按钮信号
    void sendFullScreenFunctionBtnSignal();//发送功能分类按钮信号
    void sendHideMainWindowSignal();//向MainViewWidget发送隐藏主窗口信号
    void sendFullScreenBtnSignal();//发送全屏按钮点击信号
    void sendDefaultBtnSignal();//发送默认(还原)按钮点击信号
};

#endif // SIDEBARWIDGET_H
