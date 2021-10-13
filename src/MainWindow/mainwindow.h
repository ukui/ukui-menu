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
#include <QKeyEvent>
#include "src/Interface/ukuimenuinterface.h"
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QSettings>
#include <QPropertyAnimation>
#include <QFileInfo>
#include "src/Style/style.h"
#include "src/SideBarWidget/sidebarwidget.h"
#include "src/MainViewWidget/mainviewwidget.h"
#include "src/DBus/dbus.h"
#include "src/DBus/dbus-adaptor.h"
#include <KWindowEffects>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    /**
     * @brief Set the QFrame style
     */
//    void setFrameStyle();
    /**
     * @brief Load the main window
     */
    void loadMainWindow();
    void getCurrentCPU();

private:

    bool m_isFullScreen=false;
    UkuiMenuInterface *m_ukuiMenuInterface=nullptr;

    QFrame *m_line=nullptr;//Vertical dividing line
    QFrame *m_frame=nullptr;
    SideBarWidget *m_sideBarWid=nullptr;
    MainViewWidget *m_mainViewWid=nullptr;

    QPropertyAnimation *m_animation=nullptr;
    bool m_winFlag = false;
    bool is_repaint = true;
    DBus *m_dbus=nullptr;

protected:
    void initUi();
    /**
     * @brief Handle events clicking on the outside of the window
     */
    bool event(QEvent *event);
    void paintEvent(QPaintEvent* event);
    /**
     * @brief The query box responds to keyboard events
     */

    void repaintWidget();
    void resizeWidget();

    void mouseReleaseEvent(QMouseEvent *event);

public Q_SLOTS:
    /**
     * @brief Load the full screen window
     */
    void showFullScreenWidget();
    /**
     * @brief Load the default window
     */
    void showDefaultWidget();
    void recvHideMainWindowSlot();//接收隐藏主窗口信号
    /**
     * @brief Monitor taskbar key changes
     * @param key: Key
     */
    void panelChangedSlot(QString key);
    void animationValueChangedSlot(const QVariant &value);//动画当前值变化监控
    void animationValueFinishedSlot();
    void primaryScreenChangeSlot();
    void activeWindowSolt(bool flag);
    void mainWinShowSlot();
    void mainWinHideSlot();

public:
Q_SIGNALS:
    void setFocusSignal();
};

#endif // MAINWINDOW_H
