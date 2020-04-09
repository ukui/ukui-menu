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
    connect(listview,SIGNAL(sendUpdateAppListSignal(QString,int)),this,SIGNAL(sendUpdateAppListSignal(QString,int)));
    connect(listview,SIGNAL(removeListItemSignal(QString)),this,SIGNAL(removeListItemSignal(QString)));
    connect(listview,SIGNAL(removeListAllItemSignal()),this,SIGNAL(removeListAllItemSignal()));
    connect(listview,SIGNAL(sendHideMainWindowSignal()),this,SIGNAL(sendHideMainWindowSignal()));
}

void FullCommonUseWidget::fillAppList()
{
    getCommonUseAppList();
    listview->addData(data);
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
void FullCommonUseWidget::updateListViewSlot(QString desktopfp, int type)
{
    for(int i=0;i<listview->model()->rowCount();i++)
    {
        QVariant var=listview->model()->index(i,0).data(Qt::DisplayRole);
        QString path=var.value<QString>();
        if(QString::compare(path,desktopfp)==0)
        {
            listview->model()->removeRow(i);
            break;
        }
    }
    setting->beginGroup("lockapplication");
    QStandardItem* item=new QStandardItem;
    item->setData(QVariant::fromValue<QString>(desktopfp),Qt::DisplayRole);
    QStandardItemModel* listmodel=qobject_cast<QStandardItemModel*>(listview->model());
    if(type==0)
        listmodel->insertRow(setting->allKeys().size()-1,item);
    else
        listmodel->insertRow(setting->allKeys().size(),item);
    setting->endGroup();
}

void FullCommonUseWidget::updateListViewAllSlot()
{
    getCommonUseAppList();
    listview->updateData(data);
}

void FullCommonUseWidget::removeListItemSlot(QString desktopfp)
{
    for(int i=0;i<listview->model()->rowCount();i++)
    {
        QVariant var=listview->model()->index(i,0).data(Qt::DisplayRole);
        QString path=var.value<QString>();
        if(QString::compare(path,desktopfp)==0)
        {
            listview->model()->removeRow(i);
            break;
        }
    }
}

void FullCommonUseWidget::removeListAllItemSlot()
{
    setting->beginGroup("lockapplication");
    for(int i=listview->model()->rowCount()-1;i>=0;i--)
    {
        QVariant var=listview->model()->index(i,0).data(Qt::DisplayRole);
        QString desktopfp=var.value<QString>();
        QFileInfo fileInfo(desktopfp);
        QString desktopfn=fileInfo.fileName();
        if(!setting->contains(desktopfn))
            listview->model()->removeRow(i);
    }
    setting->endGroup();
}

void FullCommonUseWidget::getCommonUseAppList()
{
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
    QStringList desktopfnList=setting->allKeys();
    for(int i=0;i<desktopfnList.count()-1;i++)
        for(int j=0;j<desktopfnList.count()-1-i;j++)
        {
            int value_1=setting->value(desktopfnList.at(j)).toInt();
            int value_2=setting->value(desktopfnList.at(j+1)).toInt();
            if(value_1 < value_2)
            {
                QString tmp=desktopfnList.at(j);
                desktopfnList.replace(j,desktopfnList.at(j+1));
                desktopfnList.replace(j+1,tmp);

            }
        }
    setting->endGroup();
    data.clear();
    Q_FOREACH(QString desktopfn,lockdesktopfnList)
    {
        QString desktopfp=QString("/usr/share/applications/"+desktopfn);
        data.append(desktopfp);
    }
    Q_FOREACH(QString desktopfn,desktopfnList)
    {
        QString desktopfp=QString("/usr/share/applications/"+desktopfn);
        data.append(desktopfp);
    }
}

void FullCommonUseWidget::widgetMakeZero()
{
    listview->verticalScrollBar()->setSliderPosition(0);
}
