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
#include "ui_fullsearchresultwidget.h"
#include <QHeaderView>
#include "src/Style/style.h"
#include <QDebug>

FullSearchResultWidget::FullSearchResultWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FullSearchResultWidget)
{
    ui->setupUi(this);
    initWidget();
}

FullSearchResultWidget::~FullSearchResultWidget()
{
    delete ui;
    delete pUkuiMenuInterface;
}

void FullSearchResultWidget::initWidget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setStyleSheet("border:0px;background:transparent;");
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setFixedSize(Style::MainViewWidWidth,
                       Style::AppListWidHeight);

    mainLayout=new QHBoxLayout(this);
    mainLayout->setContentsMargins(Style::LeftWidWidth,0,0,0);
    listview=new FullListView(this,3);
    mainLayout->addWidget(listview);
    this->setLayout(mainLayout);

    data.clear();
    listview->addData(data);
    pUkuiMenuInterface=new UkuiMenuInterface;

    connect(listview,&FullListView::sendItemClickedSignal,this,&FullSearchResultWidget::execApplication);
    connect(listview,&FullListView::sendHideMainWindowSignal,this,&FullSearchResultWidget::sendHideMainWindowSignal);
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

void FullSearchResultWidget::updateAppListView(QStringList desktopfplist)
{
    data.clear();
    data=desktopfplist;
    listview->updateData(data);
}

void FullSearchResultWidget::repaintWidget()
{
    this->setFixedSize(Style::MainViewWidWidth,
                       Style::AppListWidHeight);
    mainLayout->setContentsMargins(Style::LeftWidWidth,0,0,0);
    mainLayout->removeWidget(listview);
    listview->setParent(nullptr);
    delete listview;
    listview=new FullListView(this,3);
    mainLayout->addWidget(listview);
    data.clear();
    listview->addData(data);
    connect(listview,&FullListView::sendItemClickedSignal,this,&FullSearchResultWidget::execApplication);
    connect(listview,&FullListView::sendHideMainWindowSignal,this,&FullSearchResultWidget::sendHideMainWindowSignal);

}

void FullSearchResultWidget::moveScrollBar(int type)
{
    int height=QApplication::primaryScreen()->geometry().height();
    if(type==0)
        listview->verticalScrollBar()->setSliderPosition(listview->verticalScrollBar()->sliderPosition()-height*100/1080);
    else
        listview->verticalScrollBar()->setSliderPosition(listview->verticalScrollBar()->sliderPosition()+height*100/1080);
}
