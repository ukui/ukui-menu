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

#include "functionclassifybutton.h"
#include <QDebug>

FunctionClassifyButton::FunctionClassifyButton(QWidget *parent,
                       int width,
                       int height,
                       int iconSize,
                       int textSize,
                       QString iconstr,
                       QString piconstr,
                       QString hoverbg,
                       QString pressedbg,
                       int module,
                       QString text,
                       bool is_fullscreen,
                       bool enabled):
    QPushButton (parent)
{
    this->width=width;
    this->height=height;
    this->iconSize=iconSize;
    this->textSize=textSize;
    this->setFixedSize(width,height);
    this->iconstr=iconstr;
    this->piconstr=piconstr;
    this->hoverbg=hoverbg;
    this->pressedbg=pressedbg;
    this->module=module;
    this->text=text;
    this->is_fullscreen=is_fullscreen;
    this->enabled=enabled;
    this->setCheckable(true);

    this->setFocusPolicy(Qt::NoFocus);
    svgRender=new QSvgRenderer(this);
    svgRender->load(iconstr);
    pixmap=new QPixmap(iconSize,iconSize);
    pixmap->fill(Qt::transparent);
    QPainter p(pixmap);
    svgRender->render(&p);
    iconlabel=new QLabel(this);
    iconlabel->setFixedSize(pixmap->size());
    iconlabel->setPixmap(*pixmap);
    iconlabel->setStyleSheet("background:transparent;");

    textlabel=new QLabel(this);
    textlabel->setText(this->text);
//    QFont font;
//    font.setPixelSize(textSize);
//    textlabel->setFont(font);
    if(is_fullscreen)
        textlabel->setStyleSheet("background:transparent; color:rgba(255, 255, 255, 50%);");
    else
    {
        if(enabled)
            textlabel->setStyleSheet("background:transparent; color:rgba(255, 255, 255);");
        else
            textlabel->setStyleSheet("background:transparent; color:rgba(255, 255, 255, 25%);");
    }
    textlabel->adjustSize();

    mainlayout=new QHBoxLayout;
    mainlayout->setContentsMargins(Style::LeftSpaceIconLeft,0,0,0);
    mainlayout->setSpacing(Style::LeftSpaceIconText);
    this->setLayout(mainlayout);
    mainlayout->addWidget(iconlabel);
    mainlayout->addWidget(textlabel);
    connect(this,&FunctionClassifyButton::toggled,this,&FunctionClassifyButton::reactToToggle);
}

void FunctionClassifyButton::enterEvent(QEvent *e)
{
    Q_UNUSED(e);
    this->setFixedSize(width,height);
    QByteArray byte=hoverbg.toLocal8Bit();
    char* hover=byte.data();
    char style[100];
    if(enabled)
        sprintf(style,"border:0px;border-radius:4px;padding-left:0px;background-color:%s;",hover);
    else
        sprintf(style,"border:0px;border-radius:4px;padding-left:0px;background:transparent");
    this->setStyleSheet(QString::fromLocal8Bit(style));
    delete svgRender;
    svgRender=new QSvgRenderer(this);
    svgRender->load(piconstr);
    pixmap=new QPixmap(iconSize,iconSize);
    pixmap->fill(Qt::transparent);
    QPainter p(pixmap);
    svgRender->render(&p);
    iconlabel->setPixmap(*pixmap);
    iconlabel->setFixedSize(pixmap->size());
//    QFont font;
//    font.setPixelSize(textSize);
//    textlabel->setFont(font);
    if(enabled)
        textlabel->setStyleSheet("background:transparent;color:rgba(255, 255, 255);");
    textlabel->adjustSize();
}

void FunctionClassifyButton::leaveEvent(QEvent *e)
{
    Q_UNUSED(e);
    this->setStyleSheet("border:0px;border-radius:4px;padding-left:0px;background:transparent;");
    delete svgRender;
    svgRender=new QSvgRenderer(this);
    if(module==2 && is_pressed)
    {
        this->setFixedSize(width,height);
        svgRender->load(piconstr);
        pixmap=new QPixmap(iconSize,iconSize);
//        QFont font;
//        font.setPixelSize(textSize);
//        textlabel->setFont(font);

    }
    else
    {
        this->setFixedSize(width,height);
        svgRender->load(iconstr);
        pixmap=new QPixmap(iconSize,iconSize);
//        QFont font;
//        font.setPixelSize(textSize);
//        textlabel->setFont(font);

    }
//    pixmap=new QPixmap(iconSize,iconSize);
    pixmap->fill(Qt::transparent);
    QPainter p(pixmap);
    svgRender->render(&p);
    iconlabel->setPixmap(*pixmap);
    iconlabel->setFixedSize(pixmap->size());
    if(is_fullscreen)
    {
        if(is_pressed)
            textlabel->setStyleSheet("background:transparent;color:rgba(255, 255, 255);");
        else
            textlabel->setStyleSheet("background:transparent; color:rgba(255, 255, 255, 50%);");
    }
    textlabel->adjustSize();
}

void FunctionClassifyButton::mousePressEvent(QMouseEvent *event)
{
    this->setFixedSize(width,height);
    QByteArray byte=pressedbg.toLocal8Bit();
    char* pressed=byte.data();
    char style[100];
    if(enabled)
        sprintf(style,"border:0px;border-radius:4px;padding-left:0px;background-color:%s;",pressed);
    if(event->button()==Qt::LeftButton)
    {
        this->setStyleSheet(QString::fromLocal8Bit(style));
        delete svgRender;
        svgRender=new QSvgRenderer(this);
        svgRender->load(piconstr);
        pixmap=new QPixmap(iconSize,iconSize);
        pixmap->fill(Qt::transparent);
        QPainter p(pixmap);
        svgRender->render(&p);
        iconlabel->setPixmap(*pixmap);
        iconlabel->setFixedSize(pixmap->size());
//        QFont font;
//        font.setPixelSize(textSize);
//        textlabel->setFont(font);
        textlabel->setStyleSheet("background:transparent;color:#ffffff;");
        textlabel->adjustSize();
        is_pressed=true;
    }
}

void FunctionClassifyButton::mouseReleaseEvent(QMouseEvent *event)
{
    QByteArray byte=hoverbg.toLocal8Bit();
    char* hover=byte.data();
    char style[100];
    if(enabled)
        sprintf(style,"border:0px;border-radius:4px;padding-left:0px;background-color:%s;",hover);
    if(event->button()==Qt::LeftButton)
    {
        this->setStyleSheet(QString::fromLocal8Bit(style));
        delete svgRender;
        svgRender=new QSvgRenderer(this);
        svgRender->load(piconstr);
        pixmap=new QPixmap(iconSize,iconSize);
        pixmap->fill(Qt::transparent);
        QPainter p(pixmap);
        svgRender->render(&p);
        iconlabel->setPixmap(*pixmap);
        iconlabel->setFixedSize(pixmap->size());
//        QFont font;
//        font.setPixelSize(textSize);
//        textlabel->setFont(font);
        textlabel->setStyleSheet("background:transparent;color:#ffffff;");
        textlabel->adjustSize();
        Q_EMIT buttonClicked(this);
    }
}

void FunctionClassifyButton::reactToToggle(bool checked)
{
    if(checked)
    {
        delete svgRender;
        svgRender=new QSvgRenderer(this);
        svgRender->load(piconstr);
        pixmap=new QPixmap(iconSize,iconSize);
        pixmap->fill(Qt::transparent);
        QPainter p(pixmap);
        svgRender->render(&p);
        iconlabel->setPixmap(*pixmap);
        iconlabel->setFixedSize(pixmap->size());
        textlabel->setStyleSheet("background:transparent;color:#ffffff;");
        textlabel->adjustSize();
        is_pressed=true;
    }
    else{
        delete svgRender;
        svgRender = new QSvgRenderer(this);
        svgRender->load(iconstr);
        pixmap=new QPixmap(iconSize,iconSize);
        pixmap->fill(Qt::transparent);
        QPainter p(pixmap);
        svgRender->render(&p);
        iconlabel->setPixmap(*pixmap);
        iconlabel->setFixedSize(pixmap->size());
        textlabel->setStyleSheet("background:transparent;color:rgba(255, 255, 255,50%);");
        textlabel->adjustSize();
        is_pressed=false;
    }
}
