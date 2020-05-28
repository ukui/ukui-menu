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
    this->setStyleSheet("border:0px;background:transparent;");
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setFixedSize(Style::MainViewWidWidth,
                       Style::AppListWidHeight);

    QVBoxLayout* mainLayout=new QVBoxLayout;
    mainLayout->setContentsMargins(Style::LeftWidWidth,0,0,0);
    this->setLayout(mainLayout);

    m_ukuiMenuInterface=new UkuiMenuInterface;

    initAppListWidget();
    fillAppList();
}

void FullCommonUseWidget::initAppListWidget()
{
    listview=new FullListView(this,0);
    this->layout()->addWidget(listview);
    connect(listview,&FullListView::sendItemClickedSignal,this,&FullCommonUseWidget::execApplication);
    connect(listview,&FullListView::sendUpdateAppListSignal,this,&FullCommonUseWidget::updateListViewSlot);
}

void FullCommonUseWidget::fillAppList()
{
    m_data.clear();
    Q_FOREACH(QString desktopfp,UkuiMenuInterface::allAppVector)
        m_data.append(desktopfp);
    listview->addData(m_data);
}

/**
 * 执行应用程序
 */
void FullCommonUseWidget::execApplication(QString desktopfp)
{
    this->parentWidget()->parentWidget()->parentWidget()->hide();
    GDesktopAppInfo * desktopAppInfo=g_desktop_app_info_new_from_filename(desktopfp.toLocal8Bit().data());
    g_app_info_launch(G_APP_INFO(desktopAppInfo),nullptr, nullptr, nullptr);
    g_object_unref(desktopAppInfo);}

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
    Q_FOREACH(QString desktopfp,m_ukuiMenuInterface->getAllApp())
        m_data.append(desktopfp);
    listview->updateData(m_data);
}

void FullCommonUseWidget::repaintWidget()
{
    this->setFixedSize(Style::MainViewWidWidth,
                       Style::AppListWidHeight);
    this->layout()->removeWidget(listview);
    listview->setParent(nullptr);
    initAppListWidget();
    fillAppList();
}

void FullCommonUseWidget::widgetMakeZero()
{
    listview->verticalScrollBar()->setSliderPosition(0);
}

void FullCommonUseWidget::moveScrollBar(int type)
{
    int height=QApplication::primaryScreen()->geometry().height();
    if(type==0)
        listview->verticalScrollBar()->setSliderPosition(listview->verticalScrollBar()->sliderPosition()-height*100/1080);
    else
        listview->verticalScrollBar()->setSliderPosition(listview->verticalScrollBar()->sliderPosition()+height*100/1080);
}
