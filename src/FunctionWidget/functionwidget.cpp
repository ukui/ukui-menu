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
#include "src/Style/style.h"
#include <QDebug>

FunctionWidget::FunctionWidget(QWidget *parent) :
    QWidget(parent)
{
    initUi();
}

FunctionWidget::~FunctionWidget()
{
    delete m_ukuiMenuInterface;
}

/**
 * 主界面初始化
 */
void FunctionWidget::initUi()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setFixedSize(Style::defaultMainViewWidWidth,Style::defaultContentWidHeight);

    m_ukuiMenuInterface=new UkuiMenuInterface;
    initAppListWidget();

    m_functionBtnWid=new FunctionButtonWidget(this);
    m_functionBtnWid->hide();
    connect(this,&FunctionWidget::sendClassificationbtnList,m_functionBtnWid,&FunctionButtonWidget::recvClassificationBtnList);
    connect(m_functionBtnWid, &FunctionButtonWidget::sendFunctionBtnSignal,this,&FunctionWidget::recvFunctionBtnSignal);

    m_enterAnimation=new QPropertyAnimation;
    m_enterAnimation->setPropertyName(QString("geometry").toLocal8Bit());
    m_leaveAnimation=new QPropertyAnimation;
    m_leaveAnimation->setPropertyName(QString("geometry").toLocal8Bit());
    connect(m_leaveAnimation,&QPropertyAnimation::finished,this,&FunctionWidget::animationFinishedSLot);
    connect(m_enterAnimation,&QPropertyAnimation::finished,this,&FunctionWidget::animationFinishedSLot);
}

/**
 * 初始化应用列表界面
 */
void FunctionWidget::initAppListWidget()
{
    m_appListView=new ListView(this);
    m_appListView->setGeometry(QRect(6,0,this->width()-6,this->height()-6));
    m_appListView->show();
    fillAppListView(0);
    connect(m_appListView,&ListView::sendItemClickedSignal,this,&FunctionWidget::recvItemClickedSlot);
    connect(m_appListView,&ListView::sendHideMainWindowSignal,this,&FunctionWidget::sendHideMainWindowSignal);
}


/**
 * 填充应用列表m_appListView
 */
void FunctionWidget::fillAppListView(int type)
{
    row=0;
    m_data.clear();
    m_classificationList.clear();
    m_classificationBtnRowList.clear();
    QVector<QStringList> vector=UkuiMenuInterface::functionalVector;
    QStringList androidlist=vector.at(0);
    if(!androidlist.isEmpty())
    {
        insertClassificationBtn(tr("Mobile"));
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
//        insertClassificationBtn(tr("Others"execApplication));
        insertAppList(otherlist);
    }

    if(type==0)
        m_appListView->addData(m_data,2);
    else
        m_appListView->updateData(m_data);
}

void FunctionWidget::insertClassificationBtn(QString btnname)
{
    m_classificationList.append(btnname);
    m_data.append(QStringList()<<btnname<<"0");
    m_classificationBtnRowList.append(QString::number(row));
}

void FunctionWidget::insertAppList(QStringList appnamelist)
{
    row+=(appnamelist.count()+1);
    for(int i=0;i<appnamelist.count();i++)
        m_data.append(QStringList()<<appnamelist.at(i)<<"1");
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
    execApp(desktopfp);
}

/**
 * 更新应用列表
 */
void FunctionWidget::updateAppListView()
{
    fillAppListView(1);
}

/**
 * 应用列表功能分类按钮槽函数
 */
void FunctionWidget::appClassificationBtnClickedSlot()
{
    m_leaveAnimation->setStartValue(QRect(6,0,this->width()-6,this->height()-6));
//    m_leaveAnimation->setEndValue(QRect(20,20,this->width()-40,this->height()-40));
    m_leaveAnimation->setEndValue(QRect(6,0,this->width()-6,this->height()-6));
    m_enterAnimation->setStartValue(QRect(-40,-40,this->width()+80,this->height()+80));
    m_enterAnimation->setEndValue(QRect((this->width()-Style::LeftBtnWidth*2-5)/2,
                                      (this->height()-Style::LeftBtnHeight*6-25)/2,
                                      Style::LeftBtnWidth*2+5,
                                      Style::LeftBtnHeight*6+25));
    m_leaveAnimation->setDuration(10);
    m_enterAnimation->setDuration(100);

    //加载FunctionButtonWidget界面
    Q_EMIT sendClassificationbtnList();
    m_leaveAnimation->setTargetObject(m_appListView);
    m_enterAnimation->setTargetObject(m_functionBtnWid);
    m_leaveAnimation->start();
    m_widgetState=1;
}

/**
 * 接收FunctionButtonWidget界面按钮信号
 */
void FunctionWidget::recvFunctionBtnSignal(QString btnname)
{
    //此处需实现将功能为btnname的应用列表移动到applistWid界面最顶端
    int index=m_classificationList.indexOf(btnname);
    if(index!=-1)
    {
        int row=m_classificationBtnRowList.at(index).toInt();
        m_appListView->verticalScrollBar()->setValue(row);
    }

    m_leaveAnimation->setStartValue(QRect((this->width()-Style::LeftBtnWidth*2-5)/2,
                                        (this->height()-Style::LeftBtnHeight*6-25)/2,
                                        Style::LeftBtnWidth*2+5,
                                        Style::LeftBtnHeight*6+25));
    m_leaveAnimation->setEndValue(QRect(-40,-40,this->width()+80,this->height()+80));
//    m_enterAnimation->setStartValue(QRect(20,20,this->width()-40,this->height()-40));
    m_enterAnimation->setStartValue(QRect(6,0,this->width()-6,this->height()-6));
    m_enterAnimation->setEndValue(QRect(6,0,this->width()-6,this->height()-6));
    m_leaveAnimation->setDuration(100);
    m_enterAnimation->setDuration(10);

    m_leaveAnimation->setTargetObject(m_functionBtnWid);
    m_enterAnimation->setTargetObject(m_appListView);
    m_leaveAnimation->start();
    m_widgetState=0;
}

void FunctionWidget::animationFinishedSLot()
{
    if(m_widgetState==1)
    {
        m_appListView->hide();
        m_enterAnimation->start();
        m_widgetState=-1;
        m_functionBtnWid->show();
    }
    if(m_widgetState==0)
    {
        m_functionBtnWid->hide();
        m_enterAnimation->start();
        m_widgetState=-1;
        m_appListView->show();
    }
}

void FunctionWidget::widgetMakeZero()
{
    m_functionBtnWid->hide();
    m_appListView->setGeometry(QRect(6,0,this->width()-6,this->height()-6));
    m_appListView->show();
    m_appListView->verticalScrollBar()->setValue(0);
}

void FunctionWidget::moveScrollBar(int type)
{
    if(type==0)
        m_appListView->verticalScrollBar()->setSliderPosition(m_appListView->verticalScrollBar()->sliderPosition()-100);
    else
        m_appListView->verticalScrollBar()->setSliderPosition(m_appListView->verticalScrollBar()->sliderPosition()+100);
}

void FunctionWidget::repaintWidget()
{
    this->setFixedSize(Style::defaultMainViewWidWidth,Style::defaultContentWidHeight);
    m_appListView->setGeometry(QRect(6,0,this->width()-6,this->height()-6));
    m_appListView->show();
}

void FunctionWidget::setFocusToThis()
{
    this->setFocus();
}
