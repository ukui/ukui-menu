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
#include "src/Interface/ukuimenuinterface.h"
#include "src/Widget/scrollarea.h"
#include "src/Widget/splitbarframe.h"
#include "src/ListView/fulllistview.h"
#include "src/Widget/classifybtnscrollarea.h"
#include "src/Widget/toolbutton.h"
#include "src/Widget/functionclassifybutton.h"
#include "src/Style/style.h"
#include <QAbstractButton>
#include "src/DataProcess/getmodeldata.h"
#include "src/tablet/UtilityFunction/timewidget.h"
#include "src/ListView/tabletlistview.h"
#include "src/tablet/pagemanager.h"

class TabletWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TabletWindow(QWidget *parent=nullptr);
    ~TabletWindow();
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
    /**
     * @brief Adjust function classification button position
     */
    void setFunctionBtnGeometry();

    void functionButtonClick();

    QAbstractButton* getCurLetterButton(int value);

    void showPCMenu();          //打开pc的开始菜单

    static bool cmpApp(QString &arg_1, QString &arg_2);
    void setOpacityEffect(const qreal& num);

private:
    /*Application list interface*/
    UkuiMenuInterface* m_ukuiMenuInterface=nullptr;
    QWidget* m_applistWid=nullptr;
    ScrollArea* m_scrollArea=nullptr;
    ScrollAreaWid* m_scrollAreaWid=nullptr;
    QHBoxLayout* m_scrollAreaWidLayout=nullptr;
    QStringList m_classificationList;//Store the list of category buttons
    QStringList m_data;
    QTimer *time = nullptr;
    bool flag;
    /*Function classification list interface*/
    QWidget* m_iconListWid=nullptr;
    ClassifyBtnScrollArea* m_iconListScrollArea=nullptr;
    ClassifyBtnScrollAreaWid* m_iconListScrollAreaWid=nullptr;
    QVBoxLayout* m_iconListWidLayout=nullptr;
    QList<QAbstractButton*> m_buttonList;
    QButtonGroup* m_btnGroup=nullptr;
    QSpacerItem* m_topSpacerItem=nullptr;
    QSpacerItem* m_bottomSpacerItem=nullptr;
    QSpacerItem* m_appListBottomSpacer=nullptr;

    /*Animation*/
    QPropertyAnimation* m_animation=nullptr;//Function category button animation
    QPropertyAnimation* m_scrollAnimation=nullptr;//Application list animation
    int m_beginPos= 0;//Application list scrollbar starting value
    int m_endPos= 0;//Application list scrollbar end value
    int m_index = 0;
    QScrollBar *verticalScrollBar=nullptr;
    int m_scrollAreaWidHeight = 0;

    int m_width;
    int m_height;

    QString bgPath;
    QString bgOption;
    QPixmap pixmap1;

    QPoint  pressedpos;
    QPoint  releasepos;

    QPixmap *pixmap=nullptr;
    QGSettings* bg_setting = nullptr;
    QGSettings* bg_effect = nullptr;

    QDBusInterface *usrInterface = nullptr;
    bool m_winFlag = false;
    int i = 0;
    QHBoxLayout* layout = nullptr;
    TimeWidget* leftWidget = nullptr;
    bool isFirstPage = true;
    QWidget *firstPageWidget = nullptr;
    static QVector<QString> keyVector;
    static QVector<int> keyValueVector;
    QSettings* setting;
    TabletListView* m_listView=nullptr;
    QHBoxLayout *firstPageLayout = nullptr;


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

    bool eventFilter(QObject *watched, QEvent *event);

    void paintEvent(QPaintEvent *event);

    void backgroundPic();

    QPixmap *blurPixmap(QPixmap *pixmap);

    void reloadAppList();

    void ways();

    QPixmap getPaddingPixmap(QPixmap pixmap, int width, int height);

    QImage applyEffectToImage(QImage src, QGraphicsEffect *effect, int extent = 0);

    PageManager *m_pagemanager = nullptr;

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
    void btnGroupClickedSlot(int pageNum);
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

    void onSetSlider(int);

    void setFocusToThis();

    void on_setScrollBarValue(int value);

    void on_setAreaScrollBarValue(int value);

    void recvHideMainWindowSlot();

    void recvStartMenuSlot();

    void modelChanged(bool value);

    void XkbEventsPress(const QString &keycode);
    void XkbEventsRelease(const QString &keycode);
    void winKeyReleaseSlot(const QString &key);
    void on_pageNumberChanged(bool nextPage);

Q_SIGNALS:
    /**
     * @brief Send a hidden main window signal to the MainViewWidget
     */
    void sendHideMainWindowSignal();

    void setFocusToSideWin();

    void selectFirstItem();

    void changeScrollValue(int value, int maximumValue);
};

#endif // FULLFUNCTIONWIDGET_H
