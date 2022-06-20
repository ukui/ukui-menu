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
#include "ukuimenuinterface.h"
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QSettings>
#include <QPropertyAnimation>
#include <QFileInfo>
#include <QPainterPath>
#include "convert_winid_to_desktop.h"
#include "style.h"
#include "main_view_widget.h"
#include "tabview_widget.h"
#include "dbus.h"
#include "dbus-adaptor.h"
#include "software_database_update_thread.h"
#include <KWindowEffects>
#include <QTabWidget>
#include <QSpacerItem>
#include <QStackedWidget>
#include "listview.h"
#include "getmodeldata.h"
#include "full_mainwindow.h"
#include "desktop_watcher.h"
#include "function_button_widget.h"
#include "letter_button_widget.h"
#include "animationpage.h"
#include "rotationlabel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void showWindow();

    void hideWindow();

protected:
    void initSearchUi();
    void initLeftWidget();
    void initRightWidgetButton();
    void initCollectWidget();
    void initRecentWidget();
    void initTabOrder();
    void initGsettings();
    void initSignalConnect();
    void registDbusServer();
    void setTabletModeFlag();
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

    void ViewOpenedSlot(QString desktopfp);

    bool eventFilter(QObject *target, QEvent *event);

    //    void mouseReleaseEvent(QMouseEvent *event);

    void selectIconAnimation(const bool &flag);

public:
Q_SIGNALS:
    void sendSearchKeyword(QString arg);
    void sendClassificationbtnList();
    void sendLetterClassificationList(QStringList list);
    void sendSetFullWindowItemHide(bool flag);
    void sendStyleChangeSignal(QString style);

public Q_SLOTS:
    void updateCollectView();
    void on_minSelectButton_clicked();
    void updateMinAllView();
    void updateView();
    void recvFunctionBtnSignal(QString btnname);
    void primaryScreenChangeSlot();
    void repaintWidget();
    void updateRecentView();
    void updateAppCategorySlot(QString category);
    void databaseThreadCloseSlot();
    void resetLetterPage();
    void resetFunctionPage();
    void minAnimationFinished();
    void maxAnimationFinished();
    void iconAnimationFinished();
    void changeStyle();
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

    void showNormalWindowSlot();

    void tabletModeChangeSlot(bool flag);

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
    QPushButton *m_minSelectButton = nullptr;
    RotationLabel *m_selectMenuButton = nullptr;
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
    QLabel *m_collectPushButton = nullptr;
    QLabel *m_recentPushButton = nullptr;
    QSpacerItem *m_horizontalSpacer_3 = nullptr;
    QPushButton *m_minMaxChangeButton = nullptr;
    QSpacerItem *m_verticalSpacer = nullptr;
    QStackedWidget *m_rightStackedWidget = nullptr;
    QWidget *m_collectPage = nullptr;
    RightListView *m_collectListView = nullptr;
    QWidget *m_recentPage = nullptr;
    ListView *m_recentListView = nullptr;
    QLabel *m_recentTextlabel = nullptr;
    QSpacerItem *m_verticalSpacer_2 = nullptr;
    QHBoxLayout *m_rightBottomHorizontalLayout = nullptr;
    QSpacerItem *m_horizontalSpacer_2 = nullptr;
    QSpacerItem *m_horizontalSpacer_4 = nullptr;
    QPushButton *m_powerOffButton = nullptr;
    AnimationPage *m_animationPage = nullptr;
    MenuBox *m_dropDownMenu = nullptr;

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
    QPropertyAnimation *m_minAnimation = nullptr;
    QPropertyAnimation *m_maxAnimation = nullptr;
    QPropertyAnimation *iconAnimation = nullptr;
    int m_widgetState = -1;
    FunctionButtonWidget *m_functionBtnWid = nullptr;
    LetterButtonWidget *m_letterBtnWid = nullptr;
    SoftwareDatabaseUpdateThread *m_softwareDbThread = nullptr;
    QColor m_windowColor;
    bool m_isTabletMode = false;
    QDBusInterface* m_usrInterface = nullptr;
};

#endif // MAINWINDOW_H
