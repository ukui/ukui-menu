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
int Style::listleftMargin=0;//
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


    if(position==0 || position==1)
    {
        widthavailable=QApplication::primaryScreen()->geometry().width();
        heightavailable=QApplication::primaryScreen()->geometry().height()-panelSize;
    }
    if(position==2 || position==3)
    {
        widthavailable=QApplication::primaryScreen()->geometry().width()-panelSize;
        heightavailable=QApplication::primaryScreen()->geometry().height();
    }

    int len=0;
    QString locale = QLocale::system().name();
    if (locale == "zh_CN")
        len=0;
    else
        len=10;

    if(QApplication::primaryScreen()->geometry().width()==800 &&
            QApplication::primaryScreen()->geometry().height()==600)
    {
        minw=320;
        minh=500;
        defaultMainViewWidWidth=264;
        defaultTopWidHeight=54;
        defaultQueryLineEditWidth=245;
        defaultQueryLineEditHeight=50;
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
        defaultQueryLineEditHeight=50;
        defaultContentWidHeight=536;
        defaultSideBarWidWidth=55;
    }
    if(widthavailable>=2000 && widthavailable<=4000)
    {
        MainViewWidWidth=widthavailable-round((widthavailable-1644)/8);
        TopWidgetHeight=107;
        QueryLineEditWidth=350;
        QueryLineEditHeight=50;
        QueryLineEditFontSize=fontSize;
        QueryLineEditIconSize=16;
        listleftMargin=94;


        AppListWidWidth=1644;
        AppListWidHeight=heightavailable-TopWidgetHeight;
        AppListFontSize=fontSize;
        AppListIconSize=96;
        AppListItemSizeWidth=168;
        AppListGridSizeWidth=(MainViewWidWidth-2*listleftMargin+6)/6;;
        AppLeftSpace=36;
        AppTopSpace=15;
        AppSpaceBetweenIconText=20;
//        AppBottomSpace=27;
        SliderSize=12;


    }
    else if (widthavailable>=1920 && widthavailable<2000) {
        MainViewWidWidth=widthavailable-round((widthavailable-1314)/8);




        TopWidgetHeight=107;
        QueryLineEditWidth=350;
        QueryLineEditHeight=50;
        QueryLineEditFontSize=fontSize;
        QueryLineEditIconSize=16;
        LeftWidWidth=round((widthavailable-1314)/2);
        listleftMargin=94;

        AppListWidWidth=1314;
        AppListWidHeight=heightavailable-TopWidgetHeight;
        AppListFontSize=fontSize;
        AppListIconSize=86;
        AppListItemSizeWidth=150;
        AppListGridSizeWidth=(MainViewWidWidth-2*listleftMargin+6)/6;//186
        AppLeftSpace=32;
        AppTopSpace=13;
        AppSpaceBetweenIconText=18;
//        AppBottomSpace=27;
        SliderSize=12;


    }
    else if (widthavailable>=1600 && widthavailable<1920) {
        MainViewWidWidth=widthavailable-round((widthavailable-1090)/8);
        TopWidgetHeight=107;
        QueryLineEditWidth=350;
        QueryLineEditHeight=50;
        QueryLineEditFontSize=fontSize;
        QueryLineEditIconSize=14;
        listleftMargin=120;
        
        AppListWidWidth=1090;
        AppListWidHeight=heightavailable-TopWidgetHeight;
        AppListFontSize=fontSize;
        AppListIconSize=86;
        AppListItemSizeWidth=150;
        AppListGridSizeWidth=(MainViewWidWidth-2*listleftMargin+6)/6;;
        AppLeftSpace=32;
        AppTopSpace=10;
        AppSpaceBetweenIconText=14;
//        AppBottomSpace=static_cast<int>(round(27*0.84));
        SliderSize=10;


    }
    else if (widthavailable>=1366 && widthavailable<1600) {
        MainViewWidWidth=widthavailable-round((widthavailable-850)/8);
        TopWidgetHeight=round(107*0.78);
        QueryLineEditWidth=350;
        QueryLineEditHeight=50;
        QueryLineEditFontSize=fontSize;
        QueryLineEditIconSize=14;
        listleftMargin=120;


        AppListWidWidth=850;
        AppListWidHeight=heightavailable-TopWidgetHeight;
        AppListFontSize=fontSize;
        AppListIconSize=64;
        AppListItemSizeWidth=112;
        AppListGridSizeWidth=(MainViewWidWidth-2*listleftMargin+6)/6;;
        AppLeftSpace=24;
        AppTopSpace=10;
        AppSpaceBetweenIconText=0;
//        AppBottomSpace=static_cast<int>(round(27*0.74));
        SliderSize=10;


    }
    else if (widthavailable>=1280 && widthavailable<1366) {
        MainViewWidWidth=widthavailable-round((widthavailable-718)/8);
        TopWidgetHeight=round(107*0.78);
        QueryLineEditWidth=350;
        QueryLineEditHeight=50;
        QueryLineEditFontSize=fontSize;
        QueryLineEditIconSize=14;
        listleftMargin=120;

        AppListWidWidth=718;
        AppListWidHeight=heightavailable-TopWidgetHeight;
        AppListFontSize=fontSize;
        AppListIconSize=64;
        AppListItemSizeWidth=112;
        AppListGridSizeWidth=(MainViewWidWidth-2*listleftMargin+6)/6;;
        AppLeftSpace=24;
        AppTopSpace=10;
        AppSpaceBetweenIconText=14;
//        AppBottomSpace=static_cast<int>(round(27*0.65));
        SliderSize=8;


    }
    else if (widthavailable>=1152 && widthavailable<1280) {
        MainViewWidWidth=widthavailable-round((widthavailable-718)/8);
        TopWidgetHeight=round(107*0.78);
        QueryLineEditWidth=350;
        QueryLineEditHeight=50;
        QueryLineEditFontSize=fontSize;
        QueryLineEditIconSize=14;
        listleftMargin=130;

        AppListWidWidth=718;
        AppListWidHeight=heightavailable-TopWidgetHeight;
        AppListFontSize=fontSize;
        AppListIconSize=64;
        AppListItemSizeWidth=112;
        AppListGridSizeWidth=(MainViewWidWidth-2*listleftMargin+6)/6;;
        AppLeftSpace=24;
        AppTopSpace=10;
        AppSpaceBetweenIconText=14;
//        AppBottomSpace=static_cast<int>(round(27*0.65));
        SliderSize=8;


    }
    else if (widthavailable>800 && widthavailable<1152){
        MainViewWidWidth=widthavailable-round((widthavailable-572)/8);
        TopWidgetHeight=round(107*0.78);
        QueryLineEditWidth=350;
        QueryLineEditHeight=50;
        QueryLineEditFontSize=fontSize;
        QueryLineEditIconSize=14;

        listleftMargin=150;

        AppListWidWidth=572;
        AppListWidHeight=heightavailable-TopWidgetHeight;
        AppListFontSize=fontSize;
        AppListIconSize=64;
        AppListItemSizeWidth=112;
        AppListGridSizeWidth=(MainViewWidWidth-2*listleftMargin+6)/6;;
        AppLeftSpace=25;
        AppTopSpace=10;
        AppSpaceBetweenIconText=14;
//        AppBottomSpace=static_cast<int>(round(27*0.52));
        SliderSize=8;


    }
    else{
        MainViewWidWidth=widthavailable-round((widthavailable-560)/8);
        TopWidgetHeight=round(107*0.78);
        QueryLineEditWidth=350;
        QueryLineEditHeight=50;
        QueryLineEditFontSize=fontSize;
        QueryLineEditIconSize=14;
        listleftMargin=150;

        AppListWidWidth=560;
        AppListWidHeight=heightavailable-TopWidgetHeight;
        AppListFontSize=fontSize;
        AppListIconSize=64;
        AppListItemSizeWidth=112;
        AppListGridSizeWidth=(MainViewWidWidth-2*listleftMargin+6)/6;;
        AppLeftSpace=25;
        AppTopSpace=10;
        AppSpaceBetweenIconText=14;
//        AppBottomSpace=static_cast<int>(round(27*0.52));
        SliderSize=8;


    }
}

