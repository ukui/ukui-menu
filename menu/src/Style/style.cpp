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
#include <QDebug>

Style::Style()
{

}

/**
  * 主视图
  */
//主视图
int Style::MainViewWidWidth=0;
//搜索栏
int Style::TopWidgetHeight=0;
int Style::QueryLineEditWidth=0;
int Style::QueryLineEditHeight=0;
int Style::QueryLineEditFontSize=0;
int Style::QueryLineEditIconSize=0;
//左侧栏
int Style::LeftWidWidth=0;
int Style::LeftWidHeight=0;
int Style::LeftMargin=0;
int Style::RightMargin=0;
int Style::LeftBtnWidth=0;
int Style::LeftBtnHeight=0;
int Style::LeftFontSize=0;
int Style::LeftIconSize=0;
int Style::LeftSpaceIconLeft=0;
int Style::LeftSpaceIconText=0;
int Style::LeftSpaceBetweenItem=0;
//应用列表栏
int Style::AppListWidWidth=0;
int Style::AppListWidHeight=0;
int Style::AppListFontSize=0;
int Style::AppListIconSize=0;
int Style::AppListItemSizeWidth=0;
int Style::AppListGridSizeWidth=0;
int Style::AppLeftSpace=0;
int Style::AppTopSpace=0;
int Style::AppSpaceBetweenIconText=0;
int Style::AppBottomSpace=0;
int Style::SliderSize=0;

//侧边栏
int Style::SideBarWidWidth=0;
int Style::SideBarMargin=0;
int Style::MinMaxWidWidth=0;
int Style::MinMaxWidHeight=0;
int Style::MinMaxBtnWidth=0;
int Style::MinMaxIconSize=0;
int Style::SideBarBtnWidth=0;
int Style::SideBarBtnHeight=0;
int Style::SideBarFontSize=0;
int Style::SideBarIconSize=0;
int Style::SideBarSpaceIconLeft=0;
int Style::SideBarSpaceIconText=0;
int Style::SideBarSpaceBetweenItem=0;


void Style::init_wid_style()
{
    int width=QApplication::desktop()->width();
    int height=QApplication::desktop()->height();
    int heightavailable=QApplication::desktop()->availableGeometry().height();
    if(width>=2000 && width<=4000)
    {
        MainViewWidWidth=width-round((width-1440)/2);
        TopWidgetHeight=107;
        QueryLineEditWidth=350;
        QueryLineEditHeight=30;
        QueryLineEditFontSize=14;
        QueryLineEditIconSize=16;
        LeftWidWidth=round((width-1440)/2);
        LeftWidHeight=heightavailable-TopWidgetHeight;
        LeftBtnWidth=100;
        LeftBtnHeight=43;
        LeftMargin=20;
        RightMargin=LeftWidWidth-LeftMargin-LeftBtnWidth;
        LeftFontSize=16;
        LeftIconSize=19;
        LeftSpaceIconLeft=15;
        LeftSpaceIconText=14;
        LeftSpaceBetweenItem=15;
        AppListWidWidth=1440;
        AppListWidHeight=heightavailable-TopWidgetHeight;
        AppListFontSize=14;
        AppListIconSize=96;
        AppListItemSizeWidth=168;
        AppListGridSizeWidth=204;
        AppLeftSpace=36;
        AppTopSpace=15;
        AppSpaceBetweenIconText=20;
//        AppBottomSpace=27;
        SliderSize=12;

        SideBarWidWidth=round((width-1440)/2);
        SideBarMargin=20;
        MinMaxWidWidth=110;
        MinMaxWidHeight=TopWidgetHeight;
        MinMaxBtnWidth=49;
        MinMaxIconSize=19;
        SideBarBtnWidth=110;
        SideBarBtnHeight=43;
        SideBarFontSize=14;
        SideBarIconSize=19;
        SideBarSpaceIconLeft=14;
        SideBarSpaceIconText=9;
        SideBarSpaceBetweenItem=16;
    }
    else if (width>=1920 && width<2000) {
        MainViewWidWidth=width-round((width-1314)/2);
        TopWidgetHeight=107;
        QueryLineEditWidth=350;
        QueryLineEditHeight=30;
        QueryLineEditFontSize=14;
        QueryLineEditIconSize=16;
        LeftWidWidth=round((width-1314)/2);
        LeftWidHeight=heightavailable-TopWidgetHeight;
        LeftBtnWidth=100;
        LeftBtnHeight=43;
        LeftMargin=20;
        RightMargin=LeftWidWidth-LeftMargin-LeftBtnWidth;
        LeftFontSize=16;
        LeftIconSize=19;
        LeftSpaceIconLeft=15;
        LeftSpaceIconText=14;
        LeftSpaceBetweenItem=15;
        AppListWidWidth=1314;
        AppListWidHeight=heightavailable-TopWidgetHeight;
        AppListFontSize=14;
        AppListIconSize=86;
        AppListItemSizeWidth=150;
        AppListGridSizeWidth=186;
        AppLeftSpace=32;
        AppTopSpace=13;
        AppSpaceBetweenIconText=18;
//        AppBottomSpace=27;
        SliderSize=12;

        SideBarWidWidth=round((width-1314)/2);
        SideBarMargin=20;
        MinMaxWidWidth=110;
        MinMaxWidHeight=TopWidgetHeight;
        MinMaxBtnWidth=49;
        MinMaxIconSize=19;
        SideBarBtnWidth=110;
        SideBarBtnHeight=43;
        SideBarFontSize=14;
        SideBarIconSize=19;
        SideBarSpaceIconLeft=14;
        SideBarSpaceIconText=9;
        SideBarSpaceBetweenItem=16;
    }
    else if (width>=1600 && width<1920) {
        MainViewWidWidth=width-round((width-1090)/2);
        TopWidgetHeight=107;
        QueryLineEditWidth=260;
        QueryLineEditHeight=22;
        QueryLineEditFontSize=11;
        QueryLineEditIconSize=12;
        LeftWidWidth=round((width-1090)/2);
        LeftBtnHeight=heightavailable-TopWidgetHeight;
        LeftBtnWidth=74;
        LeftBtnHeight=32;
        LeftMargin=10;
        RightMargin=LeftWidWidth-LeftMargin-LeftBtnWidth;
        LeftFontSize=12;
        LeftIconSize=14;
        LeftSpaceIconLeft=11;
        LeftSpaceIconText=9;
        LeftSpaceBetweenItem=10;
        AppListWidWidth=1090;
        AppListWidHeight=heightavailable-TopWidgetHeight;
        AppListFontSize=12;
        AppListIconSize=86;
        AppListItemSizeWidth=150;
        AppListGridSizeWidth=180;
        AppLeftSpace=32;
        AppTopSpace=10;
        AppSpaceBetweenIconText=14;
//        AppBottomSpace=static_cast<int>(round(27*0.84));
        SliderSize=10;

        SideBarWidWidth=round((width-1090)/2);
        SideBarMargin=10;
        MinMaxWidWidth=110;
        MinMaxWidHeight=TopWidgetHeight;
        MinMaxBtnWidth=49;
        MinMaxIconSize=19;
        SideBarBtnWidth=110;
        SideBarBtnHeight=43;
        SideBarFontSize=14;
        SideBarIconSize=19;
        SideBarSpaceIconLeft=14;
        SideBarSpaceIconText=9;
        SideBarSpaceBetweenItem=16;
    }
    else if (width>=1366 && width<1600) {
        MainViewWidWidth=width-round((width-850)/2);
        TopWidgetHeight=round(107*0.78);
        QueryLineEditWidth=260;
        QueryLineEditHeight=22;
        QueryLineEditFontSize=11;
        QueryLineEditIconSize=12;
        LeftWidWidth=round((width-850)/2);
        LeftWidHeight=heightavailable-TopWidgetHeight;
        LeftMargin=10;
        LeftBtnWidth=74;
        LeftBtnHeight=32;
        RightMargin=LeftWidWidth-LeftMargin-LeftBtnWidth;
        LeftFontSize=12;
        LeftIconSize=14;
        LeftSpaceIconLeft=11;
        LeftSpaceIconText=9;
        LeftSpaceBetweenItem=10;
        AppListWidWidth=850;
        AppListWidHeight=heightavailable-TopWidgetHeight;
        AppListFontSize=12;
        AppListIconSize=64;
        AppListItemSizeWidth=112;
        AppListGridSizeWidth=140;
        AppLeftSpace=24;
        AppTopSpace=10;
        AppSpaceBetweenIconText=14;
//        AppBottomSpace=static_cast<int>(round(27*0.74));
        SliderSize=10;

        SideBarWidWidth=round((width-850)/2);
        SideBarMargin=10;
        MinMaxWidWidth=110;
        MinMaxWidHeight=TopWidgetHeight;
        MinMaxBtnWidth=49;
        MinMaxIconSize=19;
        SideBarBtnWidth=110;
        SideBarBtnHeight=43;
        SideBarFontSize=14;
        SideBarIconSize=19;
        SideBarSpaceIconLeft=14;
        SideBarSpaceIconText=9;
        SideBarSpaceBetweenItem=16;
    }
    else if (width>=1280 && width<1366) {
        MainViewWidWidth=width-round((width-718)/2);
        TopWidgetHeight=round(107*0.78);
        QueryLineEditWidth=260;
        QueryLineEditHeight=22;
        QueryLineEditFontSize=11;
        QueryLineEditIconSize=12;
        LeftWidWidth=round((width-718)/2);
        LeftWidHeight=heightavailable-TopWidgetHeight;
        LeftMargin=10;
        LeftBtnWidth=74;
        LeftBtnHeight=32;
        RightMargin=LeftWidWidth-LeftMargin-LeftBtnWidth;
        LeftFontSize=12;
        LeftIconSize=14;
        LeftSpaceIconLeft=11;
        LeftSpaceIconText=9;
        LeftSpaceBetweenItem=10;
        AppListWidWidth=718;
        AppListWidHeight=heightavailable-TopWidgetHeight;
        AppListFontSize=12;
        AppListIconSize=64;
        AppListItemSizeWidth=112;
        AppListGridSizeWidth=142;
        AppLeftSpace=24;
        AppTopSpace=10;
        AppSpaceBetweenIconText=14;
//        AppBottomSpace=static_cast<int>(round(27*0.65));
        SliderSize=8;

        SideBarWidWidth=round((width-718)/2);
        SideBarMargin=10;
        MinMaxWidWidth=110;
        MinMaxWidHeight=TopWidgetHeight;
        MinMaxBtnWidth=49;
        MinMaxIconSize=19;
        SideBarBtnWidth=110;
        SideBarBtnHeight=43;
        SideBarFontSize=14;
        SideBarIconSize=19;
        SideBarSpaceIconLeft=14;
        SideBarSpaceIconText=9;
        SideBarSpaceBetweenItem=16;
    }
    else if (width>=1152 && width<1280) {
        MainViewWidWidth=width-round((width-718)/2);
        TopWidgetHeight=round(107*0.78);
        QueryLineEditWidth=260;
        QueryLineEditHeight=22;
        QueryLineEditFontSize=11;
        QueryLineEditIconSize=12;
        LeftWidWidth=round((width-718)/2);
        LeftWidHeight=heightavailable-TopWidgetHeight;
        LeftMargin=10;
        LeftBtnWidth=74;
        LeftBtnHeight=32;
        RightMargin=LeftWidWidth-LeftMargin-LeftBtnWidth;
        LeftFontSize=12;
        LeftIconSize=14;
        LeftSpaceIconLeft=11;
        LeftSpaceIconText=9;
        LeftSpaceBetweenItem=10;
        AppListWidWidth=718;
        AppListWidHeight=heightavailable-TopWidgetHeight;
        AppListFontSize=12;
        AppListIconSize=64;
        AppListItemSizeWidth=112;
        AppListGridSizeWidth=142;
        AppLeftSpace=24;
        AppTopSpace=10;
        AppSpaceBetweenIconText=14;
//        AppBottomSpace=static_cast<int>(round(27*0.65));
        SliderSize=8;

        SideBarWidWidth=round((width-718)/2);
        SideBarMargin=10;
        MinMaxWidWidth=110;
        MinMaxWidHeight=TopWidgetHeight;
        MinMaxBtnWidth=49;
        MinMaxIconSize=19;
        SideBarBtnWidth=110;
        SideBarBtnHeight=43;
        SideBarFontSize=14;
        SideBarIconSize=19;
        SideBarSpaceIconLeft=14;
        SideBarSpaceIconText=9;
        SideBarSpaceBetweenItem=16;
    }
    else {
        MainViewWidWidth=width-round((width-572)/2);
        TopWidgetHeight=round(107*0.78);
        QueryLineEditWidth=260;
        QueryLineEditHeight=22;
        QueryLineEditFontSize=11;
        QueryLineEditIconSize=12;
        LeftWidWidth=round((width-572)/2);
        LeftWidHeight=heightavailable-TopWidgetHeight;
        LeftMargin=10;
        LeftBtnWidth=74;
        LeftBtnHeight=32;
        RightMargin=LeftWidWidth-LeftMargin-LeftBtnWidth;
        LeftFontSize=12;
        LeftIconSize=14;
        LeftSpaceIconLeft=11;
        LeftSpaceIconText=9;
        LeftSpaceBetweenItem=10;
        AppListWidWidth=572;
        AppListWidHeight=heightavailable-TopWidgetHeight;
        AppListFontSize=12;
        AppListIconSize=64;
        AppListItemSizeWidth=112;
        AppListGridSizeWidth=141;
        AppLeftSpace=25;
        AppTopSpace=10;
        AppSpaceBetweenIconText=14;
//        AppBottomSpace=static_cast<int>(round(27*0.52));
        SliderSize=8;

        SideBarWidWidth=round((width-572)/2);
        SideBarMargin=10;
        MinMaxWidWidth=110;
        MinMaxWidHeight=TopWidgetHeight;
        MinMaxBtnWidth=49;
        MinMaxIconSize=19;
        SideBarBtnWidth=110;
        SideBarBtnHeight=43;
        SideBarFontSize=14;
        SideBarIconSize=19;
        SideBarSpaceIconLeft=14;
        SideBarSpaceIconText=9;
        SideBarSpaceBetweenItem=16;
    }
}
