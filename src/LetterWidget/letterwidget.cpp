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

#include "letterwidget.h"
#include "src/Style/style.h"
#include <QDebug>
#include <QHeaderView>
#include <QScrollBar>
#include <QPushButton>
#include <QTableWidgetItem>
#include <QFrame>
#include <QSlider>
#include <QGraphicsDropShadowEffect>
#include <syslog.h>
#include "src/UtilityFunction/itemdelegate.h"

LetterWidget::LetterWidget(QWidget *parent) :
    QWidget(parent)
{
    initUi();
}

LetterWidget::~LetterWidget()
{
    delete m_ukuiMenuInterface;
}

/**
 * 主界面初始化
 */
void LetterWidget::initUi()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setStyleSheet("border:0px;background:transparent;");
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setFixedSize(Style::defaultMainViewWidWidth,Style::defaultContentWidHeight);

    m_ukuiMenuInterface=new UkuiMenuInterface;
    initAppListWidget();

    m_letterBtnWid=new LetterButtonWidget(this);
    connect(this,&LetterWidget::sendLetterBtnList,m_letterBtnWid,&LetterButtonWidget::recvLetterBtnList);
    connect(m_letterBtnWid, &LetterButtonWidget::sendLetterBtnSignal,this,&LetterWidget::recvLetterBtnSlot);

    m_enterAnimation=new QPropertyAnimation;
    m_enterAnimation->setPropertyName(QString("geometry").toLocal8Bit());
    m_leaveAnimation=new QPropertyAnimation;
    m_leaveAnimation->setPropertyName(QString("geometry").toLocal8Bit());
    connect(m_leaveAnimation,&QPropertyAnimation::finished,this,&LetterWidget::animationFinishedSLot);
    connect(m_enterAnimation,&QPropertyAnimation::finished,this,&LetterWidget::animationFinishedSLot);
}

/**
 * 初始化应用列表界面
 */
void LetterWidget::initAppListWidget()
{
    m_appListView=new ListView(this,this->width()-4,this->height(),1);
    m_appListView->setGeometry(QRect(0,0,this->width()-4,this->height()));
    m_appListView->show();
    fillAppListView();
    connect(m_appListView,&ListView::sendItemClickedSignal,this,&LetterWidget::recvItemClickedSlot);
    connect(m_appListView,&ListView::sendHideMainWindowSignal,this,&LetterWidget::sendHideMainWindowSignal);
}


/**
 * 填充应用列表
 */
void LetterWidget::fillAppListView()
{
    int row=0;
    QVector<QStringList> vector=UkuiMenuInterface::alphabeticVector;
    for(int i=0;i<vector.size();i++)
    {
        QStringList appList=vector.at(i);
        if(!appList.isEmpty())
        {
            QString letterstr;
            if(i<26)
                letterstr=QString(QChar(static_cast<char>(i+65)));
            else if(i==26)
                letterstr="&";
            else
                letterstr="#";
            m_letterList.append(letterstr);//存储分类字符
            m_letterBtnRowList.append(QString::number(row));//存储分类字符所在行
            m_data.append(QStringList()<<letterstr<<"0");
            for(int i=0;i<appList.count();i++)
                m_data.append(QStringList()<<appList.at(i)<<"1");
            row+=(appList.count()+1);

        }
    }

    m_appListView->addData(m_data);
}

void LetterWidget::recvItemClickedSlot(QStringList arg)
{
    if(arg.at(1).toInt()==0)
        appClassificationBtnClickedSlot();
    else
        execApplication(arg.at(0));
}

/**
 * 执行应用程序
 */
void LetterWidget::execApplication(QString desktopfp)
{
    this->parentWidget()->parentWidget()->parentWidget()->hide();
    GDesktopAppInfo * desktopAppInfo=g_desktop_app_info_new_from_filename(desktopfp.toLocal8Bit().data());
    bool ret=g_app_info_launch(G_APP_INFO(desktopAppInfo),nullptr, nullptr, nullptr);
    g_object_unref(desktopAppInfo);
    syslog(LOG_LOCAL0 | LOG_DEBUG ,"执行应用程序 %s:%d:%s:%s",
           desktopfp.toLocal8Bit().data(),
           ret,
           m_ukuiMenuInterface->getAppExec(desktopfp).toLocal8Bit().data(),
           m_ukuiMenuInterface->getAppIcon(desktopfp).toLocal8Bit().data());
}

/**
 * 更新应用列表
 */
void LetterWidget::updateAppListView()
{
    int row=0;
    m_data.clear();
    m_letterList.clear();
    m_letterBtnRowList.clear();
    QVector<QStringList> vector=UkuiMenuInterface::alphabeticVector;
    for(int i=0;i<vector.size();i++)
    {
        QStringList appList=vector.at(i);
        if(!appList.isEmpty())
        {
            QString letterstr;
            if(i<26)
                letterstr=QString(QChar(static_cast<char>(i+65)));
            else if(i==26)
                letterstr="&";
            else
                letterstr="#";
            m_letterList.append(letterstr);//存储分类字符
            m_letterBtnRowList.append(QString::number(row));//存储分类字符所在行
            m_data.append(QStringList()<<letterstr<<"0");
            for(int i=0;i<appList.count();i++)
                m_data.append(QStringList()<<appList.at(i)<<"1");
            row+=(appList.count()+1);

        }
    }
    m_appListView->updateData(m_data);
}

/**
 * 应用列表字母分类按钮槽函数
 */
void LetterWidget::appClassificationBtnClickedSlot()
{
    m_leaveAnimation->setStartValue(QRect(0,0,this->width()-4,this->height()));
    m_leaveAnimation->setEndValue(QRect(20,20,this->width()-40,this->height()-40));
    m_enterAnimation->setStartValue(QRect(-40,-40,this->width()+80,this->height()+80));
    m_enterAnimation->setEndValue(QRect((this->width()-235)/2,(this->height()-366)/2,235,366));
    m_leaveAnimation->setDuration(10);
    m_enterAnimation->setDuration(80);

    //加载LetterBUttonWidget界面
    Q_EMIT sendLetterBtnList(m_letterList);
    m_leaveAnimation->setTargetObject(m_appListView);
    m_enterAnimation->setTargetObject(m_letterBtnWid);
    m_letterBtnWid->setWindowOpacity(0.1);
    m_leaveAnimation->start();
    m_widgetState=1;
}

/**
 * 接收LetterButtonWidget界面按钮信号
 */
void LetterWidget::recvLetterBtnSlot(QString btnname)
{
    //此处需实现将字母为btnname的应用列表移动到applistWid界面最顶端
    int num=m_letterList.indexOf(QString(QChar(btnname.at(0))));
    if(num!=-1)
    {
        int row=m_letterBtnRowList.at(num).toInt();
        m_appListView->verticalScrollBar()->setValue(row);
    }

    m_leaveAnimation->setStartValue(QRect((this->width()-235)/2,(this->height()-366)/2,235,366));
    m_leaveAnimation->setEndValue(QRect(-40,-40,this->width()+80,this->height()+80));
    m_enterAnimation->setStartValue(QRect(20,20,this->width()-40,this->height()-40));
    m_enterAnimation->setEndValue(QRect(0,0,this->width()-4,this->height()));
    m_leaveAnimation->setDuration(80);
    m_enterAnimation->setDuration(10);

    m_leaveAnimation->setTargetObject(m_letterBtnWid);
    m_enterAnimation->setTargetObject(m_appListView);
    m_leaveAnimation->start();
    m_widgetState=0;
}

void LetterWidget::animationFinishedSLot()
{
    if(m_widgetState==1)
    {
        m_appListView->setVisible(false);
        m_letterBtnWid->setVisible(true);
        m_enterAnimation->start();
        m_widgetState=-1;
        m_letterBtnWid->show();
    }
    if(m_widgetState==0)
    {
        m_letterBtnWid->setVisible(false);
        m_appListView->setVisible(true);
        m_enterAnimation->start();
        m_widgetState=-1;
        m_appListView->show();
    }
}

void LetterWidget::widgetMakeZero()
{
    m_letterBtnWid->setVisible(false);
    m_appListView->setVisible(true);
    m_appListView->setGeometry(QRect(0,0,this->width()-4,this->height()));
    m_appListView->show();
    m_appListView->verticalScrollBar()->setValue(0);
}

void LetterWidget::moveScrollBar(int type)
{
    if(type==0)
        m_appListView->verticalScrollBar()->setSliderPosition(m_appListView->verticalScrollBar()->sliderPosition()-100);
    else
        m_appListView->verticalScrollBar()->setSliderPosition(m_appListView->verticalScrollBar()->sliderPosition()+100);
}

void LetterWidget::repaintWidget()
{
    this->setFixedSize(Style::defaultMainViewWidWidth,Style::defaultContentWidHeight);
    m_appListView->setGeometry(QRect(0,0,this->width()-4,this->height()));
    m_appListView->show();
}
