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
#include "src/SideBarWidget/tabviewwidget.h"
#include "src/DBus/dbus.h"
#include "src/DBus/dbus-adaptor.h"
#include <KWindowEffects>
#include <QTabWidget>
#include <QSpacerItem>
#include <QStackedWidget>
#include "src/UtilityFunction/listview.h"
#include "getmodeldata.h"
#include "fullmainwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    /**
     * @brief Set the QFrame style
     */
//    void setFrameStyle();
    /**
     * @brief Load the main window
     */
//    void loadMainWindow();

private:

    bool m_isFullScreen=false;
    UkuiMenuInterface *m_ukuiMenuInterface=nullptr;

    QPropertyAnimation *m_animation=nullptr;
    bool m_winFlag = false;
    DBus *m_dbus=nullptr;
    GetModelData *modaldata = nullptr;
    QAction *m_allAction = nullptr;
    QAction *m_letterAction = nullptr;
    QAction *m_funcAction = nullptr;
    SearchAppThread *m_searchAppThread=nullptr;
    int m_state = 0;
    FullMainWindow *fullWindow = nullptr;

protected:
    void initUi();
//    /**
//     * @brief Handle events clicking on the outside of the window
//     */
    bool event(QEvent *event);
    void paintEvent(QPaintEvent* event);
//    /**
//     * @brief The query box responds to keyboard events
//     */
    void keyPressEvent(QKeyEvent* e);
//    /**
//     * @brief Repaint window
//     */
//    void repaintWidget();
    void searchAppSlot(QString arg);
    void recvSearchResult(QVector<QStringList> arg);

//    void mouseReleaseEvent(QMouseEvent *event);

public:
Q_SIGNALS:
    void sendSearchKeyword(QString arg);

public Q_SLOTS:
    void updateCollectView();
    void on_minSelectButton_clicked();
    void updateMinAllView();
    void showNormalWindow();

private Q_SLOTS:
    void on_selectMenuButton_triggered(QAction *arg1);

    void on_collectPushButton_clicked();

    void on_recentPushButton_clicked();

    void on_cancelSearchPushButton_clicked();

    void on_searchPushButton_clicked();

    void on_minMaxChangeButton_clicked();

    void on_powerOffButton_customContextMenuRequested(const QPoint &pos);

private:

    QAction *searchIconAction;
    QAction *PowerOffAction;
    QWidget *centralwidget;
    QHBoxLayout *centerLayout_0;
    MainViewWidget *widget;
    QGridLayout *gridLayout;
    QVBoxLayout *mainLeftVerticalLayout_1;
    QStackedWidget *topStackedWidget;
    QWidget *minMenuPage;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *letfTopSelectHorizontalLayout;
    QLabel *minSelectTextLabel;
    QSpacerItem *horizontalSpacer;
    QPushButton *searchPushButton;
    QToolButton *minSelectButton;
    QToolButton *selectMenuButton;
    QWidget *minSearchPage;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *leftTopSearchHorizontalLayout;
    QLineEdit *lineEdit;
    QPushButton *cancelSearchPushButton;
    QStackedWidget *leftStackedWidget;
    QWidget *minAllPage;
    ListView *minAllListView;
    QWidget *minLetterPage;
    ListView *minLetterListView;
    QWidget *minFuncPage;
    ListView *minFuncListView;
    QWidget *minSearchResultPage;
    ListView *minSearchResultListView;
    QVBoxLayout *mainRightVerticalLayout_1;
    QVBoxLayout *mainRightVerticalLayout_2;
    QHBoxLayout *rightTopHorizontalLayout_3;
    QVBoxLayout *rightCollectLayout;
    QPushButton *collectPushButton;
    QPushButton *recentPushButton;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *minMaxChangeButton;
    QSpacerItem *verticalSpacer;
    QStackedWidget *rightStackedWidget;
    QWidget *collectPage;
    RightListView *collectListView;
    QWidget *recentPage;
    ListView *recentListView;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *rightBottomHorizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *powerOffButton;
   // Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
