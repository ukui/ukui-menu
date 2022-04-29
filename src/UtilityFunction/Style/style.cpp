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

#include "style.h"
#include "src/UtilityFunction/utility.h"
#include <QDebug>
#include "src/UtilityFunction/utility.h"
#include "src/UtilityFunction/utility.h"
#include <QApplication>

Style::Style()
{
}

int Style::m_primaryScreenX = 0;
int Style::m_primaryScreenY = 0;
int Style::m_panelPosition = 0;
//默认窗口
int Style::minw = 0;
int Style::minh = 0;
int Style::leftPageWidth = 0;
int Style::leftPageHeight = 0;
int Style::defaultQueryLineEditIconSize = 0;
/**
  * 全屏窗口
  */
//主窗口
int Style::m_availableScreenWidth = 0;
int Style::m_availableScreenHeight = 0;
//搜索栏
int Style::QueryLineEditHeight = 0;
//左侧栏
int Style::m_leftWidWidth = 0;
int Style::LeftMargin = 0;
int Style::LeftBtnWidth = 0;
int Style::LeftBtnHeight = 0;
int Style::LeftLetterBtnHeight = 0;
int Style::LeftLetterFontSize = 0;
int Style::LeftIconSize = 0;
int Style::LeftSpaceIconText = 0;
int Style::LeftSpaceBetweenItem = 0;
//应用列表栏
int Style::m_applistWidWidth = 0;
int Style::m_applistWidHeight = 0;
int Style::AppListIconSize = 0;
int Style::AppListBigIconSize = 0;
int Style::AppListItemSizeWidth = 0;
int Style::TabletItemSizeWidthOther = 0;
int Style::TabletItemSizeWidthFirst = 0;
int Style::FirsPageViewWidth = 0;
int Style::OtherPageViewWidth = 0;
int Style::m_applistGridSizeWidth = 0;
int Style::AppLeftSpace = 0;
int Style::AppTopSpace = 0;
int Style::AppSpaceBetweenIconText = 0;
int Style::FirstPageSpace = 0;
int Style::OtherPageSpace = 0;

//侧边栏primaryScreen
int Style::SideBarWidWidth = 0;

int Style::appLine = 0;
int Style::appColumn = 0;
int Style::appPage = 1;
int Style::appNum = 0;
int Style::nowpagenum = 1;
int Style::appRows = 0;
bool Style::ScreenRotation = false;
int Style::AppListItemSizeHeight = 270;
int Style::AppListViewTopMargin = 0;
int Style::ScreenWidth = 0;
int Style::ScreenHeight = 0;
int Style::AppListViewLeftMargin = 52;
int Style::AppListViewBottomMargin = 20;
int Style::AppListViewRightMargin = 0;
int Style::TimeWidgetTop = 81;
int Style::TimeWidgetLeft = 112;
int Style::CenterWindHeight = 0;

int Style::GroupAppColumn = 1;
int Style::GroupAppRow = 1;

int Style::ButtonWidgetWidth = 56;

int Style::ToolWidgetLeft = 24;
bool Style::IsWideScreen = false;

int Style::DropMenuWidth = 0;
int Style::DropMenuHeight = 0;
int Style::miniIconSize = 0;

void Style::initWidStyle()
{
    if (!g_projectCodeName.contains("V10SP1-edu")) {
        QVariantList list = getScreenGeometryList();
        m_primaryScreenX = list.at(0).toInt();
        m_primaryScreenY = list.at(1).toInt();
        m_availableScreenWidth = list.at(2).toInt();
        m_availableScreenHeight = list.at(3).toInt();
        m_panelPosition = list.at(5).toInt();
        int len = 0;
        QString locale = QLocale::system().name();

        if (locale == "zh_CN") {
            len = 0;
        } else {
            len = 10;
        }

        if (m_availableScreenWidth == 800 && m_availableScreenHeight == 600) {
            minw = 320;
            minh = 500;
            leftPageWidth = 300;
            leftPageHeight = 517;
        } else {
            minw = 652;
            minh = 590;
            leftPageWidth = 300;
            leftPageHeight = 517;
        }

        if (m_availableScreenWidth >= 2000 && m_availableScreenWidth <= 4000) {
            LeftMargin = 20;
            LeftLetterFontSize = 14;
            LeftSpaceBetweenItem = 15;
            AppListIconSize = 96;
            AppListItemSizeWidth = 168;
            m_applistGridSizeWidth = 240;
            AppSpaceBetweenIconText = 20;
        } else if (m_availableScreenWidth >= 1920 && m_availableScreenWidth < 2000) {
            LeftMargin = 20;
            LeftLetterFontSize = 14;
            LeftSpaceBetweenItem = 15;
            AppListIconSize = 86;
            AppListItemSizeWidth = 150;
            m_applistGridSizeWidth = 220;
            AppSpaceBetweenIconText = 18;
        } else if (m_availableScreenWidth >= 1600 && m_availableScreenWidth < 1920) {
            LeftMargin = 10;
            LeftLetterFontSize = 11;
            LeftSpaceBetweenItem = 10;
            AppListIconSize = 86;
            AppListItemSizeWidth = 150;
            m_applistGridSizeWidth = 180;
            AppSpaceBetweenIconText = 14;
        } else if (m_availableScreenWidth >= 1366 && m_availableScreenWidth < 1600) {
            LeftMargin = 10;
            LeftLetterFontSize = 11;
            LeftSpaceBetweenItem = 10;
            AppListIconSize = 64;
            AppListItemSizeWidth = 112;
            m_applistGridSizeWidth = 140;
            AppSpaceBetweenIconText = 0;
        } else if (m_availableScreenWidth >= 1280 && m_availableScreenWidth < 1366) {
            LeftMargin = 10;
            LeftLetterFontSize = 11;
            LeftSpaceBetweenItem = 10;
            AppListIconSize = 64;
            AppListItemSizeWidth = 112;
            m_applistGridSizeWidth = 142;
            AppSpaceBetweenIconText = 14;
        } else if (m_availableScreenWidth >= 1152 && m_availableScreenWidth < 1280) {
            LeftMargin = 10;
            LeftLetterFontSize = 11;
            LeftSpaceBetweenItem = 10;
            AppListIconSize = 64;
            AppListItemSizeWidth = 112;
            m_applistGridSizeWidth = 142;
            AppSpaceBetweenIconText = 14;
        } else if (m_availableScreenWidth > 800 && m_availableScreenWidth < 1152) {
            LeftMargin = 10;
            LeftLetterFontSize = 11;
            LeftSpaceBetweenItem = 10;
            AppListIconSize = 64;
            AppListItemSizeWidth = 112;
            m_applistGridSizeWidth = 141;
            AppSpaceBetweenIconText = 14;
        } else {
            LeftMargin = 10;
            LeftLetterFontSize = 11;
            LeftSpaceBetweenItem = 10;
            AppListIconSize = 64;
            AppListItemSizeWidth = 112;
            m_applistGridSizeWidth = 138;
            AppSpaceBetweenIconText = 14;
        }

        LeftBtnWidth = 68 + 5 * len;
        LeftBtnHeight = 34;
        QueryLineEditHeight = 30;
        LeftLetterBtnHeight = 25;
        LeftIconSize = 19;
        DropMenuWidth = 128;
        DropMenuHeight = 112;
        miniIconSize = 8;
        LeftSpaceIconText = 14;
        m_applistWidWidth = m_availableScreenWidth / 1.25;
        m_applistWidWidth = m_applistWidWidth - (m_applistWidWidth % m_applistGridSizeWidth) + 1;
        m_applistWidHeight = m_availableScreenHeight - 120;
        m_applistWidHeight = m_applistWidHeight - (m_applistWidHeight % m_applistGridSizeWidth) + 1;
        m_leftWidWidth = (m_availableScreenWidth - m_applistWidWidth) / 2 + 1;
    } else {
        ScreenWidth = QApplication::primaryScreen()->geometry().width();
        ScreenHeight = QApplication::primaryScreen()->geometry().height();

        if (ScreenHeight != 1080) {
            AppListViewTopMargin = 30;
        } else {
            AppListViewTopMargin = 70;
        }

        AppListViewLeftMargin = 52;
        AppListViewBottomMargin = AppListViewTopMargin - 30;
        AppListViewRightMargin = 0;
        m_leftWidWidth = ScreenWidth * 0.3;
        FirsPageViewWidth = ScreenWidth - m_leftWidWidth - 5;
        OtherPageViewWidth = ScreenWidth;
        TabletItemSizeWidthFirst = FirsPageViewWidth / 6;
        TabletItemSizeWidthOther = (ScreenWidth - 5) / 6;
        CenterWindHeight =  ScreenHeight - AppListViewBottomMargin - AppListViewTopMargin;
        AppListItemSizeHeight = CenterWindHeight / 4;
        AppListIconSize = 96;
        AppListBigIconSize = 108;
        AppTopSpace = (AppListItemSizeHeight - AppListIconSize) / 2;
        AppLeftSpace = (TabletItemSizeWidthFirst - AppListIconSize) / 2;
        TimeWidgetTop = (CenterWindHeight - 870) / 2;
        TimeWidgetLeft = (m_leftWidWidth - 400) / 2;
        FirstPageSpace = AppLeftSpace;
        OtherPageSpace = (TabletItemSizeWidthOther - AppListIconSize) / 2;
    }
}
