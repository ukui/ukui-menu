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

#ifndef FULLSEARCHRESULTWIDGET_H
#define FULLSEARCHRESULTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QDesktopWidget>
#include <QLabel>
#include <QToolButton>
#include <QScrollBar>
#include <gio/gdesktopappinfo.h>
#include "ukuimenuinterface.h"
#include "fulllistview.h"
#include "style.h"
#include "scrollarea.h"

class FullSearchResultWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FullSearchResultWidget(QWidget *parent = nullptr);
    ~FullSearchResultWidget();
    /**
     * @brief Update application list
     */
    void updateAppListView(QVector<QStringList> arg);
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

    void setViewFocus();

private:
    UkuiMenuInterface *m_ukuiMenuInterface = nullptr;
    FullListView *m_listView = nullptr;
    QStringList m_data;
    QSpacerItem *m_spaceItem = nullptr;

    ScrollArea *m_scrollArea = nullptr;
    ScrollAreaWid *m_scrollAreaWid = nullptr;
    QVBoxLayout *m_scrollAreaWidLayout = nullptr;

    QScrollBar *m_verticalScrollBar = nullptr;
    int m_scrollAreaWidHeight = 0;
    QString m_scrollBarStyle;
    QPushButton *m_powerOffButton = nullptr;

protected:
    /**
     * @brief Initializes UI
     */
    void initUi();

    void initAppListWidget();

    void fillAppList();

    void resizeScrollAreaControls();

    void initVerticalScrollBar();

    bool eventFilter(QObject *watched, QEvent *event);

public Q_SLOTS:
    void selectFirstItemTab();

    void on_setScrollBarValue(int value);

    void onSetSlider(int value);

    void on_setAreaScrollBarValue(int value);

    void on_powerOffButton_clicked();

    void on_powerOffButton_customContextMenuRequested(const QPoint &pos);

Q_SIGNALS:
    /**
     * @brief Send a hidden main window signal to the MainViewWidget
     */
    void sendHideMainWindowSignal();
    void setFocusToSideWin();
};

#endif // FULLSEARCHRESULTWIDGET_H
