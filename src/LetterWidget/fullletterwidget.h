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

#ifndef FULLLETTERWIDGET_H
#define FULLLETTERWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QToolButton>
#include <QAbstractItemView>
#include <QSpacerItem>
#include <QLabel>
#include <QDesktopWidget>
#include <QHeaderView>
#include <QScrollBar>
#include "src/Interface/ukuimenuinterface.h"
#include <QPushButton>
#include <QTableWidgetItem>
#include <QFrame>
#include <QSvgRenderer>
#include <QPainter>
#include "src/Interface/ukuimenuinterface.h"
#include <QButtonGroup>
#include <QTimer>
#include "src/UtilityFunction/scrollarea.h"
#include "src/UtilityFunction/pushbutton.h"
#include "src/UtilityFunction/fulllistview.h"
#include "src/UtilityFunction/classifyscrollarea.h"
#include "src/UtilityFunction/toolbutton.h"
#include "src/Style/style.h"

namespace Ui {
class FullLetterWidget;
}

class FullLetterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FullLetterWidget(QWidget *parent=nullptr);
    ~FullLetterWidget();
    void widgetMakeZero();//MainWindow隐藏时，此界面恢复至初始状态

private:
    Ui::FullLetterWidget *ui;

    UkuiMenuInterface* pUkuiMenuInterface=nullptr;

    //主界面
    QHBoxLayout* mainLayout=nullptr;

    //应用列表界面
    QWidget* applistWid=nullptr;

    ScrollArea* scrollarea=nullptr;
    QWidget* scrollareawid=nullptr;
    QVBoxLayout* scrollareawidLayout=nullptr;
    QStringList letterbtnlist;//存放字母按钮
    QStringList letterbtnrowlist;//存放字母按钮所在行
    QStringList letterposlist;//存放分类字符位置列表
    QStringList appsortlist;//存放应用排序列表
    QStringList data;

    //字母列表界面
    QWidget* letterlistWid=nullptr;
    QHBoxLayout* letterlistLayout=nullptr;

    ClassifyScrollArea* letterlistscrollarea=nullptr;
    QWidget* letterlistscrollareaWid=nullptr;
    QVBoxLayout* letterlistscrollareawidLayout=nullptr;
    QList<QAbstractButton*> buttonList;
    QButtonGroup* pBtnGroup=nullptr;

    //计时器
    QTimer* timer=nullptr;
    int beginPos=0;//滑动条起始位置
    int endPos=0;//滑动条终止位置

    int btnPos=0;//记住分类按钮位置

protected:
    void initWidget();
    void fillAppList();//填充应用列表
    void initLetterListWidget();//初始化字母列表界面
    void initLetterListScrollArea();//初始化字母列表
    void initAppListWidget();//初始化应用列表界面
    void resizeScrollAreaControls();//设置scrollarea填充控件大小

private Q_SLOTS:
    void btnGroupClickedSlot(QAbstractButton *btn);
    void execApplication(QString appname);//执行应用程序
    void updateAppListView();//更新应用列表
    void timeOutSlot();//计时器槽函数
    void valueChangedSlot(int value);//滑动条滚动槽函数

Q_SIGNALS:
    void sendUpdateAppListSignal(QString desktopfp,int type);//向常用软件模块发送更新应用列表信号
    void sendHideMainWindowSignal();//向MainViewWidget发送隐藏主窗口信号
};

#endif // FULLLETTERWIDGET_H
