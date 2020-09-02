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
    QToolButton(parent)
{
    this->hoverbg=hoverbg;
    this->pressedbg=pressedbg;
    this->setStyleSheet(QString("background:transparent;color:#8b8b8b;padding-left:0px;"));
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
    this->setStyleSheet(QString("background:transparent;color:#ffffff;padding-left:0px;"));
    QFont font;
    font.setPixelSize(Style::LeftLetterFontSize*3);
    this->setFont(font);
    this->setFixedSize(Style::LeftLetterBtnHeight*2,Style::LeftLetterBtnHeight*2);
}

void LetterClassifyButton::leaveEvent(QEvent *e)
{
    Q_UNUSED(e);
    if(is_pressed)
        this->setStyleSheet(QString("background:transparent;color:#ffffff;padding-left:0px;"));
    else
        this->setStyleSheet(QString("background:transparent;color:#8b8b8b;padding-left:0px;"));
    QFont font;
    font.setPixelSize(Style::LeftLetterFontSize);
    this->setFont(font);
    this->setFixedSize(Style::LeftLetterBtnHeight,Style::LeftLetterBtnHeight);
}

void LetterClassifyButton::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    this->setStyleSheet(QString("background:transparent;color:#ffffff;padding-left:0px;"));
    QFont font;
    font.setPixelSize(Style::LeftLetterFontSize*3);
    this->setFont(font);
    this->setFixedSize(Style::LeftLetterBtnHeight*2,Style::LeftLetterBtnHeight*2);
    is_pressed=true;
}

void LetterClassifyButton::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    this->setStyleSheet(QString("background:transparent;color:#ffffff;padding-left:0px;"));
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
        this->setStyleSheet(QString("background:transparent;color:#ffffff;padding-left:0px;"));
        this->is_pressed=true;
    }
    else
    {
        this->setStyleSheet(QString("background:transparent;color:#8b8b8b;padding-left:0px;"));
        this->is_pressed=false;
    }
}
