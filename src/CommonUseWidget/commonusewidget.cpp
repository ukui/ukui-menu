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

#include "commonusewidget.h"
#include "src/Style/style.h"
#include <QDBusInterface>
#include <QDBusReply>
#include <QDir>
#include <QDebug>

CommonUseWidget::CommonUseWidget(QWidget *parent) :
    QWidget(parent)
{
    initWidget();

}

CommonUseWidget::~CommonUseWidget()
{
    delete pUkuiMenuInterface;
}

void CommonUseWidget::initWidget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setStyleSheet("border:0px;background:transparent;");
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setFixedSize(320,535);

    mainLayout=new  QHBoxLayout(this);
    mainLayout->setContentsMargins(2,0,2,0);
    mainLayout->setSpacing(0);
    this->setLayout(mainLayout);

    pUkuiMenuInterface=new UkuiMenuInterface;

    QString path=QDir::homePath()+"/.config/ukui/ukui-menu.ini";
    setting=new QSettings(path,QSettings::IniFormat);

    initAppListWidget();
    fillAppList();
}

/**
 * 初始化应用列表界面
 */
void CommonUseWidget::initAppListWidget()
{
    listview=new ListView(this,this->width()-4,this->height(),0);
    mainLayout->addWidget(listview);
    connect(listview,&ListView::sendItemClickedSignal,this,&CommonUseWidget::execApplication);
    connect(listview,&ListView::sendHideMainWindowSignal,this,&CommonUseWidget::sendHideMainWindowSignal);
    connect(listview,&ListView::sendUpdateAppListSignal,this,&CommonUseWidget::updateListViewSlot);
}

/**
 * 填充应用列表
 */
void CommonUseWidget::fillAppList()
{
    data.clear();
    Q_FOREACH(QString desktopfp,UkuiMenuInterface::allAppVector)
    {
        data.append(QStringList()<<desktopfp<<"1");
    }
    listview->addData(data);
}

/**
 * 执行应用程序
 */
void CommonUseWidget::execApplication(QStringList arg)
{
    Q_EMIT sendHideMainWindowSignal();
    QString desktopfp=arg.at(0);
    GDesktopAppInfo * desktopAppInfo=g_desktop_app_info_new_from_filename(desktopfp.toLocal8Bit().data());
    g_app_info_launch(G_APP_INFO(desktopAppInfo),nullptr, nullptr, nullptr);
    g_object_unref(desktopAppInfo);
}


/**
 * 更新应用列表槽函数
 */
void CommonUseWidget::updateListViewSlot()
{
    updateListView();
}

void CommonUseWidget::updateListView()
{
    data.clear();
    Q_FOREACH(QString desktopfp,pUkuiMenuInterface->getAllApp())
    {
        data.append(QStringList()<<desktopfp<<"1");
    }
    listview->updateData(data);
}

void CommonUseWidget::widgetMakeZero()
{
    listview->verticalScrollBar()->setSliderPosition(0);
}

void CommonUseWidget::moveScrollBar(int type)
{
    if(type==0)
        listview->verticalScrollBar()->setSliderPosition(listview->verticalScrollBar()->sliderPosition()-100);
    else
        listview->verticalScrollBar()->setSliderPosition(listview->verticalScrollBar()->sliderPosition()+100);
}
