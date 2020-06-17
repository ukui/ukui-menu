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
#include <QGSettings>
#include <QPushButton>
#include <QToolTip>
#include "src/Style/style.h"
#include "src/RightClickMenu/rightclickmenu.h"

class SideBarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SideBarWidget(QWidget *parent = nullptr);
    ~SideBarWidget();
    /**
     * @brief Load the default sidebar
     */
    void loadMinSidebar();
    /**
     * @brief Load the full-screen sidebar
     */
    void loadMaxSidebar();
    /**
     * @brief Initializes the interface state
     */
    void widgetMakeZero();
    /**
     * @brief Sidebar animation
     */
    void enterAnimation();
    /**
     * @brief Adjust the position of the sidebar buttons
     */
    void setSideBarBtnGeometry();

private:
    UkuiMenuInterface *m_ukuiMenuInterface=nullptr;

    QWidget *m_mainWidget=nullptr;
    QVBoxLayout *m_mainWidgetLayout=nullptr;

    QWidget *m_minMaxWidget=nullptr;
    QHBoxLayout *m_minMaxLayout=nullptr;
    QToolButton *m_minMaxBtn=nullptr;

    QButtonGroup *m_btnGroup=nullptr;
    QList<QAbstractButton*> m_buttonList;
    QList<QLabel*> m_buttonTextList;
    QPushButton *m_allBtn=nullptr;
    QPushButton *m_letterBtn=nullptr;
    QPushButton *m_functionBtn=nullptr;

    QPushButton *m_userIconBtn=nullptr;
    QPushButton *m_computerBtn=nullptr;
    QPushButton *m_personalBtn=nullptr;
    QPushButton *m_controlBtn=nullptr;
    QPushButton *m_trashBtn=nullptr;
    QPushButton *m_shutDownBtn=nullptr;
    QSpacerItem *m_verticalSpacer=nullptr;
    bool m_isFullScreen=false;
    QGSettings *m_gsetting=nullptr;
    RightClickMenu *m_shutDownMenu;
    RightClickMenu *m_otherMenu;
    QPropertyAnimation* m_animation=nullptr;

protected:
    /**
     * @brief Initializes UI
     */
    void initUi();
    /**
     * @brief Add a sidebar button
     */
    void addSidebarBtn();
    /**
     * @brief Set button style
     * @param btn: QPushButton
     * @param btnicon: Button icon
     * @param label: QLabel
     * @param text: Button text
     * @param num: Button Number
     */
    void initBtn(QPushButton *btn, QString btnicon,QString text,int num);
    /**
     * @brief Set the default sidebar button
     * @param btn: QPushButton
     */
    void setMinSidebarBtn(QPushButton *btn);
    /**
     * @brief Set the full-screen sidebar button
     * @param btn: QPushButton
     */
    void setMaxSidebarBtn(QPushButton *btn);
    /**
     * @brief Add right-click menu
     * @param btn: QPushButton
     */
    void addRightClickMenu(QPushButton* btn);
    /**
     * @brief Set the minimize button
     */
    void setMinBtn();
    /**
     * @brief Set the maximize button
     */
    void setMaxBtn();
    QPixmap PixmapToRound(const QPixmap &src, int radius);
//    void mousePressEvent(QMouseEvent* event);

private Q_SLOTS:
    /**
     * @brief Respond to button click
     * @param btn: QButtonGroup button
     */
    void btnGroupClickedSlot(QAbstractButton *btn);
    /**
     * @brief Load power button right-click menu
     */
    void shutdownBtnRightClickSlot();
    /**
     * @brief Load other application right-click menu
     */
    void otherBtnRightClickSlot();
    /**
     * @brief Respond to computer btn
     */
    void computerBtnClickedSlot();
    /**
     * @brief Respond to personal btn
     */
    void personalBtnClickedSlot();
    /**
     * @brief Respond to control btn
     */
    void controlBtnClickedSlot();
    /**
     * @brief Respond to trash btn
     */
    void trashBtnClickedSlot();
    /**
     * @brief Respond to power btn
     */
    void shutdownBtnClickedSlot();
    /**
     * @brief Respond to user icon btn
     */
    void userIconBtnClickedSlot();
    /**
     * @brief Monitor user account changes
     */
    void userAccountsChanged();
    /**
     * @brief Reset the sidebar button
     */
    void resetSidebarBtnSlot();

Q_SIGNALS:
    /**
     * @brief Send all category button signal
     */
    void sendCommonUseBtnSignal();
    /**
     * @brief Send the letter classification button signal
     */
    void sendLetterBtnSignal();
    /**
     * @brief Send function classification button signal
     */
    void sendFunctionBtnSignal();
    /**
     * @brief Send all category button signal of full screen
     */
    void sendFullScreenCommonUseBtnSignal();
    /**
     * @brief Send the letter classification button of full screen
     */
    void sendFullScreenLetterBtnSignal();
    /**
     * @brief Send function classification button signal of full screen
     */
    void sendFullScreenFunctionBtnSignal();
    /**
     * @brief Send a full-screen button click signal
     */
    void sendFullScreenBtnSignal();
    /**
     * @brief Send the default button click signal
     */
    void sendDefaultBtnSignal();//发送默认(还原)按钮点击信号
};

#endif // SIDEBARWIDGET_H
