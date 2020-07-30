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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    /**
     * @brief Set the QFrame style
     */
    void setFrameStyle();
    /**
     * @brief Load the main window
     */
    void loadMainWindow();

private:

    bool m_isFullScreen=false;
    UkuiMenuInterface *m_ukuiMenuInterface=nullptr;
    QSettings *m_setting=nullptr;

    QFrame *m_line=nullptr;//Vertical dividing line
    QFrame *m_frame=nullptr;
    SideBarWidget *m_sideBarWid=nullptr;
    MainViewWidget *m_mainViewWid=nullptr;

    QPropertyAnimation *m_animation=nullptr;
    bool m_winFlag = false;

protected:
    void initUi();
    /**
     * @brief Handle events clicking on the outside of the window
     */
    bool event(QEvent *event);
//    void paintEvent(QPaintEvent*);
    /**
     * @brief The query box responds to keyboard events
     */
    void keyPressEvent(QKeyEvent* e);
    /**
     * @brief Repaint window
     */
    void repaintWidget();

public Q_SLOTS:
    /**
     * @brief Monitor win key events
     */
    void XkbEventsPress(const QString &keycode);
    void XkbEventsRelease(const QString &keycode);
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
     * @brief Monitor screen resolution
     * @param rect: Screen resolution
     */
    void monitorResolutionChange(QRect rect);
    /**
     * @brief Monitor primary screen changes
     * @param screen: Primary screen
     */
    void primaryScreenChangedSlot(QScreen *screen);
    /**
     * @brief Monitor taskbar key changes
     * @param key: Key
     */
    void panelChangedSlot(QString key);
    /**
     * @brief Monitor the state change of the animation
     */
    void stateChangedSlot(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);
    void animationValueChangedSlot(const QVariant &value);//动画当前值变化监控

    /**
     * @brief Open the start menu by signaling
     */
    void recvStartMenuSlot();
    /**
     * @brief Monitor win-key-release key value.
     */
    void winKeyReleaseSlot(const QString &key);
};

#endif // MAINWINDOW_H
