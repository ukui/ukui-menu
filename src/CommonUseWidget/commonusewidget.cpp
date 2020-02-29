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
#include "ui_commonusewidget.h"
#include "src/Style/style.h"
#include <QDBusInterface>
#include <QDBusReply>
#include <QDir>
#include <QDebug>

CommonUseWidget::CommonUseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommonUseWidget)
{
    ui->setupUi(this);
    initWidget();

}

CommonUseWidget::~CommonUseWidget()
{
    delete ui;
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
    connect(listview,SIGNAL(sendItemClickedSignal(QStringList)),this,SLOT(execApplication(QStringList)));
    connect(listview,SIGNAL(sendUpdateAppListSignal()),this,SIGNAL(sendUpdateAppListSignal()));
    connect(listview,SIGNAL(sendHideMainWindowSignal()),this,SIGNAL(sendHideMainWindowSignal()));

}

/**
 * 填充应用列表
 */
void CommonUseWidget::fillAppList()
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
        data.append(QStringList()<<desktopfp<<"1");
    }

    m_delegate= new ItemDelegate(this,0);
    listview->setItemDelegate(m_delegate);
    listview->addData(data);
    setting->endGroup();
}

/**
 * 执行应用程序
 */
void CommonUseWidget::execApplication(QStringList arg)
{
    Q_EMIT sendHideMainWindowSignal();
    QString execpath=pUkuiMenuInterface->getAppExec(arg.at(0));
    //移除启动参数%u或者%U
    if(execpath.contains("%"))
    {
        int index=execpath.indexOf(QString("%").at(0));
        execpath.remove(index-1,3);
    }
    QProcess::startDetached(execpath);
}


/**
 * 更新应用列表槽函数
 */
void CommonUseWidget::updateListViewSlot()
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
        data.append(QStringList()<<desktopfp<<"1");
    }

    listview->updateData(data);
    setting->endGroup();

}

void CommonUseWidget::widgetMakeZero()
{
    listview->verticalScrollBar()->setSliderPosition(0);
}
