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

    pUkuiMenuInterface=new UkuiMenuInterface;
    initAppListWidget();

    functionbtnwid=new FunctionButtonWidget(this);
    connect(this,SIGNAL(sendClassificationbtnList(QStringList)),functionbtnwid,SLOT(recvClassificationBtnList(QStringList)));
    connect(functionbtnwid, SIGNAL(sendFunctionBtnSignal(QString)),this,SLOT(recvFunctionBtnSignal(QString)));
    functionbtnwid->setGeometry(QRect((this->width()-4)/2,(this->height())/2,0,0));

    enterAnimation=new QPropertyAnimation;
    enterAnimation->setDuration(50);
    enterAnimation->setPropertyName(QString("geometry").toLocal8Bit());
    leaveAnimation=new QPropertyAnimation;
    leaveAnimation->setDuration(50);
    leaveAnimation->setPropertyName(QString("geometry").toLocal8Bit());
    connect(leaveAnimation,SIGNAL(finished()),this,SLOT(animationFinishedSLot()));
    connect(enterAnimation,SIGNAL(finished()),this,SLOT(animationFinishedSLot()));

}

/**
 * 初始化应用列表界面
 */
void FunctionWidget::initAppListWidget()
{
    applistview=new ListView(this,this->width()-4,this->height(),2);
    applistview->setGeometry(QRect(0,0,this->width()-4,this->height()));
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
    QStringList androidlist=vector.at(0);
    if(!androidlist.isEmpty())
    {
        insertClassificationBtn(tr("Android"));
        insertAppList(androidlist);
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
        appClassificationBtnClickedSlot();
    else
        execApplication(arg.at(0));
}

/**
 * 执行应用程序
 */
void FunctionWidget::execApplication(QString desktopfp)
{
    Q_EMIT sendHideMainWindowSignal();
    GDesktopAppInfo * desktopAppInfo=g_desktop_app_info_new_from_filename(desktopfp.toLocal8Bit().data());
    g_app_info_launch(G_APP_INFO(desktopAppInfo),nullptr, nullptr, nullptr);
    g_object_unref(desktopAppInfo);
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
    QStringList androidlist=vector.at(0);
    if(!androidlist.isEmpty())
    {
        insertClassificationBtn(tr("Android"));
        insertAppList(androidlist);
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

/**
 * 应用列表功能分类按钮槽函数
 */
void FunctionWidget::appClassificationBtnClickedSlot()
{
    leaveAnimation->setStartValue(QRect(0,0,this->width()-4,this->height()));
    leaveAnimation->setEndValue(QRect(20,20,this->width()-40,this->height()-40));
    enterAnimation->setStartValue(QRect(0,0,this->width()-4,this->height()));
    enterAnimation->setEndValue(QRect((this->width()-Style::LeftBtnWidth*2-5)/2,
                                      (this->height()-Style::LeftBtnHeight*6-25)/2,
                                      Style::LeftBtnWidth*2+5,
                                      Style::LeftBtnHeight*6+25));

    //加载FunctionButtonWidget界面
    QLayoutItem *child=nullptr;
    while ((child = functionbtnwid->layout()->takeAt(0)) != 0) {
        QWidget* wid=child->widget();
        functionbtnwid->layout()->removeWidget(wid);
        wid->setParent(nullptr);
        delete wid;
        delete child;
    }
    Q_EMIT sendClassificationbtnList(classificationbtnlist);
    leaveAnimation->setTargetObject(applistview);
    enterAnimation->setTargetObject(functionbtnwid);
    leaveAnimation->start();
    widgetState=1;
}

/**
 * 接收FunctionButtonWidget界面按钮信号
 */
void FunctionWidget::recvFunctionBtnSignal(QString btnname)
{
    //此处需实现将功能为btnname的应用列表移动到applistWid界面最顶端
    int index=classificationbtnlist.indexOf(btnname);
    if(index!=-1)
    {
        int row=classificationbtnrowlist.at(index).toInt();
        applistview->verticalScrollBar()->setValue(row);
    }

    leaveAnimation->setStartValue(QRect((this->width()-Style::LeftBtnWidth*2-5)/2,
                                        (this->height()-Style::LeftBtnHeight*6-25)/2,
                                        Style::LeftBtnWidth*2+5,
                                        Style::LeftBtnHeight*6+25));
    leaveAnimation->setEndValue(QRect(0,0,this->width()-4,this->height()));
    enterAnimation->setStartValue(QRect(20,20,this->width()-40,this->height()-40));
    enterAnimation->setEndValue(QRect(0,0,this->width()-4,this->height()));

    leaveAnimation->setTargetObject(functionbtnwid);
    enterAnimation->setTargetObject(applistview);
    leaveAnimation->start();
    widgetState=0;
}

void FunctionWidget::animationFinishedSLot()
{
    if(widgetState==1)
    {
        applistview->setVisible(false);
        functionbtnwid->setVisible(true);
        enterAnimation->start();
        widgetState=-1;
    }
    if(widgetState==0)
    {
        functionbtnwid->setVisible(false);
        applistview->setVisible(true);
        enterAnimation->start();
        widgetState=-1;
    }
}

void FunctionWidget::widgetMakeZero()
{
    functionbtnwid->setVisible(false);
    applistview->setVisible(true);
    applistview->setGeometry(QRect(0,0,this->width()-4,this->height()));
    applistview->verticalScrollBar()->setValue(0);
}

void FunctionWidget::moveScrollBar(int type)
{
    if(type==0)
        applistview->verticalScrollBar()->setSliderPosition(applistview->verticalScrollBar()->sliderPosition()-1);
    else
        applistview->verticalScrollBar()->setSliderPosition(applistview->verticalScrollBar()->sliderPosition()+1);
}
