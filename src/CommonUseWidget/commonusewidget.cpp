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
    connect(listview,SIGNAL(sendUpdateAppListSignal(QString,int)),this,SIGNAL(sendUpdateAppListSignal(QString,int)));
    connect(listview,SIGNAL(removeListItemSignal(QString)),this,SIGNAL(removeListItemSignal(QString)));
    connect(listview,SIGNAL(removeListAllItemSignal()),this,SIGNAL(removeListAllItemSignal()));
    connect(listview,SIGNAL(sendHideMainWindowSignal()),this,SIGNAL(sendHideMainWindowSignal()));
}

/**
 * 填充应用列表
 */
void CommonUseWidget::fillAppList()
{
    getCommonUseAppList();
    listview->addData(data);
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
void CommonUseWidget::updateListViewSlot(QString desktopfp, int type)
{
    for(int i=0;i<listview->model()->rowCount();i++)
    {
        QVariant var=listview->model()->index(i,0).data(Qt::DisplayRole);
        QString path=var.value<QStringList>().at(0);
        if(QString::compare(path,desktopfp)==0)
        {
            listview->model()->removeRow(i);
            break;
        }
    }
    setting->beginGroup("lockapplication");
    QStandardItem* item=new QStandardItem;
    item->setData(QVariant::fromValue<QStringList>(QStringList()<<desktopfp<<"1"),Qt::DisplayRole);
    QStandardItemModel* listmodel=qobject_cast<QStandardItemModel*>(listview->model());
    if(type==0)
        listmodel->insertRow(setting->allKeys().size()-1,item);
    else
        listmodel->insertRow(setting->allKeys().size(),item);
    setting->endGroup();
}

void CommonUseWidget::updateListViewAllSlot()
{
    getCommonUseAppList();
    listview->updateData(data);
}

void CommonUseWidget::removeListItemSlot(QString desktopfp)
{
    for(int i=0;i<listview->model()->rowCount();i++)
    {
        QVariant var=listview->model()->index(i,0).data(Qt::DisplayRole);
        QString path=var.value<QStringList>().at(0);
        if(QString::compare(path,desktopfp)==0)
        {
            listview->model()->removeRow(i);
            break;
        }
    }
}

void CommonUseWidget::removeListAllItemSlot()
{
    setting->beginGroup("lockapplication");
    for(int i=listview->model()->rowCount()-1;i>=0;i--)
    {
        QVariant var=listview->model()->index(i,0).data(Qt::DisplayRole);
        QString desktopfp=var.value<QStringList>().at(0);
        QFileInfo fileInfo(desktopfp);
        QString desktopfn=fileInfo.fileName();
        if(!setting->contains(desktopfn))
            listview->model()->removeRow(i);
    }
    setting->endGroup();
}

void CommonUseWidget::getCommonUseAppList()
{
    QStringList desktopfnList;
    desktopfnList.clear();
    setting->beginGroup("lockapplication");
    QStringList lockdesktopfnList=setting->allKeys();
    for(int i=0;i<lockdesktopfnList.count()-1;i++)
        for(int j=0;j<lockdesktopfnList.count()-1-i;j++)
        {
            int value_1=setting->value(lockdesktopfnList.at(j)).toInt();
            int value_2=setting->value(lockdesktopfnList.at(j+1)).toInt();
            if(value_1 > value_2)
            {
                QString tmp=lockdesktopfnList.at(j);
                lockdesktopfnList.replace(j,lockdesktopfnList.at(j+1));
                lockdesktopfnList.replace(j+1,tmp);

            }
        }
    setting->endGroup();
    setting->beginGroup("application");
    Q_FOREACH(QString desktopfn,setting->childKeys())
    {
        if(setting->value(desktopfn)==2)
            desktopfnList.append(desktopfn);
    }
    setting->endGroup();
    data.clear();
    Q_FOREACH(QString desktopfn,lockdesktopfnList)
    {
        QString desktopfp=QString("/usr/share/applications/"+desktopfn);
        data.append(QStringList()<<desktopfp<<"1");
    }
    Q_FOREACH(QString desktopfn,desktopfnList)
    {
        QString desktopfp=QString("/usr/share/applications/"+desktopfn);
        data.append(QStringList()<<desktopfp<<"1");
    }
}

void CommonUseWidget::widgetMakeZero()
{
    listview->verticalScrollBar()->setSliderPosition(0);
}
