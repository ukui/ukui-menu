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
#include "ui_letterwidget.h"
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
    QWidget(parent),
    ui(new Ui::LetterWidget)
{
    ui->setupUi(this);

    initWidget();
}

LetterWidget::~LetterWidget()
{
    delete ui;
    delete pUkuiMenuInterface;
}

/**
 * 主界面初始化
 */
void LetterWidget::initWidget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setStyleSheet("border:0px;background:transparent;");
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setFixedSize(320,535);

    pUkuiMenuInterface=new UkuiMenuInterface;
    initAppListWidget();

    letterbtnwid=new LetterButtonWidget(this);
    connect(this,&LetterWidget::sendLetterBtnList,letterbtnwid,&LetterButtonWidget::recvLetterBtnList);
    connect(letterbtnwid, &LetterButtonWidget::sendLetterBtnSignal,this,&LetterWidget::recvLetterBtnSlot);
    letterbtnwid->setGeometry(QRect((this->width()-4)/2,(this->height())/2,0,0));

    enterAnimation=new QPropertyAnimation;
    enterAnimation->setDuration(50);
    enterAnimation->setPropertyName(QString("geometry").toLocal8Bit());
    leaveAnimation=new QPropertyAnimation;
    leaveAnimation->setDuration(50);
    leaveAnimation->setPropertyName(QString("geometry").toLocal8Bit());
    connect(leaveAnimation,&QPropertyAnimation::finished,this,&LetterWidget::animationFinishedSLot);
    connect(enterAnimation,&QPropertyAnimation::finished,this,&LetterWidget::animationFinishedSLot);
}

/**
 * 初始化应用列表界面
 */
void LetterWidget::initAppListWidget()
{
    applistview=new ListView(this,this->width()-4,this->height(),1);
    applistview->setGeometry(QRect(0,0,this->width()-4,this->height()));
    fillAppListView();
    connect(applistview,&ListView::sendItemClickedSignal,this,&LetterWidget::recvItemClickedSlot);
    connect(applistview,&ListView::sendHideMainWindowSignal,this,&LetterWidget::sendHideMainWindowSignal);

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
            letterbtnlist.append(letterstr);//存储分类字符
            letterbtnrowlist.append(QString::number(row));//存储分类字符所在行
            data.append(QStringList()<<letterstr<<"0");
            for(int i=0;i<appList.count();i++)
                data.append(QStringList()<<appList.at(i)<<"1");
            row+=(appList.count()+1);

        }
    }

    applistview->addData(data);
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
    Q_EMIT sendHideMainWindowSignal();
    GDesktopAppInfo * desktopAppInfo=g_desktop_app_info_new_from_filename(desktopfp.toLocal8Bit().data());
    bool ret=g_app_info_launch(G_APP_INFO(desktopAppInfo),nullptr, nullptr, nullptr);
    g_object_unref(desktopAppInfo);
    syslog(LOG_LOCAL0 | LOG_DEBUG ,"执行应用程序 %s:%d:%s",
           desktopfp.toLocal8Bit().data(),
           ret,
           pUkuiMenuInterface->getAppExec(desktopfp).toLocal8Bit().data());
}

/**
 * 更新应用列表
 */
void LetterWidget::updateAppListView()
{
    int row=0;
    data.clear();
    letterbtnlist.clear();
    letterbtnrowlist.clear();
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
            letterbtnlist.append(letterstr);//存储分类字符
            letterbtnrowlist.append(QString::number(row));//存储分类字符所在行
            data.append(QStringList()<<letterstr<<"0");
            for(int i=0;i<appList.count();i++)
                data.append(QStringList()<<appList.at(i)<<"1");
            row+=(appList.count()+1);

        }
    }
    applistview->updateData(data);
}

/**
 * 应用列表字母分类按钮槽函数
 */
void LetterWidget::appClassificationBtnClickedSlot()
{
    leaveAnimation->setStartValue(QRect(0,0,this->width()-4,this->height()));
    leaveAnimation->setEndValue(QRect(20,20,this->width()-40,this->height()-40));
    enterAnimation->setStartValue(QRect(-40,-40,this->width()+80,this->height()+80));
    enterAnimation->setEndValue(QRect(42.5,84.5,235,366));

    //加载LetterBUttonWidget界面
    Q_EMIT sendLetterBtnList(letterbtnlist);
    leaveAnimation->setTargetObject(applistview);
    enterAnimation->setTargetObject(letterbtnwid);
    leaveAnimation->start();
    widgetState=1;
}

/**
 * 接收LetterButtonWidget界面按钮信号
 */
void LetterWidget::recvLetterBtnSlot(QString btnname)
{
    //此处需实现将字母为btnname的应用列表移动到applistWid界面最顶端
    int num=letterbtnlist.indexOf(QString(QChar(btnname.at(0))));
    if(num!=-1)
    {
        int row=letterbtnrowlist.at(num).toInt();
        applistview->verticalScrollBar()->setValue(row);
    }

    leaveAnimation->setStartValue(QRect(42.5,84.5,235,366));
    leaveAnimation->setEndValue(QRect(-40,-40,this->width()+80,this->height()+80));
    enterAnimation->setStartValue(QRect(20,20,this->width()-40,this->height()-40));
    enterAnimation->setEndValue(QRect(0,0,this->width()-4,this->height()));

    leaveAnimation->setTargetObject(letterbtnwid);
    enterAnimation->setTargetObject(applistview);
    leaveAnimation->start();
    widgetState=0;
}

void LetterWidget::animationFinishedSLot()
{
    if(widgetState==1)
    {
        applistview->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
        applistview->setVisible(false);
        letterbtnwid->setVisible(true);
        enterAnimation->start();
        widgetState=-1;
    }
    if(widgetState==0)
    {
        applistview->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        letterbtnwid->setVisible(false);
        applistview->setVisible(true);
        enterAnimation->start();
        widgetState=-1;
    }
}

void LetterWidget::widgetMakeZero()
{
    letterbtnwid->setVisible(false);
    applistview->setVisible(true);
    applistview->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    applistview->setGeometry(QRect(0,0,this->width()-4,this->height()));
    applistview->verticalScrollBar()->setValue(0);
}

void LetterWidget::moveScrollBar(int type)
{
    if(type==0)
        applistview->verticalScrollBar()->setSliderPosition(applistview->verticalScrollBar()->sliderPosition()-100);
    else
        applistview->verticalScrollBar()->setSliderPosition(applistview->verticalScrollBar()->sliderPosition()+100);
}
