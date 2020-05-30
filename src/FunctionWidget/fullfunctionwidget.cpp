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

#include "fullfunctionwidget.h"
#include <QDesktopWidget>
#include <QHeaderView>
#include "src/Style/style.h"
#include <QDebug>
#include <QSvgRenderer>
#include <QPainter>

FullFunctionWidget::FullFunctionWidget(QWidget *parent) :
    QWidget(parent)
{
    initUi();
}

FullFunctionWidget::~FullFunctionWidget()
{
    delete m_ukuiMenuInterface;
}

void FullFunctionWidget::initUi()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setStyleSheet("border:0px;background:transparent;");
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_applistWid=new QWidget(this);
    m_iconListWid=new QWidget(this);
    this->setFixedSize(Style::MainViewWidWidth,
                       Style::AppListWidHeight);
    m_applistWid->setFixedSize(Style::AppListWidWidth,this->height());
    m_iconListWid->setFixedSize(Style::LeftWidWidth,this->height());

    QHBoxLayout* mainLayout=new QHBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    m_applistWid->setStyleSheet("border:0px;background:transparent;");
    m_iconListWid->setStyleSheet("border:0px;background:transparent");
    mainLayout->addWidget(m_iconListWid);
    mainLayout->addWidget(m_applistWid);
    this->setLayout(mainLayout);

    m_ukuiMenuInterface=new UkuiMenuInterface;

    initAppListWidget();
    initIconListWidget();
}

/**
 * 初始化应用列表界面
 */
void FullFunctionWidget::initAppListWidget()
{
    QHBoxLayout* layout=new QHBoxLayout(m_applistWid);
    layout->setContentsMargins(0,0,0,0);
    m_applistWid->setLayout(layout);

    m_scrollArea=new ScrollArea;
    m_scrollAreaWid=new QWidget;
    m_scrollArea->setFixedSize(m_applistWid->width(),m_applistWid->height());
    m_scrollArea->setWidget(m_scrollAreaWid);
    m_scrollArea->setWidgetResizable(true);
    m_scrollAreaWidLayout=new QVBoxLayout;
    m_scrollAreaWidLayout->setContentsMargins(0,0,0,0);
    m_scrollAreaWidLayout->setSpacing(10);
    m_scrollAreaWid->setLayout(m_scrollAreaWidLayout);
    layout->addWidget(m_scrollArea);
    connect(m_scrollArea->verticalScrollBar(),&QScrollBar::valueChanged,
            this,&FullFunctionWidget::valueChangedSlot);
    fillAppList();
}

/**
 * 填充应用列表
 */
void FullFunctionWidget::fillAppList()
{
    m_classificationList.clear();
    QVector<QStringList> vector=UkuiMenuInterface::functionalVector;
    QStringList androidlist=vector.at(0);
    if(!androidlist.isEmpty())
    {
        insertClassificationBtn("Mobile");
        insertAppList(androidlist);
    }

    QStringList netlist=vector.at(1);
    if(!netlist.isEmpty())
    {
        insertClassificationBtn("Internet");
        insertAppList(netlist);
    }
    QStringList sociallist=vector.at(2);
    if(!sociallist.isEmpty())
    {
        insertClassificationBtn("Social");
        insertAppList(sociallist);
    }
    QStringList avlist=vector.at(3);
    if(!avlist.isEmpty())
    {
        insertClassificationBtn("Video");
        insertAppList(avlist);
    }
    QStringList developlist=vector.at(4);
    if(!developlist.isEmpty())
    {
        insertClassificationBtn("Development");
        insertAppList(developlist);
    }
    QStringList graphicslist=vector.at(5);
    if(!graphicslist.isEmpty())
    {
        insertClassificationBtn("Image");
        insertAppList(graphicslist);
    }
    QStringList gamelist=vector.at(6);
    if(!gamelist.isEmpty())
    {
        insertClassificationBtn("Game");
        insertAppList(gamelist);
    }
    QStringList officelist=vector.at(7);
    if(!officelist.isEmpty())
    {
        insertClassificationBtn("Office");
        insertAppList(officelist);
    }
    QStringList educationlist=vector.at(8);
    if(!educationlist.isEmpty())
    {
        insertClassificationBtn("Education");
        insertAppList(educationlist);
    }

    QStringList systemadminlist=vector.at(9);
    if(!systemadminlist.isEmpty())
    {
        insertClassificationBtn("System");
        insertAppList(systemadminlist);
    }
    QStringList otherlist=vector.at(10);
    if(!otherlist.isEmpty())
    {
        insertClassificationBtn("Others");
        insertAppList(otherlist);
    }

    resizeScrollAreaControls();
}

void FullFunctionWidget::insertClassificationBtn(QString category)
{
    PushButton* classificationbtn=new PushButton(this,category,m_scrollArea->width()-12,20,2);
    classificationbtn->setFixedSize(m_scrollArea->width()-12,20);
    m_scrollAreaWidLayout->addWidget(classificationbtn);
    m_classificationList.append(category);

}

void FullFunctionWidget::insertAppList(QStringList desktopfplist)
{
    FullListView* listview=new FullListView(this,2);
    m_scrollAreaWidLayout->addWidget(listview);
    m_data.clear();
    for(int i=0;i<desktopfplist.count();i++)
        m_data.append(desktopfplist.at(i));
    listview->addData(m_data);
    connect(listview,&FullListView::sendItemClickedSignal,this,&FullFunctionWidget::execApplication);
    connect(listview,&FullListView::sendHideMainWindowSignal,this,&FullFunctionWidget::sendHideMainWindowSignal);
}

//void FullFunctionWidget::updateRecentListView()
//{
//    data.clear();
//    QStringList recentlist;
//    recentlist.clear();
//    recentlist=pUkuiMenuInterface->getRecentApp();
//    if(!recentlist.isEmpty())//最近添加非空
//    {
//        if(m_classificationList.contains(tr("Recently")))//有最近添加分类
//        {
//            QLayoutItem *child;
//            if((child = m_scrollAreaWidLayout->itemAt(1)) != 0)
//            {
//                QWidget* wid=child->widget();
//                FullListView* listview=qobject_cast<FullListView*>(wid);
//                for(int i=0;i<recentlist.count();i++)
//                {
//                    QString desktopfp=pUkuiMenuInterface->getDesktopPathByAppName(recentlist.at(i));
//                    data.append(desktopfp);
//                }
//                listview->updateData(data);
//            }
//        }
//        else//无最近添加分类
//        {
//            PushButton* classificationbtn=new PushButton(this,tr("Recently"),scrollarea->width()-12,20);
//            classificationbtn->setFixedSize(scrollarea->width()-12,20);
//            m_scrollAreaWidLayout->insertWidget(0,classificationbtn);
//            m_classificationList.insert(0,tr("Recently"));

//            FullListView* listview=new FullListView(this,2);
//            m_scrollAreaWidLayout->insertWidget(1,listview);
//            for(int i=0;i<recentlist.count();i++)
//            {

//                QString desktopfp=pUkuiMenuInterface->getDesktopPathByAppName(recentlist.at(i));
//                data.append(desktopfp);
//            }
//            listview->addData(data);
//            connect(listview,SIGNAL(sendItemClickedSignal(QString)),this,SLOT(execApplication(QString)));

//            //刷新图标列表界面
//            Q_FOREACH (QAbstractButton* button, m_buttonList){
//                m_btnGroup->removeButton(button);
//            }
//            m_buttonList.clear();
//            QLayoutItem *child;
//            while ((child = m_iconListScrollAreaWidLayout->takeAt(0)) != 0) {
//                QWidget* wid=child->widget();
//                m_iconListScrollAreaWidLayout->removeWidget(wid);
//                wid->setParent(nullptr);
//                delete wid;
//                delete child;
//            }
//            initIconListScrollArea();
//        }
//    }
//    else//最近添加为空
//    {
//        if(m_classificationList.contains(tr("Recently")))
//        {
//            int num=0;
//            QLayoutItem *child;
//             while ((child = m_scrollAreaWidLayout->takeAt(0)) != 0) {
//                 QWidget* wid=child->widget();
//                 m_scrollAreaWidLayout->removeWidget(wid);
//                 wid->setParent(nullptr);
//                 delete wid;
//                 delete child;
//                 num++;
//                 if(num==2)
//                     break;
//             }
//             m_classificationList.removeAt(0);

//             //刷新图标列表界面
//             Q_FOREACH (QAbstractButton* button, m_buttonList){
//                 m_btnGroup->removeButton(button);
//             }
//             m_buttonList.clear();
//             while ((child = m_iconListScrollAreaWidLayout->takeAt(0)) != 0) {
//                 QWidget* wid=child->widget();
//                 m_iconListScrollAreaWidLayout->removeWidget(wid);
//                 wid->setParent(nullptr);
//                 delete wid;
//                 delete child;
//             }
//             initIconListScrollArea();
//        }
//    }
//    resizeScrollAreaControls();
//    setting->endGroup();
//}

/**
 * 执行应用程序
 */
void FullFunctionWidget::execApplication(QString desktopfp)
{
    this->parentWidget()->parentWidget()->parentWidget()->hide();
    GDesktopAppInfo * desktopAppInfo=g_desktop_app_info_new_from_filename(desktopfp.toLocal8Bit().data());
    g_app_info_launch(G_APP_INFO(desktopAppInfo),nullptr, nullptr, nullptr);
    g_object_unref(desktopAppInfo);}

/**
 * 更新应用列表
 */
void FullFunctionWidget::updateAppListView()
{
    //刷新应用列表界面
    QLayoutItem *child;
     while ((child = m_scrollAreaWidLayout->takeAt(0)) != 0) {
         QWidget* wid=child->widget();
         m_scrollAreaWidLayout->removeWidget(wid);
         wid->setParent(nullptr);
         delete wid;
         delete child;
     }
     fillAppList();

    //刷新图标列表界面
    Q_FOREACH (QAbstractButton* button, m_buttonList){
        m_btnGroup->removeButton(button);
    }
    m_buttonList.clear();
    while ((child = m_iconListScrollAreaWidLayout->takeAt(0)) != 0) {
        QWidget* wid=child->widget();
        m_iconListScrollAreaWidLayout->removeWidget(wid);
        wid->setParent(nullptr);
        delete wid;
        delete child;
    }
    initIconListScrollArea();
}

/**
 * 设置scrollarea所填充控件大小
 */
void FullFunctionWidget::resizeScrollAreaControls()
{
    int row=0;
    while(row<m_scrollAreaWidLayout->count()/2)
    {
        //应用界面
        QLayoutItem* widItem=m_scrollAreaWidLayout->itemAt(row*2+1);
        QWidget* wid=widItem->widget();
        FullListView* listview=qobject_cast<FullListView*>(wid);
        listview->adjustSize();
        int dividend=(m_scrollArea->width()-Style::SliderSize)/Style::AppListGridSizeWidth;
        int rowcount=0;
        if(listview->model()->rowCount()%dividend>0)
        {
            rowcount=listview->model()->rowCount()/dividend+1;
        }
        else
        {
            rowcount=listview->model()->rowCount()/dividend;

        }

        listview->setFixedSize(m_scrollArea->width()-Style::SliderSize+1,listview->gridSize().height()*rowcount);
        row++;
    }
    m_scrollArea->widget()->adjustSize();
}

/**
 * 初始化图标列表界面
 */
void FullFunctionWidget::initIconListWidget()
{
    m_iconListScrollArea=new ClassifyScrollArea(m_iconListWid);
    m_iconListScrollAreaWid=new QWidget;
    m_iconListScrollAreaWidLayout=new QVBoxLayout;
    m_iconListScrollAreaWidLayout->setContentsMargins(0,0,0,0);
    m_iconListScrollAreaWidLayout->setSpacing(Style::LeftSpaceBetweenItem);
    m_iconListScrollAreaWid->setLayout(m_iconListScrollAreaWidLayout);
    m_iconListScrollArea->setWidget(m_iconListScrollAreaWid);

    m_btnGroup=new QButtonGroup(m_iconListScrollAreaWid);
    m_animation = new QPropertyAnimation(m_iconListScrollArea, "geometry");

    m_scrollAnimation = new QPropertyAnimation(m_scrollArea->verticalScrollBar(), "value");
    m_scrollAnimation->setEasingCurve(QEasingCurve::OutQuad);
    connect(m_scrollAnimation, &QPropertyAnimation::finished, this, &FullFunctionWidget::animationFinishSlot);
    connect(m_scrollAnimation, &QPropertyAnimation::valueChanged, this, &FullFunctionWidget::animationValueChangedSlot);

    initIconListScrollArea();
}

/**
 * 初始化图标列表界面数据表格iconlisttableWid
 */
void FullFunctionWidget::initIconListScrollArea()
{
    for(int i=0;i<m_classificationList.size();i++)
//    QVector<QStringList> vector=UkuiMenuInterface::functionalVector;
//    QStringList androidlist=vector.at(0);
//    for(int index=0;index<vector.size();index++)
    {
//        if(!vector.at(index).isEmpty())
//        {
            FunctionClassifyButton* iconbtn=new FunctionClassifyButton(this,
                                                                       Style::LeftBtnWidth,
                                                                       Style::LeftBtnHeight,
                                                                       Style::LeftIconSize,
                                                                       m_classificationList.at(i),
                                                                       true,
                                                                       true);
            iconbtn->setChecked(false);
            m_buttonList.append(iconbtn);
            m_iconListScrollAreaWidLayout->addWidget(iconbtn);
//        }

    }

    int id=0;
    Q_FOREACH (QAbstractButton* btn, m_buttonList) {
        m_btnGroup->addButton(btn,id++);
    }
    connect(m_btnGroup,static_cast<void(QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked),this,&FullFunctionWidget::btnGroupClickedSlot);
    m_iconListScrollArea->widget()->adjustSize();
    m_btnGroup->button(0)->click();
}

void FullFunctionWidget::btnGroupClickedSlot(QAbstractButton *btn)
{
    disconnect(m_scrollArea->verticalScrollBar(),&QScrollBar::valueChanged,
               this,&FullFunctionWidget::valueChangedSlot);
    Q_FOREACH (QAbstractButton* button, m_buttonList) {
        if(m_btnGroup->id(btn)==m_buttonList.indexOf(button))
        {
            m_beginPos=m_scrollArea->verticalScrollBar()->sliderPosition();
            m_endPos=m_scrollAreaWidLayout->itemAt(m_btnGroup->id(btn)*2)->widget()->y();
            m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            m_scrollAnimation->stop();
            m_scrollAnimation->setStartValue(m_beginPos);
            m_scrollAnimation->setEndValue(m_endPos);
            m_scrollAnimation->start();
            button->setChecked(true);
        }
        else{
            button->setChecked(false);
        }
    }
}

void FullFunctionWidget::animationFinishSlot()
{
    if(m_scrollArea->verticalScrollBar()->value()==m_endPos ||
            m_scrollArea->verticalScrollBar()->value()==m_scrollArea->verticalScrollBar()->maximum())
    {
        m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        connect(m_scrollArea->verticalScrollBar(),&QScrollBar::valueChanged,
                this,&FullFunctionWidget::valueChangedSlot);
    }
}

void FullFunctionWidget::animationValueChangedSlot(const QVariant &value)
{
    Q_UNUSED(value);
    if (sender() != m_scrollAnimation)
        return;

    QPropertyAnimation *ani = qobject_cast<QPropertyAnimation *>(sender());

    if (m_endPos != ani->endValue())
        ani->setEndValue(m_endPos);
}

void FullFunctionWidget::valueChangedSlot(int value)
{
    int index=0;
    while(index<=m_classificationList.count()-1)
    {
        int min=m_scrollAreaWidLayout->itemAt(2*index)->widget()->y();
        int max=0;
        if(index==m_classificationList.count()-1)
            max=m_scrollAreaWid->height();
        else
            max=m_scrollAreaWidLayout->itemAt(2*(index+1))->widget()->y();
        if(value>=min && value<max)
        {
            Q_FOREACH (QAbstractButton* button, m_buttonList) {
                FunctionClassifyButton* fcbutton=qobject_cast<FunctionClassifyButton*>(button);
                if(index==m_buttonList.indexOf(button))
                {
                    fcbutton->setChecked(true);
                }
                else{
                    fcbutton->setChecked(false);
                }
            }
            break;
        }
        else
            index++;
    }
}

void FullFunctionWidget::enterAnimation()
{
    int height=m_classificationList.size()*Style::LeftBtnHeight+(m_classificationList.size()-1)*Style::LeftSpaceBetweenItem;
    m_animation->setDuration(200);//动画总时间
    m_animation->setStartValue(QRect(0,(m_iconListWid->height()-height)/2,
                                    0,height));
    m_animation->setEndValue(QRect(Style::LeftMargin,
                                  (m_iconListWid->height()-height)/2,
                                  Style::LeftBtnWidth,
                                  height));
    m_animation->setEasingCurve(QEasingCurve::InQuart);
    m_animation->start();
}

void FullFunctionWidget::repaintWidget()
{
    this->setFixedSize(Style::MainViewWidWidth,
                       Style::AppListWidHeight);
    m_applistWid->setFixedSize(Style::AppListWidWidth,this->height());
    m_iconListWid->setFixedSize(Style::LeftWidWidth,this->height());
    m_scrollArea->setFixedSize(m_applistWid->width(),m_applistWid->height());
    updateAppListView();
}

void FullFunctionWidget::widgetMakeZero()
{
//    Q_FOREACH (QAbstractButton* button, m_buttonList) {
//        int num=m_classificationList.indexOf(functionnamelist.at(m_buttonList.indexOf(button)));
//        if(num!=-1)
//        {
//            m_btnGroup->button(num)->setChecked(true);
//            m_btnGroup->button(num)->click();
//            m_iconListScrollArea->verticalScrollBar()->setSliderPosition(0);
//            break;
//        }
//    }
    m_btnGroup->button(0)->click();
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

void FullFunctionWidget::moveScrollBar(int type)
{
    int height=QApplication::primaryScreen()->geometry().height();
    if(type==0)
        m_scrollArea->verticalScrollBar()->setSliderPosition(m_scrollArea->verticalScrollBar()->sliderPosition()-height*100/1080);
    else
        m_scrollArea->verticalScrollBar()->setSliderPosition(m_scrollArea->verticalScrollBar()->sliderPosition()+height*100/1080);
}
