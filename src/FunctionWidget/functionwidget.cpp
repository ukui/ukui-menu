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

#include "functionwidget.h"
#include "ui_functionwidget.h"
#include "src/Style/style.h"
#include <QDebug>

FunctionWidget::FunctionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FunctionWidget)
{
    ui->setupUi(this);

    initWidget();
}

FunctionWidget::~FunctionWidget()
{
    delete ui;
    delete pUkuiMenuInterface;
}

/**
 * 主界面初始化
 */
void FunctionWidget::initWidget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setStyleSheet("border:0px;background:transparent;");
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setFixedSize(320,535);

    mainLayout=new QHBoxLayout(this);
    mainLayout->setContentsMargins(2,0,2,0);
    mainLayout->setSpacing(0);
    this->setLayout(mainLayout);
    pUkuiMenuInterface=new UkuiMenuInterface;

    initAppListWidget();

}

/**
 * 初始化应用列表界面
 */
void FunctionWidget::initAppListWidget()
{
    applistview=new ListView(this,this->width()-4,this->height(),2);
    mainLayout->addWidget(applistview);
    fillAppListView();
    connect(applistview,SIGNAL(sendItemClickedSignal(QStringList)),this,SLOT(recvItemClickedSlot(QStringList)));
    connect(applistview,SIGNAL(sendFixedOrUnfixedSignal(QString,int)),this,SIGNAL(sendUpdateAppListSignal(QString,int)));
    connect(applistview,SIGNAL(sendHideMainWindowSignal()),this,SIGNAL(sendHideMainWindowSignal()));

}


/**
 * 填充应用列表
 */
void FunctionWidget::fillAppListView()
{
    classificationbtnlist.clear();
    classificationbtnrowlist.clear();

    QVector<QStringList> vector=UkuiMenuInterface::functionalVector;
    QStringList recentlist=vector.at(0);
    if(!recentlist.isEmpty())
    {
        insertClassificationBtn(tr("Recently"));
        insertAppList(recentlist);
    }

    QStringList netlist=vector.at(1);
    if(!netlist.isEmpty())
    {
        insertClassificationBtn(tr("Internet"));
        insertAppList(netlist);
    }
    QStringList sociallist=vector.at(2);
    if(!sociallist.isEmpty())
    {
        insertClassificationBtn(tr("Social"));
        insertAppList(sociallist);
    }
    QStringList avlist=vector.at(3);
    if(!avlist.isEmpty())
    {
        insertClassificationBtn(tr("Video"));
        insertAppList(avlist);
    }
    QStringList developlist=vector.at(4);
    if(!developlist.isEmpty())
    {
        insertClassificationBtn(tr("Development"));
        insertAppList(developlist);
    }
    QStringList graphicslist=vector.at(5);
    if(!graphicslist.isEmpty())
    {
        insertClassificationBtn(tr("Image"));
        insertAppList(graphicslist);
    }
    QStringList gamelist=vector.at(6);
    if(!gamelist.isEmpty())
    {
        insertClassificationBtn(tr("Game"));
        insertAppList(gamelist);
    }
    QStringList officelist=vector.at(7);
    if(!officelist.isEmpty())
    {
        insertClassificationBtn(tr("Office"));
        insertAppList(officelist);
    }
    QStringList educationlist=vector.at(8);
    if(!educationlist.isEmpty())
    {
        insertClassificationBtn(tr("Education"));

        insertAppList(educationlist);
    }

    QStringList systemadminlist=vector.at(9);
    if(!systemadminlist.isEmpty())
    {
        insertClassificationBtn(tr("System"));
        insertAppList(systemadminlist);
    }
    QStringList otherlist=vector.at(10);
    if(!otherlist.isEmpty())
    {
        insertClassificationBtn(tr("Others"));
        insertAppList(otherlist);
    }

    applistview->addData(data);
}

void FunctionWidget::insertClassificationBtn(QString btnname)
{
    classificationbtnlist.append(btnname);
    data.append(QStringList()<<btnname<<"0");
    classificationbtnrowlist.append(QString::number(row));
}

void FunctionWidget::insertAppList(QStringList appnamelist)
{
    row+=(appnamelist.count()+1);
    for(int i=0;i<appnamelist.count();i++)
    {

        QString desktopfp=pUkuiMenuInterface->getDesktopPathByAppName(appnamelist.at(i));
        data.append(QStringList()<<desktopfp<<"1");
    }
}

void FunctionWidget::recvItemClickedSlot(QStringList arg)
{
    if(arg.at(1).toInt()==0)
    {
        appClassificationBtnClickedSlot();
    }
    else{
        QString exec=pUkuiMenuInterface->getAppExec(arg.at(0));
        execApplication(exec);
    }

}

/**
 * 执行应用程序
 */
void FunctionWidget::execApplication(QString exec)
{
    Q_EMIT sendHideMainWindowSignal();
    //移除启动参数%u或者%U
    if(exec.contains("%"))
    {
        int index=exec.indexOf(QString("%").at(0));
        exec.remove(index-1,3);
    }
    QProcess::startDetached(exec);
}

/**
 * 更新应用列表
 */
void FunctionWidget::updateAppListView()
{
    classificationbtnlist.clear();
    classificationbtnrowlist.clear();
    row=0;
    data.clear();

    QVector<QStringList> vector=UkuiMenuInterface::functionalVector;
    QStringList recentlist=vector.at(0);
    if(!recentlist.isEmpty())
    {
        insertClassificationBtn(tr("Recently"));
        insertAppList(recentlist);
    }

    QStringList netlist=vector.at(1);
    if(!netlist.isEmpty())
    {
        insertClassificationBtn(tr("Internet"));
        insertAppList(netlist);
    }
    QStringList sociallist=vector.at(2);
    if(!sociallist.isEmpty())
    {
        insertClassificationBtn(tr("Social"));
        insertAppList(sociallist);
    }
    QStringList avlist=vector.at(3);
    if(!avlist.isEmpty())
    {
        insertClassificationBtn(tr("Video"));
        insertAppList(avlist);
    }
    QStringList developlist=vector.at(4);
    if(!developlist.isEmpty())
    {
        insertClassificationBtn(tr("Development"));
        insertAppList(developlist);
    }
    QStringList graphicslist=vector.at(5);
    if(!graphicslist.isEmpty())
    {
        insertClassificationBtn(tr("Image"));
        insertAppList(graphicslist);
    }
    QStringList gamelist=vector.at(6);
    if(!gamelist.isEmpty())
    {
        insertClassificationBtn(tr("Game"));
        insertAppList(gamelist);
    }
    QStringList officelist=vector.at(7);
    if(!officelist.isEmpty())
    {
        insertClassificationBtn(tr("Office"));
        insertAppList(officelist);
    }
    QStringList educationlist=vector.at(8);
    if(!educationlist.isEmpty())
    {
        insertClassificationBtn(tr("Education"));

        insertAppList(educationlist);
    }

    QStringList systemadminlist=vector.at(9);
    if(!systemadminlist.isEmpty())
    {
        insertClassificationBtn(tr("System"));
        insertAppList(systemadminlist);
    }
    QStringList otherlist=vector.at(10);
    if(!otherlist.isEmpty())
    {
        insertClassificationBtn(tr("Others"));
        insertAppList(otherlist);
    }
    applistview->updateData(data);
}

void FunctionWidget::updateListView()
{
    UkuiMenuInterface::functionalVector=pUkuiMenuInterface->getFunctionalClassification();
    updateAppListView();
}

/**
 * 应用列表功能分类按钮槽函数
 */
void FunctionWidget::appClassificationBtnClickedSlot()
{
    //加载FunctionButtonWidget界面
    functionbtnwid=new FunctionButtonWidget(this);
    connect(this,SIGNAL(sendClassificationbtnList(QStringList)),functionbtnwid,SLOT(recvClassificationBtnList(QStringList)));
    Q_EMIT sendClassificationbtnList(classificationbtnlist);
    mainLayout->removeWidget(applistview);
    applistview->setParent(nullptr);
    mainLayout->addWidget(functionbtnwid);

    connect(functionbtnwid, SIGNAL(sendFunctionBtnSignal(QString)),this,SLOT(recvFunctionBtnSignal(QString)));
}

/**
 * 接收FunctionButtonWidget界面按钮信号
 */
void FunctionWidget::recvFunctionBtnSignal(QString btnname)
{
    mainLayout->removeWidget(functionbtnwid);
    functionbtnwid->setParent(nullptr);
    if(functionbtnwid!=nullptr)
    {

        delete functionbtnwid;
        functionbtnwid=nullptr;
    }
    mainLayout->addWidget(applistview);

    //此处需实现将功能为btnname的应用列表移动到applistWid界面最顶端
    int index=classificationbtnlist.indexOf(btnname);
    if(index!=-1)
    {
        int row=classificationbtnrowlist.at(index).toInt();
        applistview->verticalScrollBar()->setValue(row);
    }
}

void FunctionWidget::widgetMakeZero()
{
    if(functionbtnwid!=nullptr)
    {
        mainLayout->removeWidget(functionbtnwid);
        functionbtnwid->setParent(nullptr);
        delete functionbtnwid;
        functionbtnwid=nullptr;
        mainLayout->addWidget(applistview);
    }
    applistview->verticalScrollBar()->setValue(0);
}

void FunctionWidget::moveScrollBar(int type)
{
    if(type==0)
        applistview->verticalScrollBar()->setSliderPosition(applistview->verticalScrollBar()->sliderPosition()-1);
    else
        applistview->verticalScrollBar()->setSliderPosition(applistview->verticalScrollBar()->sliderPosition()+1);
}
