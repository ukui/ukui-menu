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

#ifndef LETTERWIDGET_H
#define LETTERWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <gio/gdesktopappinfo.h>
#include "src/Interface/ukuimenuinterface.h"
#include "letterbuttonwidget.h"
#include "src/UtilityFunction/listview.h"
#include "src/UtilityFunction/itemdelegate.h"

namespace Ui {
class LetterWidget;
}

class LetterWidget : public QWidget
{
    Q_OBJECT

public:
     explicit LetterWidget(QWidget *parent=nullptr);
    ~LetterWidget();
    void widgetMakeZero();//MainWindow隐藏时，此界面恢复至初始状态
    /**
     * @brief moveScrollBar移动滚动条
     * @param type为0时表示向上移动，为1时表示向下移动
     */
    void moveScrollBar(int type);

private:
    Ui::LetterWidget *ui;
    UkuiMenuInterface* pUkuiMenuInterface=nullptr;
    LetterButtonWidget* letterbtnwid=nullptr;//分类按钮界面
    ListView* applistview=nullptr;
    QVector<QStringList> data;
    QStringList letterbtnlist;//存放字母按钮
    QStringList letterbtnrowlist;//存放字母按钮所在行
    QStringList letterposlist;//存放分类字符位置列表
    QStringList appsortlist;//存放应用排序列表
    QPropertyAnimation* enterAnimation=nullptr;
    QPropertyAnimation* leaveAnimation=nullptr;
    int widgetState=-1;

protected:
    void initWidget();
    void initAppListWidget();//初始化应用列表界面
    void fillAppListView();//填充应用列表

public Q_SLOTS:
    void appClassificationBtnClickedSlot();//应用列表字母分类按钮槽函数
    void recvLetterBtnSlot(QString btnname);//接收LetterButtonWidget界面按钮信号
    void execApplication(QString desktopfp);//执行应用程序
    void updateAppListView();//更新应用列表
    void recvItemClickedSlot(QStringList arg);
    void animationFinishedSLot();

Q_SIGNALS:
    void sendLetterBtnList(QStringList list);//向LetterButtonWidget发送字母按钮列表
    void sendUpdateAppListSignal(QString desktopfp,int type);//向常用软件模块发送更新应用列表信号
    void sendHideMainWindowSignal();//向MainViewWidget发送隐藏主窗口信号
};

#endif // LETTERWIDGET_H
