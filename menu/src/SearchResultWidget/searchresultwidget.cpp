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
    init_widget();
}

SearchResultWidget::~SearchResultWidget()
{
    delete ui;
    delete pUkuiMenuInterface;
}

void SearchResultWidget::init_widget()
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
    itemdelegate=new ItemDelegate(this,3);
    listview->setItemDelegate(itemdelegate);
    listview->addData(data);
    pUkuiMenuInterface=new UkuiMenuInterface;

    connect(listview,SIGNAL(sendItemClickedSignal(QStringList)),this,SLOT(exec_app_name(QStringList)));
    connect(listview,SIGNAL(sendFixedOrUnfixedSignal()),this,SIGNAL(send_update_applist_signal()));
    connect(listview,SIGNAL(send_hide_mainwindow_signal()),this,SIGNAL(send_hide_mainwindow_signal()));
}

/**
 * 执行应用程序
 */
void SearchResultWidget::exec_app_name(QStringList arg)
{
    emit send_hide_mainwindow_signal();
    QString execpath=pUkuiMenuInterface->get_app_exec(arg.at(0));
    //移除启动参数%u或者%U
    if(execpath.contains("%"))
    {
        int index=execpath.indexOf(QString("%").at(0));
        execpath.remove(index-1,3);
    }
    QProcess::startDetached(execpath);
}

void SearchResultWidget::update_app_listview(QStringList desktopfplist)
{
    data.clear();
    for(int i=0;i<desktopfplist.count();i++)
    {
        data.append(QStringList()<<desktopfplist.at(i)<<"1");
    }
    listview->updateData(data);
//    itemdelegate=new FullItemDelegate(this,3);
//    listview->setItemDelegate(itemdelegate);
//    listview->addData(data);
}
