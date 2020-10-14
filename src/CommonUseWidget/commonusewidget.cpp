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
    initUi();

}

CommonUseWidget::~CommonUseWidget()
{
    delete m_ukuiMenuInterface;
}

void CommonUseWidget::initUi()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFixedSize(Style::defaultMainViewWidWidth,Style::defaultContentWidHeight);

    m_ukuiMenuInterface=new UkuiMenuInterface;

    initAppListWidget();
    fillAppList();
}

/**
 * 初始化应用列表界面
 */
void CommonUseWidget::initAppListWidget()
{
    m_listView=new ListView(this,this->width()-6,this->height()-6,0);
    m_listView->setGeometry(QRect(6,0,this->width()-6,this->height()-6));
    m_listView->show();
    connect(m_listView,&ListView::sendItemClickedSignal,this,&CommonUseWidget::execApplication);
    connect(m_listView,&ListView::sendUpdateAppListSignal,this,&CommonUseWidget::updateListViewSlot);
    connect(m_listView,&ListView::sendHideMainWindowSignal,this,&CommonUseWidget::sendHideMainWindowSignal);
}

/**
 * 填充应用列表
 */
void CommonUseWidget::fillAppList()
{
    m_data.clear();
    Q_FOREACH(QString desktopfp,UkuiMenuInterface::allAppVector)
        m_data.append(QStringList()<<desktopfp<<"1");
    m_listView->addData(m_data);
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
    m_data.clear();
    Q_FOREACH(QString desktopfp,m_ukuiMenuInterface->getAllClassification())
        m_data.append(QStringList()<<desktopfp<<"1");
    m_listView->updateData(m_data);
}

void CommonUseWidget::widgetMakeZero()
{
    m_listView->verticalScrollBar()->setSliderPosition(0);
}

void CommonUseWidget::moveScrollBar(int type)
{
    if(type==0)
        m_listView->verticalScrollBar()->setSliderPosition(m_listView->verticalScrollBar()->sliderPosition()-100);
    else
        m_listView->verticalScrollBar()->setSliderPosition(m_listView->verticalScrollBar()->sliderPosition()+100);
}

void CommonUseWidget::repaintWidget()
{
    this->setFixedSize(Style::defaultMainViewWidWidth,Style::defaultContentWidHeight);
    m_listView->setGeometry(QRect(6,0,this->width()-6,this->height()-6));
    m_listView->show();
}
