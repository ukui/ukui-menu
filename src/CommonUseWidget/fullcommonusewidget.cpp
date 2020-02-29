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
#include "ui_fullcommonusewidget.h"
#include <QDesktopWidget>
#include "src/Style/style.h"
#include <QDebug>

FullCommonUseWidget::FullCommonUseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FullCommonUseWidget)
{
    ui->setupUi(this);
    initWidget();
}

FullCommonUseWidget::~FullCommonUseWidget()
{
    delete ui;
    delete pUkuiMenuInterface;
}

void FullCommonUseWidget::initWidget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setStyleSheet("border:0px;background:transparent;");
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setFixedSize(Style::MainViewWidWidth,
                       Style::AppListWidHeight);

    mainLayout=new QVBoxLayout(this);
    this->setLayout(mainLayout);

    pUkuiMenuInterface=new UkuiMenuInterface;

    QString path=QDir::homePath()+"/.config/ukui/ukui-menu.ini";
    setting=new QSettings(path,QSettings::IniFormat);

    initAppListWidget();
    fillAppList();
}

void FullCommonUseWidget::initAppListWidget()
{
    mainLayout->setContentsMargins(Style::LeftWidWidth,0,0,0);
    listview=new FullListView(this,0);
    mainLayout->addWidget(listview);
    connect(listview,SIGNAL(sendItemClickedSignal(QString)),this,SLOT(execApplication(QString)));
    connect(listview,SIGNAL(sendUpdateAppListSignal()),this,SIGNAL(sendUpdateAppListSignal()));
    connect(listview,SIGNAL(sendHideMainWindowSignal()),this,SIGNAL(sendHideMainWindowSignal()));
}

void FullCommonUseWidget::fillAppList()
{
    QStringList keys;
    keys.clear();
    setting->beginGroup("application");
    keys=setting->childKeys();
    QStringList applist;
    applist.clear();
    for(int i=0;i<keys.count();i++)
    {
        int val=setting->value(keys.at(i)).toInt();
        if(val==2 || val==0)
            applist.append(keys.at(i));
    }

    data.clear();
    for(int i=0;i<applist.count();i++)
    {
//        QString desktopfp=pUkuiMenuInterface->getDesktopPathByAppName(applist.at(i));
        QString desktopfp=QString("/usr/share/applications/"+applist.at(i));
        data.append(desktopfp);
    }

    listview->addData(data);
    setting->endGroup();
}

/**
 * 执行应用程序
 */
void FullCommonUseWidget::execApplication(QString appname)
{
    Q_EMIT sendHideMainWindowSignal();
    QString execpath=pUkuiMenuInterface->getAppExec(pUkuiMenuInterface->getDesktopPathByAppName(appname));
    //移除启动参数%u或者%U
    if(execpath.contains("%"))
    {
        int index=execpath.indexOf(QString("%").at(0));
        execpath.remove(index-1,3);
    }
    QProcess::startDetached(execpath);
}

/**
 * 更新应用列表
 */
void FullCommonUseWidget::updateListViewSlot()
{
    QStringList keys;
    keys.clear();
    setting->beginGroup("application");
    keys=setting->childKeys();
    QStringList applist;
    applist.clear();
    for(int i=0;i<keys.count();i++)
    {
        int val=setting->value(keys.at(i)).toInt();
        if(val==2 || val==0)
            applist.append(keys.at(i));
    }

    data.clear();
    for(int i=0;i<applist.count();i++)
    {
//        QString desktopfp=pUkuiMenuInterface->getDesktopPathByAppName(applist.at(i));
        QString desktopfp=QString("/usr/share/applications/"+applist.at(i));
        data.append(desktopfp);
    }

    listview->updateData(data);
    setting->endGroup();
}

void FullCommonUseWidget::widgetMakeZero()
{
    listview->verticalScrollBar()->setSliderPosition(0);
}
