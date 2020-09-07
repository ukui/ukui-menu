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

#include "fullsearchresultwidget.h"
#include <QHeaderView>
#include "src/Style/style.h"
#include <QDebug>

FullSearchResultWidget::FullSearchResultWidget(QWidget *parent) :
    QWidget(parent)
{
    initUi();
}

FullSearchResultWidget::~FullSearchResultWidget()
{
    delete m_ukuiMenuInterface;
}

void FullSearchResultWidget::initUi()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setStyleSheet("border:0px;background:transparent;");//transparent
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setFixedSize(Style::widthavailable,
                       Style::AppListWidHeight);//Style::MainViewWidWidth

    QHBoxLayout* mainLayout=new QHBoxLayout;
    mainLayout->setContentsMargins(Style::listleftMargin,0,Style::listleftMargin+6,0);//127listleftMargin=94;
    m_spaceItem=new QSpacerItem(0,20,QSizePolicy::Expanding,QSizePolicy::Fixed);//40-0
    mainLayout->addItem(m_spaceItem);
    m_listView=new FullListView(this,3);
    m_listView->setFixedSize(this->width()-Style::listleftMargin*2+6,this->height());//this->width()-Style::LeftWidWidth-Style::LeftWidWidth
    mainLayout->addWidget(m_listView);
    this->setLayout(mainLayout);

    m_data.clear();
    m_listView->addData(m_data);
    m_ukuiMenuInterface=new UkuiMenuInterface;

    connect(m_listView,&FullListView::sendItemClickedSignal,this,&FullSearchResultWidget::execApplication);
    connect(m_listView,&FullListView::sendHideMainWindowSignal,this,&FullSearchResultWidget::sendHideMainWindowSignal);



}

/**
 * 执行应用程序
 */
void FullSearchResultWidget::execApplication(QString desktopfp)
{
    Q_EMIT sendHideMainWindowSignal();
    GDesktopAppInfo * desktopAppInfo=g_desktop_app_info_new_from_filename(desktopfp.toLocal8Bit().data());
    g_app_info_launch(G_APP_INFO(desktopAppInfo),nullptr, nullptr, nullptr);
    g_object_unref(desktopAppInfo);
}

void FullSearchResultWidget::updateAppListView(QVector<QStringList> arg)
{
    m_data.clear();
    Q_FOREACH(QStringList appinfo,arg)
        m_data.append(appinfo.at(0));
    m_listView->updateData(m_data);
}

void FullSearchResultWidget::repaintWidget()
{
    this->setFixedSize(Style::widthavailable,
                       Style::AppListWidHeight);//MainViewWidWidth
    this->layout()->setContentsMargins(0,0,0,0);//Style::LeftWidWidth
    this->layout()->removeWidget(m_listView);
    m_listView->setParent(nullptr);
    delete m_listView;
    m_listView=new FullListView(this,3);
    m_listView->setFixedSize(this->width()-Style::listleftMargin*2+6,this->height());//this->width()-Style::LeftWidWidth//Style::widthavailable
    QHBoxLayout *mainLayout=qobject_cast<QHBoxLayout*>(this->layout());
    mainLayout->setContentsMargins(Style::listleftMargin,0,Style::listleftMargin+6,0);//127listleftMargin=94;
    mainLayout->insertWidget(0,m_listView);
    m_data.clear();
    m_listView->addData(m_data);
    connect(m_listView,&FullListView::sendItemClickedSignal,this,&FullSearchResultWidget::execApplication);
    connect(m_listView,&FullListView::sendHideMainWindowSignal,this,&FullSearchResultWidget::sendHideMainWindowSignal);
}

void FullSearchResultWidget::moveScrollBar(int type)
{
    int height=QApplication::primaryScreen()->geometry().height();
    if(type==0)
        m_listView->verticalScrollBar()->setSliderPosition(m_listView->verticalScrollBar()->sliderPosition()-height*100/1080);
    else
        m_listView->verticalScrollBar()->setSliderPosition(m_listView->verticalScrollBar()->sliderPosition()+height*100/1080);
}
