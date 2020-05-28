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

#ifndef FUNCTIONWIDGET_H
#define FUNCTIONWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QScrollBar>
#include <QPushButton>
#include <gio/gdesktopappinfo.h>
#include "src/Interface/ukuimenuinterface.h"
#include "functionbuttonwidget.h"
#include "src/RightClickMenu/rightclickmenu.h"
#include "src/UtilityFunction/listview.h"
#include "src/UtilityFunction/itemdelegate.h"

class FunctionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FunctionWidget(QWidget *parent=nullptr);
    ~FunctionWidget();
    /**
     * @brief Initializes the interface state
     */
    void widgetMakeZero();
    /**
     * @brief Move the scroll bar
     * @param type: Scroll way,Only the following parameters can be entered:
     *  0: moving up
     *  1: moving down
     */
    void moveScrollBar(int type);

private:
    UkuiMenuInterface* m_ukuiMenuInterface=nullptr;
    FunctionButtonWidget* m_functionBtnWid=nullptr;//Classification list interface
    ListView* m_appListView=nullptr;
    QVector<QStringList> m_data;
    QStringList m_classificationList;//Store the list of category buttons
    QStringList m_classificationBtnRowList;//Store the row in which the category button is located
    int row=0;
    /*Interface switching animation*/
    QPropertyAnimation* m_enterAnimation=nullptr;
    QPropertyAnimation* m_leaveAnimation=nullptr;
    int m_widgetState=-1;

protected:
    /**
     * @brief Initializes UI
     */
    void initUi();
    /**
     * @brief Initialize the application list interface
     */
    void initAppListWidget();
    /**
     * @brief fill application list
     */
    void fillAppListView(int type);
    /**
     * @brief Insert category button
     * @param category: Functional classification name
     */
    void insertClassificationBtn(QString btnname);
    /**
     * @brief Insert application list
     * @param desktopfplist: Desktop file path list
     */
    void insertAppList(QStringList appnamelist);

public Q_SLOTS:
    /**
     * @brief Switch to the function classification button interface
     */
    void appClassificationBtnClickedSlot();
    /**
     * @brief Receive function classification button interface signal
     * @param Category button name
     */
    void recvFunctionBtnSignal(QString btnname);
    /**
     * @brief Open the application
     * @param arg: Desktop file path
     */
    void execApplication(QString desktopfp);
    /**
     * @brief Update application list slot function
     */
    void updateAppListView();
    /**
     * @brief Respond to the list item click
     * @param arg: The desktop file information that the application contains
     */
    void recvItemClickedSlot(QStringList arg);
    /**
     * @brief Respond to animation finish
     */
    void animationFinishedSLot();

Q_SIGNALS:
    /**
     * @brief Send the classification button click signal to functionbuttonwidget
     */
    void sendClassificationbtnList();
};

#endif // FUNCTIONWIDGET_H
