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

#include "fullletterwidget.h"
#include "ui_fullletterwidget.h"
#include <QDebug>

FullLetterWidget::FullLetterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FullLetterWidget)
{
    ui->setupUi(this);

    initWidget();
}

FullLetterWidget::~FullLetterWidget()
{
    delete ui;
    delete pUkuiMenuInterface;
}

/**
 * 主界面初始化
 */
void FullLetterWidget::initWidget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setStyleSheet("border:0px;background:transparent;");
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    applistWid=new QWidget(this);
    letterlistWid=new QWidget(this);
    this->setFixedSize(Style::MainViewWidWidth,
                       Style::AppListWidHeight);
    applistWid->setFixedSize(Style::AppListWidWidth,this->height());
    letterlistWid->setFixedSize(Style::LeftWidWidth,this->height());

    mainLayout=new QHBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    applistWid->setStyleSheet("border:0px;background: transparent;");
    letterlistWid->setStyleSheet("border:0px;background: transparent;");
    mainLayout->addWidget(letterlistWid);
    mainLayout->addWidget(applistWid);
    this->setLayout(mainLayout);
    pUkuiMenuInterface=new UkuiMenuInterface;

    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeOutSlot()));

    initAppListWidget();
    initLetterListWidget();
}

/**
 * 初始化应用列表界面
 */
void FullLetterWidget::initAppListWidget()
{
    QHBoxLayout* layout=new QHBoxLayout(applistWid);
    layout->setContentsMargins(0,0,0,0);
    applistWid->setLayout(layout);

    scrollarea=new ScrollArea;
    scrollareawid=new QWidget;
    scrollarea->setWidget(scrollareawid);
    scrollarea->setFixedSize(applistWid->width(),applistWid->height());
    scrollarea->setWidgetResizable(true);
    scrollareawidLayout=new QVBoxLayout;
    scrollareawidLayout->setContentsMargins(0,0,0,0);
    scrollareawidLayout->setSpacing(10);
    scrollareawid->setLayout(scrollareawidLayout);
    layout->addWidget(scrollarea);
    connect(scrollarea->verticalScrollBar(),SIGNAL(valueChanged(int)),
            this,SLOT(valueChangedSlot(int)));

    fillAppList();

}

/**
 * 填充应用列表
 */
void FullLetterWidget::fillAppList()
{
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
            //插入字母分类按钮
            PushButton* letterbtn=new PushButton(this,letterstr,scrollarea->width()-12,20);
//            letterbtn->setFixedSize(scrollarea->width(),20);
            scrollareawidLayout->addWidget(letterbtn);

            //插入应用列表
            FullListView* listview=new FullListView(this,1);
            scrollareawidLayout->addWidget(listview);
            data.clear();
            for(int i=0;i<appList.count();i++)
            {
                QString desktopfp=pUkuiMenuInterface->getDesktopPathByAppName(appList.at(i));
                data.append(desktopfp);

            }
            listview->addData(data);

            connect(listview,SIGNAL(sendItemClickedSignal(QString)),this,SLOT(execApplication(QString)));
            connect(listview,SIGNAL(sendFixedOrUnfixedSignal(QString,int)),this,SIGNAL(sendUpdateAppListSignal(QString,int)));
            connect(listview,SIGNAL(sendHideMainWindowSignal()),this,SIGNAL(sendHideMainWindowSignal()));
        }
    }

        resizeScrollAreaControls();
}

/**
 * 执行应用程序
 */
void FullLetterWidget::execApplication(QString appname)
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
void FullLetterWidget::updateAppListView()
{
    //刷新应用列表界面
    QLayoutItem *child;
     while ((child = scrollareawidLayout->takeAt(0)) != 0) {
         QWidget* wid=child->widget();
         scrollareawidLayout->removeWidget(wid);
         wid->setParent(nullptr);
         delete wid;
         delete child;
     }
     fillAppList();

    //刷新字母列表界面
    Q_FOREACH (QAbstractButton* button, buttonList) {
        pBtnGroup->removeButton(button);
    }
    buttonList.clear();
    letterlistscrollareawidLayout->removeItem(pLetterListBottomSpacer);
    while ((child = letterlistscrollareawidLayout->takeAt(0)) != 0) {
        QWidget* wid=child->widget();
        letterlistscrollareawidLayout->removeWidget(wid);
        wid->setParent(nullptr);
        delete wid;
        delete child;
    }

    //防止按钮位置偏移
    initLetterListScrollArea();
}

/**
 * 设置scrollarea所填充控件大小
 */
void FullLetterWidget::resizeScrollAreaControls()
{
    int pos=0;
    letterbtnrowlist.append(QString::number(pos));
    int row=0;
    while(row<scrollareawidLayout->count()/2)
    {
        //应用界面
        QLayoutItem* widItem=scrollareawidLayout->itemAt(row*2+1);
        QWidget* wid=widItem->widget();
        FullListView* listview=qobject_cast<FullListView*>(wid);
        listview->adjustSize();
        int dividend=(scrollarea->width()-Style::SliderSize)/Style::AppListGridSizeWidth;

        int rowcount=0;
        if(listview->model()->rowCount()%dividend>0)
        {
            rowcount=listview->model()->rowCount()/dividend+1;
        }
        else
        {
            rowcount=listview->model()->rowCount()/dividend;

        }

        listview->setFixedSize(scrollarea->width()-Style::SliderSize+1,listview->gridSize().height()*rowcount);
        if(row<scrollareawidLayout->count()/2-1)
        {
            pos+=(20+20+listview->height());
            letterbtnrowlist.append(QString::number(pos));
        }
        row++;
    }
    scrollarea->widget()->adjustSize();
}

/**
 * 初始化字母列表界面
 */
void FullLetterWidget::initLetterListWidget()
{
    letterlistscrollarea=new ClassifyScrollArea(letterlistWid);
    letterlistscrollareaWid=new QWidget(letterlistscrollarea);
    letterlistscrollareawidLayout=new QVBoxLayout;
    letterlistscrollareawidLayout->setContentsMargins(0,0,0,0);
    letterlistscrollareawidLayout->setSpacing(0);
    letterlistscrollareaWid->setLayout(letterlistscrollareawidLayout);
    letterlistscrollarea->setWidget(letterlistscrollareaWid);
    letterlistscrollarea->setWidgetResizable(true);

    pLetterListTopSpacer=new QSpacerItem(20,40,QSizePolicy::Fixed,QSizePolicy::Expanding);
    pLetterListBottomSpacer=new QSpacerItem(20,40,QSizePolicy::Fixed,QSizePolicy::Expanding);
    pBtnGroup=new QButtonGroup(letterlistscrollareaWid);
    pAnimation = new QPropertyAnimation(letterlistscrollarea, "geometry");
    initLetterListScrollArea();
}

/**
 * 初始化字母列表
 */
void FullLetterWidget::initLetterListScrollArea()
{
//    letterlistscrollarea->setStyleSheet("border:1px solid #ff0000;");
//    letterlistscrollarea->setFixedSize(Style::LeftLetterBtnHeight*2,
//                                       (letterbtnlist.size()+1)*Style::LeftLetterBtnHeight);

//    letterlistscrollarea->setStyleSheet("border:1px solid #ff0000;");
    if(letterbtnlist.contains("&"))
        letterbtnlist.replace(letterbtnlist.indexOf("&"),"&&");
    for(int i=0;i<letterbtnlist.size();i++)
    {
        LetterClassifyButton* letterbtn=new LetterClassifyButton(letterlistscrollareaWid,
                                                                 ClassifyBtnHoverBackground,
                                                                 ClassifyBtnHoverBackground,
                                                                 letterbtnlist.at(i));
        buttonList.append(letterbtn);
        letterlistscrollareawidLayout->addWidget(letterbtn);
        letterlistscrollareawidLayout->setAlignment(letterbtn,Qt::AlignHCenter);
        connect(letterbtn,SIGNAL(buttonClicked(QAbstractButton*)),pBtnGroup, SIGNAL(buttonClicked(QAbstractButton*)));
    }
    letterlistscrollareawidLayout->addItem(pLetterListBottomSpacer);

    int id=0;
    Q_FOREACH (QAbstractButton* btn, buttonList) {
        pBtnGroup->addButton(btn,id++);
    }

    connect(pBtnGroup,SIGNAL(buttonClicked(QAbstractButton*)),this,SLOT(btnGroupClickedSlot(QAbstractButton*)));
    letterlistscrollarea->widget()->adjustSize();
    pBtnGroup->button(0)->click();
}

void FullLetterWidget::btnGroupClickedSlot(QAbstractButton *btn)
{
    disconnect(scrollarea->verticalScrollBar(),SIGNAL(valueChanged(int)),
            this,SLOT(valueChangedSlot(int)));
    Q_FOREACH (QAbstractButton* button, buttonList) {
        LetterClassifyButton* letterbtn=qobject_cast<LetterClassifyButton*>(button);
        if(pBtnGroup->id(btn)==buttonList.indexOf(button))
        {
            letterbtn->setChecked(true);
            //此处需实现将被选定的字母包含的应用列表移动到applistWid界面最顶端
            QString letterstr=letterbtn->text();
            int num=letterbtnlist.indexOf(letterstr);
            if(num!=-1)
            {
                beginPos=scrollarea->verticalScrollBar()->sliderPosition();
                endPos=letterbtnrowlist.at(num).toInt();
                scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                timer->start(1);
            }
        }
        else{
            letterbtn->setChecked(false);
        }
    }
}

void FullLetterWidget::timeOutSlot()
{
    int height=QApplication::primaryScreen()->geometry().height();
    int speed=0;
    if(qAbs(endPos-scrollarea->verticalScrollBar()->sliderPosition()) <= height*300/1080)
        speed=sqrt(qAbs(endPos-scrollarea->verticalScrollBar()->sliderPosition()));
    else
        speed=height*200/1080;

    if(beginPos<endPos && endPos-scrollarea->verticalScrollBar()->sliderPosition() >= speed)
    {
        scrollarea->verticalScrollBar()->setSliderPosition(scrollarea->verticalScrollBar()->sliderPosition()+speed);
    }
    if(beginPos>endPos && scrollarea->verticalScrollBar()->sliderPosition()-endPos >= speed)
    {
        scrollarea->verticalScrollBar()->setSliderPosition(scrollarea->verticalScrollBar()->sliderPosition()-speed);
    }
    if(scrollarea->verticalScrollBar()->sliderPosition()==endPos ||
            scrollarea->verticalScrollBar()->sliderPosition()>=scrollarea->verticalScrollBar()->maximum())
    {
        timer->stop();
        scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        connect(scrollarea->verticalScrollBar(),SIGNAL(valueChanged(int)),
                this,SLOT(valueChangedSlot(int)));
    }
}

void FullLetterWidget::valueChangedSlot(int value)
{
    int count=0;
    while(count<letterbtnrowlist.count()-1)
    {
        if(value>=letterbtnrowlist.at(count).toInt() &&
                value <letterbtnrowlist.at(count+1).toInt())
        {
            Q_FOREACH (QAbstractButton* button, buttonList) {
                LetterClassifyButton* letterbtn=qobject_cast<LetterClassifyButton*>(button);
                if(count==buttonList.indexOf(button))
                {
                    letterbtn->setChecked(true);
                }
                else
                {
                    letterbtn->setChecked(false);
                }
            }
            break;
        }
        else
            count++;
    }
//    if(count==letterbtnrowlist.count()-1 ||
//            scrollarea->verticalScrollBar()->sliderPosition()==scrollarea->verticalScrollBar()->maximum())
//    {
//        Q_FOREACH (QAbstractButton* button, buttonList) {
//            LetterClassifyButton* letterbtn=qobject_cast<LetterClassifyButton*>(button);
//            if(letterbtnrowlist.count()-1==buttonList.indexOf(button))
//            {
//                letterbtn->setStyleSheet(QString("background:transparent;color:#ffffff;padding-left:0px;"));
//                letterbtn->is_pressed=true;
//            }
//            else
//            {
//                letterbtn->setStyleSheet(QString("background:transparent;color:#8b8b8b;padding-left:0px;"));
//                letterbtn->is_pressed=false;
//            }
//        }
//        letterlistscrollarea->verticalScrollBar()->setSliderPosition(letterlistscrollarea->verticalScrollBar()->maximum());

//    }

    //向下滚动
    if((buttonList.at(count)->pos().y()+buttonList.at(count)->height()+letterlistscrollarea->widget()->pos().y()) >= letterlistscrollarea->height())
    {
        int val=letterlistscrollarea->verticalScrollBar()->sliderPosition()+buttonList.at(count)->height();
        letterlistscrollarea->verticalScrollBar()->setSliderPosition(val);
    }

    //向上滚动
    if((buttonList.at(count)->pos().y()+letterlistscrollarea->widget()->pos().y()) <= 0)
    {

        int val=letterlistscrollarea->verticalScrollBar()->value()-buttonList.at(count)->height();
        letterlistscrollarea->verticalScrollBar()->setSliderPosition(val);
    }

}

void FullLetterWidget::enterAnimation()
{
    pAnimation->setDuration(200);//动画总时间
    pAnimation->setStartValue(QRect(0,(letterlistWid->height()-(letterbtnlist.size()+1)*Style::LeftLetterBtnHeight)/2,
                                    0,(letterbtnlist.size()+1)*Style::LeftLetterBtnHeight));
    pAnimation->setEndValue(QRect(Style::LeftMargin,
                                  (letterlistWid->height()-(letterbtnlist.size()+1)*Style::LeftLetterBtnHeight)/2,
                                  Style::LeftLetterBtnHeight*2,
                            (letterbtnlist.size()+1)*Style::LeftLetterBtnHeight));
    pAnimation->setEasingCurve(QEasingCurve::InQuart);
//    pAnimation->setEasingCurve(QEasingCurve::Linear);
    pAnimation->start();
}

void FullLetterWidget::repaintWidget()
{
    this->setFixedSize(Style::MainViewWidWidth,
                       Style::AppListWidHeight);
    applistWid->setFixedSize(Style::AppListWidWidth,this->height());
    letterlistWid->setFixedSize(Style::LeftWidWidth,this->height());
    scrollarea->setFixedSize(applistWid->width(),applistWid->height());
    updateAppListView();
}

void FullLetterWidget::widgetMakeZero()
{
    Q_FOREACH (QAbstractButton* button, buttonList) {
        QString letterstr=button->text().at(0);
        int num=letterbtnlist.indexOf(letterstr);
        if(num!=-1)
        {
            pBtnGroup->button(num)->click();
            letterlistscrollarea->verticalScrollBar()->setSliderPosition(0);
            break;
        }
    }
    scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

void FullLetterWidget::moveScrollBar(int type)
{
    int height=QApplication::primaryScreen()->geometry().height();
    if(type==0)
        scrollarea->verticalScrollBar()->setSliderPosition(scrollarea->verticalScrollBar()->sliderPosition()-height*100/1080);
    else
        scrollarea->verticalScrollBar()->setSliderPosition(scrollarea->verticalScrollBar()->sliderPosition()+height*100/1080);
}
