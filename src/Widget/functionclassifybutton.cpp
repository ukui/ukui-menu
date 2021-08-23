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
#include "src/UtilityFunction/utility.h"
#include <QDebug>
#include <syslog.h>
#include <QStylePainter>
#include <QStyleOptionButton>

FunctionClassifyButton::FunctionClassifyButton(int width,
                       int height,
                       int iconSize,
                       QString category,
                       bool fullscreen,
                       bool enabled,
                       QWidget *parent):
    QPushButton (parent),
    m_width(width),
    m_height(height),
    m_iconSize(iconSize),
    m_category(category),
    m_fullscreen(fullscreen),
    m_enabled(enabled),
    m_iconLabel(new QLabel),
    m_textLabel(new QLabel)
{
    this->setFlat(true);
    this->setFixedSize(m_width,m_height);
    this->setFocusPolicy(Qt::NoFocus);
    m_iconLabel->setFixedSize(m_iconSize,m_iconSize);
    m_textLabel->adjustSize();
    if(m_fullscreen)
    {
        QPalette pe = m_textLabel->palette();
        pe.setColor(QPalette::ButtonText,QColor(Qt::white));
        m_textLabel->setPalette(pe);
    }
    setLabelText();
    if(m_fullscreen)
    {
        updateIconState(Normal);
        this->setCheckable(true);
    }
    else
    {
        if(m_enabled)
            updateIconState(Enabled);
        else
            updateIconState(Disabled);
        this->setCheckable(false);
    }

    QHBoxLayout* mainlayout=new QHBoxLayout;
    mainlayout->setContentsMargins(Style::LeftSpaceIconLeft,0,0,0);
    mainlayout->setSpacing(Style::LeftSpaceIconText);
    this->setLayout(mainlayout);
    mainlayout->addWidget(m_iconLabel);
    mainlayout->addWidget(m_textLabel);
    connect(this,&FunctionClassifyButton::toggled,this,&FunctionClassifyButton::reactToToggle);
    connect(this,&FunctionClassifyButton::clicked,this,&FunctionClassifyButton::buttonClickedSlot);
}

void FunctionClassifyButton::paintEvent(QPaintEvent* e)
{
    Q_UNUSED(e);
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
    subopt.palette.setBrush(QPalette::HighlightedText, subopt.palette.text());
    painter.style()->drawControl(QStyle::CE_PushButtonLabel, &subopt, &painter, this);
    return;
}

void FunctionClassifyButton::updateBtnState()
{
    if(m_enabled)
        updateIconState(Enabled);
    else
        updateIconState(Disabled);
}

void FunctionClassifyButton::enterEvent(QEvent *e)
{
    Q_UNUSED(e);
    if(m_enabled)
    {
        updateIconState(Checked);
    }
}

void FunctionClassifyButton::leaveEvent(QEvent *e)
{
    Q_UNUSED(e);
    if(m_fullscreen)
    {
        if(!isChecked())
            updateIconState(Normal);
    }
    else
    {
        if(m_enabled)
            updateIconState(Enabled);
    }
}

void FunctionClassifyButton::reactToToggle(bool checked)
{
    if(m_fullscreen)
    {
        if(checked)
            updateIconState(Checked);
        else
            updateIconState(Normal);
    }
}

void FunctionClassifyButton::buttonClickedSlot()
{
    Q_EMIT buttonClicked();
}

void FunctionClassifyButton::updateIconState()
{
    this->setFlat(true);
    QString picState;
    switch (m_state)
    {
    case Enabled:   picState="Enabled"; break;
    case Disabled:  picState="Disabled"; break;
    case Normal:    picState="Normal"; break;
    case Checked:   picState="Checked"; break;
    default:        break;
    }

    const auto ratio = devicePixelRatioF();
    QPixmap pixmap = loadSvg(QString(":/data/img/mainviewwidget/%1-%2.svg").arg(m_category).arg(picState), m_iconSize*ratio);
    QGSettings gsetting(QString("org.ukui.style").toLocal8Bit());
    if(gsetting.get("style-name").toString()=="ukui-light")//反黑
    {
        pixmap=drawSymbolicBlackColoredPixmap(pixmap);
    }
    else
    {
        pixmap=drawSymbolicColoredPixmap(pixmap);//反白
    }
    pixmap.setDevicePixelRatio(qApp->devicePixelRatio());
    m_iconLabel->setPixmap(pixmap);
}

void FunctionClassifyButton::updateIconState(const FunctionClassifyButton::State state)
{
    if (state == m_state)
        return;
    m_state = state;

    QString picState;
    switch (state)
    {
    case Enabled:   picState="Enabled"; break;
    case Disabled:  picState="Disabled"; break;
    case Normal:    picState="Normal"; break;
    case Checked:   picState="Checked"; break;
    default:        break;
    }

    const auto ratio = devicePixelRatioF();
    QPixmap pixmap = loadSvg(QString(":/data/img/mainviewwidget/%1-%2.svg").arg(m_category).arg(picState), m_iconSize*ratio);
    if(!m_fullscreen)
    {
        QGSettings gsetting(QString("org.ukui.style").toLocal8Bit());
        if(gsetting.get("style-name").toString()=="ukui-light")//反黑
        {
            pixmap=drawSymbolicBlackColoredPixmap(pixmap);
        }
        else
        {
            pixmap=drawSymbolicColoredPixmap(pixmap);//反白
        }
    }
    else
        pixmap=drawSymbolicColoredPixmap(pixmap);//反白
    pixmap.setDevicePixelRatio(qApp->devicePixelRatio());
    m_iconLabel->setPixmap(pixmap);
    updateTextState(state);
}

void FunctionClassifyButton::updateTextState(const FunctionClassifyButton::State state)
{
//    QPalette p= m_textLabel->palette();
    switch (state)
    {
    case Enabled:
//        p.setColor(QPalette::WindowText,QColor::fromRgbF(1, 1, 1));
//        m_textLabel->setStyleSheet("background:transparent; color:rgba(255, 255, 255);");
        break;
    case Disabled:
//        p.setColor(QPalette::WindowText,QColor::fromRgbF(1, 1, 1, 0.25));
//        m_textLabel->setStyleSheet("background:transparent; color:rgba(255, 255, 255, 25%);");
        break;
    case Normal:
//        p.setColor(QPalette::WindowText,QColor::fromRgbF(1, 1, 1, 0.50));
//        m_textLabel->setStyleSheet("background:transparent; color:rgba(255, 255, 255, 50%);");
        break;
    case Checked:
//        p.setColor(QPalette::WindowText,QColor::fromRgbF(1, 1, 1));
//        m_textLabel->setStyleSheet("background:transparent;color:rgba(255, 255, 255);");
        break;
    default:
        break;
    }
//    p.setColor(QPalette::Window,Qt::transparent);
//    m_textLabel->setPalette(p);
}

void FunctionClassifyButton::setLabelText()
{
    QMetaEnum metaEnum=QMetaEnum::fromType<FunctionClassifyButton::Category>();
    switch (metaEnum.keyToValue(m_category.toLocal8Bit().data())) {
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
