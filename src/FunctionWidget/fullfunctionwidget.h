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

#ifndef FULLFUNCTIONWIDGET_H
#define FULLFUNCTIONWIDGET_H

#include <QWidget>
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
#include "src/UtilityFunction/scrollarea.h"
#include "src/UtilityFunction/pushbutton.h"
#include "src/UtilityFunction/fulllistview.h"
#include "src/UtilityFunction/classifyscrollarea.h"
#include "src/UtilityFunction/toolbutton.h"
#include "src/UtilityFunction/functionclassifybutton.h"
#include "src/Style/style.h"

namespace Ui {
class FullFunctionWidget;
}

class FullFunctionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FullFunctionWidget(QWidget *parent=nullptr);
    ~FullFunctionWidget();
    void widgetMakeZero();//MainWindow隐藏时，此界面恢复至初始状态
    void enterAnimation();
//    void updateRecentListView();//更新最近添加应用
    void repaintWidget();
    /**
     * @brief moveScrollBar移动滚动条
     * @param type为0时表示向上移动，为1时表示向下移动
     */
    void moveScrollBar(int type);

private:
    Ui::FullFunctionWidget *ui;

    UkuiMenuInterface* pUkuiMenuInterface=nullptr;

    //主界面
    QHBoxLayout* mainLayout=nullptr;

    //应用列表界面
    QWidget* applistWid=nullptr;

    ScrollArea* scrollarea=nullptr;
    QWidget* scrollareawid=nullptr;
    QVBoxLayout* scrollareawidLayout=nullptr;
    QStringList classificationbtnlist;//存放分类按钮
    QStringList classificationbtnrowlist;//存放分类按钮所在行
    QStringList data;
//    int pos=0;

    //图标列表界面
    QStringList iconlist;
    QStringList iconlightlist;
    QStringList functionnamelist;
    QWidget* iconlistWid=nullptr;
    QHBoxLayout* iconlistLayout=nullptr;
    QSpacerItem* pIconListTopSpacer=nullptr;
    QSpacerItem* pIconListBottomSpacer=nullptr;

    ClassifyScrollArea* iconlistscrollarea=nullptr;
    QWidget* iconlistscrollareaWid=nullptr;
    QVBoxLayout* iconlistscrollareawidLayout=nullptr;
    QList<QAbstractButton*> buttonList;
    QButtonGroup* pBtnGroup=nullptr;

    QPropertyAnimation* pAnimation=nullptr;
    QSettings* setting=nullptr;

    int beginPos=0;//滑动条起始位置
    int endPos=0;//滑动条终止位置
    QPropertyAnimation* m_scrollAnimation=nullptr;

protected:
    void initWidget();
    void initIconListWidget();//初始化图标列表界面
    void initIconListScrollArea();//初始化图标列表界面数据表格iconlisttableWid

    void initAppListWidget();//初始化应用列表界面
    void fillAppList();//填充应用列表
    void insertClassificationBtn(QString btnname);//插入分类按钮
    void insertAppList(QStringList appnamelist);//插入应用列表
    void resizeScrollAreaControls();//设置scrollarea填充控件大小

public Q_SLOTS:
    void execApplication(QString desktopfp);//执行应用程序
    void btnGroupClickedSlot(QAbstractButton *btn);
    void updateAppListView();//更新应用列表
    void valueChangedSlot(int value);//滑动条滚动槽函数
    void animationFinishSlot();
    void animationValueChangedSlot(const QVariant &value);

Q_SIGNALS:
    void sendUpdateAppListSignal(QString desktopfp,int type);//向常用软件模块发送更新应用列表信号
    void sendHideMainWindowSignal();//向MainViewWidget发送隐藏主窗口信号
};

#endif // FULLFUNCTIONWIDGET_H
