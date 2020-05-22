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
#include "ui_functionbuttonwidget.h"
#include "src/Style/style.h"
#include <QSvgRenderer>
#include <QPainter>
#include <QDebug>

FunctionButtonWidget::FunctionButtonWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FunctionButtonWidget)
{
    ui->setupUi(this);
    initWidget();
}

FunctionButtonWidget::~FunctionButtonWidget()
{
    delete ui;
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

    functionnamelist.clear();
    functionnamelist.append(tr("Mobile"));
    functionnamelist.append(tr("Internet"));
    functionnamelist.append(tr("Social"));
    functionnamelist.append(tr("Video"));
    functionnamelist.append(tr("Development"));
    functionnamelist.append(tr("Image"));
    functionnamelist.append(tr("Game"));
    functionnamelist.append(tr("Office"));
    functionnamelist.append(tr("Education"));
    functionnamelist.append(tr("System"));
    functionnamelist.append(tr("Others"));

    categorylist.clear();
    categorylist.append("Mobile");
    categorylist.append("Internet");
    categorylist.append("Social");
    categorylist.append("Video");
    categorylist.append("Development");
    categorylist.append("Image");
    categorylist.append("Game");
    categorylist.append("Office");
    categorylist.append("Education");
    categorylist.append("System");
    categorylist.append("Others");
}

/**
 * 功能分类按钮槽函数
 */
void FunctionButtonWidget::functionBtnClickedSlot(QString category)
{
    Q_EMIT sendFunctionBtnSignal(functionnamelist.at(categorylist.indexOf(category)));
}

/**
 * 接收FunctionWidget界面分类按钮列表
 */
void FunctionButtonWidget::recvClassificationBtnList(QStringList list)
{
    for(int row=0;row<6;row++)
        for(int col=0;col<2;col++)
        {
            FunctionClassifyButton* iconbtn=nullptr;
            if(list.indexOf(functionnamelist.at(row*2+col))==-1)
            {
                iconbtn=new FunctionClassifyButton(this,
                                                   Style::LeftBtnWidth,
                                                   Style::LeftBtnHeight,
                                                   Style::LeftIconSize,
                                                   categorylist.at(row*2+col),
                                                   functionnamelist.at(row*2+col),
                                                   false,
                                                   false);
                iconbtn->setEnabled(false);
            }
            else {
                iconbtn=new FunctionClassifyButton(this,
                                                   Style::LeftBtnWidth,
                                                   Style::LeftBtnHeight,
                                                   Style::LeftIconSize,
                                                   categorylist.at(row*2+col),
                                                   functionnamelist.at(row*2+col),
                                                   false,
                                                   true);
                connect(iconbtn,&FunctionClassifyButton::buttonClicked,this, &FunctionButtonWidget::functionBtnClickedSlot);
            }
            gridLayout->addWidget(iconbtn,row,col);
            if(row*2+col==10)break;
        }
}
