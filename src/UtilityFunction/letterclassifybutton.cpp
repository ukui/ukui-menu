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

#include "letterclassifybutton.h"
#include <QStylePainter>
#include <QStyleOptionButton>

LetterClassifyButton::LetterClassifyButton(QWidget *parent,
                                           bool fullscreen,
                                           QString letter):
    QPushButton(parent),
    m_fullscreen(fullscreen)
{
    this->setFlat(true);
    if(m_fullscreen)
    {
        QFont font;
        font.setPixelSize(Style::LeftLetterFontSize);
        this->setFont(font);
    }

    this->setText(letter);
    this->setCheckable(true);
    connect(this,&LetterClassifyButton::toggled,this,&LetterClassifyButton::reactToToggle);
}

void LetterClassifyButton::paintEvent(QPaintEvent* e)
{
    QStylePainter painter(this);
    QStyleOptionButton option;
    initStyleOption(&option);
    if ((option.state & QStyle::State_Enabled) && (option.state & QStyle::State_MouseOver)) {
        painter.save();
        painter.setPen(Qt::NoPen);
        if(!m_fullscreen)
        {
            QColor color = option.palette.color(QPalette::Text);
            color.setAlphaF(0.15);
            painter.setBrush(color);
        }
        else
        {
            painter.setOpacity(0.15);
            painter.setBrush(Qt::white);
        }
        painter.drawRoundedRect(option.rect, 4, 4);
        painter.restore();
    }
    if(m_fullscreen && (option.state & QStyle::State_On))
    {
        painter.save();
        painter.setPen(Qt::NoPen);
//        QColor color = option.palette.color(QPalette::Text);
//        color.setAlphaF(0.15);
//        painter.setBrush(color);
        painter.setOpacity(0.15);
        painter.setBrush(Qt::white);
        painter.drawRoundedRect(option.rect, 4, 4);
        painter.restore();
    }

    QStyleOptionButton subopt = option;
    subopt.rect = painter.style()->subElementRect(QStyle::SE_PushButtonContents, &option, this);
    if(m_fullscreen)
        subopt.palette.setBrush(QPalette::ButtonText, QColor(Qt::white));
    else
        subopt.palette.setBrush(QPalette::HighlightedText, subopt.palette.text());
    painter.style()->drawControl(QStyle::CE_PushButtonLabel, &subopt, &painter, this);
    return;
}

void LetterClassifyButton::enterEvent(QEvent *e)
{
    Q_UNUSED(e);
    if(m_fullscreen)
    {
        QFont font;
        font.setPixelSize(Style::LeftLetterFontSize*3);
        this->setFont(font);
        this->setFixedSize(Style::LeftLetterBtnHeight*2,Style::LeftLetterBtnHeight*2);
    }
}

void LetterClassifyButton::leaveEvent(QEvent *e)
{
    Q_UNUSED(e);
    if(m_fullscreen)
    {
        QFont font;
        font.setPixelSize(Style::LeftLetterFontSize);
        this->setFont(font);
        this->setFixedSize(Style::LeftLetterBtnHeight,Style::LeftLetterBtnHeight);
    }
}

void LetterClassifyButton::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if(m_fullscreen)
    {
        QFont font;
        font.setPixelSize(Style::LeftLetterFontSize*3);
        this->setFont(font);
        this->setFixedSize(Style::LeftLetterBtnHeight*2,Style::LeftLetterBtnHeight*2);
        is_pressed=true;
    }
}

void LetterClassifyButton::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if(m_fullscreen)
    {
        QFont font;
        font.setPixelSize(Style::LeftLetterFontSize*3);
        this->setFont(font);
        this->setFixedSize(Style::LeftLetterBtnHeight*2,Style::LeftLetterBtnHeight*2);
    }
    Q_EMIT buttonClicked(this);
}

void LetterClassifyButton::reactToToggle(bool checked)
{
    if(checked)
    {
        this->is_pressed=true;
    }
    else
    {
        this->is_pressed=false;
    }
}
