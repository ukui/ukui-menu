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

#ifndef TABLETWINDOW_H
#define TABLETWINDOW_H

#include <QMainWindow>
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
#include "ukuimenuinterface.h"
#include "scrollarea.h"
#include "splitbar_frame.h"
#include "fulllistview.h"
#include "classify_btn_scrollarea.h"
#include "tool_button.h"
#include "function_classify_button.h"
#include "style.h"
#include <QAbstractButton>
#include "function_Widget.h"
#include "tabletlistview.h"
#include "pagemanager.h"
#include "tablet_directorychanged_thread.h"
#include <QtDBus>
//#include "dbus.h"
//#include "dbus-adaptor.h"

class TabletWindow : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.ukui.menu")

public:
    explicit TabletWindow(QWidget *parent = nullptr);

    ~TabletWindow();

    void repaintWidget();

    void showPCMenu();          //打开pc的开始菜单

    static bool cmpApp(QString &arg_1, QString &arg_2);

    void setOpacityEffect(const qreal &num);

private:
    /*Application list interface*/
    QScrollArea *m_scrollArea = nullptr;
    ScrollAreaWid *m_scrollAreaWid = nullptr;
    QHBoxLayout *m_scrollAreaWidLayout = nullptr;
    QStringList m_data;
    /*Function classification list interface*/
    QWidget *m_iconListWid = nullptr;
    ClassifyBtnScrollArea *m_iconListScrollArea = nullptr;
    ClassifyBtnScrollAreaWid *m_iconListScrollAreaWid = nullptr;
    QVBoxLayout *m_iconListWidLayout = nullptr;
    QList<QAbstractButton *> m_buttonList;
    QButtonGroup *m_btnGroup = nullptr;
    QSpacerItem *m_topSpacerItem = nullptr;
    QSpacerItem *m_bottomSpacerItem = nullptr;

    /*Animation*/
    QPropertyAnimation *m_scrollAnimation = nullptr;//Application list animation
    int m_beginPos = 0;//Application list scrollbar starting value
    int m_endPos = 0;//Application list scrollbar end value
    int m_index = 0;
    int m_scrollAreaWidHeight = 0;

    int m_width;
    int m_height;

    QString m_bgPath;
    QString m_bgOption;
    QPixmap m_pixmap;

    QPixmap *m_backPixmap = nullptr;
    QGSettings *m_bgSetting = nullptr;
    QGSettings *m_bgEffect = nullptr;

    QDBusInterface *m_usrInterface = nullptr;
    bool m_winFlag = false;
    int m_curPageNum = 0;
    QVBoxLayout *m_layout = nullptr;
    FunctionWidget *m_leftWidget = nullptr;
    bool m_isFirstPage = true;
    QWidget *m_firstPageWidget = nullptr;
    static QVector<QString> m_keyVector;
    static QVector<int> m_keyValueVector;
    QSettings *m_setting = nullptr;
    PageManager *m_pagemanager = nullptr;
    TabletListView *m_listView = nullptr;
    QHBoxLayout *m_firstPageLayout = nullptr;
    QHBoxLayout *m_buttonBoxLayout = nullptr;
    QButtonGroup *m_buttonGroup = nullptr;
    QWidget *m_buttonWidget = nullptr;
    QPushButton *m_pageButton = nullptr;

    QFileSystemWatcher *m_appFileWatcher = nullptr;
    QFileSystemWatcher *m_appListFileWatcher = nullptr;
    QFileSystemWatcher *m_configFileWatcher = nullptr;
    TabletDirectoryChangedThread *m_directoryChangedThread = nullptr;
    QSettings *m_disableAppSet = nullptr;

    QLabel m_animationPage;
//    DBus *m_dbus = nullptr;

protected:
    /**
     * @brief Initializes UI
     */
    void initUi();
    /**
     * @brief initSize
     */
    void initSize();

    void fileWatcher();

    void setBackground();

    void registDbusService();

    void initTransparency();

    void initStatusManager();

    void initXEventMonitor();
    /**
     * @brief Initialize the application list interface
     */
    void initAppListWidget();
    /**
     * @brief fill application list
     */
    void fillAppList();
    /**
     * @brief Insert application list
     * @param desktopfplist: Desktop file path list
     */
    void insertAppList(QStringList desktopfplist);

    void paintEvent(QPaintEvent *event);

    void backgroundPic();

    QPixmap *blurPixmap(QPixmap *pixmap);

    void ways();

    QPixmap getPaddingPixmap(QPixmap pixmap, int width, int height);

    QImage applyEffectToImage(QImage src, QGraphicsEffect *effect, int extent = 0);

    void buttonWidgetShow();

    bool event(QEvent *event);

    void wheelEvent(QWheelEvent *e);

    bool eventFilter(QObject *target, QEvent *event);

    bool checkapplist();

    void menuStatus();

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
    void btnGroupClickedSlot(int prePageNum, int pageNum);

    void reloadAppList();

    /**
     * @brief Respond to application list animation finish
     */
    void animationFinishSlot();
    /**
     * @brief Respond to application list animation current value change
     * @param value: animation current value
     */
    void animationValueChangedSlot(const QVariant &value);

    void recvHideMainWindowSlot();

//    void recvStartMenuSlot();

    void modelChanged(bool value);

    void xkbEventsPress(const QString &keycode);
    void xkbEventsRelease(const QString &keycode);
    void winKeyReleaseSlot(const QString &key);
    void on_pageNumberChanged(bool nextPage);
    void buttonClicked(QAbstractButton *m_pageButton);
    void pageNumberChanged(int pageNum);
    void requestDeleteAppSlot();
    void requestUpdateSlot(QString desktopfp);
    void directoryChangedSlot();
    void reloadWidget();
Q_SIGNALS:
    /**
     * @brief Send a hidden main window signal to the MainViewWidget
     */
    void sendHideMainWindowSignal();

    void setFocusToSideWin();

    void selectFirstItem();

    void changeScrollValue(int value, int maximumValue);

    void pagenumchanged(int pageNum); //翻页信号

    void sendDirectoryPath(QString arg);

    void UpdateSignal();

    void menuChangeStatus(bool flag);
};

#endif // FULLFUNCTIONWIDGET_H
