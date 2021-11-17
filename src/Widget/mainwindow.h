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
#include "src/Widget/mainviewwidget.h"
#include "src/Widget/tabviewwidget.h"
#include "src/DBus/dbus.h"
#include "src/DBus/dbus-adaptor.h"
#include <KWindowEffects>
#include <QTabWidget>
#include <QSpacerItem>
#include <QStackedWidget>
#include "src/ListView/listview.h"
#include "src/DataProcess/getmodeldata.h"
#include "src/Widget/fullmainwindow.h"
#include "src/FileWatcher/desktopwatcher.h"
#include "functionbuttonwidget.h"
#include "letterbuttonwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void showWindow();

    void hideWindow();

protected:
    void initUi();
    /**
     * @brief 处理外部点击事件和主窗口键盘控制
     * @param event
     * @return
     */
    bool event(QEvent *event);
    /**
     * @brief 主窗口圆角绘制
     * @param event
     */
    void paintEvent(QPaintEvent *event);

    /**
     * @brief 键盘字符输入处理
     * @param e
     */
    void keyPressEvent(QKeyEvent *e);

    /**
     * @brief searchAppSlot
     * @param arg
     */
    void searchAppSlot(QString arg);

    void recvSearchResult(QVector<QStringList> arg);

    bool eventFilter(QObject *target, QEvent *event);

    //    void mouseReleaseEvent(QMouseEvent *event);

public:
Q_SIGNALS:
    void sendSearchKeyword(QString arg);
    void sendClassificationbtnList();
    void sendLetterClassificationList(QStringList list);

public Q_SLOTS:
    void updateCollectView();
    void on_minSelectButton_clicked();
    void updateMinAllView();
    void showNormalWindow();
    void updateView();
    void recvFunctionBtnSignal(QString btnname);
    void primaryScreenChangeSlot();
    void repaintWidget();
    void updateRecentView();

private Q_SLOTS:
    void on_selectMenuButton_triggered(QAction *arg1);

    void on_collectPushButton_clicked();

    void on_recentPushButton_clicked();

    void on_cancelSearchPushButton_clicked();

    void on_searchPushButton_clicked();

    void on_minMaxChangeButton_clicked();

    void on_powerOffButton_clicked();

    void on_powerOffButton_customContextMenuRequested(const QPoint &pos);

    void appClassificationBtnClickedSlot();

    void animationFinishedSLot();

private:

    QWidget *m_centralwidget = nullptr;
    QHBoxLayout *m_centerLayout = nullptr;
    MainViewWidget *m_viewWidget = nullptr;
    QGridLayout *m_gridLayout = nullptr;
    QVBoxLayout *m_mainLeftVerticalLayout = nullptr;
    QStackedWidget *m_topStackedWidget = nullptr;
    QWidget *m_minMenuPage = nullptr;
    QHBoxLayout *m_letfTopSelectHorizontalLayout = nullptr;
    QLabel *m_minSelectTextLabel = nullptr;
    QSpacerItem *m_horizontalSpacer = nullptr;
    QPushButton *m_searchPushButton = nullptr;
    QToolButton *m_minSelectButton = nullptr;
    QToolButton *m_selectMenuButton = nullptr;
    QWidget *m_minSearchPage = nullptr;
    QHBoxLayout *m_leftTopSearchHorizontalLayout = nullptr;
    QLineEdit *m_lineEdit = nullptr;
    QPushButton *m_cancelSearchPushButton = nullptr;
    QStackedWidget *m_leftStackedWidget = nullptr;
    QWidget *m_minAllPage = nullptr;
    ListView *m_minAllListView = nullptr;
    QWidget *m_minLetterPage = nullptr;
    ListView *m_minLetterListView = nullptr;
    QWidget *m_minFuncPage = nullptr;
    ListView *m_minFuncListView = nullptr;
    QWidget *m_minSearchResultPage = nullptr;
    ListView *m_minSearchResultListView = nullptr;
    QVBoxLayout *m_mainRightVerticalLayout = nullptr;
    QVBoxLayout *m_mainRightVerticalLayout_1 = nullptr;
    QHBoxLayout *m_rightTopHorizontalLayout = nullptr;
    QVBoxLayout *m_rightCollectLayout = nullptr;
    QVBoxLayout *m_rightRecentLayout = nullptr;
    QPushButton *m_collectPushButton = nullptr;
    QPushButton *m_recentPushButton = nullptr;
    QSpacerItem *m_horizontalSpacer_3 = nullptr;
    QPushButton *m_minMaxChangeButton = nullptr;
    QSpacerItem *m_verticalSpacer = nullptr;
    QStackedWidget *m_rightStackedWidget = nullptr;
    QWidget *m_collectPage = nullptr;
    RightListView *m_collectListView = nullptr;
    QWidget *m_recentPage = nullptr;
    ListView *m_recentListView = nullptr;
    QSpacerItem *m_verticalSpacer_2 = nullptr;
    QHBoxLayout *m_rightBottomHorizontalLayout = nullptr;
    QSpacerItem *m_horizontalSpacer_2 = nullptr;
    QPushButton *m_powerOffButton = nullptr;

    bool m_canHide = true;
    bool m_isFullScreen = false;
    QString m_buttonStyle;
    UkuiMenuInterface *m_ukuiMenuInterface = nullptr;
    QPropertyAnimation *m_animation = nullptr;
    bool m_winFlag = false;
    DBus *m_dbus = nullptr;
    GetModelData *m_modaldata = nullptr;
    QAction *m_allAction = nullptr;
    QAction *m_letterAction = nullptr;
    QAction *m_funcAction = nullptr;
    SearchAppThread *m_searchAppThread = nullptr;
    int m_state = 0;
    FullMainWindow *m_fullWindow = nullptr;
    DesktopWatcher *m_desktopWatcher = nullptr;
    QPropertyAnimation *m_enterAnimation = nullptr;
    QPropertyAnimation *m_leaveAnimation = nullptr;
    int m_widgetState = -1;
    FunctionButtonWidget *m_functionBtnWid = nullptr;
    LetterButtonWidget *m_letterBtnWid = nullptr;
};

#endif // MAINWINDOW_H
