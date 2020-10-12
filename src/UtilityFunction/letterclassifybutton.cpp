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

LetterClassifyButton::LetterClassifyButton(QWidget *parent,
                                           QString hoverbg,
                                           QString pressedbg,
                                           QString letter):
    QPushButton(parent)
{
//    QPalette palette=this->palette();
//    palette.setBrush(QPalette::Button,QBrush(QColor(1,1,1,0)));
//    this->setPalette(palette);

    this->setFlat(true);
    this->hoverbg=hoverbg;
    this->pressedbg=pressedbg;
    QFont font;
    font.setPixelSize(Style::LeftLetterFontSize);
    this->setFont(font);
    this->setText(letter);
    this->setFixedSize(Style::LeftLetterBtnHeight,Style::LeftLetterBtnHeight);
    this->setCheckable(true);
    connect(this,&LetterClassifyButton::toggled,this,&LetterClassifyButton::reactToToggle);
}

void LetterClassifyButton::enterEvent(QEvent *e)
{
    Q_UNUSED(e);
    QFont font;
    font.setPixelSize(Style::LeftLetterFontSize*3);
    this->setFont(font);
    this->setFixedSize(Style::LeftLetterBtnHeight*2,Style::LeftLetterBtnHeight*2);
}

void LetterClassifyButton::leaveEvent(QEvent *e)
{
    Q_UNUSED(e);
    QFont font;
    font.setPixelSize(Style::LeftLetterFontSize);
    this->setFont(font);
    this->setFixedSize(Style::LeftLetterBtnHeight,Style::LeftLetterBtnHeight);
}

void LetterClassifyButton::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    QFont font;
    font.setPixelSize(Style::LeftLetterFontSize*3);
    this->setFont(font);
    this->setFixedSize(Style::LeftLetterBtnHeight*2,Style::LeftLetterBtnHeight*2);
    is_pressed=true;
}

void LetterClassifyButton::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    QFont font;
    font.setPixelSize(Style::LeftLetterFontSize*3);
    this->setFont(font);
    this->setFixedSize(Style::LeftLetterBtnHeight*2,Style::LeftLetterBtnHeight*2);
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
