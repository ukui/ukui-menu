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

#include "pushbutton.h"

PushButton::PushButton(QString name, int width, int height)
{
    this->name=name;
    this->width=width;
    this->height=height;
    init_app_btn();
}

PushButton::~PushButton()
{
}

void PushButton::init_app_btn()
{
    char btnstyle[300];
    sprintf(btnstyle,"QPushButton{background:transparent;border:0px;color:#ffffff;font-size:14px;padding-left:0px;text-align: left center;}\
            QPushButton:hover{background-color:%s;}\
            QPushButton:pressed{background-color:%s;}", ClassifyBtnHoverBackground,ClassifyBtnHoverBackground);

    this->setFixedSize(this->width,this->height);
    this->setStyleSheet(btnstyle);
    this->setFocusPolicy(Qt::NoFocus);
    QHBoxLayout* layout=new QHBoxLayout(this);
    layout->setContentsMargins(15,0,0,0);
    layout->setSpacing(6);

    QLabel* textlabel=new QLabel(this);
    textlabel->setAlignment(Qt::AlignCenter);
    textlabel->setStyleSheet("color:#ffffff;font-size:14px;");
    textlabel->setText(name);
    textlabel->adjustSize();

    QFrame* line=new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFixedHeight(1);
    line->setStyleSheet("background-color:rgba(255, 255, 255, 0.06)");
    line->setFixedSize(this->width-textlabel->width()-15,1);

    layout->addWidget(textlabel);
    layout->addWidget(line);
    this->setLayout(layout);
    this->setEnabled(false);
}
