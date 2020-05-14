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

#ifndef RIGHTCLICKMENU_H
#define RIGHTCLICKMENU_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QMenu>
#include <QAction>
#include <QSvgRenderer>
#include <QPainter>
#include <QWidgetAction>
#include <QLabel>
#include <QSettings>
#include <QDir>
#include <QFileInfo>
#include <QDBusInterface>
#include <QProcess>
#include <QEvent>
#include "src/Interface/ukuimenuinterface.h"
#include "src/Style/style.h"

class RightClickMenu: public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief RightClickMenu
     * @param parent
     * @param module为0表示常用应用，1表示普通应用，2表示关机，3表示其他
     */
    RightClickMenu(QWidget *parent,int module);
    virtual ~RightClickMenu();
    int showCommonUseAppBtnMenu(QString desktopfp);
    int showAppBtnMenu(QString desktopfp);
    int showShutdownMenu();
    int showOtherMenu(QString desktopfp);

private:
    QString desktopfp;
    int action_number;//记录执行的action编号
    QSettings* setting=nullptr;

    char style[300];

    QProcess* cmdProc=nullptr;

    //间隔线
    QLabel* separatorLabel=nullptr;
    QWidget* separatorWid=nullptr;
    QWidgetAction* separatorAction_1=nullptr;
    QWidgetAction* separatorAction_2=nullptr;
    QWidgetAction* separatorAction_3=nullptr;
    QWidgetAction* separatorAction_4=nullptr;
    QWidgetAction* separatorAction_5=nullptr;

    QMenu* cuappbtnmenu=nullptr;
    QWidgetAction* CuFix2CommonUseAction=nullptr;
    QWidget* CuFix2CommonUseWid=nullptr;
    QWidgetAction* CuUnfixed4CommonUseAction=nullptr;
    QWidget* CuUnfixed4CommonUseWid=nullptr;
    QWidgetAction* CuFix2TaskBarAction=nullptr;
    QWidget* CuFix2TaskBarWid=nullptr;
    QWidgetAction* CuUnfixed4TaskBarAction=nullptr;
    QWidget* CuUnfixed4TaskBarWid=nullptr;
    QWidgetAction* CuAdd2DesktopAction=nullptr;
    QWidget* CuAdd2DesktopWid=nullptr;
    QWidgetAction* CuUninstallAction=nullptr;
    QWidget* CuUninstallWid=nullptr;
    QWidgetAction* CuAttributeAction=nullptr;
    QWidget* CuAttributeWid=nullptr;

    QMenu* appbtnmenu=nullptr;
    QWidgetAction* Fix2CommonUseAction=nullptr;
    QWidget* Fix2CommonUseWid=nullptr;
    QWidgetAction* Unfixed4CommonUseAction=nullptr;
    QWidget* Unfixed4CommonUseWid=nullptr;
    QWidgetAction* Fix2TaskBarAction=nullptr;
    QWidget* Fix2TaskBarWid=nullptr;
    QWidgetAction* Unfixed4TaskBarAction=nullptr;
    QWidget* Unfixed4TaskBarWid=nullptr;
    QWidgetAction* Add2DesktopAction=nullptr;
    QWidget* Add2DesktopWid=nullptr;
    QWidgetAction* UninstallAction=nullptr;
    QWidget* UninstallWid=nullptr;
    QWidgetAction* AttributeAction=nullptr;
    QWidget* AttributeWid=nullptr;

    QMenu* shutdownmenu=nullptr;
    QWidgetAction* LockScreenAction=nullptr;
    QWidget* LockScreenWid=nullptr;
    QWidgetAction* SwitchUserAction=nullptr;
    QWidget* SwitchUserWid=nullptr;
    QWidgetAction* LogOutAction=nullptr;
    QWidget* LogOutWid=nullptr;
    QWidgetAction* RebootAction=nullptr;
    QWidget* RebootWid=nullptr;
    QWidgetAction* ShutDownAction=nullptr;
    QWidget* ShutDownWid=nullptr;

    QMenu* othermenu=nullptr;
    QWidgetAction* OtherFix2TaskBarAction=nullptr;
    QWidget* OtherFix2TaskBarWid=nullptr;
    QWidgetAction* OtherUnfix2TaskBarAction=nullptr;
    QWidget* OtherUnfix2TaskBarWid=nullptr;
    QWidgetAction* OtherListAction=nullptr;
    QWidget* OtherListWid=nullptr;

    UkuiMenuInterface* pUkuiMenuInterface=nullptr;

protected:
    void addCommonUseAppBtnAction();
    void initCommonUseAppBtnAction();
    void addAppBtnAction();
    void initAppBtnAction();
    void addShutdownAction();
    void addOtherAction();
    void initWidgetAction(QWidget* wid, QString iconstr, QString textstr,int type);

private Q_SLOTS:
    void fixToCommonUseActionTriggerSlot();
    void unfixedFromCommonUseActionTriggerSlot();
    void fixToTaskbarActionTriggerSlot();
    void unfixedFromTaskbarActionTriggerSlot();
    void addToDesktopActionTriggerSlot();
    void uninstallActionTriggerSlot();
    void attributeActionTriggerSlot();

    void lockScreenActionTriggerSlot();
    void switchUserActionTriggerSlot();
    void logoutActionTriggerSlot();
    void rebootActionTriggerSlot();
    void shutdownActionTriggerSlot();

    void otherListActionTriggerSlot();

    void onReadOutput();
};

#endif // RIGHTCLICKMENU_H
