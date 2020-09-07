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

PushButton::PushButton(QWidget *parent, QString category, int width, int height, int module):
    QPushButton(parent),
    m_category(category),
    m_width(width),
    m_height(height),
    m_module(module),
    m_textLabel(new QLabel),
    m_line(new QFrame)
{
    initAppBtn();
}

PushButton::~PushButton()
{
}

void PushButton::initAppBtn()
{
    char btnstyle[300];
    sprintf(btnstyle,"QPushButton{background:transparent;border:0px;color:#ffffff;font-size:14px;padding-left:0px;text-align: left center;}\
            QPushButton:hover{background-color:%s;}\
            QPushButton:pressed{background-color:%s;}", ClassifyBtnHoverBackground,ClassifyBtnHoverBackground);

    this->setFixedSize(m_width,m_height);
    this->setStyleSheet(btnstyle);
    this->setFocusPolicy(Qt::NoFocus);
    QHBoxLayout* layout=new QHBoxLayout(this);
    layout->setContentsMargins(15,0,0,0);
    layout->setSpacing(6);

    char style[100];
    sprintf(style,"color:#ffffff;");
    m_textLabel->setAlignment(Qt::AlignCenter);
    m_textLabel->setStyleSheet(style);
    if(m_module==1)
        m_textLabel->setText(m_category);
    else
        setLabelText();
    m_textLabel->adjustSize();
    m_line->setFrameShape(QFrame::HLine);
    m_line->setFixedHeight(1);
    m_line->setStyleSheet("background-color:rgba(255, 255, 255, 0.06)");
    m_line->setFixedSize(m_width-m_textLabel->width()-15,1);

    layout->addWidget(m_textLabel);
    layout->addWidget(m_line);
    this->setLayout(layout);
    this->setEnabled(false);
}

void PushButton::setLabelText()
{
    QMetaEnum metaEnum=QMetaEnum::fromType<PushButton::Category>();
    switch (metaEnum.keyToValue(m_category.toLocal8Bit().data()))
    {
    case Mobile:
        m_textLabel->setText(tr("Mobile"));
        break;
    case Internet:
        m_textLabel->setText(tr("Internet"));
        break;
    case Social:
        m_textLabel->setText(tr("Social"));
        break;
    case Video:
        m_textLabel->setText(tr("Video"));
        break;
    case Development:
        m_textLabel->setText(tr("Development"));
        break;
    case Image:
        m_textLabel->setText(tr("Image"));
        break;
    case Game:
        m_textLabel->setText(tr("Game"));
        break;
    case Office:
        m_textLabel->setText(tr("Office"));
        break;
    case Education:
        m_textLabel->setText(tr("Education"));
        break;
    case System:
        m_textLabel->setText(tr("System"));
        break;
    default:
        m_textLabel->setText(tr("Others"));
        break;
    }
}
