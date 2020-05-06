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

//    mainLayout=new QVBoxLayout(this);
//    mainLayout->setContentsMargins(2,0,2,0);
//    mainLayout->setSpacing(0);
//    this->setLayout(mainLayout);
    pUkuiMenuInterface=new UkuiMenuInterface;
    initAppListWidget();

    letterbtnwid=new LetterButtonWidget(this);
    connect(this,SIGNAL(sendLetterBtnList(QStringList)),letterbtnwid,SLOT(recvLetterBtnList(QStringList)));
    connect(letterbtnwid, SIGNAL(sendLetterBtnSignal(QString)),this,SLOT(recvLetterBtnSlot(QString)));
    letterbtnwid->setGeometry(QRect((this->width()-4)/2,(this->height())/2,0,0));

    enterAnimation=new QPropertyAnimation;
    enterAnimation->setDuration(200);
    enterAnimation->setPropertyName(QString("geometry").toLocal8Bit());
    enterAnimation->setStartValue(QRect((this->width()-4)/2,(this->height())/2,0,0));
    enterAnimation->setEndValue(QRect(0,0,this->width()-4,this->height()));
    leaveAnimation=new QPropertyAnimation;
    leaveAnimation->setDuration(200);
    leaveAnimation->setPropertyName(QString("geometry").toLocal8Bit());
    leaveAnimation->setStartValue(QRect(0,0,this->width()-4,this->height()));
    leaveAnimation->setEndValue(QRect((this->width()-4)/2,(this->height())/2,0,0));
    sGroup=new QSequentialAnimationGroup;
    sGroup->addAnimation(leaveAnimation);
    sGroup->addAnimation(enterAnimation);
}

/**
 * 初始化应用列表界面
 */
void LetterWidget::initAppListWidget()
{
    applistview=new ListView(this,this->width()-4,this->height(),1);
    applistview->setGeometry(QRect(0,0,this->width()-4,this->height()));
//    mainLayout->addWidget(applistview);
    fillAppListView();
    connect(applistview,SIGNAL(sendItemClickedSignal(QStringList)),this,SLOT(recvItemClickedSlot(QStringList)));
    connect(applistview,SIGNAL(sendFixedOrUnfixedSignal(QString,int)),this,SIGNAL(sendUpdateAppListSignal(QString,int)));
    connect(applistview,SIGNAL(sendHideMainWindowSignal()),this,SIGNAL(sendHideMainWindowSignal()));

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
            {
                QString desktopfp=pUkuiMenuInterface->getDesktopPathByAppName(appList.at(i));
                data.append(QStringList()<<desktopfp<<"1");
            }
            row+=(appList.count()+1);

        }
    }

    applistview->addData(data);
}

void LetterWidget::recvItemClickedSlot(QStringList arg)
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
void LetterWidget::execApplication(QString exec)
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
            {
                QString desktopfp=pUkuiMenuInterface->getDesktopPathByAppName(appList.at(i));
                data.append(QStringList()<<desktopfp<<"1");
            }
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
    //加载LetterBUttonWidget界面
//    letterbtnwid=new LetterButtonWidget(this);
//    connect(this,SIGNAL(sendLetterBtnList(QStringList)),letterbtnwid,SLOT(recvLetterBtnList(QStringList)));
    Q_EMIT sendLetterBtnList(letterbtnlist);
//    mainLayout->removeWidget(applistview);
//    applistview->setParent(nullptr);
//    mainLayout->addWidget(letterbtnwid);

//    connect(letterbtnwid, SIGNAL(sendLetterBtnSignal(QString)),this,SLOT(recvLetterBtnSlot(QString)));
//    leaveAnimation->setTargetObject(applistview);
//    enterAnimation->setTargetObject(applistview);
    leaveAnimation->setTargetObject(applistview);
    enterAnimation->setTargetObject(letterbtnwid);
    sGroup->start();
//    leaveAnimation->start();
//    enterAnimation->start();
//    letterbtnwid->setGeometry(QRect(0,0,this->width()-4,this->height()));

}

/**
 * 接收LetterButtonWidget界面按钮信号
 */
void LetterWidget::recvLetterBtnSlot(QString btnname)
{
    leaveAnimation->setTargetObject(letterbtnwid);
    enterAnimation->setTargetObject(applistview);
    sGroup->start();

//    mainLayout->removeWidget(letterbtnwid);
//    letterbtnwid->setParent(nullptr);
//    if(letterbtnwid!=nullptr)
//    {
//        delete letterbtnwid;
//        letterbtnwid=nullptr;
//    }
//    mainLayout->addWidget(applistview);

    //此处需实现将字母为btnname的应用列表移动到applistWid界面最顶端
    int num=letterbtnlist.indexOf(QString(QChar(btnname.at(0))));
    if(num!=-1)
    {
        int row=letterbtnrowlist.at(num).toInt();
        applistview->verticalScrollBar()->setValue(row);

    }
}

void LetterWidget::widgetMakeZero()
{
//    if(letterbtnwid!=nullptr)
//    {
//        mainLayout->removeWidget(letterbtnwid);
//        letterbtnwid->setParent(nullptr);
//        delete letterbtnwid;
//        letterbtnwid=nullptr;
//        mainLayout->addWidget(applistview);
//    }
    applistview->verticalScrollBar()->setValue(0);
}

void LetterWidget::moveScrollBar(int type)
{
    if(type==0)
        applistview->verticalScrollBar()->setSliderPosition(applistview->verticalScrollBar()->sliderPosition()-1);
    else
        applistview->verticalScrollBar()->setSliderPosition(applistview->verticalScrollBar()->sliderPosition()+1);
}
