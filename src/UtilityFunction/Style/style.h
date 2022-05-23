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
#define RightClickMenuSelected "rgba(61, 107, 229, 1)"//右键菜单悬浮
#define RightClickMenuBorder "rgba(255, 255, 255, 0.2)"//右键菜单边框
#define RightClickMenuFont "rgba(255, 255, 255, 0.91)"
#define RightClickMenuOpacity 0.95
#define ToolTipBackground "rgba(26, 26, 26, 0.7)"
#define DefaultBackground "rgba(19, 19, 20, 0.7)" //默认态背景色
#define LineBackground "rgba(255,255,255)" //分割线背景色
#define SBClassifyBtnSelectedBackground "#3D6BE5" //侧边栏上部分类按钮选择背景
#define SBFunBtnHoverBackground "rgba(255, 255, 255, 0.14)" //侧边栏目下部功能按钮悬浮背景
#define ClassifyBtnHoverBackground "rgba(255, 255, 255, 0.14)" //分类按钮悬浮背景
#define MMBtnHoverBackground "rgba(255, 255, 255, 0.14)" //最大化最小化按钮悬浮背景
#define QueryLineEditDefaultBackground "rgba(0, 0, 0,0.04)" //搜索框默认态背景
#define QueryLineEditBackground "rgba(255, 255, 255,0.06)" //搜索框背景
#define QueryLineEditClickedDefaultBackground "rgba(0, 0, 0,0.04)" //搜索框默认态背景选中
#define QueryLineEditClickedBackground "rgba(255, 255, 255,0.06)" //搜索框背景选中
#define QueryLineEditClickedBorderDefault "rgba(0, 0, 0,0.1)" //搜索框默认态背景选中边框
#define QueryLineEditClickedBorder "rgba(5, 151, 255, 1)" //搜索框背景选中边框
#define AppBtnHover "#ffffff" //按钮悬浮

/**
 * 大小(全屏)
 */
////左侧栏
//#define LeftWidSize leftsize //大小
//#define LeftMarginLeftSize Leftmarginleftsize //内左侧边距
//#define LeftSpaceIconText leftspaceicontext //按钮图标与文字内边距
//#define LeftSpaceBetweenItem leftspacebetweenitem //item间距

////应用列表栏
//#define AppListIconSize applisticonsize //图标大小
//#define AppListSpaceBetweenItem applistspacebetweenitem //item间距

////右侧栏
//#define RightWidSize rightsize //大小
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

    static int m_primaryScreenX;
    static int m_primaryScreenY;
    static int m_panelPosition;
    //默认窗口
    static int minw;
    static int minh;
    static int leftPageWidth;
    static int leftPageHeight;
    static int defaultQueryLineEditIconSize;

    /**
      * 全屏
      */
    //主窗口
    static int m_availableScreenWidth;
    static int m_availableScreenHeight;
    //搜索栏
    static int QueryLineEditHeight;//搜索框高度
    //左侧栏
    static int m_leftWidWidth; //宽度
    static int LeftMargin; //内左侧边距
    static int LeftBtnWidth;//按钮宽度
    static int LeftBtnHeight;//按钮高度
    static int LeftLetterBtnHeight;//按钮高度
    static int LeftLetterFontSize; //字体大小
    static int LeftIconSize; //图标大小
    static int LeftSpaceIconText; //按钮图标与文字内边距
    static int LeftSpaceBetweenItem; //item间距
    //应用列表栏
    static int m_applistWidWidth; //宽度
    static int m_applistWidHeight; //高度
    static int AppListIconSize; //图标大小
    static int AppListBigIconSize;
    static int AppListItemSizeWidth; //item大小
    static int TabletItemSizeWidthFirst;
    static int TabletItemSizeWidthOther;
    static int FirsPageViewWidth;
    static int OtherPageViewWidth;
    static int m_applistGridSizeWidth; //网格大小
    static int AppLeftSpace;//左侧边距
    static int AppTopSpace;//上侧边距
    static int AppSpaceBetweenIconText;//图标与文字边距
    static int FirstPageSpace;
    static int OtherPageSpace;

    /**
     * 侧边栏
     */
    static int SideBarWidWidth; //宽度

    static int appLine;
    static int appColumn;
    static int appLineFirst;
    static int appColumnFirst;
    static int appNum;
    static int appPage;
    static int nowpagenum;
    static int appRows; //app的总行数
    static bool ScreenRotation;

    static int AppListItemSizeHeight;

//    static int ScreenWidth;
//    static int ScreenHeight;


    static int LeftWidgetWidth;

    static int ScreenWidth;
    static int ScreenHeight;
    static int AppListViewTopMargin;
    static int AppListViewLeftMargin;
    static int AppListViewBottomMargin;
    static int AppListViewRightMargin;
    static int CenterWindHeight;

    static int GroupAppRow;
    static int GroupAppColumn;

    static int TimeWidgetTop;
    static int TimeWidgetLeft;

    static int ButtonWidgetWidth;

    //扩展的开始菜单应用列表
    static int ToolWidgetLeft;
    static int WideAppListItemSizeWidth;
    static int WideAppListItemSizeHeight;
    static int WideAppViewLeftMargin;
    static bool IsWideScreen;
    static int DropMenuWidth;
    static int DropMenuHeight;
    static int miniIconSize;

    static int rightViewWidth;
    static int rightViewHeight;

};

#endif // STYLE_H
