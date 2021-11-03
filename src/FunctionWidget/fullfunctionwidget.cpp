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
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setAttribute(Qt::WA_TranslucentBackground);
    m_applistWid=new QWidget(this);
    m_iconListWid=new QWidget(this);
    this->setFixedSize(Style::MainViewWidWidth,
                       Style::AppListWidHeight);
    m_applistWid->setFixedSize(Style::AppListWidWidth,this->height());
    m_iconListWid->setFixedSize(Style::LeftWidWidth,this->height());

    QHBoxLayout* mainLayout=new QHBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(m_iconListWid);
    mainLayout->addWidget(m_applistWid);
    this->setLayout(mainLayout);

    m_ukuiMenuInterface=new UkuiMenuInterface;

    initAppListWidget();
    initIconListWidget();

    flag = true;
    //翻页灵敏度时间调节
    time = new QTimer(this);
    connect(time,&QTimer::timeout,[=](){
        if(flag == false)
        {
            flag = true;
            time->stop();
        }
    });
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
    m_scrollAreaWid=new ScrollAreaWid;
    m_scrollAreaWid->setAttribute(Qt::WA_TranslucentBackground);
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
    m_appListBottomSpacer=new QSpacerItem(20,40,QSizePolicy::Fixed,QSizePolicy::Expanding);
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
    m_scrollAreaWidLayout->addItem(m_appListBottomSpacer);
    resizeScrollAreaControls();
}

void FullFunctionWidget::insertClassificationBtn(QString category)
{
    SplitBarFrame* classificationbtn=new SplitBarFrame(this,category,m_scrollArea->width()-16,30,2);
    m_scrollAreaWidLayout->addWidget(classificationbtn);
    m_classificationList.append(category);

}

void FullFunctionWidget::insertAppList(QStringList desktopfplist)
{
    FullListView* listview=new FullListView(this,2);
    //修复异常黑框问题
    connect(m_scrollArea, &ScrollArea::requestUpdate, listview->viewport(), [=](){
        QEventLoop loop;
        QTimer::singleShot(1, &loop, SLOT(quit()));
        loop.exec();
        listview->repaint(listview->rect());
    });
    connect(listview, &FullListView::sendSetslidebar, this, &FullFunctionWidget::onSetSlider);
    connect(this, &FullFunctionWidget::selectFirstItem, listview, &FullListView::selectFirstItem);
    listview->installEventFilter(this);
    m_scrollAreaWidLayout->addWidget(listview);
    m_data.clear();
    for(int i=0;i<desktopfplist.count();i++)
        m_data.append(desktopfplist.at(i));
    listview->addData(m_data);
    connect(listview,&FullListView::sendItemClickedSignal,this,&FullFunctionWidget::execApplication);
    connect(listview,&FullListView::sendHideMainWindowSignal,this,&FullFunctionWidget::sendHideMainWindowSignal);
}

/**
 * 执行应用程序
 */
void FullFunctionWidget::execApplication(QString desktopfp)
{
    Q_EMIT sendHideMainWindowSignal();
    execApp(desktopfp);
}

/**
 * 更新应用列表
 */
void FullFunctionWidget::updateAppListView()
{
    //刷新应用列表界面
    QLayoutItem *child;
    m_scrollAreaWidLayout->removeItem(m_appListBottomSpacer);
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
    m_iconListScrollAreaWidLayout->removeItem(m_topSpacerItem);
    m_iconListScrollAreaWidLayout->removeItem(m_bottomSpacerItem);
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

        listview->setFixedSize(m_scrollArea->width()-Style::SliderSize + 1,listview->gridSize().height()*rowcount);
        row++;
    }
    m_scrollArea->widget()->adjustSize();
}

/**
 * 初始化图标列表界面
 */
void FullFunctionWidget::initIconListWidget()
{
    m_iconListScrollArea=new ClassifyBtnScrollArea(m_iconListWid);
//    m_iconListScrollArea->resize(Style::LeftBtnWidth,
//                                       m_iconListWid->height());
    m_iconListScrollAreaWid=new ClassifyBtnScrollAreaWid;
    m_iconListScrollAreaWid->setFixedSize(Style::LeftBtnWidth,
                                          m_iconListWid->height());
    m_iconListScrollAreaWidLayout=new QVBoxLayout;
    m_iconListScrollAreaWidLayout->setContentsMargins(0,0,0,0);
    m_iconListScrollAreaWidLayout->setSpacing(Style::LeftSpaceBetweenItem);
    m_iconListScrollAreaWid->setLayout(m_iconListScrollAreaWidLayout);
    m_iconListScrollArea->setWidget(m_iconListScrollAreaWid);

    m_topSpacerItem=new QSpacerItem(20,40,QSizePolicy::Fixed,QSizePolicy::Expanding);
    m_bottomSpacerItem=new QSpacerItem(20,40,QSizePolicy::Fixed,QSizePolicy::Expanding);

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
    m_iconListScrollAreaWidLayout->addItem(m_topSpacerItem);
    for(int i=0;i<m_classificationList.size();i++)
    {
        FunctionClassifyButton* iconbtn=new FunctionClassifyButton(
                    Style::LeftBtnWidth,
                    Style::LeftBtnHeight,
                    Style::LeftIconSize,
                    m_classificationList.at(i),
                    true,
                    true);
        iconbtn->setChecked(false);
        m_buttonList.append(iconbtn);
        m_iconListScrollAreaWidLayout->addWidget(iconbtn);
    }
    m_iconListScrollAreaWidLayout->addItem(m_bottomSpacerItem);

    int id=0;
    Q_FOREACH (QAbstractButton* btn, m_buttonList) {
        m_btnGroup->addButton(btn,id++);
    }
    connect(m_btnGroup,static_cast<void(QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked),this,&FullFunctionWidget::btnGroupClickedSlot);
//    m_iconListScrollArea->widget()->adjustSize();
    if(m_btnGroup->button(0)!=nullptr)
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

QAbstractButton* FullFunctionWidget::getCurLetterButton(int value)
{
    return m_buttonList.at(value);
}


void FullFunctionWidget::enterAnimation()
{
    m_animation->setDuration(200);//动画总时间
    m_animation->setStartValue(QRect(0,0,
                                    0,m_iconListWid->height()));
    m_animation->setEndValue(QRect(Style::LeftMargin,
                                  0,
                                  Style::LeftBtnWidth,
                                  m_iconListWid->height()));
    m_animation->setEasingCurve(QEasingCurve::InQuart);
    m_animation->start();
    m_iconListScrollArea->show();
}

void FullFunctionWidget::setFunctionBtnGeometry()
{
//    int height=m_classificationList.size()*Style::LeftBtnHeight+(m_classificationList.size()-1)*Style::LeftSpaceBetweenItem;
    m_iconListScrollArea->setGeometry(QRect(Style::LeftMargin,
                                            0,
                                            Style::LeftBtnWidth,
                                            m_iconListWid->height()));
    m_iconListScrollArea->show();

}

void FullFunctionWidget::repaintWidget()
{
    this->setFixedSize(Style::MainViewWidWidth,
                       Style::AppListWidHeight);
    m_applistWid->setFixedSize(Style::AppListWidWidth,this->height());
    m_scrollArea->setFixedSize(m_applistWid->width(),m_applistWid->height());
    m_iconListWid->setFixedSize(Style::LeftWidWidth,this->height());
    m_iconListScrollAreaWid->setFixedSize(Style::LeftBtnWidth,
                                          m_iconListWid->height());
    updateAppListView();
}

void FullFunctionWidget::widgetMakeZero()
{
    if(m_btnGroup->button(0)!=nullptr)
        m_btnGroup->button(0)->click();
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

void FullFunctionWidget::moveScrollBar(int type)
{
    int height=Style::primaryScreenHeight;
    if(type==0)
        m_scrollArea->verticalScrollBar()->setSliderPosition(m_scrollArea->verticalScrollBar()->sliderPosition()-height*100/1080);
    else
        m_scrollArea->verticalScrollBar()->setSliderPosition(m_scrollArea->verticalScrollBar()->sliderPosition()+height*100/1080);
}

void FullFunctionWidget::onSetSlider(int value)
{
//    if(flag)
//    {
//        flag = false;
//        time->start(100);
        int curvalue = m_scrollArea->verticalScrollBar()->value();
        m_scrollArea->verticalScrollBar()->setValue(curvalue + value);
//        qDebug() << "FullFunctionWidget::onSetSlider" << curvalue;
//    }
}

bool FullFunctionWidget::eventFilter(QObject *watched, QEvent *event)
{
    if( event->type() == QEvent::KeyPress )
    {
        QLayoutItem* widItem = m_scrollAreaWidLayout->itemAt(2 * m_buttonList.size() - 1);
        QWidget* wid = widItem->widget();
        FullListView* m_listview = qobject_cast<FullListView*>(wid);

        QLayoutItem* widItemTop = m_scrollAreaWidLayout->itemAt(1);
        QWidget* widTop = widItemTop->widget();
        FullListView* m_listviewTop = qobject_cast<FullListView*>(widTop);

        QKeyEvent *ke = (QKeyEvent *)event;
        if( ke->key() == Qt::Key_Tab )
        {
           Q_EMIT setFocusToSideWin();
        }
//        if(ke->key() == Qt::Key_Up)
//        {
//            QLayoutItem* widItemTop = m_scrollAreaWidLayout->itemAt(1);
//            QWidget* widTop = widItemTop->widget();
//            FullListView* m_listviewTop = qobject_cast<FullListView*>(widTop);
//            if(!m_listviewTop->hasFocus())
//            {
//                QAbstractButton* buttonTop = getCurLetterButton(( --m_index) % m_buttonList.size());
//                btnGroupClickedSlot(buttonTop);
//                this->m_scrollArea->setFocusToPreChild();
//                return true;
//            }
//        }
//        if(ke->key() == Qt::Key_Down)
//        {
//            QLayoutItem* widItem = m_scrollAreaWidLayout->itemAt(2 * m_buttonList.size() - 1);
//            QWidget* wid = widItem->widget();
//            FullListView* m_listview = qobject_cast<FullListView*>(wid);

//            if(!m_listview->hasFocus())
//            {
//                QAbstractButton* button = getCurLetterButton(( ++m_index) % m_buttonList.size());
//                btnGroupClickedSlot(button);
//                this->m_scrollArea->setFocusToNextChild();
//                return true;
//            }
//        }

        if(ke->key() == Qt::Key_Up)
        {
            if(!m_listviewTop->hasFocus())
            {
                QAbstractButton* buttonTop = getCurLetterButton(( --m_index) % m_buttonList.size());
                btnGroupClickedSlot(buttonTop);
                this->m_scrollArea->setFocusToPreChild();
            }
            else
            {
               m_listview->setFocus();
               QAbstractButton* button = getCurLetterButton(m_buttonList.size() - 1);
               btnGroupClickedSlot(button);
               m_index = m_buttonList.size() - 1;
            }
            Q_EMIT selectFirstItem();
            return true;
        }
        if(ke->key() == Qt::Key_Down)
        {
            if(!m_listview->hasFocus())
            {
                QAbstractButton* button = getCurLetterButton(( ++m_index) % m_buttonList.size());
                btnGroupClickedSlot(button);
                this->m_scrollArea->setFocusToNextChild();
            }
            else
            {
                m_listviewTop->setFocus();
                QAbstractButton* buttonTop = getCurLetterButton(0);
                btnGroupClickedSlot(buttonTop);
                m_listviewTop->setCurrentIndex(m_listviewTop->model()->index(0,0));
                m_index = 0;
            }
            Q_EMIT selectFirstItem();
            return true;
        }
    }
    return QWidget::eventFilter(watched,event);
}

void FullFunctionWidget::functionButtonClick()
{
    if(m_btnGroup->button(0)!=nullptr)
        m_btnGroup->button(0)->click();
    m_index = 0;
}

void FullFunctionWidget::setFocusToThis()
{
    functionButtonClick();
    this->setFocus();
    Q_EMIT selectFirstItem();
}
