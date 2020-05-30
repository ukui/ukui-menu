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

#ifndef FULLFUNCTIONWIDGET_H
#define FULLFUNCTIONWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QToolButton>
#include <QAbstractItemView>
#include <QSpacerItem>
#include <QLabel>
#include <QScrollBar>
#include <QPushButton>
#include <QButtonGroup>
#include <QTimer>
#include <QPropertyAnimation>
#include <gio/gdesktopappinfo.h>
#include "src/Interface/ukuimenuinterface.h"
#include "src/UtilityFunction/scrollarea.h"
#include "src/UtilityFunction/pushbutton.h"
#include "src/UtilityFunction/fulllistview.h"
#include "src/UtilityFunction/classifyscrollarea.h"
#include "src/UtilityFunction/toolbutton.h"
#include "src/UtilityFunction/functionclassifybutton.h"
#include "src/Style/style.h"

class FullFunctionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FullFunctionWidget(QWidget *parent=nullptr);
    ~FullFunctionWidget();
    /**
     * @brief Initializes the interface state
     */
    void widgetMakeZero();
    /**
     * @brief The function category button uses animation to enter
     */
    void enterAnimation();
    /**
     * @brief Repaint window
     */
    void repaintWidget();
    /**
     * @brief Move the scroll bar
     * @param type: Scroll way,Only the following parameters can be entered:
     *  0: moving up
     *  1: moving down
     */
    void moveScrollBar(int type);

private:
    /*Application list interface*/
    UkuiMenuInterface* m_ukuiMenuInterface=nullptr;
    QWidget* m_applistWid=nullptr;
    ScrollArea* m_scrollArea=nullptr;
    QWidget* m_scrollAreaWid=nullptr;
    QVBoxLayout* m_scrollAreaWidLayout=nullptr;
    QStringList m_classificationList;//Store the list of category buttons
    QStringList m_data;

    /*Function classification list interface*/
    QWidget* m_iconListWid=nullptr;
    ClassifyScrollArea* m_iconListScrollArea=nullptr;
    QWidget* m_iconListScrollAreaWid=nullptr;
    QVBoxLayout* m_iconListScrollAreaWidLayout=nullptr;
    QList<QAbstractButton*> m_buttonList;
    QButtonGroup* m_btnGroup=nullptr;

    /*Animation*/
    QPropertyAnimation* m_animation=nullptr;//Function category button animation
    QPropertyAnimation* m_scrollAnimation=nullptr;//Application list animation
    int m_beginPos=0;//Application list scrollbar starting value
    int m_endPos=0;//Application list scrollbar end value

protected:
    /**
     * @brief Initializes UI
     */
    void initUi();
    /**
     * @brief Initializes the icon list interface
     */
    void initIconListWidget();
    void initIconListScrollArea();

    /**
     * @brief Initialize the application list interface
     */
    void initAppListWidget();
    /**
     * @brief fill application list
     */
    void fillAppList();
    /**
     * @brief Insert category button
     * @param category: Functional classification name
     */
    void insertClassificationBtn(QString category);
    /**
     * @brief Insert application list
     * @param desktopfplist: Desktop file path list
     */
    void insertAppList(QStringList desktopfplist);
    /**
     * @brief Set the control size in qscrollarea
     */
    void resizeScrollAreaControls();

public Q_SLOTS:
    /**
     * @brief Open the application
     * @param arg: Desktop file path
     */
    void execApplication(QString desktopfp);
    /**
     * @brief Respond to button click
     * @param btn: QButtonGroup button
     */
    void btnGroupClickedSlot(QAbstractButton *btn);
    /**
     * @brief Update application list slot function
     */
    void updateAppListView();
    /**
     * @brief Respond to application list scrolling
     * @param value: Scrollbar current value
     */
    void valueChangedSlot(int value);
    /**
     * @brief Respond to application list animation finish
     */
    void animationFinishSlot();
    /**
     * @brief Respond to application list animation current value change
     * @param value: animation current value
     */
    void animationValueChangedSlot(const QVariant &value);

Q_SIGNALS:
    /**
     * @brief Send a hidden main window signal to the MainViewWidget
     */
    void sendHideMainWindowSignal();
};

#endif // FULLFUNCTIONWIDGET_H
