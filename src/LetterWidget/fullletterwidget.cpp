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
#include <QDebug>
#include <syslog.h>

FullLetterWidget::FullLetterWidget(QWidget *parent) :
    QWidget(parent)
{
    initUi();
}

FullLetterWidget::~FullLetterWidget()
{
    delete m_ukuiMenuInterface;
}

/**
 * 主界面初始化
 */
void FullLetterWidget::initUi()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setAttribute(Qt::WA_TranslucentBackground);

    m_applistWid=new QWidget(this);
    m_letterListWid=new QWidget(this);
    this->setFixedSize(Style::MainViewWidWidth,
                       Style::AppListWidHeight);
    m_applistWid->setFixedSize(Style::AppListWidWidth,this->height());
    m_letterListWid->setFixedSize(Style::LeftWidWidth,this->height());

    QHBoxLayout* mainLayout=new QHBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(m_letterListWid);
    mainLayout->addWidget(m_applistWid);
    this->setLayout(mainLayout);
    m_ukuiMenuInterface=new UkuiMenuInterface;

    initAppListWidget();
    initLetterListWidget();
}

/**
 * 初始化应用列表界面
 */
void FullLetterWidget::initAppListWidget()
{
    QHBoxLayout* layout=new QHBoxLayout(m_applistWid);
    layout->setContentsMargins(0,0,0,0);
    m_applistWid->setLayout(layout);

    m_scrollArea=new ScrollArea;
    m_scrollAreaWid=new ScrollAreaWid;
    m_scrollArea->setWidget(m_scrollAreaWid);
    m_scrollArea->setFixedSize(m_applistWid->width(),m_applistWid->height());
    m_scrollArea->setWidgetResizable(true);
    m_scrollAreaWidLayout=new QVBoxLayout;
    m_scrollAreaWidLayout->setContentsMargins(0,0,0,0);
    m_scrollAreaWidLayout->setSpacing(10);
    m_scrollAreaWid->setLayout(m_scrollAreaWidLayout);
    layout->addWidget(m_scrollArea);
    connect(m_scrollArea->verticalScrollBar(),&QScrollBar::valueChanged,
            this,&FullLetterWidget::valueChangedSlot);

    fillAppList();


}

/**
 * 填充应用列表
 */
void FullLetterWidget::fillAppList()
{
    m_letterList.clear();
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
            //插入字母分类按钮
            PushButton* letterbtn=new PushButton(this,letterstr,m_scrollArea->width()-12,30,1);
            m_scrollAreaWidLayout->addWidget(letterbtn);

            //插入应用列表
            FullListView* listview=new FullListView(this,1);

            //修复异常黑框问题
            connect(m_scrollArea, &ScrollArea::requestUpdate, listview->viewport(), [=](){
                listview->repaint(listview->rect());
            });

            m_scrollAreaWidLayout->addWidget(listview);
            m_data.clear();
            for(int i=0;i<appList.count();i++)
                m_data.append(appList.at(i));

            listview->addData(m_data);

            connect(listview,&FullListView::sendItemClickedSignal,this,&FullLetterWidget::execApplication);
            connect(listview,&FullListView::sendHideMainWindowSignal,this,&FullLetterWidget::sendHideMainWindowSignal);
        }
    }

        resizeScrollAreaControls();
}

/**
 * 执行应用程序
 */
void FullLetterWidget::execApplication(QString desktopfp)
{
    Q_EMIT sendHideMainWindowSignal();
    GDesktopAppInfo * desktopAppInfo=g_desktop_app_info_new_from_filename(desktopfp.toLocal8Bit().data());
    g_app_info_launch(G_APP_INFO(desktopAppInfo),nullptr, nullptr, nullptr);
    g_object_unref(desktopAppInfo);
}

/**
 * 更新应用列表
 */
void FullLetterWidget::updateAppListView()
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

    //刷新字母列表界面
    Q_FOREACH (QAbstractButton* button, m_buttonList) {
        m_btnGroup->removeButton(button);
    }
    m_buttonList.clear();
    m_letterListScrollAreaWidLayout->removeItem(m_letterListBottomSpacer);
    while ((child = m_letterListScrollAreaWidLayout->takeAt(0)) != 0) {
        QWidget* wid=child->widget();
        m_letterListScrollAreaWidLayout->removeWidget(wid);
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
 * 初始化字母列表界面
 */
void FullLetterWidget::initLetterListWidget()
{
    m_letterListScrollArea=new ClassifyScrollArea(m_letterListWid);
    m_letterListScrollAreaWid=new ClassifyScrollAreaWid;
    m_letterListScrollAreaWidLayout=new QVBoxLayout;
    m_letterListScrollAreaWidLayout->setContentsMargins(0,0,0,0);
    m_letterListScrollAreaWidLayout->setSpacing(0);
    m_letterListScrollAreaWid->setLayout(m_letterListScrollAreaWidLayout);
    m_letterListScrollArea->setWidget(m_letterListScrollAreaWid);
    m_letterListScrollArea->setWidgetResizable(true);

    m_letterListBottomSpacer=new QSpacerItem(20,40,QSizePolicy::Fixed,QSizePolicy::Expanding);
    m_btnGroup=new QButtonGroup(m_letterListScrollAreaWid);
    m_animation = new QPropertyAnimation(m_letterListScrollArea, "geometry");

    m_scrollAnimation = new QPropertyAnimation(m_scrollArea->verticalScrollBar(), "value");
    m_scrollAnimation->setEasingCurve(QEasingCurve::OutQuad);
    connect(m_scrollAnimation, &QPropertyAnimation::finished, this, &FullLetterWidget::animationFinishSlot);
    connect(m_scrollAnimation, &QPropertyAnimation::valueChanged, this, &FullLetterWidget::animationValueChangedSlot);

    initLetterListScrollArea();
}

/**
 * 初始化字母列表
 */
void FullLetterWidget::initLetterListScrollArea()
{
    if(m_letterList.contains("&"))
        m_letterList.replace(m_letterList.indexOf("&"),"&&");
    for(int i=0;i<m_letterList.size();i++)
    {
        LetterClassifyButton* letterbtn=new LetterClassifyButton(m_letterListScrollAreaWid,
                                                                 true,
                                                                 m_letterList.at(i));
        letterbtn->setFixedSize(Style::LeftLetterBtnHeight,Style::LeftLetterBtnHeight);
        m_buttonList.append(letterbtn);
        m_letterListScrollAreaWidLayout->addWidget(letterbtn);
        m_letterListScrollAreaWidLayout->setAlignment(letterbtn,Qt::AlignHCenter);
        connect(letterbtn,&LetterClassifyButton::buttonClicked,m_btnGroup, static_cast<void(QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked));
    }
    m_letterListScrollAreaWidLayout->addItem(m_letterListBottomSpacer);

    int id=0;
    Q_FOREACH (QAbstractButton* btn, m_buttonList) {
        m_btnGroup->addButton(btn,id++);
    }

    connect(m_btnGroup,static_cast<void(QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked),this,&FullLetterWidget::btnGroupClickedSlot);
    m_letterListScrollArea->widget()->adjustSize();
    m_btnGroup->button(0)->click();
}

void FullLetterWidget::btnGroupClickedSlot(QAbstractButton *btn)
{
    disconnect(m_scrollArea->verticalScrollBar(),&QScrollBar::valueChanged,
            this,&FullLetterWidget::valueChangedSlot);
    Q_FOREACH (QAbstractButton* button, m_buttonList) {
        LetterClassifyButton* letterbtn=qobject_cast<LetterClassifyButton*>(button);
        if(m_btnGroup->id(btn)==m_buttonList.indexOf(button))
        {
            letterbtn->setChecked(true);
            //此处需实现将被选定的字母包含的应用列表移动到applistWid界面最顶端
            QString letterstr=letterbtn->text();
            int num=m_letterList.indexOf(letterstr);
            if(num!=-1)
            {
                m_beginPos=m_scrollArea->verticalScrollBar()->sliderPosition();
                m_endPos=m_scrollAreaWidLayout->itemAt(m_btnGroup->id(btn)*2)->widget()->y();
                m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                m_scrollAnimation->stop();
                m_scrollAnimation->setStartValue(m_beginPos);
                m_scrollAnimation->setEndValue(m_endPos);
                m_scrollAnimation->start();

            }
        }
        else{
            letterbtn->setChecked(false);
        }
    }
}

void FullLetterWidget::animationFinishSlot()
{
    if(m_scrollArea->verticalScrollBar()->value()==m_endPos ||
            m_scrollArea->verticalScrollBar()->value()==m_scrollArea->verticalScrollBar()->maximum())
    {
        m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        connect(m_scrollArea->verticalScrollBar(),&QScrollBar::valueChanged,
                this,&FullLetterWidget::valueChangedSlot);
    }
}

void FullLetterWidget::animationValueChangedSlot(const QVariant &value)
{
    Q_UNUSED(value);
    if (sender() != m_scrollAnimation)
        return;

    QPropertyAnimation *ani = qobject_cast<QPropertyAnimation *>(sender());

    if (m_endPos != ani->endValue())
        ani->setEndValue(m_endPos);
}

void FullLetterWidget::valueChangedSlot(int value)
{
    int index=0;
    while(index<=m_letterList.count()-1)
    {
        int min=m_scrollAreaWidLayout->itemAt(2*index)->widget()->y();
        int max=0;
        if(index==m_letterList.count()-1)
            max=m_scrollAreaWid->height();
        else
            max=m_scrollAreaWidLayout->itemAt(2*(index+1))->widget()->y();

        if(value>=min && value <max)
        {
            Q_FOREACH (QAbstractButton* button, m_buttonList) {
                LetterClassifyButton* letterbtn=qobject_cast<LetterClassifyButton*>(button);
                if(index==m_buttonList.indexOf(button))
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
            index++;
    }

    //向下滚动
    if((m_buttonList.at(index)->pos().y()+m_buttonList.at(index)->height()+m_letterListScrollArea->widget()->pos().y()) >= m_letterListScrollArea->height())
    {
        int val=m_letterListScrollArea->verticalScrollBar()->sliderPosition()+m_buttonList.at(index)->height();
        m_letterListScrollArea->verticalScrollBar()->setSliderPosition(val);
    }

    //向上滚动
    if((m_buttonList.at(index)->pos().y()+m_letterListScrollArea->widget()->pos().y()) <= 0)
    {

        int val=m_letterListScrollArea->verticalScrollBar()->value()-m_buttonList.at(index)->height();
        m_letterListScrollArea->verticalScrollBar()->setSliderPosition(val);
    }

}

void FullLetterWidget::enterAnimation()
{
    m_animation->setDuration(200);//动画总时间
    m_animation->setStartValue(QRect(0,(m_letterListWid->height()-(m_letterList.size()+1)*Style::LeftLetterBtnHeight)/2,
                                    0,(m_letterList.size()+1)*Style::LeftLetterBtnHeight));
    m_animation->setEndValue(QRect(Style::LeftMargin,
                                  (m_letterListWid->height()-(m_letterList.size()+1)*Style::LeftLetterBtnHeight)/2,
                                  Style::LeftLetterBtnHeight*2,
                            (m_letterList.size()+1)*Style::LeftLetterBtnHeight));
    m_animation->setEasingCurve(QEasingCurve::InQuart);
    m_animation->start();
    m_letterListScrollArea->show();
}

void FullLetterWidget::setLetterBtnGeometry()
{
    m_letterListScrollArea->setGeometry(QRect(Style::LeftMargin,
                                              (m_letterListWid->height()-(m_letterList.size()+1)*Style::LeftLetterBtnHeight)/2,
                                              Style::LeftLetterBtnHeight*2,
                                        (m_letterList.size()+1)*Style::LeftLetterBtnHeight));
    m_letterListScrollArea->show();
}

void FullLetterWidget::repaintWidget()
{
    this->setFixedSize(Style::MainViewWidWidth,
                       Style::AppListWidHeight);
    m_applistWid->setFixedSize(Style::AppListWidWidth,this->height());
    m_letterListWid->setFixedSize(Style::LeftWidWidth,this->height());
    m_scrollArea->setFixedSize(m_applistWid->width(),m_applistWid->height());
    updateAppListView();
}

void FullLetterWidget::widgetMakeZero()
{
    Q_FOREACH (QAbstractButton* button, m_buttonList) {
        QString letterstr=button->text().at(0);
        int num=m_letterList.indexOf(letterstr);
        if(num!=-1)
        {
            m_btnGroup->button(num)->click();
            m_letterListScrollArea->verticalScrollBar()->setSliderPosition(0);
            break;
        }
    }
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

void FullLetterWidget::moveScrollBar(int type)
{
    int height=QApplication::primaryScreen()->geometry().height();
    if(type==0)
        m_scrollArea->verticalScrollBar()->setSliderPosition(m_scrollArea->verticalScrollBar()->sliderPosition()-height*100/1080);
    else
        m_scrollArea->verticalScrollBar()->setSliderPosition(m_scrollArea->verticalScrollBar()->sliderPosition()+height*100/1080);
}
