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

#ifndef COMMONUSEWIDGET_H
#define COMMONUSEWIDGET_H

#include <QWidget>
#include <QDBusMessage>
#include <QSettings>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QScrollBar>
#include <QLabel>
#include <QPushButton>
#include <QProcess>
#include <QSvgRenderer>
#include <QHeaderView>
#include <gio/gdesktopappinfo.h>
#include "src/Interface/ukuimenuinterface.h"
#include "src/RightClickMenu/rightclickmenu.h"
#include "src/UtilityFunction/listview.h"
#include "src/UtilityFunction/itemdelegate.h"

class CommonUseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CommonUseWidget(QWidget *parent = nullptr);
    ~CommonUseWidget();
    /**
     * @brief Initializes the interface state
     */
    void widgetMakeZero();
    /**
     * @brief Update application list
     */
    void updateListView();
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
    ListView* m_listView=nullptr;
    QVector<QStringList> m_data;

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
    void fillAppList();

public Q_SLOTS:
    /**
     * @brief Open the application
     * @param arg: The desktop file information that the application contains
     */
    void execApplication(QStringList arg);
    /**
     * @brief Update the application list slot function
     */
    void updateListViewSlot();

Q_SIGNALS:
    /**
     * @brief Send a hidden main window signal to the MainViewWidget
     */
    void sendHideMainWindowSignal();

};

#endif // COMMONUSEWIDGET_H
