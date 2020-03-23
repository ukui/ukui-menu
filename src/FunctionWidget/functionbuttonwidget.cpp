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
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setFixedSize(320,500);

    gridLayout=new QGridLayout(this);
    gridLayout->setContentsMargins(15,0,6,this->height()-48*6);
    gridLayout->setSpacing(5);
    this->setLayout(gridLayout);

    icondisabledlist.clear();
    icondisabledlist.append(":/data/img/mainviewwidget/recent-disabled.svg");
    icondisabledlist.append(":/data/img/mainviewwidget/net-disabled.svg");
    icondisabledlist.append(":/data/img/mainviewwidget/social-disabled.svg");
    icondisabledlist.append(":/data/img/mainviewwidget/video-disabled.svg");
    icondisabledlist.append(":/data/img/mainviewwidget/develop-disabled.svg");
    icondisabledlist.append(":/data/img/mainviewwidget/img-disabled.svg");
    icondisabledlist.append(":/data/img/mainviewwidget/game-disabled.svg");
    icondisabledlist.append(":/data/img/mainviewwidget/office-disabled.svg");
    icondisabledlist.append(":/data/img/mainviewwidget/reading-disabled.svg");
    icondisabledlist.append(":/data/img/mainviewwidget/system-disabled.svg");
    icondisabledlist.append(":/data/img/mainviewwidget/other-disabled.svg");

    iconlist.clear();
    iconlist.append(":/data/img/mainviewwidget/recent-enabled.svg");
    iconlist.append(":/data/img/mainviewwidget/net-enabled.svg");
    iconlist.append(":/data/img/mainviewwidget/social-enabled.svg");
    iconlist.append(":/data/img/mainviewwidget/video-enabled.svg");
    iconlist.append(":/data/img/mainviewwidget/develop-enabled.svg");
    iconlist.append(":/data/img/mainviewwidget/img-enabled.svg");
    iconlist.append(":/data/img/mainviewwidget/game-enabled.svg");
    iconlist.append(":/data/img/mainviewwidget/office-enabled.svg");
    iconlist.append(":/data/img/mainviewwidget/reading-enabled.svg");
    iconlist.append(":/data/img/mainviewwidget/system-enabled.svg");
    iconlist.append(":/data/img/mainviewwidget/other-enabled.svg");

    iconlightlist.clear();
    iconlightlist.append(":/data/img/mainviewwidget/recent.svg");
    iconlightlist.append(":/data/img/mainviewwidget/net.svg");
    iconlightlist.append(":/data/img/mainviewwidget/social.svg");
    iconlightlist.append(":/data/img/mainviewwidget/video.svg");
    iconlightlist.append(":/data/img/mainviewwidget/develop.svg");
    iconlightlist.append(":/data/img/mainviewwidget/img.svg");
    iconlightlist.append(":/data/img/mainviewwidget/game.svg");
    iconlightlist.append(":/data/img/mainviewwidget/office.svg");
    iconlightlist.append(":/data/img/mainviewwidget/reading.svg");
    iconlightlist.append(":/data/img/mainviewwidget/system.svg");
    iconlightlist.append(":/data/img/mainviewwidget/other.svg");

    functionnamelist.clear();
    functionnamelist.append(tr("Recently"));
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

//    for(int i=0;i<11;i++)
//    {
//        FunctionClassifyButton* iconbtn=new FunctionClassifyButton(this,
//                                                                   106,
//                                                                   48,
//                                                                   iconlist.at(i),
//                                                                   iconlightlist.at(i),
//                                                                   ClassifyBtnHoverBackground,
//                                                                   ClassifyBtnHoverBackground,
//                                                                   2,
//                                                                   functionnamelist.at(i));
//        vector.append(iconbtn);
//        connect(iconbtn,SIGNAL(buttonClicked(QAbstractButton*)),this, SLOT(functionBtnClickedSlot()));
//    }

//    for(int row=0;row<6;row++)
//        for(int col=0;col<2;col++)
//        {
//            if(row*2+col < vector.size())
//                gridLayout->addWidget(vector.at(row*2+col),row,col);
//            else
//                break;
//        }
}

/**
 * 功能分类按钮槽函数
 */
void FunctionButtonWidget::functionBtnClickedSlot()
{
    FunctionClassifyButton* btn=dynamic_cast<FunctionClassifyButton *>(QObject::sender());
    QWidget* wid=btn->layout()->itemAt(1)->widget();
    QLabel* label=qobject_cast<QLabel *>(wid);
    QString btnname=label->text();
    Q_EMIT sendFunctionBtnSignal(btnname);
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
                                                   106,
                                                   48,
                                                   19,
                                                   Style::LeftFontSize,
                                                   icondisabledlist.at(row*2+col),
                                                   icondisabledlist.at(row*2+col),
                                                   ClassifyBtnHoverBackground,
                                                   ClassifyBtnHoverBackground,
                                                   2,
                                                   functionnamelist.at(row*2+col),
                                                   false,
                                                   false);
                iconbtn->setEnabled(false);
            }
            else {
                iconbtn=new FunctionClassifyButton(this,
                                                   106,
                                                   48,
                                                   19,
                                                   Style::LeftFontSize,
                                                   iconlist.at(row*2+col),
                                                   iconlightlist.at(row*2+col),
                                                   ClassifyBtnHoverBackground,
                                                   ClassifyBtnHoverBackground,
                                                   2,
                                                   functionnamelist.at(row*2+col),
                                                   false,
                                                   true);
                connect(iconbtn,SIGNAL(buttonClicked(QAbstractButton*)),this, SLOT(functionBtnClickedSlot()));
            }
            QLayoutItem* item=gridLayout->itemAt(row*2+col);
            if(item!=nullptr)
            {
                QWidget* wid=item->widget();
                gridLayout->replaceWidget(wid,iconbtn);
            }
            else {
                gridLayout->addWidget(iconbtn,row,col);
            }

            if(row*2+col==10)break;
        }


//    for(int i=0;i<list.size();i++)
//    {
//        FunctionClassifyButton* iconbtn=new FunctionClassifyButton(this,
//                                                                   106,
//                                                                   48,
//                                                                   iconlist.at(functionnamelist.indexOf(list.at(i))),
//                                                                   iconlightlist.at(functionnamelist.indexOf(list.at(i))),
//                                                                   ClassifyBtnHoverBackground,
//                                                                   ClassifyBtnHoverBackground,
//                                                                   2,
//                                                                   list.at(i));
//        vector.append(iconbtn);
//        connect(iconbtn,SIGNAL(buttonClicked(QAbstractButton*)),this, SLOT(functionBtnClickedSlot()));
//    }

//    for(int row=0;row<6;row++)
//        for(int col=0;col<2;col++)
//        {
//            if(row*2+col < vector.size())
//                gridLayout->addWidget(vector.at(row*2+col),row,col);
//            else
//                break;
//        }
}
