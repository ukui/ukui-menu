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

#ifndef STYLE_H
#define STYLE_H
#include <QDesktopWidget>
#include <QApplication>
#include <math.h>
#include <QDBusConnection>
#include <QDBusReply>
#include <QDBusInterface>
#include <qgsettings.h>
#include <QScreen>
#include <QFileInfo>

//颜色
#define BORDERCOLOR "rgba(37,41,43)" //边框
#define SHADOWCOLOR "#80000000" //阴影

#define UserIconBackground "#ffffff"
#define UserIconOpacity 0.20
#define RightClickMenuBackground "rgba(12, 12, 13, 0.7)"//右键菜单背景
#define RightClickMenuSeparator "rgba(255, 255, 255, 0.08)"//右键菜单分割线
#define RightClickMenuHover "rgba(255, 255, 255, 0.12)"//右键菜单悬浮
#define RightClickMenuBorder "rgba(255, 255, 255, 0.2)"//右键菜单边框
#define RightClickMenuFont "rgba(255, 255, 255, 0.91)"
#define RightClickMenuOpacity 0.95
#define ToolTipBackground "rgba(26, 26, 26, 0.7)"
#define DefaultBackground "rgba(19, 19, 20, 0.7)" //默认态背景色
#define LineBackground "rgba(255,255,255,0.06)" //分割线背景色
#define SBClassifyBtnSelectedBackground "#3D6BE5" //侧边栏上部分类按钮选择背景
#define SBFunBtnHoverBackground "rgba(255, 255, 255, 0.14)" //侧边栏目下部功能按钮悬浮背景
#define ClassifyBtnHoverBackground "rgba(255, 255, 255, 0.14)" //分类按钮悬浮背景
#define MMBtnHoverBackground "rgba(255, 255, 255, 0.14)" //最大化最小化按钮悬浮背景
#define QueryLineEditBackground "rgba(0, 0, 0,0.4)" //搜索框背景
#define QueryLineEditClickedBackground "rgba(0, 0, 0,0.5)" //搜索框背景选中
#define QueryLineEditClickedBorder "rgba(61, 107, 229, 1)" //搜索框背景选中边框
#define AppBtnHover "#ffffff" //按钮悬浮

/**
 * 大小(全屏)
 */
////左侧栏
//#define LeftWidSize leftsize //大小
//#define LeftMarginLeftSize Leftmarginleftsize //内左侧边距
//#define LeftFontSize leftfontsize //字体大小
//#define LeftIconSize lefticonsize //图标大小
//#define LeftSpaceIconLeft leftspaceiconleft //按钮内左侧边距
//#define LeftSpaceIconText leftspaceicontext //按钮图标与文字内边距
//#define LeftSpaceBetweenItem leftspacebetweenitem //item间距

////应用列表栏
//#define AppListWidSize applistwidsize //大小
//#define AppListFontSize applistfontsize //字体大小
//#define AppListIconSize applisticonsize //图标大小
//#define AppListSpaceBetweenItem applistspacebetweenitem //item间距
//#define SliderSize slidersize //滑块大小

////右侧栏
//#define RightWidSize rightsize //大小
//#define RightMarginLeftSize rightmarginleftsize //内右侧边距
//#define RightFontSize rightfontsize //字体大小
//#define RightIconSize righticonsize //图标大小
//#define RightSpaceIconLeft rightspaceiconleft //按钮内左侧边距
//#define RightSpaceIconText rightspaceicontext //按钮图标与文字内边距
//#define RightSpaceBetweenItem rightspacebetweenitem //item间距

class Style
{
public:
    Style();
    static void initWidStyle();

    //主窗口
    static int widthavailable;
    static int heightavailable;
    //主视图
    static int MainViewWidWidth;//宽度
    //搜索栏
    static int TopWidgetHeight;//高度
    static int QueryLineEditWidth;//搜索框宽度
    static int QueryLineEditHeight;//搜索框高度
    static int QueryLineEditIconSize;//搜索框图标
    static int QueryLineEditFontSize;//搜索框字体大小
    //左侧栏
    static int LeftWidWidth; //宽度
    static int LeftWidHeight; //高度
    static int LeftMargin; //内左侧边距
    static int RightMargin;//内右侧边距
    static int LeftBtnWidth;//按钮宽度
    static int LeftBtnHeight;//按钮高度
    static int LeftLetterBtnHeight;//按钮高度
    static int LeftFontSize; //字体大小
    static int LeftLetterFontSize; //字体大小
    static int LeftIconSize; //图标大小
    static int LeftSpaceIconLeft; //按钮内左侧边距
    static int LeftSpaceIconText; //按钮图标与文字内边距
    static int LeftSpaceBetweenItem; //item间距
    //应用列表栏
    static int AppListWidWidth; //宽度
    static int AppListWidHeight; //高度
    static int AppListFontSize; //字体大小
    static int AppListIconSize; //图标大小
    static int AppListItemSizeWidth; //item大小
    static int AppListGridSizeWidth; //网格大小
    static int AppLeftSpace;//左侧边距
    static int AppTopSpace;//上侧边距
    static int AppSpaceBetweenIconText;//图标与文字边距
    static int AppBottomSpace;//下侧边距
    static int SliderSize; //滑块大小

    /**
     * 侧边栏
     */
    static int SideBarWidWidth; //宽度
    static int SideBarMargin; //内右侧边距
    static int MinMaxWidWidth; //放大缩小界面宽度
    static int MinMaxWidHeight; //放大缩小界面高度
    static int MinMaxBtnWidth; //放大缩小按钮宽度
    static int MinMaxIconSize; //放大缩小按钮大小
    static int SideBarBtnWidth;//按钮宽度
    static int SideBarBtnHeight;//按钮高度
    static int SideBarFontSize; //字体大小
    static int SideBarIconSize; //图标大小
    static int SideBarSpaceIconLeft; //按钮内左侧边距
    static int SideBarSpaceIconText; //按钮图标与文字内边距
    static int SideBarSpaceBetweenItem; //item间距

};

#endif // STYLE_H
