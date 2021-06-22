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

#include "fullcommonusewidget.h"
#include <QDesktopWidget>
#include "src/Style/style.h"
#include "src/UtilityFunction/utility.h"
#include <QDebug>

FullCommonUseWidget::FullCommonUseWidget(QWidget *parent) :
    QWidget(parent)
{
    initUi();
}

FullCommonUseWidget::~FullCommonUseWidget()
{
    delete m_ukuiMenuInterface;
}

void FullCommonUseWidget::initUi()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setFixedSize(Style::MainViewWidWidth,
                       Style::AppListWidHeight);
    this->setFocusPolicy(Qt::NoFocus);
    QHBoxLayout* mainLayout=new QHBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    this->setLayout(mainLayout);
    m_spaceItem=new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);
    mainLayout->addItem(m_spaceItem);
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

    m_ukuiMenuInterface=new UkuiMenuInterface;

    initAppListWidget();
    fillAppList();

    flag = true;
    //翻页灵敏度时间调节
    time = new QTimer(this);
    connect(time,&QTimer::timeout,[=](){
        if(flag == false)
        {
            flag = true;
            time->stop();
        }
    });
}

void FullCommonUseWidget::initAppListWidget()
{
    m_listView=new FullListView(this,0);
//    m_listView->setFixedSize(this->width()-Style::LeftWidWidth+3,this->height());
//    QHBoxLayout *mainLayout=qobject_cast<QHBoxLayout*>(this->layout());
//    mainLayout->insertWidget(1,m_listView);
    m_scrollAreaWidLayout->addWidget(m_listView);
    connect(m_listView,&FullListView::sendItemClickedSignal,this,&FullCommonUseWidget::execApplication);
    connect(m_listView,&FullListView::sendUpdateAppListSignal,this,&FullCommonUseWidget::updateListViewSlot);
    connect(m_listView,&FullListView::sendHideMainWindowSignal,this,&FullCommonUseWidget::sendHideMainWindowSignal);
    connect(m_listView,&FullListView::sendSetslidebar,this,&FullCommonUseWidget::onSetSlider);
}

void FullCommonUseWidget::resizeScrollAreaControls()
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

void FullCommonUseWidget::fillAppList()
{
    m_data.clear();
    Q_FOREACH(QString desktopfp,UkuiMenuInterface::allAppVector)
        m_data.append(desktopfp);
    m_listView->addData(m_data);
    resizeScrollAreaControls();
}

/**
 * 执行应用程序
 */
void FullCommonUseWidget::execApplication(QString desktopfp)
{
    Q_EMIT sendHideMainWindowSignal();
    execApp(desktopfp);
}

void FullCommonUseWidget::onSetSlider(int value)
{
//    if(flag)
//    {
//        flag = false;
//        time->start(100);
        if(value == 0)
        {
            m_scrollArea->verticalScrollBar()->setValue(0);
        }
        else
        {
            int curvalue = m_scrollArea->verticalScrollBar()->value();
            m_scrollArea->verticalScrollBar()->setValue(curvalue + value);
        }
//    }
}
/**
 * 更新应用列表
 */
void FullCommonUseWidget::updateListViewSlot()
{
    updateListView();
}

void FullCommonUseWidget::updateListView()
{
    m_data.clear();
    Q_FOREACH(QString desktopfp,m_ukuiMenuInterface->getAllClassification())
        m_data.append(desktopfp);
    m_listView->updateData(m_data);
}

void FullCommonUseWidget::repaintWidget()
{
    this->setFixedSize(Style::MainViewWidWidth,
                       Style::AppListWidHeight);
    m_scrollArea->setFixedSize(Style::AppListWidWidth,this->height());
    m_scrollAreaWidLayout->removeWidget(m_listView);
    m_listView->setParent(nullptr);
    delete m_listView;
    initAppListWidget();
    fillAppList();
}

void FullCommonUseWidget::widgetMakeZero()
{
    m_listView->verticalScrollBar()->setSliderPosition(0);
}

void FullCommonUseWidget::moveScrollBar(int type)
{
    int height=Style::primaryScreenHeight;
    if(type==0)
        m_listView->verticalScrollBar()->setSliderPosition(m_listView->verticalScrollBar()->sliderPosition()-height*100/1080);
    else
        m_listView->verticalScrollBar()->setSliderPosition(m_listView->verticalScrollBar()->sliderPosition()+height*100/1080);
}
