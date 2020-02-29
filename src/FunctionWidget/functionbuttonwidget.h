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

#ifndef FUNCTIONBUTTONWIDGET_H
#define FUNCTIONBUTTONWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QLabel>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include "src/UtilityFunction/functionclassifybutton.h"

namespace Ui {
class FunctionButtonWidget;
}

class FunctionButtonWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FunctionButtonWidget(QWidget *parent = nullptr);
    ~FunctionButtonWidget();

private:
    Ui::FunctionButtonWidget *ui;
    QGridLayout* gridLayout=nullptr;
    QVector<QAbstractButton*> vector;
    QStringList icondisabledlist, iconlist,iconlightlist,functionnamelist;

protected:
    void initWidget();

Q_SIGNALS:
    /**
     * 向FunctionWidget界面发送功能分类按钮被点击信号
     * @param btnname存放被点击按钮名称
     */
    void sendFunctionBtnSignal(QString btnname);

private Q_SLOTS:
    void functionBtnClickedSlot();//功能分类按钮槽函数
    void recvClassificationBtnList(QStringList list);//接收FunctionWidget界面分类按钮列表
};

#endif // FUNCTIONBUTTONWIDGET_H
