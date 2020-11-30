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

#ifndef MAINVIEWWIDGET_H
#define MAINVIEWWIDGET_H

#include <QWidget>
#include <QSpacerItem>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QScrollBar>
#include <QFileSystemWatcher>
#include <QSettings>
#include "src/Interface/ukuimenuinterface.h"
#include "src/CommonUseWidget/commonusewidget.h"
#include "src/LetterWidget/letterwidget.h"
#include "src/FunctionWidget/functionwidget.h"
#include "src/CommonUseWidget/fullcommonusewidget.h"
#include "src/FunctionWidget/fullfunctionwidget.h"
#include "src/LetterWidget/fullletterwidget.h"
#include "src/SearchResultWidget/fullsearchresultwidget.h"
#include "src/SearchResultWidget/searchresultwidget.h"
#include "src/SearchResultWidget/searchappthread.h"
#include "directorychangedthread.h"
#include "src/Style/style.h"

class MainViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainViewWidget(QWidget *parent = nullptr);
    ~MainViewWidget();
    /**
     * @brief Load the default main view
     */
    void loadMinMainView();
    /**
     * @brief Load the full screen main view
     */
    void loadMaxMainView();
    /**
     * @brief Initializes the interface state
     */
    void widgetMakeZero();
    /**
     * @brief Set the focus of the search box
     * @param arg: Text
     */
    void setLineEditFocus(QString arg);
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
     * @brief Resize the control
     */
    void resizeControl();
    void reloadUkuiMenu();

private:
    UkuiMenuInterface *m_ukuiMenuInterface=nullptr;
    QWidget *m_topWidget=nullptr;
    QHBoxLayout *m_topLayout=nullptr;

    QWidget *m_contentWid=nullptr;

    QLineEdit *m_queryLineEdit=nullptr;
    QWidget *m_queryWid=nullptr;
    QLabel *m_queryIcon=nullptr;
    QLabel *m_queryText=nullptr;
    bool m_isSearching;
    QString m_searchKeyWords;
    QPropertyAnimation *m_animation=nullptr;

    FullSearchResultWidget *m_fullSearchResultWid=nullptr;
    SearchResultWidget *m_searchResultWid=nullptr;
    SearchAppThread *m_searchAppThread=nullptr;
    CommonUseWidget *m_commonUseWid=nullptr;
    LetterWidget *m_letterWid=nullptr;
    FunctionWidget *m_functionWid=nullptr;
    FullCommonUseWidget *m_fullCommonUseWid=nullptr;
    FullLetterWidget *m_fullLetterWid=nullptr;
    FullFunctionWidget *m_fullFunctionWid=nullptr;

    DirectoryChangedThread *m_directoryChangedThread=nullptr;

    int m_widgetState=1;//Classification window number
    int m_saveCurrentWidState=-1;//Store the current category window number

    bool m_isFullScreen=false;
//    bool m_isHiden=false;

    QFileSystemWatcher *m_fileWatcher=nullptr;//Monitor desktop folder status

    QSettings *m_setting=nullptr;
    QGSettings *m_gsetting=nullptr;

protected:
    /**
     * @brief Initializes UI
     */
    void initUi();
    /**
     * @brief Add a top window control
     */
    void addTopControl();
    /**
     * @brief Initializes the query box
     */
    void initQueryLineEdit();
//    void paintEvent(QPaintEvent* event);
    void loadSearchResultWidget();
    void loadFullSearchResultWidget();
    void hideWidget();

public Q_SLOTS:
    /**
     * @brief Search box focus event filtering
     */
    bool eventFilter(QObject *watched, QEvent *event);
    /**
     * @brief Load the common classification interface
     */
    void loadCommonUseWidget();
    /**
     * @brief Load the letter classification interface
     */
    void loadLetterWidget();
    /**
     * @brief Load function classification interface
     */
    void loadFunctionWidget();
    /**
     * @brief Load full screen common classification interface
     */
    void loadFullCommonUseWidget();
    /**
     * @brief Load the full screen letter classification interface
     */
    void loadFullLetterWidget();
    /**
     * @brief Load the full screen function classification interface
     */
    void loadFullFunctionWidget();
    /**
     * @brief Respond to search box
     * @param arg: Search keywords
     */
    void searchAppSlot(QString arg);
    /**
     * @brief Respond to search box animation finish
     */
    void animationFinishedSlot();
    /**
     * @brief Monitor application startup
     * @param msg: Application information
     */
    void ViewOpenedSlot(QDBusMessage msg);
    /**
     * @brief Monitor desktop file directory changes
     */
    void directoryChangedSlot(const QString &path);
    /**
     * @brief Receive search results
     * @param arg: Search results
     */
    void recvSearchResult(QVector<QStringList> arg);
    /**
     * @brief Monitor icon theme changes
     * @param key: Key
     */
    void iconThemeChangeSlot(QString key);
    /**
     * @brief Request for updating the application list
     */
    void requestUpdateSlot();

Q_SIGNALS:
    /**
     * @brief Desktop file directory change signal
     */
    void directoryChangedSignal();
    /**
     * @brief Send the search keyword to the SearchAppThread
     * @param arg: Search keyword
     */
    void sendSearchKeyword(QString arg);
    /**
     * @brief Send mounting and unmounting signals to DirectoryChangedThread
     */
    void sendDirectoryPath();
    /**
     * @brief Send a hidden main window signal to the MainViewWidget
     */
    void sendHideMainWindowSignal();
};

#endif // MAINVIEWWIDGET_H
