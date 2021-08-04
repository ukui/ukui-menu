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

#include "searchresultwidget.h"
#include "src/Style/style.h"
#include <QDebug>
#include <QHeaderView>
#include "src/UtilityFunction/utility.h"

SearchResultWidget::SearchResultWidget(QWidget *parent) :
    QWidget(parent)
{
    initUi();
}

SearchResultWidget::~SearchResultWidget()
{
    delete m_ukuiMenuInterface;
}

void SearchResultWidget::initUi()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setFixedSize(Style::defaultMainViewWidWidth,Style::defaultContentWidHeight);
    this->setFocusPolicy(Qt::StrongFocus);
    m_listView=new ListView(this);
    m_listView->setGeometry(QRect(6,0,this->width()-6,this->height()-6));
    m_listView->show();

    m_data.clear();
//    m_listView->addData(m_data);
    m_ukuiMenuInterface=new UkuiMenuInterface;

    connect(m_listView,&ListView::sendItemClickedSignal,this,&SearchResultWidget::execApplication);
    connect(m_listView,&ListView::sendHideMainWindowSignal,this,&SearchResultWidget::sendHideMainWindowSignal);
}

/**
 * 执行应用程序
 */
void SearchResultWidget::execApplication(QStringList arg)
{
    Q_EMIT sendHideMainWindowSignal();
    QString desktopfp=arg.at(0);
    execApp(desktopfp);
}

void SearchResultWidget::updateAppListView(QVector<QStringList> arg)
{
    m_data.clear();
    Q_FOREACH(QStringList appinfo,arg)
        m_data.append(QStringList()<<appinfo.at(0)<<"1");
    m_listView->updateData(m_data);
}

void SearchResultWidget::moveScrollBar(int type)
{
    if(type==0)
        m_listView->verticalScrollBar()->setSliderPosition(m_listView->verticalScrollBar()->sliderPosition()-100);
    else
        m_listView->verticalScrollBar()->setSliderPosition(m_listView->verticalScrollBar()->sliderPosition()+100);
}

void SearchResultWidget::repaintWidget()
{
    this->setFixedSize(Style::defaultMainViewWidWidth,Style::defaultContentWidHeight);
    m_listView->setGeometry(QRect(6,0,this->width()-6,this->height()-6));
    m_listView->show();
}

void SearchResultWidget::setViewFocus()
{
    m_listView->setFocus();
}
