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
#include "ui_searchresultwidget.h"
#include "src/Style/style.h"
#include <QDebug>
#include <QHeaderView>

SearchResultWidget::SearchResultWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchResultWidget)
{
    ui->setupUi(this);
    initWidget();
}

SearchResultWidget::~SearchResultWidget()
{
    delete ui;
    delete pUkuiMenuInterface;
}

void SearchResultWidget::initWidget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setStyleSheet("border:0px;background:transparent;");
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setFixedSize(320,535);

    mainLayout=new QHBoxLayout(this);
    mainLayout->setContentsMargins(2,0,2,0);
    listview=new ListView(this,this->width()-4,this->height(),3);
    mainLayout->addWidget(listview);
    this->setLayout(mainLayout);
    data.clear();
    listview->addData(data);
    pUkuiMenuInterface=new UkuiMenuInterface;

    connect(listview,&ListView::sendItemClickedSignal,this,&SearchResultWidget::execApplication);
    connect(listview,&ListView::sendHideMainWindowSignal,this,&SearchResultWidget::sendHideMainWindowSignal);
}

/**
 * 执行应用程序
 */
void SearchResultWidget::execApplication(QStringList arg)
{
    Q_EMIT sendHideMainWindowSignal();
    QString desktopfp=arg.at(0);
    GDesktopAppInfo * desktopAppInfo=g_desktop_app_info_new_from_filename(desktopfp.toLocal8Bit().data());
    g_app_info_launch(G_APP_INFO(desktopAppInfo),nullptr, nullptr, nullptr);
    g_object_unref(desktopAppInfo);
}

void SearchResultWidget::updateAppListView(QStringList desktopfplist)
{
    data.clear();
    for(int i=0;i<desktopfplist.count();i++)
    {
        data.append(QStringList()<<desktopfplist.at(i)<<"1");
    }
    listview->updateData(data);
}

void SearchResultWidget::moveScrollBar(int type)
{
    if(type==0)
        listview->verticalScrollBar()->setSliderPosition(listview->verticalScrollBar()->sliderPosition()-100);
    else
        listview->verticalScrollBar()->setSliderPosition(listview->verticalScrollBar()->sliderPosition()+100);
}
