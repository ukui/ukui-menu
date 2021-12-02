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

Style::Style()
{
}

int Style::primaryScreenX = 0;
int Style::primaryScreenY = 0;
int Style::primaryScreenWidth = 0;
int Style::primaryScreenHeight = 0;
int Style::panelPosition = 0;
int Style::panelSize = 0;
//默认窗口
int Style::minw = 0;
int Style::minh = 0;
int Style::leftPageWidth = 0;
int Style::leftPageHeight = 0;
int Style::defaultMainViewWidWidth = 0;
int Style::defaultMainViewWidHeight = 0;
int Style::defaultTopWidHeight = 0;
int Style::defaultQueryLineEditWidth = 0;
int Style::defaultQueryLineEditHeight = 0;
int Style::defaultQueryLineEditIconSize = 0;
int Style::defaultContentWidHeight = 0;
int Style::defaultSideBarWidWidth = 0;
/**
  * 全屏窗口
  */
//主窗口
int Style::widthavailable = 0;
int Style::heightavailable = 0;
//主视图
int Style::MainViewWidWidth = 0;
//搜索栏
int Style::TopWidgetHeight = 0;
int Style::QueryLineEditWidth = 0;
int Style::QueryLineEditHeight = 0;
int Style::QueryLineEditFontSize = 0;
int Style::QueryLineEditIconSize = 0;
//左侧栏
int Style::LeftWidWidth = 0;
int Style::LeftWidHeight = 0;
int Style::LeftMargin = 0;
int Style::RightMargin = 0;
int Style::LeftBtnWidth = 0;
int Style::LeftBtnHeight = 0;
int Style::LeftLetterBtnHeight = 0;
int Style::LeftFontSize = 0;
int Style::LeftLetterFontSize = 0;
int Style::LeftIconSize = 0;
int Style::LeftSpaceIconText = 0;
int Style::LeftSpaceBetweenItem = 0;
//应用列表栏
int Style::AppListWidWidth = 0;
int Style::AppListWidHeight = 0;
int Style::AppListFontSize = 0;
int Style::AppListIconSize = 0;
int Style::AppListBigIconSize = 0;
int Style::AppListItemSizeWidth = 0;
int Style::TabletItemSizeWidthOther = 0;
int Style::TabletItemSizeWidthFirst = 0;
int Style::AppListGridSizeWidth = 0;
int Style::AppLeftSpace = 0;
int Style::AppTopSpace = 0;
int Style::AppSpaceBetweenIconText = 0;
int Style::AppBottomSpace = 0;
int Style::SliderSize = 0;
int Style::FirstPageSpace = 0;
int Style::OtherPageSpace = 0;

//侧边栏primaryScreen
int Style::SideBarWidWidth = 0;
int Style::SideBarMargin = 0;
int Style::MinMaxWidWidth = 0;
int Style::MinMaxWidHeight = 0;
int Style::MinMaxBtnWidth = 0;
int Style::MinMaxIconSize = 0;
int Style::SideBarBtnWidth = 0;
int Style::SideBarBtnHeight = 0;
int Style::SideBarFontSize = 0;
int Style::SideBarIconSize = 0;
int Style::SideBarSpaceIconLeft = 0;
int Style::SideBarSpaceIconText = 0;
int Style::SideBarSpaceBetweenItem = 0;



int Style::appLine = 0;
int Style::appColumn = 0;
int Style::appPage = 1;
int Style::appNum = 0;
int Style::nowpagenum = 1;
int Style::appRows = 0;
bool Style::ScreenRotation = false;
int Style::AppListItemSizeHeight = 270;
//int Style::ScreenWidth=1920;
//int Style::ScreenHeight=1080;
int Style::AppListViewTopMargin = 0;
int Style::AppListViewLeftMargin = 52;
int Style::AppListViewBottomMargin = 20;
int Style::AppListViewRightMargin = 0;
int Style::TimeWidgetTop = 81;
int Style::TimeWidgetLeft = 112;

int Style::GroupAppColumn = 1;
int Style::GroupAppRow = 1;

int Style::ButtonWidgetWidth = 56;

int Style::ToolWidgetLeft = 24;
bool Style::IsWideScreen = false;

void Style::initWidStyle()
{
    if (projectCodeName == "V10SP1") {
        QVariantList list = getScreenGeometry();

        if (list.count() >= 6) {
            primaryScreenX = list.at(0).toInt();
            primaryScreenY = list.at(1).toInt();
            primaryScreenWidth = list.at(2).toInt();
            primaryScreenHeight = list.at(3).toInt();
            panelSize = list.at(4).toInt();
            panelPosition = list.at(5).toInt();
        }

        QGSettings *setting = new QGSettings(QString("org.mate.interface").toLocal8Bit());
        QString value = setting->get("font-name").toString();
        QStringList valstr = value.split(" ");
        int fontSize = valstr.at(valstr.count() - 1).toInt();

        if (panelPosition == 0 || panelPosition == 1) {
            widthavailable = primaryScreenWidth;
            heightavailable = primaryScreenHeight - panelSize;
        }

        if (panelPosition == 2 || panelPosition == 3) {
            widthavailable = primaryScreenWidth - panelSize;
            heightavailable = primaryScreenHeight;
        }

        int len = 0;
        QString locale = QLocale::system().name();

        if (locale == "zh_CN") {
            len = 0;
        } else {
            len = 10;
        }

        if (primaryScreenWidth == 800 && primaryScreenHeight == 600) {
            minw = 320;
            minh = 500;
            defaultMainViewWidWidth = 264;
            defaultTopWidHeight = 54;
            defaultQueryLineEditWidth = 245;
            defaultQueryLineEditHeight = 30;
            defaultContentWidHeight = 446;
            defaultSideBarWidWidth = 55;
        } else {
            minw = 652; //1
            minh = 590; //1
            defaultMainViewWidWidth = 308; //1
            defaultMainViewWidHeight = 540;//1
            leftPageWidth = 292;
            leftPageHeight = 517;
            defaultTopWidHeight = 54;
            defaultQueryLineEditWidth = 288;
            defaultQueryLineEditHeight = 30;
            defaultContentWidHeight = 536;
            defaultSideBarWidWidth = 55;
        }

        if (widthavailable >= 2000 && widthavailable <= 4000) {
            MainViewWidWidth = widthavailable - round((widthavailable - 1644) / 2);
            TopWidgetHeight = 107;
            QueryLineEditWidth = 350;
            QueryLineEditHeight = 30;
            QueryLineEditFontSize = fontSize;
            QueryLineEditIconSize = 16;
            //LeftWidWidth=round((widthavailable-1644)/2);
            LeftWidHeight = heightavailable - TopWidgetHeight;
            LeftMargin = 20;
            LeftBtnWidth = 100 + 5 * len;
            LeftBtnHeight = 43;
            LeftLetterBtnHeight = 25;
            RightMargin = LeftWidWidth - LeftMargin - LeftBtnWidth;
            LeftFontSize = fontSize;
            LeftLetterFontSize = 14;
            LeftIconSize = 19;
            LeftSpaceIconText = 14;
            LeftSpaceBetweenItem = 15;
            AppListFontSize = fontSize;
            AppListIconSize = 96;
            AppListItemSizeWidth = 168;
            AppListGridSizeWidth = 240;
            AppLeftSpace = 36;
            AppTopSpace = 15;
            AppSpaceBetweenIconText = 20;
            //        AppBottomSpace=27;
            SliderSize = 12;
            SideBarWidWidth = round((widthavailable - 1644) / 2);
            SideBarMargin = 20;
            MinMaxWidWidth = 110;
            MinMaxWidHeight = TopWidgetHeight;
            MinMaxBtnWidth = 49;
            MinMaxIconSize = 19;
            SideBarBtnWidth = 110 + 2 * len;
            SideBarBtnHeight = 43;
            SideBarFontSize = fontSize;
            SideBarIconSize = 19;
            SideBarSpaceIconLeft = 14;
            SideBarSpaceIconText = 10;
            SideBarSpaceBetweenItem = 16;
        } else if (widthavailable >= 1920 && widthavailable < 2000) {
            MainViewWidWidth = widthavailable - round((widthavailable - 1314) / 2);
            TopWidgetHeight = 107;
            QueryLineEditWidth = 350;
            QueryLineEditHeight = 30;
            QueryLineEditFontSize = fontSize;
            QueryLineEditIconSize = 16;
            //LeftWidWidth=round((widthavailable-1314)/2);
            LeftWidHeight = heightavailable - TopWidgetHeight;
            LeftMargin = 20;
            LeftBtnWidth = 100 + 5 * len;
            LeftBtnHeight = 43;
            LeftLetterBtnHeight = 25;
            RightMargin = LeftWidWidth - LeftMargin - LeftBtnWidth;
            LeftFontSize = fontSize;
            LeftLetterFontSize = 14;
            LeftIconSize = 19;
            LeftSpaceIconText = 14;
            LeftSpaceBetweenItem = 15;
            AppListFontSize = fontSize;
            AppListIconSize = 86;
            AppListItemSizeWidth = 150;
            AppListGridSizeWidth = 220;
            AppLeftSpace = 32;
            AppTopSpace = 13;
            AppSpaceBetweenIconText = 18;
            //        AppBottomSpace=27;
            SliderSize = 0;
            SideBarWidWidth = round((widthavailable - 1314) / 2);
            SideBarMargin = 20;
            MinMaxWidWidth = 110;
            MinMaxWidHeight = TopWidgetHeight;
            MinMaxBtnWidth = 49;
            MinMaxIconSize = 19;
            SideBarBtnWidth = 110 + 2 * len;
            SideBarBtnHeight = 43;
            SideBarFontSize = fontSize;
            SideBarIconSize = 19;
            SideBarSpaceIconLeft = 14;
            SideBarSpaceIconText = 10;
            SideBarSpaceBetweenItem = 16;
        } else if (widthavailable >= 1600 && widthavailable < 1920) {
            MainViewWidWidth = widthavailable - round((widthavailable - 1090) / 2);
            TopWidgetHeight = 107;
            QueryLineEditWidth = 350;
            QueryLineEditHeight = 30;
            QueryLineEditFontSize = fontSize;
            QueryLineEditIconSize = 14;
            //LeftWidWidth=round((widthavailable-1090)/2);
            LeftWidHeight = heightavailable - TopWidgetHeight;
            LeftMargin = 10;
            LeftBtnWidth = 100 + 5 * len;
            LeftBtnHeight = 43;
            LeftLetterBtnHeight = 25;
            RightMargin = LeftWidWidth - LeftMargin - LeftBtnWidth;
            LeftFontSize = fontSize;
            LeftLetterFontSize = 11;
            LeftIconSize = 19;
            LeftSpaceIconText = 14;
            LeftSpaceBetweenItem = 10;
            AppListFontSize = fontSize;
            AppListIconSize = 86;
            AppListItemSizeWidth = 150;
            AppListGridSizeWidth = 180;
            AppLeftSpace = 32;
            AppTopSpace = 10;
            AppSpaceBetweenIconText = 14;
            //        AppBottomSpace=static_cast<int>(round(27*0.84));
            SliderSize = 10;
            SideBarWidWidth = round((widthavailable - 1090) / 2);
            SideBarMargin = 10;
            MinMaxWidWidth = 110;
            MinMaxWidHeight = TopWidgetHeight;
            MinMaxBtnWidth = 49;
            MinMaxIconSize = 19;
            SideBarBtnWidth = 110 + 2 * len;
            SideBarBtnHeight = 43;
            SideBarFontSize = fontSize;
            SideBarIconSize = 19;
            SideBarSpaceIconLeft = 14;
            SideBarSpaceIconText = 10;
            SideBarSpaceBetweenItem = 16;
        } else if (widthavailable >= 1366 && widthavailable < 1600) {
            MainViewWidWidth = widthavailable - round((widthavailable - 850) / 2);
            TopWidgetHeight = round(107 * 0.78);
            QueryLineEditWidth = 350;
            QueryLineEditHeight = 30;
            QueryLineEditFontSize = fontSize;
            QueryLineEditIconSize = 14;
            //LeftWidWidth=round((widthavailable-850)/2);
            LeftWidHeight = heightavailable - TopWidgetHeight;
            LeftMargin = 10;
            LeftBtnWidth = 100 + 5 * len;
            LeftBtnHeight = 43;
            LeftLetterBtnHeight = 20;
            RightMargin = LeftWidWidth - LeftMargin - LeftBtnWidth;
            LeftFontSize = fontSize;
            LeftLetterFontSize = 11;
            LeftIconSize = 19;
            LeftSpaceIconText = 14;
            LeftSpaceBetweenItem = 10;
            AppListFontSize = fontSize;
            AppListIconSize = 64;
            AppListItemSizeWidth = 112;
            AppListGridSizeWidth = 140;
            AppLeftSpace = 24;
            AppTopSpace = 10;
            AppSpaceBetweenIconText = 0;
            //        AppBottomSpace=static_cast<int>(round(27*0.74));
            SliderSize = 10;
            SideBarWidWidth = round((widthavailable - 850) / 2);
            SideBarMargin = 10;
            MinMaxWidWidth = 110;
            MinMaxWidHeight = TopWidgetHeight;
            MinMaxBtnWidth = 49;
            MinMaxIconSize = 19;
            SideBarBtnWidth = 110 + 2 * len;
            SideBarBtnHeight = 43;
            SideBarFontSize = fontSize;
            SideBarIconSize = 19;
            SideBarSpaceIconLeft = 14;
            SideBarSpaceIconText = 10;
            SideBarSpaceBetweenItem = 16;
        } else if (widthavailable >= 1280 && widthavailable < 1366) {
            MainViewWidWidth = widthavailable - round((widthavailable - 718) / 2);
            TopWidgetHeight = round(107 * 0.78);
            QueryLineEditWidth = 350;
            QueryLineEditHeight = 30;
            QueryLineEditFontSize = fontSize;
            QueryLineEditIconSize = 14;
            //LeftWidWidth=round((widthavailable-718)/2);
            LeftWidHeight = heightavailable - TopWidgetHeight;
            LeftMargin = 10;
            LeftBtnWidth = 100 + 5 * len;
            LeftBtnHeight = 43;
            LeftLetterBtnHeight = 20;
            RightMargin = LeftWidWidth - LeftMargin - LeftBtnWidth;
            LeftFontSize = fontSize;
            LeftLetterFontSize = 11;
            LeftIconSize = 19;
            LeftSpaceIconText = 14;
            LeftSpaceBetweenItem = 10;
            AppListFontSize = fontSize;
            AppListIconSize = 64;
            AppListItemSizeWidth = 112;
            AppListGridSizeWidth = 142;
            AppLeftSpace = 24;
            AppTopSpace = 10;
            AppSpaceBetweenIconText = 14;
            //        AppBottomSpace=static_cast<int>(round(27*0.65));
            SliderSize = 8;
            SideBarWidWidth = round((widthavailable - 718) / 2);
            SideBarMargin = 10;
            MinMaxWidWidth = 110;
            MinMaxWidHeight = TopWidgetHeight;
            MinMaxBtnWidth = 49;
            MinMaxIconSize = 19;
            SideBarBtnWidth = 110 + 2 * len;
            SideBarBtnHeight = 43;
            SideBarFontSize = fontSize;
            SideBarIconSize = 19;
            SideBarSpaceIconLeft = 14;
            SideBarSpaceIconText = 10;
            SideBarSpaceBetweenItem = 16;
        } else if (widthavailable >= 1152 && widthavailable < 1280) {
            MainViewWidWidth = widthavailable - round((widthavailable - 718) / 2);
            TopWidgetHeight = round(107 * 0.78);
            QueryLineEditWidth = 350;
            QueryLineEditHeight = 30;
            QueryLineEditFontSize = fontSize;
            QueryLineEditIconSize = 14;
            //LeftWidWidth=round((widthavailable-718)/2);
            LeftWidHeight = heightavailable - TopWidgetHeight;
            LeftMargin = 10;
            LeftBtnWidth = 100 + 5 * len;
            LeftBtnHeight = 43;
            LeftLetterBtnHeight = 20;
            RightMargin = LeftWidWidth - LeftMargin - LeftBtnWidth;
            LeftFontSize = fontSize;
            LeftLetterFontSize = 11;
            LeftIconSize = 19;
            LeftSpaceIconText = 14;
            LeftSpaceBetweenItem = 10;
            AppListFontSize = fontSize;
            AppListIconSize = 64;
            AppListItemSizeWidth = 112;
            AppListGridSizeWidth = 142;
            AppLeftSpace = 24;
            AppTopSpace = 10;
            AppSpaceBetweenIconText = 14;
            //        AppBottomSpace=static_cast<int>(round(27*0.65));
            SliderSize = 8;
            SideBarWidWidth = round((widthavailable - 718) / 2);
            SideBarMargin = 10;
            MinMaxWidWidth = 110;
            MinMaxWidHeight = TopWidgetHeight;
            MinMaxBtnWidth = 49;
            MinMaxIconSize = 19;
            SideBarBtnWidth = 110 + 2 * len;
            SideBarBtnHeight = 43;
            SideBarFontSize = fontSize;
            SideBarIconSize = 19;
            SideBarSpaceIconLeft = 14;
            SideBarSpaceIconText = 10;
            SideBarSpaceBetweenItem = 16;
        } else if (widthavailable > 800 && widthavailable < 1152) {
            MainViewWidWidth = widthavailable - round((widthavailable - 572) / 2);
            TopWidgetHeight = round(107 * 0.78);
            QueryLineEditWidth = 350;
            QueryLineEditHeight = 30;
            QueryLineEditFontSize = fontSize;
            QueryLineEditIconSize = 14;
            //LeftWidWidth=round((widthavailable-572)/2);
            LeftWidHeight = heightavailable - TopWidgetHeight;
            LeftMargin = 10;
            LeftBtnWidth = 100 + 5 * len;
            LeftBtnHeight = 43;
            LeftLetterBtnHeight = 20;
            RightMargin = LeftWidWidth - LeftMargin - LeftBtnWidth;
            LeftFontSize = fontSize;
            LeftLetterFontSize = 11;
            LeftIconSize = 19;
            LeftSpaceIconText = 14;
            LeftSpaceBetweenItem = 10;
            AppListFontSize = fontSize;
            AppListIconSize = 64;
            AppListItemSizeWidth = 112;
            AppListGridSizeWidth = 141;
            AppLeftSpace = 25;
            AppTopSpace = 10;
            AppSpaceBetweenIconText = 14;
            //        AppBottomSpace=static_cast<int>(round(27*0.52));
            SliderSize = 8;
            SideBarWidWidth = round((widthavailable - 572) / 2);
            SideBarMargin = 10;
            MinMaxWidWidth = 110;
            MinMaxWidHeight = TopWidgetHeight;
            MinMaxBtnWidth = 49;
            MinMaxIconSize = 19;
            SideBarBtnWidth = 110 + 2 * len;
            SideBarBtnHeight = 43;
            SideBarFontSize = fontSize;
            SideBarIconSize = 19;
            SideBarSpaceIconLeft = 14;
            SideBarSpaceIconText = 10;
            SideBarSpaceBetweenItem = 16;
        } else {
            MainViewWidWidth = widthavailable - round((widthavailable - 560) / 2);
            TopWidgetHeight = round(107 * 0.78);
            QueryLineEditWidth = 350;
            QueryLineEditHeight = 30;
            QueryLineEditFontSize = fontSize;
            QueryLineEditIconSize = 14;
            //LeftWidWidth=round((widthavailable-560)/2);
            LeftWidHeight = heightavailable - TopWidgetHeight;
            LeftMargin = 10;
            LeftBtnWidth = 100 + 5 * len;
            LeftBtnHeight = 43;
            LeftLetterBtnHeight = 20;
            RightMargin = LeftWidWidth - LeftMargin - LeftBtnWidth;
            LeftFontSize = fontSize;
            LeftLetterFontSize = 11;
            LeftIconSize = 19;
            LeftSpaceIconText = 14;
            LeftSpaceBetweenItem = 10;
            AppListFontSize = fontSize;
            AppListIconSize = 64;
            AppListItemSizeWidth = 112;
            AppListGridSizeWidth = 138;
            AppLeftSpace = 25;
            AppTopSpace = 10;
            AppSpaceBetweenIconText = 14;
            //        AppBottomSpace=static_cast<int>(round(27*0.52));
            SliderSize = 8;
            SideBarWidWidth = round((widthavailable - 560) / 2);
            SideBarMargin = 10;
            MinMaxWidWidth = 110;
            MinMaxWidHeight = TopWidgetHeight;
            MinMaxBtnWidth = 49;
            MinMaxIconSize = 19;
            SideBarBtnWidth = 110;
            SideBarBtnHeight = 43;
            SideBarFontSize = fontSize;
            SideBarIconSize = 19;
            SideBarSpaceIconLeft = 14;
            SideBarSpaceIconText = 10;
            SideBarSpaceBetweenItem = 16;
        }

        AppListWidWidth = primaryScreenWidth / 1.25;
        AppListWidWidth = AppListWidWidth - (AppListWidWidth % AppListGridSizeWidth) + 1;
        AppListWidHeight = primaryScreenHeight - 130;
        AppListWidHeight = AppListWidHeight - (AppListWidHeight % AppListGridSizeWidth) + 1;
        LeftWidWidth = (primaryScreenWidth - AppListWidWidth) / 2 + 1;
    } else {
        AppListViewTopMargin = 0;
        AppListViewLeftMargin = 52;
        AppListViewBottomMargin = 20;
        AppListViewRightMargin = 0;
        TabletItemSizeWidthFirst = 216;
        TabletItemSizeWidthOther = 318;
        AppListItemSizeHeight = 270;
        AppTopSpace = 60;
        AppLeftSpace = 60;
        TimeWidgetTop = 81;
        TimeWidgetLeft = 112;
        ButtonWidgetWidth = 56;
        AppListIconSize = 96;
        AppListBigIconSize = 108;
        FirstPageSpace = 60;
        OtherPageSpace = 111;
    }
}
