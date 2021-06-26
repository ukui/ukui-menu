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

#ifndef LETTERWIDGET_H
#define LETTERWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <gio/gdesktopappinfo.h>
#include "src/Interface/ukuimenuinterface.h"
#include "letterbuttonwidget.h"
#include "src/UtilityFunction/listview.h"
#include "src/UtilityFunction/itemdelegate.h"

class LetterWidget : public QWidget
{
    Q_OBJECT

public:
     explicit LetterWidget(QWidget *parent=nullptr);
    ~LetterWidget();
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
    /**
     * @brief Repaint window
     */
    void repaintWidget();

private:
    UkuiMenuInterface* m_ukuiMenuInterface=nullptr;
    LetterButtonWidget* m_letterBtnWid=nullptr;
    ListView* m_appListView=nullptr;
    QVector<QStringList> m_data;
    QStringList m_letterList;
    QStringList m_letterBtnRowList;

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
    void fillAppListView();

public Q_SLOTS:
    /**
     * @brief Switch to the alphabetic classification button interface
     */
    void appClassificationBtnClickedSlot();
    /**
     * @brief Receive alphabetic classification button interface signal
     * @param Category button name
     */
    void recvLetterBtnSlot(QString btnname);
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

    void setFocusToThis();

Q_SIGNALS:
    /**
     * @brief Send alphabetic classification list to the letterbuttonwidget
     * @param list: Alphabetic classification list
     */
    void sendLetterBtnList(QStringList list);
    /**
     * @brief Send a hidden main window signal to the MainViewWidget
     */
    void sendHideMainWindowSignal();
};

#endif // LETTERWIDGET_H
