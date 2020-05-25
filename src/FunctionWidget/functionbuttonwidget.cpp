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

#include "functionbuttonwidget.h"
#include "src/Style/style.h"
#include <QSvgRenderer>
#include <QPainter>
#include <QDebug>

FunctionButtonWidget::FunctionButtonWidget(QWidget *parent) :
    QWidget(parent)
{
    categoryList.append("Mobile");
    categoryList.append("Internet");
    categoryList.append("Social");
    categoryList.append("Video");
    categoryList.append("Development");
    categoryList.append("Image");
    categoryList.append("Game");
    categoryList.append("Office");
    categoryList.append("Education");
    categoryList.append("System");
    categoryList.append("Others");

    initWidget();
}

FunctionButtonWidget::~FunctionButtonWidget()
{
}

void FunctionButtonWidget::initWidget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setStyleSheet("border:0px;background:transparent;");
    this->resize(Style::LeftBtnWidth*2+5,Style::LeftBtnHeight*6+25);

    gridLayout=new QGridLayout(this);
    gridLayout->setContentsMargins(0,0,0,0);
    gridLayout->setSpacing(5);
    this->setLayout(gridLayout);
}

/**
 * 功能分类按钮槽函数
 */
void FunctionButtonWidget::functionBtnClickedSlot()
{
    FunctionClassifyButton* btn=dynamic_cast<FunctionClassifyButton*>(sender());
    QWidget* wid=btn->layout()->itemAt(1)->widget();
    QLabel* label=qobject_cast<QLabel*>(wid);
    Q_EMIT sendFunctionBtnSignal(label->text());
}

/**
 * 接收FunctionWidget界面分类按钮列表
 */
void FunctionButtonWidget::recvClassificationBtnList()
{
    QVector<QStringList> vector=UkuiMenuInterface::functionalVector;
    for(int row=0;row<6;row++)
        for(int col=0;col<2;col++)
        {
            FunctionClassifyButton* iconbtn=nullptr;
            if(vector.at(row*2+col).isEmpty())
            {
                iconbtn=new FunctionClassifyButton(this,
                                                   Style::LeftBtnWidth,
                                                   Style::LeftBtnHeight,
                                                   Style::LeftIconSize,
                                                   categoryList.at(row*2+col),
                                                   false,
                                                   false);
                iconbtn->setEnabled(false);
            }
            else {
                iconbtn=new FunctionClassifyButton(this,
                                                   Style::LeftBtnWidth,
                                                   Style::LeftBtnHeight,
                                                   Style::LeftIconSize,
                                                   categoryList.at(row*2+col),
                                                   false,
                                                   true);
                connect(iconbtn,&FunctionClassifyButton::buttonClicked,this, &FunctionButtonWidget::functionBtnClickedSlot);
            }
            gridLayout->addWidget(iconbtn,row,col);
            if(row*2+col==10)break;
        }
}
