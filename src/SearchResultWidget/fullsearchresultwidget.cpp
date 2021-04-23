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
#include "src/UtilityFunction/utility.h"
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
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setFixedSize(Style::MainViewWidWidth,
                       Style::AppListWidHeight);

    QHBoxLayout* mainLayout=new QHBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    m_spaceItem=new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);
    mainLayout->addItem(m_spaceItem);
    m_listView=new FullListView(this,3);
    this->setLayout(mainLayout);

    m_scrollArea=new ScrollArea;
    m_scrollAreaWid=new ScrollAreaWid;
    m_scrollAreaWid->setAttribute(Qt::WA_TranslucentBackground);
    m_scrollArea->setFixedSize(Style::AppListWidWidth,this->height());
    m_scrollArea->setWidget(m_scrollAreaWid);
    m_scrollArea->setWidgetResizable(true);
    m_scrollAreaWidLayout=new QVBoxLayout;
    m_scrollAreaWidLayout->setContentsMargins(0,0,0,0);
    m_scrollAreaWidLayout->setSpacing(10);
    m_scrollAreaWid->setLayout(m_scrollAreaWidLayout);
    mainLayout->addWidget(m_scrollArea);
    m_scrollAreaWidLayout->addWidget(m_listView);
    m_scrollAreaWidLayout->addItem(new QSpacerItem(20,40,QSizePolicy::Fixed,QSizePolicy::Expanding));

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
    execApp(desktopfp);
}

void FullSearchResultWidget::updateAppListView(QVector<QStringList> arg)
{
    m_data.clear();
    Q_FOREACH(QStringList appinfo,arg)
        m_data.append(appinfo.at(0));
    m_listView->updateData(m_data);
    resizeScrollAreaControls();
}

void FullSearchResultWidget::resizeScrollAreaControls()
{
    QLayoutItem* widItem=m_scrollAreaWidLayout->itemAt(0);
    QWidget* wid=widItem->widget();
    FullListView* listview=qobject_cast<FullListView*>(wid);
    listview->adjustSize();
    int dividend=(m_scrollArea->width()-Style::SliderSize)/Style::AppListGridSizeWidth;
    int rowcount=0;
    if(listview->model()->rowCount()%dividend>0)
    {
        rowcount=listview->model()->rowCount()/dividend+1;
    }
    else
    {
        rowcount=listview->model()->rowCount()/dividend;

    }

    listview->setFixedSize(m_scrollArea->width()-Style::SliderSize+1,listview->gridSize().height()*rowcount);
    m_scrollArea->widget()->adjustSize();
}

void FullSearchResultWidget::repaintWidget()
{
    this->setFixedSize(Style::MainViewWidWidth,
                       Style::AppListWidHeight);
    m_scrollArea->setFixedSize(Style::AppListWidWidth,this->height());
//    this->layout()->setContentsMargins(Style::LeftWidWidth,0,0,0);
    m_scrollAreaWidLayout->removeWidget(m_listView);
    m_listView->setParent(nullptr);
    delete m_listView;
    m_listView=new FullListView(this,3);
    m_scrollAreaWidLayout->insertWidget(0,m_listView);

    m_data.clear();
    m_listView->addData(m_data);
    resizeScrollAreaControls();
    connect(m_listView,&FullListView::sendItemClickedSignal,this,&FullSearchResultWidget::execApplication);
    connect(m_listView,&FullListView::sendHideMainWindowSignal,this,&FullSearchResultWidget::sendHideMainWindowSignal);
}

void FullSearchResultWidget::moveScrollBar(int type)
{
    int height=Style::primaryScreenHeight;
    if(type==0)
        m_listView->verticalScrollBar()->setSliderPosition(m_listView->verticalScrollBar()->sliderPosition()-height*100/1080);
    else
        m_listView->verticalScrollBar()->setSliderPosition(m_listView->verticalScrollBar()->sliderPosition()+height*100/1080);
}
