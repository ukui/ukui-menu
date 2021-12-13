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

#ifndef FULLCOMMONUSEWIDGET_H
#define FULLCOMMONUSEWIDGET_H

#include <QWidget>
#include <QSettings>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QScrollBar>
#include <QLabel>
#include <QPushButton>
#include <QDir>
#include <QProcess>
#include <QSvgRenderer>
#include <QHeaderView>
#include <gio/gdesktopappinfo.h>
#include <QSpacerItem>
#include "src/Interface/ukuimenuinterface.h"
#include "src/RightClickMenu/rightclickmenu.h"
#include "src/ListView/fulllistview.h"
#include "src/Style/style.h"
#include "src/Widget/scrollarea.h"
#include "src/DataProcess/getmodeldata.h"

class FullCommonUseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FullCommonUseWidget(QWidget *parent = nullptr);
    ~FullCommonUseWidget();
    /**
     * @brief Initializes the interface state
     */
    void widgetMakeZero();
    /**
     * @brief Update application list
     */
    void updateListView();
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

    FullListView *m_listView = nullptr;

private:
    UkuiMenuInterface *m_ukuiMenuInterface = nullptr;
    QStringList m_data;
    QSpacerItem *m_spaceItem = nullptr;
    ScrollArea *m_scrollArea = nullptr;
    ScrollAreaWid *m_scrollAreaWid = nullptr;
    QVBoxLayout *m_scrollAreaWidLayout = nullptr;
    bool flag;
    QTimer *time = nullptr;
    GetModelData *modaldata = nullptr;
    QScrollBar *m_verticalScrollBar = nullptr;
    int m_scrollAreaWidHeight = 0;
    QString m_scrollBarStyle;
    QPushButton *m_powerOffButton = nullptr;

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
    void resizeScrollAreaControls();
    void initVerticalScrollBar();

    bool eventFilter(QObject *watched, QEvent *event);

public Q_SLOTS:
    /**
     * @brief Open the application
     * @param arg: Desktop file path
     */
    void execApplication(QString desktopfp);
    /**
     * @brief Update the application list slot function
     */
    void updateListViewSlot();

    void on_setScrollBarValue(int value);

    void onSetSlider(int value);

    void selectFirstItem();

    void selectFirstItemTab();

    void on_setAreaScrollBarValue(int value);

    void on_powerOffButton_clicked();

    void on_powerOffButton_customContextMenuRequested(const QPoint &pos);

Q_SIGNALS:
    /**
     * @brief Send a hidden main window signal to the MainViewWidget
     */
    void sendHideMainWindowSignal();
    void changeScrollValue(int value, int maximumValue);
    void setFocusToSideWin();
    void sendUpdateOtherView();
};

#endif // FULLCOMMONUSEWIDGET_H
