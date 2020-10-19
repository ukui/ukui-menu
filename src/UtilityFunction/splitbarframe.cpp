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

#include "splitbarframe.h"

SplitBarFrame::SplitBarFrame(QWidget *parent, QString category, int width, int height, int module):
    QFrame(parent),
    m_category(category),
    m_width(width),
    m_height(height),
    m_module(module),
    m_textLabel(new QLabel),
    m_line(new QFrame)
{
    initAppBtn();
}

SplitBarFrame::~SplitBarFrame()
{
}

void SplitBarFrame::initAppBtn()
{   
    this->setFixedSize(m_width,m_height);
//    this->setStyleSheet("background:transparent");
    //按钮透明
    this->setFocusPolicy(Qt::NoFocus);
    this->setAttribute(Qt::WA_TranslucentBackground);
    QHBoxLayout* layout=new QHBoxLayout;
    layout->setContentsMargins(15,0,0,0);
    layout->setSpacing(6);

    m_textLabel->setAutoFillBackground(false);
    m_textLabel->setAlignment(Qt::AlignCenter);
    if(m_module==1)
        m_textLabel->setText(m_category);
    else
        setLabelText();
    m_textLabel->adjustSize();
    QPalette pe=m_textLabel->palette();
    pe.setColor(QPalette::WindowText,Qt::white);
    m_textLabel->setPalette(pe);
    m_line->setFrameShape(QFrame::HLine);
    m_line->setFixedHeight(1);
    m_line->setFixedSize(m_width-m_textLabel->width()-15,1);
    m_line->setEnabled(false);
    QPalette linePe=m_line->palette();
    QColor color(255,255,255);
    color.setAlphaF(0.25);
    linePe.setColor(QPalette::WindowText,color);
    m_line->setPalette(linePe);

    this->setLayout(layout);
    layout->addWidget(m_textLabel);
    layout->addWidget(m_line);
}

void SplitBarFrame::setLabelText()
{
    QMetaEnum metaEnum=QMetaEnum::fromType<SplitBarFrame::Category>();
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
