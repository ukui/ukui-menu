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

Style::Style()
{

}

//默认窗口
int Style::minw=0;
int Style::minh=0;
int Style::defaultMainViewWidWidth=0;
int Style::defaultTopWidHeight=0;
int Style::defaultQueryLineEditWidth=0;
int Style::defaultQueryLineEditHeight=0;
int Style::defaultQueryLineEditIconSize=0;
int Style::defaultContentWidHeight=0;
int Style::defaultSideBarWidWidth=0;
/**
  * 全屏窗口
  */
//主窗口
int Style::widthavailable=0;
int Style::heightavailable=0;
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
int Style::LeftLetterBtnHeight=0;
int Style::LeftFontSize=0;
int Style::LeftLetterFontSize=0;
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


void Style::initWidStyle()
{
    QGSettings* setting=new QGSettings(QString("org.mate.interface").toLocal8Bit());
    QString value=setting->get("font-name").toString();
    QStringList valstr=value.split(" ");
    int fontSize=valstr.at(valstr.count()-1).toInt();

    int position=0;
    int panelSize=0;
    if(QGSettings::isSchemaInstalled(QString("org.ukui.panel.settings").toLocal8Bit()))
    {
        QGSettings* gsetting=new QGSettings(QString("org.ukui.panel.settings").toLocal8Bit());
        if(gsetting->keys().contains(QString("panelposition")))
            position=gsetting->get("panelposition").toInt();
        else
            position=0;
        if(gsetting->keys().contains(QString("panelsize")))
            panelSize=gsetting->get("panelsize").toInt();
        else
            panelSize=46;
    }
    else
    {
        position=0;
        panelSize=46;
    }

    int width = getScreenGeometry("width");
    int height = getScreenGeometry("height");
    if(width==0 || height==0)
    {
        QRect rect=QApplication::desktop()->screenGeometry(0);
        width=rect.width();
        height=rect.height();
    }

    if(position==0 || position==1)
    {
        widthavailable=width;
        heightavailable=height-panelSize;
    }
    if(position==2 || position==3)
    {
        widthavailable=width-panelSize;
        heightavailable=height;
    }

    int len=0;
    QString locale = QLocale::system().name();
    if (locale == "zh_CN")
        len=0;
    else
        len=10;

    if(width==800 && height==600)
    {
        minw=320;
        minh=500;
        defaultMainViewWidWidth=264;
        defaultTopWidHeight=54;
        defaultQueryLineEditWidth=245;
        defaultQueryLineEditHeight=30;
        defaultContentWidHeight=446;
        defaultSideBarWidWidth=55;
    }
    else
    {
        minw=376;
        minh=590;
        defaultMainViewWidWidth=320;
        defaultTopWidHeight=54;
        defaultQueryLineEditWidth=288;
        defaultQueryLineEditHeight=30;
        defaultContentWidHeight=536;
        defaultSideBarWidWidth=55;
    }

    if(widthavailable>=2000 && widthavailable<=4000)
    {
        MainViewWidWidth=widthavailable-round((widthavailable-1644)/2);
        TopWidgetHeight=107;
        QueryLineEditWidth=350;
        QueryLineEditHeight=30;
        QueryLineEditFontSize=fontSize;
        QueryLineEditIconSize=16;
        LeftWidWidth=round((widthavailable-1644)/2);
        LeftWidHeight=heightavailable-TopWidgetHeight;
        LeftMargin=20;
        LeftBtnWidth=100+5*len;
        LeftBtnHeight=43;
        LeftLetterBtnHeight=25;
        RightMargin=LeftWidWidth-LeftMargin-LeftBtnWidth;
        LeftFontSize=fontSize;
        LeftLetterFontSize=14;
        LeftIconSize=19;
        LeftSpaceIconLeft=15;
        LeftSpaceIconText=14;
        LeftSpaceBetweenItem=15;
        AppListWidWidth=1644;
        AppListWidHeight=heightavailable-TopWidgetHeight;
        AppListFontSize=fontSize;
        AppListIconSize=96;
        AppListItemSizeWidth=168;
        AppListGridSizeWidth=204;
        AppLeftSpace=36;
        AppTopSpace=15;
        AppSpaceBetweenIconText=20;
//        AppBottomSpace=27;
        SliderSize=12;

        SideBarWidWidth=round((widthavailable-1644)/2);
        SideBarMargin=20;
        MinMaxWidWidth=110;
        MinMaxWidHeight=TopWidgetHeight;
        MinMaxBtnWidth=49;
        MinMaxIconSize=19;
        SideBarBtnWidth=110+2*len;
        SideBarBtnHeight=43;
        SideBarFontSize=fontSize;
        SideBarIconSize=19;
        SideBarSpaceIconLeft=14;
        SideBarSpaceIconText=10;
        SideBarSpaceBetweenItem=16;
    }
    else if (widthavailable>=1920 && widthavailable<2000) {
        MainViewWidWidth=widthavailable-round((widthavailable-1314)/2);
        TopWidgetHeight=107;
        QueryLineEditWidth=350;
        QueryLineEditHeight=30;
        QueryLineEditFontSize=fontSize;
        QueryLineEditIconSize=16;
        LeftWidWidth=round((widthavailable-1314)/2);
        LeftWidHeight=heightavailable-TopWidgetHeight;
        LeftMargin=20;
        LeftBtnWidth=100+5*len;
        LeftBtnHeight=43;
        LeftLetterBtnHeight=25;
        RightMargin=LeftWidWidth-LeftMargin-LeftBtnWidth;
        LeftFontSize=fontSize;
        LeftLetterFontSize=14;
        LeftIconSize=19;
        LeftSpaceIconLeft=15;
        LeftSpaceIconText=14;
        LeftSpaceBetweenItem=15;
        AppListWidWidth=1314;
        AppListWidHeight=heightavailable-TopWidgetHeight;
        AppListFontSize=fontSize;
        AppListIconSize=86;
        AppListItemSizeWidth=150;
        AppListGridSizeWidth=186;
        AppLeftSpace=32;
        AppTopSpace=13;
        AppSpaceBetweenIconText=18;
//        AppBottomSpace=27;
        SliderSize=12;

        SideBarWidWidth=round((widthavailable-1314)/2);
        SideBarMargin=20;
        MinMaxWidWidth=110;
        MinMaxWidHeight=TopWidgetHeight;
        MinMaxBtnWidth=49;
        MinMaxIconSize=19;
        SideBarBtnWidth=110+2*len;
        SideBarBtnHeight=43;
        SideBarFontSize=fontSize;
        SideBarIconSize=19;
        SideBarSpaceIconLeft=14;
        SideBarSpaceIconText=10;
        SideBarSpaceBetweenItem=16;
    }
    else if (widthavailable>=1600 && widthavailable<1920) {
        MainViewWidWidth=widthavailable-round((widthavailable-1090)/2);
        TopWidgetHeight=107;
        QueryLineEditWidth=350;
        QueryLineEditHeight=30;
        QueryLineEditFontSize=fontSize;
        QueryLineEditIconSize=14;
        LeftWidWidth=round((widthavailable-1090)/2);
        LeftWidHeight=heightavailable-TopWidgetHeight;
        LeftMargin=10;
        LeftBtnWidth=100+5*len;
        LeftBtnHeight=43;
        LeftLetterBtnHeight=25;
        RightMargin=LeftWidWidth-LeftMargin-LeftBtnWidth;
        LeftFontSize=fontSize;
        LeftLetterFontSize=11;
        LeftIconSize=19;
        LeftSpaceIconLeft=15;
        LeftSpaceIconText=14;
        LeftSpaceBetweenItem=10;
        AppListWidWidth=1090;
        AppListWidHeight=heightavailable-TopWidgetHeight;
        AppListFontSize=fontSize;
        AppListIconSize=86;
        AppListItemSizeWidth=150;
        AppListGridSizeWidth=180;
        AppLeftSpace=32;
        AppTopSpace=10;
        AppSpaceBetweenIconText=14;
//        AppBottomSpace=static_cast<int>(round(27*0.84));
        SliderSize=10;

        SideBarWidWidth=round((widthavailable-1090)/2);
        SideBarMargin=10;
        MinMaxWidWidth=110;
        MinMaxWidHeight=TopWidgetHeight;
        MinMaxBtnWidth=49;
        MinMaxIconSize=19;
        SideBarBtnWidth=110+2*len;
        SideBarBtnHeight=43;
        SideBarFontSize=fontSize;
        SideBarIconSize=19;
        SideBarSpaceIconLeft=14;
        SideBarSpaceIconText=10;
        SideBarSpaceBetweenItem=16;
    }
    else if (widthavailable>=1366 && widthavailable<1600) {
        MainViewWidWidth=widthavailable-round((widthavailable-850)/2);
        TopWidgetHeight=round(107*0.78);
        QueryLineEditWidth=350;
        QueryLineEditHeight=30;
        QueryLineEditFontSize=fontSize;
        QueryLineEditIconSize=14;
        LeftWidWidth=round((widthavailable-850)/2);
        LeftWidHeight=heightavailable-TopWidgetHeight;
        LeftMargin=10;
        LeftBtnWidth=100+5*len;
        LeftBtnHeight=43;
        LeftLetterBtnHeight=20;
        RightMargin=LeftWidWidth-LeftMargin-LeftBtnWidth;
        LeftFontSize=fontSize;
        LeftLetterFontSize=11;
        LeftIconSize=19;
        LeftSpaceIconLeft=15;
        LeftSpaceIconText=14;
        LeftSpaceBetweenItem=10;
        AppListWidWidth=850;
        AppListWidHeight=heightavailable-TopWidgetHeight;
        AppListFontSize=fontSize;
        AppListIconSize=64;
        AppListItemSizeWidth=112;
        AppListGridSizeWidth=140;
        AppLeftSpace=24;
        AppTopSpace=10;
        AppSpaceBetweenIconText=0;
//        AppBottomSpace=static_cast<int>(round(27*0.74));
        SliderSize=10;

        SideBarWidWidth=round((widthavailable-850)/2);
        SideBarMargin=10;
        MinMaxWidWidth=110;
        MinMaxWidHeight=TopWidgetHeight;
        MinMaxBtnWidth=49;
        MinMaxIconSize=19;
        SideBarBtnWidth=110+2*len;
        SideBarBtnHeight=43;
        SideBarFontSize=fontSize;
        SideBarIconSize=19;
        SideBarSpaceIconLeft=14;
        SideBarSpaceIconText=10;
        SideBarSpaceBetweenItem=16;
    }
    else if (widthavailable>=1280 && widthavailable<1366) {
        MainViewWidWidth=widthavailable-round((widthavailable-718)/2);
        TopWidgetHeight=round(107*0.78);
        QueryLineEditWidth=350;
        QueryLineEditHeight=30;
        QueryLineEditFontSize=fontSize;
        QueryLineEditIconSize=14;
        LeftWidWidth=round((widthavailable-718)/2);
        LeftWidHeight=heightavailable-TopWidgetHeight;
        LeftMargin=10;
        LeftBtnWidth=100+5*len;
        LeftBtnHeight=43;
        LeftLetterBtnHeight=20;
        RightMargin=LeftWidWidth-LeftMargin-LeftBtnWidth;
        LeftFontSize=fontSize;
        LeftLetterFontSize=11;
        LeftIconSize=19;
        LeftSpaceIconLeft=15;
        LeftSpaceIconText=14;
        LeftSpaceBetweenItem=10;
        AppListWidWidth=718;
        AppListWidHeight=heightavailable-TopWidgetHeight;
        AppListFontSize=fontSize;
        AppListIconSize=64;
        AppListItemSizeWidth=112;
        AppListGridSizeWidth=142;
        AppLeftSpace=24;
        AppTopSpace=10;
        AppSpaceBetweenIconText=14;
//        AppBottomSpace=static_cast<int>(round(27*0.65));
        SliderSize=8;

        SideBarWidWidth=round((widthavailable-718)/2);
        SideBarMargin=10;
        MinMaxWidWidth=110;
        MinMaxWidHeight=TopWidgetHeight;
        MinMaxBtnWidth=49;
        MinMaxIconSize=19;
        SideBarBtnWidth=110+2*len;
        SideBarBtnHeight=43;
        SideBarFontSize=fontSize;
        SideBarIconSize=19;
        SideBarSpaceIconLeft=14;
        SideBarSpaceIconText=10;
        SideBarSpaceBetweenItem=16;
    }
    else if (widthavailable>=1152 && widthavailable<1280) {
        MainViewWidWidth=widthavailable-round((widthavailable-718)/2);
        TopWidgetHeight=round(107*0.78);
        QueryLineEditWidth=350;
        QueryLineEditHeight=30;
        QueryLineEditFontSize=fontSize;
        QueryLineEditIconSize=14;
        LeftWidWidth=round((widthavailable-718)/2);
        LeftWidHeight=heightavailable-TopWidgetHeight;
        LeftMargin=10;
        LeftBtnWidth=100+5*len;
        LeftBtnHeight=43;
        LeftLetterBtnHeight=20;
        RightMargin=LeftWidWidth-LeftMargin-LeftBtnWidth;
        LeftFontSize=fontSize;
        LeftLetterFontSize=11;
        LeftIconSize=19;
        LeftSpaceIconLeft=15;
        LeftSpaceIconText=14;
        LeftSpaceBetweenItem=10;
        AppListWidWidth=718;
        AppListWidHeight=heightavailable-TopWidgetHeight;
        AppListFontSize=fontSize;
        AppListIconSize=64;
        AppListItemSizeWidth=112;
        AppListGridSizeWidth=142;
        AppLeftSpace=24;
        AppTopSpace=10;
        AppSpaceBetweenIconText=14;
//        AppBottomSpace=static_cast<int>(round(27*0.65));
        SliderSize=8;

        SideBarWidWidth=round((widthavailable-718)/2);
        SideBarMargin=10;
        MinMaxWidWidth=110;
        MinMaxWidHeight=TopWidgetHeight;
        MinMaxBtnWidth=49;
        MinMaxIconSize=19;
        SideBarBtnWidth=110+2*len;
        SideBarBtnHeight=43;
        SideBarFontSize=fontSize;
        SideBarIconSize=19;
        SideBarSpaceIconLeft=14;
        SideBarSpaceIconText=10;
        SideBarSpaceBetweenItem=16;
    }
    else if (widthavailable>800 && widthavailable<1152){
        MainViewWidWidth=widthavailable-round((widthavailable-572)/2);
        TopWidgetHeight=round(107*0.78);
        QueryLineEditWidth=350;
        QueryLineEditHeight=30;
        QueryLineEditFontSize=fontSize;
        QueryLineEditIconSize=14;
        LeftWidWidth=round((widthavailable-572)/2);
        LeftWidHeight=heightavailable-TopWidgetHeight;
        LeftMargin=10;
        LeftBtnWidth=100+5*len;
        LeftBtnHeight=43;
        LeftLetterBtnHeight=20;
        RightMargin=LeftWidWidth-LeftMargin-LeftBtnWidth;
        LeftFontSize=fontSize;
        LeftLetterFontSize=11;
        LeftIconSize=19;
        LeftSpaceIconLeft=15;
        LeftSpaceIconText=14;
        LeftSpaceBetweenItem=10;
        AppListWidWidth=572;
        AppListWidHeight=heightavailable-TopWidgetHeight;
        AppListFontSize=fontSize;
        AppListIconSize=64;
        AppListItemSizeWidth=112;
        AppListGridSizeWidth=141;
        AppLeftSpace=25;
        AppTopSpace=10;
        AppSpaceBetweenIconText=14;
//        AppBottomSpace=static_cast<int>(round(27*0.52));
        SliderSize=8;

        SideBarWidWidth=round((widthavailable-572)/2);
        SideBarMargin=10;
        MinMaxWidWidth=110;
        MinMaxWidHeight=TopWidgetHeight;
        MinMaxBtnWidth=49;
        MinMaxIconSize=19;
        SideBarBtnWidth=110+2*len;
        SideBarBtnHeight=43;
        SideBarFontSize=fontSize;
        SideBarIconSize=19;
        SideBarSpaceIconLeft=14;
        SideBarSpaceIconText=10;
        SideBarSpaceBetweenItem=16;
    }
    else{
        MainViewWidWidth=widthavailable-round((widthavailable-560)/2);
        TopWidgetHeight=round(107*0.78);
        QueryLineEditWidth=350;
        QueryLineEditHeight=30;
        QueryLineEditFontSize=fontSize;
        QueryLineEditIconSize=14;
        LeftWidWidth=round((widthavailable-560)/2);
        LeftWidHeight=heightavailable-TopWidgetHeight;
        LeftMargin=10;
        LeftBtnWidth=100+5*len;
        LeftBtnHeight=43;
        LeftLetterBtnHeight=20;
        RightMargin=LeftWidWidth-LeftMargin-LeftBtnWidth;
        LeftFontSize=fontSize;
        LeftLetterFontSize=11;
        LeftIconSize=19;
        LeftSpaceIconLeft=15;
        LeftSpaceIconText=14;
        LeftSpaceBetweenItem=10;
        AppListWidWidth=560;
        AppListWidHeight=heightavailable-TopWidgetHeight;
        AppListFontSize=fontSize;
        AppListIconSize=64;
        AppListItemSizeWidth=112;
        AppListGridSizeWidth=138;
        AppLeftSpace=25;
        AppTopSpace=10;
        AppSpaceBetweenIconText=14;
//        AppBottomSpace=static_cast<int>(round(27*0.52));
        SliderSize=8;

        SideBarWidWidth=round((widthavailable-560)/2);
        SideBarMargin=10;
        MinMaxWidWidth=110;
        MinMaxWidHeight=TopWidgetHeight;
        MinMaxBtnWidth=49;
        MinMaxIconSize=19;
        SideBarBtnWidth=110;
        SideBarBtnHeight=43;
        SideBarFontSize=fontSize;
        SideBarIconSize=19;
        SideBarSpaceIconLeft=14;
        SideBarSpaceIconText=10;
        SideBarSpaceBetweenItem=16;
    }
}
