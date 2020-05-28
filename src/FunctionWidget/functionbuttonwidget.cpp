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
    m_categoryList.append("Mobile");
    m_categoryList.append("Internet");
    m_categoryList.append("Social");
    m_categoryList.append("Video");
    m_categoryList.append("Development");
    m_categoryList.append("Image");
    m_categoryList.append("Game");
    m_categoryList.append("Office");
    m_categoryList.append("Education");
    m_categoryList.append("System");
    m_categoryList.append("Others");

    initUi();
}

FunctionButtonWidget::~FunctionButtonWidget()
{
}

void FunctionButtonWidget::initUi()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setStyleSheet("border:0px;background:transparent;");
    this->resize(Style::LeftBtnWidth*2+5,Style::LeftBtnHeight*6+25);

    QGridLayout* gridLayout=new QGridLayout;
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
                                                   m_categoryList.at(row*2+col),
                                                   false,
                                                   false);
                iconbtn->setEnabled(false);
            }
            else {
                iconbtn=new FunctionClassifyButton(this,
                                                   Style::LeftBtnWidth,
                                                   Style::LeftBtnHeight,
                                                   Style::LeftIconSize,
                                                   m_categoryList.at(row*2+col),
                                                   false,
                                                   true);
                connect(iconbtn,&FunctionClassifyButton::buttonClicked,this, &FunctionButtonWidget::functionBtnClickedSlot);
            }
            QGridLayout* gridLayout=qobject_cast<QGridLayout*>(this->layout());
            gridLayout->addWidget(iconbtn,row,col);
            if(row*2+col==10)break;
        }
}
