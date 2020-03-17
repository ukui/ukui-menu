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

    mainLayout=new QVBoxLayout(this);
    mainLayout->setContentsMargins(2,0,2,0);
    mainLayout->setSpacing(0);
    this->setLayout(mainLayout);
    pUkuiMenuInterface=new UkuiMenuInterface;

//    line=new QLabel(this);
//    line->setFixedSize(this->width(),2);
//    char linestyle[100];
//    sprintf(linestyle, "border:none;background-color:%s;","rgba(0, 0, 0, 0.20)");
//    line->setStyleSheet(linestyle);
//    QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect(line);
//    shadow_effect->setOffset(0, 2);
//    shadow_effect->setColor(QColor("rgba(0, 0, 0, 0.35)"));
//    shadow_effect->setBlurRadius(10);
//    line->setGraphicsEffect(shadow_effect);

//    mainLayout->addWidget(line);

    initAppListWidget();

}

/**
 * 初始化应用列表界面
 */
void LetterWidget::initAppListWidget()
{
    applistview=new ListView(this,this->width()-4,this->height(),1);
    mainLayout->addWidget(applistview);
    fillAppListView();
    connect(applistview,SIGNAL(sendItemClickedSignal(QStringList)),this,SLOT(recvItemClickedSlot(QStringList)));
    connect(applistview,SIGNAL(sendFixedOrUnfixedSignal()),this,SIGNAL(sendUpdateAppListSignal()));
    connect(applistview,SIGNAL(sendHideMainWindowSignal()),this,SIGNAL(sendHideMainWindowSignal()));

}


/**
 * 填充应用列表
 */
void LetterWidget::fillAppListView()
{
    int row=0;
    QVector<QStringList> vector=pUkuiMenuInterface->getAlphabeticClassification();
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
    QVector<QStringList> vector=pUkuiMenuInterface->getAlphabeticClassification();
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

                qDebug()<<"图片："<<pUkuiMenuInterface->getAppIcon(desktopfp);
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
    letterbtnwid=new LetterButtonWidget(this);
    connect(this,SIGNAL(sendLetterBtnList(QStringList)),letterbtnwid,SLOT(recvLetterBtnList(QStringList)));
    Q_EMIT sendLetterBtnList(letterbtnlist);
//    mainLayout->removeWidget(line);
//    line->setParent(nullptr);
    mainLayout->removeWidget(applistview);
    applistview->setParent(nullptr);
    mainLayout->addWidget(letterbtnwid);

    connect(letterbtnwid, SIGNAL(sendLetterBtnSignal(QString)),this,SLOT(recvLetterBtnSignal(QString)));
}

/**
 * 接收LetterButtonWidget界面按钮信号
 */
void LetterWidget::recvLetterBtnSignal(QString btnname)
{
    mainLayout->removeWidget(letterbtnwid);
    letterbtnwid->setParent(nullptr);
    if(letterbtnwid!=nullptr)
    {
        delete letterbtnwid;
        letterbtnwid=nullptr;
    }
//    mainLayout->addWidget(line);
    mainLayout->addWidget(applistview);

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
    if(letterbtnwid!=nullptr)
    {
        mainLayout->removeWidget(letterbtnwid);
        letterbtnwid->setParent(nullptr);
        delete letterbtnwid;
        letterbtnwid=nullptr;
        mainLayout->addWidget(applistview);
    }
    applistview->verticalScrollBar()->setValue(0);
}
