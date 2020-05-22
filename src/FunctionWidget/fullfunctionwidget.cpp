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
#include "ui_fullfunctionwidget.h"
#include <QDesktopWidget>
#include <QHeaderView>
#include "src/Style/style.h"
#include <QDebug>
#include <QSvgRenderer>
#include <QPainter>

FullFunctionWidget::FullFunctionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FullFunctionWidget)
{
    ui->setupUi(this);

    initWidget();
}

FullFunctionWidget::~FullFunctionWidget()
{
    delete ui;
    delete pUkuiMenuInterface;
}

void FullFunctionWidget::initWidget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setStyleSheet("border:0px;background:transparent;");
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    applistWid=new QWidget(this);
    iconlistWid=new QWidget(this);
    this->setFixedSize(Style::MainViewWidWidth,
                       Style::AppListWidHeight);
    applistWid->setFixedSize(Style::AppListWidWidth,this->height());
    iconlistWid->setFixedSize(Style::LeftWidWidth,this->height());

    mainLayout=new QHBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    applistWid->setStyleSheet("border:0px;background:transparent;");
    iconlistWid->setStyleSheet("border:0px;background:transparent");
    mainLayout->addWidget(iconlistWid);
    mainLayout->addWidget(applistWid);
    this->setLayout(mainLayout);

    pUkuiMenuInterface=new UkuiMenuInterface;

    functionnamelist.clear();
    functionnamelist.append(tr("Mobile"));
    functionnamelist.append(tr("Internet"));
    functionnamelist.append(tr("Social"));
    functionnamelist.append(tr("Video"));
    functionnamelist.append(tr("Development"));
    functionnamelist.append(tr("Image"));
    functionnamelist.append(tr("Game"));
    functionnamelist.append(tr("Office"));
    functionnamelist.append(tr("Education"));
    functionnamelist.append(tr("System"));
    functionnamelist.append(tr("Others"));

    categorylist.clear();
    categorylist.append("Mobile");
    categorylist.append("Internet");
    categorylist.append("Social");
    categorylist.append("Video");
    categorylist.append("Development");
    categorylist.append("Image");
    categorylist.append("Game");
    categorylist.append("Office");
    categorylist.append("Education");
    categorylist.append("System");
    categorylist.append("Others");

    QString path=QDir::homePath()+"/.config/ukui/ukui-menu.ini";
    setting=new QSettings(path,QSettings::IniFormat);

    initAppListWidget();
    initIconListWidget();
}

/**
 * 初始化应用列表界面
 */
void FullFunctionWidget::initAppListWidget()
{
    QHBoxLayout* layout=new QHBoxLayout(applistWid);
    layout->setContentsMargins(0,0,0,0);
    applistWid->setLayout(layout);

    scrollarea=new ScrollArea;
    scrollareawid=new QWidget;
    scrollarea->setFixedSize(applistWid->width(),applistWid->height());
    scrollarea->setWidget(scrollareawid);
    scrollarea->setWidgetResizable(true);
    scrollareawidLayout=new QVBoxLayout;
    scrollareawidLayout->setContentsMargins(0,0,0,0);
    scrollareawidLayout->setSpacing(10);
    scrollareawid->setLayout(scrollareawidLayout);
    layout->addWidget(scrollarea);
    connect(scrollarea->verticalScrollBar(),&QScrollBar::valueChanged,
            this,&FullFunctionWidget::valueChangedSlot);
    fillAppList();
}

/**
 * 填充应用列表
 */
void FullFunctionWidget::fillAppList()
{
    classificationbtnlist.clear();
    classificationbtnrowlist.clear();
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
        insertClassificationBtn(tr("Others"));
        insertAppList(otherlist);
    }

    resizeScrollAreaControls();
}

void FullFunctionWidget::insertClassificationBtn(QString btnname)
{
    PushButton* classificationbtn=new PushButton(this,btnname,scrollarea->width()-12,20);
    classificationbtn->setFixedSize(scrollarea->width()-12,20);
    scrollareawidLayout->addWidget(classificationbtn);
    classificationbtnlist.append(btnname);

}

void FullFunctionWidget::insertAppList(QStringList appnamelist)
{
    FullListView* listview=new FullListView(this,2);
    scrollareawidLayout->addWidget(listview);
    data.clear();
    for(int i=0;i<appnamelist.count();i++)
        data.append(appnamelist.at(i));
    listview->addData(data);
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
//        if(classificationbtnlist.contains(tr("Recently")))//有最近添加分类
//        {
//            QLayoutItem *child;
//            if((child = scrollareawidLayout->itemAt(1)) != 0)
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
//            scrollareawidLayout->insertWidget(0,classificationbtn);
//            classificationbtnlist.insert(0,tr("Recently"));

//            FullListView* listview=new FullListView(this,2);
//            scrollareawidLayout->insertWidget(1,listview);
//            for(int i=0;i<recentlist.count();i++)
//            {

//                QString desktopfp=pUkuiMenuInterface->getDesktopPathByAppName(recentlist.at(i));
//                data.append(desktopfp);
//            }
//            listview->addData(data);
//            connect(listview,SIGNAL(sendItemClickedSignal(QString)),this,SLOT(execApplication(QString)));
//            connect(listview,SIGNAL(sendFixedOrUnfixedSignal(QString,int)),this,SIGNAL(sendUpdateAppListSignal(QString,int)));
//            connect(listview,SIGNAL(sendHideMainWindowSignal()),this,SIGNAL(sendHideMainWindowSignal()));

//            //刷新图标列表界面
//            Q_FOREACH (QAbstractButton* button, buttonList){
//                pBtnGroup->removeButton(button);
//            }
//            buttonList.clear();
//            QLayoutItem *child;
//            while ((child = iconlistscrollareawidLayout->takeAt(0)) != 0) {
//                QWidget* wid=child->widget();
//                iconlistscrollareawidLayout->removeWidget(wid);
//                wid->setParent(nullptr);
//                delete wid;
//                delete child;
//            }
//            initIconListScrollArea();
//        }
//    }
//    else//最近添加为空
//    {
//        if(classificationbtnlist.contains(tr("Recently")))
//        {
//            int num=0;
//            QLayoutItem *child;
//             while ((child = scrollareawidLayout->takeAt(0)) != 0) {
//                 QWidget* wid=child->widget();
//                 scrollareawidLayout->removeWidget(wid);
//                 wid->setParent(nullptr);
//                 delete wid;
//                 delete child;
//                 num++;
//                 if(num==2)
//                     break;
//             }
//             classificationbtnlist.removeAt(0);

//             //刷新图标列表界面
//             Q_FOREACH (QAbstractButton* button, buttonList){
//                 pBtnGroup->removeButton(button);
//             }
//             buttonList.clear();
//             while ((child = iconlistscrollareawidLayout->takeAt(0)) != 0) {
//                 QWidget* wid=child->widget();
//                 iconlistscrollareawidLayout->removeWidget(wid);
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
    Q_EMIT sendHideMainWindowSignal();
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
     while ((child = scrollareawidLayout->takeAt(0)) != 0) {
         QWidget* wid=child->widget();
         scrollareawidLayout->removeWidget(wid);
         wid->setParent(nullptr);
         delete wid;
         delete child;
     }
     fillAppList();

    //刷新图标列表界面
    Q_FOREACH (QAbstractButton* button, buttonList){
        pBtnGroup->removeButton(button);
    }
    buttonList.clear();
    while ((child = iconlistscrollareawidLayout->takeAt(0)) != 0) {
        QWidget* wid=child->widget();
        iconlistscrollareawidLayout->removeWidget(wid);
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
    int pos=0;
    classificationbtnrowlist.clear();
    classificationbtnrowlist.append(QString::number(pos));
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
            classificationbtnrowlist.append(QString::number(pos));
        }
        row++;
    }
    scrollarea->widget()->adjustSize();
}

/**
 * 初始化图标列表界面
 */
void FullFunctionWidget::initIconListWidget()
{
    iconlistscrollarea=new ClassifyScrollArea(iconlistWid);
    iconlistscrollareaWid=new QWidget;
    iconlistscrollareawidLayout=new QVBoxLayout;
    iconlistscrollareawidLayout->setContentsMargins(0,0,0,0);
    iconlistscrollareawidLayout->setSpacing(Style::LeftSpaceBetweenItem);
    iconlistscrollareaWid->setLayout(iconlistscrollareawidLayout);
    iconlistscrollarea->setWidget(iconlistscrollareaWid);

    pIconListTopSpacer=new QSpacerItem(20,40,QSizePolicy::Fixed,QSizePolicy::Expanding);
    pIconListBottomSpacer=new QSpacerItem(20,40,QSizePolicy::Fixed,QSizePolicy::Expanding);
    pBtnGroup=new QButtonGroup(iconlistscrollareaWid);
    pAnimation = new QPropertyAnimation(iconlistscrollarea, "geometry");

    m_scrollAnimation = new QPropertyAnimation(scrollarea->verticalScrollBar(), "value");
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
    for(int i=0;i<classificationbtnlist.size();i++)
    {
        FunctionClassifyButton* iconbtn=new FunctionClassifyButton(this,
                                                                   Style::LeftBtnWidth,
                                                                   Style::LeftBtnHeight,
                                                                   Style::LeftIconSize,
                                                                   categorylist.at(functionnamelist.indexOf(classificationbtnlist.at(i))),
                                                                   classificationbtnlist.at(i),
                                                                   true,
                                                                   true);
        iconbtn->setChecked(false);
        buttonList.append(iconbtn);
        iconlistscrollareawidLayout->addWidget(iconbtn);
    }

    int id=0;
    Q_FOREACH (QAbstractButton* btn, buttonList) {
        pBtnGroup->addButton(btn,id++);
    }
    connect(pBtnGroup,static_cast<void(QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked),this,&FullFunctionWidget::btnGroupClickedSlot);
    iconlistscrollarea->widget()->adjustSize();
    pBtnGroup->button(0)->click();
}

void FullFunctionWidget::btnGroupClickedSlot(QAbstractButton *btn)
{
    disconnect(scrollarea->verticalScrollBar(),&QScrollBar::valueChanged,
               this,&FullFunctionWidget::valueChangedSlot);
    Q_FOREACH (QAbstractButton* button, buttonList) {
        if(pBtnGroup->id(btn)==buttonList.indexOf(button))
        {
            beginPos=scrollarea->verticalScrollBar()->sliderPosition();
            endPos=classificationbtnrowlist.at(pBtnGroup->id(btn)).toInt();
            scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            m_scrollAnimation->stop();
            m_scrollAnimation->setStartValue(beginPos);
            m_scrollAnimation->setEndValue(endPos);
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
    if(scrollarea->verticalScrollBar()->value()==endPos ||
            scrollarea->verticalScrollBar()->value()==scrollarea->verticalScrollBar()->maximum())
    {
        scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        connect(scrollarea->verticalScrollBar(),&QScrollBar::valueChanged,
                this,&FullFunctionWidget::valueChangedSlot);
    }
}

void FullFunctionWidget::animationValueChangedSlot(const QVariant &value)
{
    Q_UNUSED(value);
    if (sender() != m_scrollAnimation)
        return;

    QPropertyAnimation *ani = qobject_cast<QPropertyAnimation *>(sender());

    if (endPos != ani->endValue())
        ani->setEndValue(endPos);
}

void FullFunctionWidget::valueChangedSlot(int value)
{
    int count=0;
    while(count<classificationbtnrowlist.count()-1)
    {
        if(value>=classificationbtnrowlist.at(count).toInt() &&
                value <classificationbtnrowlist.at(count+1).toInt())
        {
            Q_FOREACH (QAbstractButton* button, buttonList) {
                FunctionClassifyButton* fcbutton=qobject_cast<FunctionClassifyButton*>(button);
                if(count==buttonList.indexOf(button))
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
            count++;
    }
}

void FullFunctionWidget::enterAnimation()
{
    int height=classificationbtnlist.size()*Style::LeftBtnHeight+(classificationbtnlist.size()-1)*Style::LeftSpaceBetweenItem;
    pAnimation->setDuration(200);//动画总时间
    pAnimation->setStartValue(QRect(0,(iconlistWid->height()-height)/2,
                                    0,height));
    pAnimation->setEndValue(QRect(Style::LeftMargin,
                                  (iconlistWid->height()-height)/2,
                                  Style::LeftBtnWidth,
                                  height));
    pAnimation->setEasingCurve(QEasingCurve::InQuart);
    pAnimation->start();
}

void FullFunctionWidget::repaintWidget()
{
    this->setFixedSize(Style::MainViewWidWidth,
                       Style::AppListWidHeight);
    applistWid->setFixedSize(Style::AppListWidWidth,this->height());
    iconlistWid->setFixedSize(Style::LeftWidWidth,this->height());
    scrollarea->setFixedSize(applistWid->width(),applistWid->height());
    updateAppListView();
}

void FullFunctionWidget::widgetMakeZero()
{
//    Q_FOREACH (QAbstractButton* button, buttonList) {
//        int num=classificationbtnlist.indexOf(functionnamelist.at(buttonList.indexOf(button)));
//        if(num!=-1)
//        {
//            pBtnGroup->button(num)->setChecked(true);
//            pBtnGroup->button(num)->click();
//            iconlistscrollarea->verticalScrollBar()->setSliderPosition(0);
//            break;
//        }
//    }
    pBtnGroup->button(0)->click();
    scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

void FullFunctionWidget::moveScrollBar(int type)
{
    int height=QApplication::primaryScreen()->geometry().height();
    if(type==0)
        scrollarea->verticalScrollBar()->setSliderPosition(scrollarea->verticalScrollBar()->sliderPosition()-height*100/1080);
    else
        scrollarea->verticalScrollBar()->setSliderPosition(scrollarea->verticalScrollBar()->sliderPosition()+height*100/1080);
}
