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

    init_widget();
}

FullFunctionWidget::~FullFunctionWidget()
{
    delete ui;
    delete pUkuiMenuInterface;
}

void FullFunctionWidget::init_widget()
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

    iconlist.clear();
    QString recentstr=":/data/img/mainviewwidget/recent-gray.svg";
    QString netstr=":/data/img/mainviewwidget/net-gray.svg";
    QString socialstr=":/data/img/mainviewwidget/social-gray.svg";
    QString avstr=":/data/img/mainviewwidget/video-gray.svg";
    QString developstr=":/data/img/mainviewwidget/develop-gray.svg";
    QString imgstr=":/data/img/mainviewwidget/img-gray.svg";
    QString gamestr=":/data/img/mainviewwidget/game-gray.svg";
    QString officestr=":/data/img/mainviewwidget/office-gray.svg";
    QString readingstr=":/data/img/mainviewwidget/reading-gray.svg";
    QString systemstr=":/data/img/mainviewwidget/system-gray.svg";
    QString otherstr=":/data/img/mainviewwidget/other-gray.svg";
    iconlist.append(recentstr);
    iconlist.append(netstr);
    iconlist.append(socialstr);
    iconlist.append(avstr);
    iconlist.append(developstr);
    iconlist.append(imgstr);
    iconlist.append(gamestr);
    iconlist.append(officestr);
    iconlist.append(readingstr);
    iconlist.append(systemstr);
    iconlist.append(otherstr);

    iconlightlist.clear();
    QString recentlightstr=":/data/img/mainviewwidget/recent.svg";
    QString netlightstr=":/data/img/mainviewwidget/net.svg";
    QString sociallightstr=":/data/img/mainviewwidget/social.svg";
    QString avlightstr=":/data/img/mainviewwidget/video.svg";
    QString developlightstr=":/data/img/mainviewwidget/develop.svg";
    QString imglightstr=":/data/img/mainviewwidget/img.svg";
    QString gamelightstr=":/data/img/mainviewwidget/game.svg";
    QString officelightstr=":/data/img/mainviewwidget/office.svg";
    QString readinglightstr=":/data/img/mainviewwidget/reading.svg";
    QString systemlightstr=":/data/img/mainviewwidget/system.svg";
    QString otherlightstr=":/data/img/mainviewwidget/other.svg";
    iconlightlist.append(recentlightstr);
    iconlightlist.append(netlightstr);
    iconlightlist.append(sociallightstr);
    iconlightlist.append(avlightstr);
    iconlightlist.append(developlightstr);
    iconlightlist.append(imglightstr);
    iconlightlist.append(gamelightstr);
    iconlightlist.append(officelightstr);
    iconlightlist.append(readinglightstr);
    iconlightlist.append(systemlightstr);
    iconlightlist.append(otherlightstr);

    functionnamelist.clear();
    functionnamelist.append("最近");
    functionnamelist.append("网络");
    functionnamelist.append("社交");
    functionnamelist.append("影音");
    functionnamelist.append("开发");
    functionnamelist.append("图像");
    functionnamelist.append("游戏");
    functionnamelist.append("办公");
    functionnamelist.append("教育");
    functionnamelist.append("系统");
    functionnamelist.append("其它");

    init_applist_widget();
    init_iconlist_widget();
}

/**
 * 初始化应用列表界面
 */
void FullFunctionWidget::init_applist_widget()
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
    scrollareawidLayout->setSpacing(0);
    scrollareawid->setLayout(scrollareawidLayout);
    layout->addWidget(scrollarea);
    fill_app_list();
}

/**
 * 填充应用列表
 */
void FullFunctionWidget::fill_app_list()
{
    classificationbtnlist.clear();
    classificationbtnrowlist.clear();

    QVector<QStringList> vector=pUkuiMenuInterface->get_functional_classification();

    QStringList recentlist=vector.at(0);
    if(!recentlist.isEmpty())
    {
        insert_classification_btn("最近");
        insert_app_list(recentlist);
    }

    QStringList netlist=vector.at(1);
    if(!netlist.isEmpty())
    {
        insert_classification_btn("网络");
        insert_app_list(netlist);
    }
    QStringList sociallist=vector.at(2);
    if(!sociallist.isEmpty())
    {
        insert_classification_btn("社交");
        insert_app_list(sociallist);
    }
    QStringList avlist=vector.at(3);
    if(!avlist.isEmpty())
    {
        insert_classification_btn("影音");
        insert_app_list(avlist);
    }
    QStringList developlist=vector.at(4);
    if(!developlist.isEmpty())
    {
        insert_classification_btn("开发");
        insert_app_list(developlist);
    }
    QStringList graphicslist=vector.at(5);
    if(!graphicslist.isEmpty())
    {
        insert_classification_btn("图像");
        insert_app_list(graphicslist);
    }
    QStringList gamelist=vector.at(6);
    if(!gamelist.isEmpty())
    {
        insert_classification_btn("游戏");
        insert_app_list(gamelist);
    }
    QStringList officelist=vector.at(7);
    if(!officelist.isEmpty())
    {
        insert_classification_btn("办公");
        insert_app_list(officelist);
    }
    QStringList educationlist=vector.at(8);
    if(!educationlist.isEmpty())
    {
        insert_classification_btn("教育");

        insert_app_list(educationlist);
    }

    QStringList systemadminlist=vector.at(9);
    if(!systemadminlist.isEmpty())
    {
        insert_classification_btn("系统");
        insert_app_list(systemadminlist);
    }
    QStringList otherlist=vector.at(10);
    if(!otherlist.isEmpty())
    {
        insert_classification_btn("其它");
        insert_app_list(otherlist);
    }

    resize_scrollarea_controls();
}

void FullFunctionWidget::insert_classification_btn(QString btnname)
{
    PushButton* classificationbtn=new PushButton(btnname,scrollarea->width()-12,20);
    classificationbtn->setFixedSize(scrollarea->width()-12,20);
    scrollareawidLayout->addWidget(classificationbtn);
    classificationbtnlist.append(btnname);

}

void FullFunctionWidget::insert_app_list(QStringList appnamelist)
{
    FullListView* listview=new FullListView(this,2);
    scrollareawidLayout->addWidget(listview);
    data.clear();
    for(int i=0;i<appnamelist.count();i++)
    {

        QString desktopfp=pUkuiMenuInterface->get_desktop_path_by_app_name(appnamelist.at(i));
        data.append(desktopfp);
    }

    listview->addData(data);
    connect(listview,SIGNAL(sendItemClickedSignal(QString)),this,SLOT(exec_app_name(QString)));
    connect(listview,SIGNAL(sendFixedOrUnfixedSignal()),this,SIGNAL(send_update_applist_signal()));
}

/**
 * 执行应用程序
 */
void FullFunctionWidget::exec_app_name(QString appname)
{
    emit send_hide_mainwindow_signal();
    QString execpath=pUkuiMenuInterface->get_app_exec(pUkuiMenuInterface->get_desktop_path_by_app_name(appname));
    //    qDebug()<<execpath;
    //移除启动参数%u或者%U
    for(int i=0;i<execpath.length();i++)
    {
        if(execpath.at(i)=='%')
        {
            execpath.remove(i,2);
        }
    }
    QProcess *process=new QProcess(this);
    process->startDetached(execpath);
}

/**
 * 更新应用列表
 */
void FullFunctionWidget::update_app_listview()
{
    //刷新应用列表界面
    for(int index=scrollareawidLayout->count()-1;index>=0;index--)
    {
            QLayoutItem* item=scrollareawidLayout->takeAt(index);
            QWidget* wid=item->widget();
            scrollareawidLayout->removeWidget(wid);
            wid->setParent(nullptr);
            delete wid;
    }

    fill_app_list();

    //刷新图标列表界面
    foreach (QAbstractButton* button, buttonList){
        pBtnGroup->removeButton(button);
    }
    buttonList.clear();
    for(int index=iconlistscrollareawidLayout->count()-2;index>0;index--)
    {
        QLayoutItem* item=iconlistscrollareawidLayout->takeAt(index);
        QWidget* wid=item->widget();
        iconlistscrollareawidLayout->removeWidget(wid);
        wid->setParent(nullptr);
        delete wid;
    }
    for(int i=0;i<classificationbtnlist.size();i++)
    {
        FunctionClassifyButton* iconbtn=new FunctionClassifyButton(this,
                                                                   Style::LeftBtnWidth,
                                                                   Style::LeftBtnHeight,
                                                                   iconlist.at(functionnamelist.indexOf(classificationbtnlist.at(i))),
                                                                   iconlightlist.at(functionnamelist.indexOf(classificationbtnlist.at(i))),
                                                                   ClassifyBtnHoverBackground,
                                                                   ClassifyBtnHoverBackground,
                                                                   2,
                                                                   classificationbtnlist.at(i));
        buttonList.append(iconbtn);
        iconlistscrollareawidLayout->insertWidget(i+1,iconbtn);
        connect(iconbtn,SIGNAL(buttonClicked(QAbstractButton*)),pBtnGroup, SIGNAL(buttonClicked(QAbstractButton*)));
    }

    int id=0;
    foreach (QAbstractButton* btn, buttonList) {
        pBtnGroup->addButton(btn,id++);
    }
    iconlistscrollarea->widget()->adjustSize();
    pBtnGroup->button(0)->click();
}

/**
 * 设置scrollarea所填充控件大小
 */
void FullFunctionWidget::resize_scrollarea_controls()
{
    int pos=0;
    classificationbtnrowlist.append(QString::number(pos));
    int row=0;
    while(row<scrollareawidLayout->count()/2)
    {
        //分类按钮
//        QLayoutItem* classificationbtnwidItem=scrollareawidLayout->itemAt(row*2);
//        QWidget* classificationbtnwid=classificationbtnwidItem->widget();
//        QPushButton* classificationbtn=qobject_cast<QPushButton*>(classificationbtnwid);
//        classificationbtn->setFixedSize(scrollarea->width()-32,20);
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
            pos+=(20+listview->height());
            classificationbtnrowlist.append(QString::number(pos));
        }
        row++;
    }
    scrollarea->widget()->adjustSize();
}

/**
 * 初始化图标列表界面
 */
void FullFunctionWidget::init_iconlist_widget()
{
    iconlistLayout=new QHBoxLayout(iconlistWid);
    iconlistLayout->setContentsMargins(Style::LeftMargin,0,Style::RightMargin,0);
    iconlistLayout->setSpacing(0);
    iconlistWid->setLayout(iconlistLayout);

    iconlistscrollarea=new ClassifyScrollArea();
    iconlistscrollarea->setFixedSize(Style::LeftBtnWidth,iconlistWid->height());
    iconlistscrollareaWid=new QWidget;
    iconlistscrollareawidLayout=new QVBoxLayout;
    iconlistscrollareawidLayout->setContentsMargins(0,0,0,0);
    iconlistscrollareawidLayout->setSpacing(Style::LeftSpaceBetweenItem);
    iconlistscrollareaWid->setLayout(iconlistscrollareawidLayout);
    iconlistscrollarea->setWidget(iconlistscrollareaWid);
    iconlistLayout->addWidget(iconlistscrollarea);
    pBtnGroup=new QButtonGroup(iconlistscrollareaWid);
    init_iconlist_scrollarea();
}

/**
 * 初始化图标列表界面数据表格iconlisttableWid
 */
void FullFunctionWidget::init_iconlist_scrollarea()
{
    pIconListTopSpacer=new QSpacerItem(40,20,QSizePolicy::Fixed,QSizePolicy::Expanding);
    pIconListBottomSpacer=new QSpacerItem(40,20,QSizePolicy::Fixed,QSizePolicy::Expanding);

    iconlistscrollareawidLayout->addItem(pIconListTopSpacer);
    for(int i=0;i<classificationbtnlist.size();i++)
    {
        FunctionClassifyButton* iconbtn=new FunctionClassifyButton(this,
                                                                   Style::LeftBtnWidth,
                                                                   Style::LeftBtnHeight,
                                                                   iconlist.at(functionnamelist.indexOf(classificationbtnlist.at(i))),
                                                                   iconlightlist.at(functionnamelist.indexOf(classificationbtnlist.at(i))),
                                                                   ClassifyBtnHoverBackground,
                                                                   ClassifyBtnHoverBackground,
                                                                   2,
                                                                   classificationbtnlist.at(i));
        buttonList.append(iconbtn);
        iconlistscrollareawidLayout->addWidget(iconbtn);
        connect(iconbtn,SIGNAL(buttonClicked(QAbstractButton*)),pBtnGroup, SIGNAL(buttonClicked(QAbstractButton*)));
    }
    iconlistscrollareawidLayout->addItem(pIconListBottomSpacer);

    int id=0;
    foreach (QAbstractButton* btn, buttonList) {
        pBtnGroup->addButton(btn,id++);
    }
    connect(pBtnGroup,SIGNAL(buttonClicked(QAbstractButton*)),this,SLOT(btngroup_clicked_slot(QAbstractButton*)));
    iconlistscrollarea->widget()->adjustSize();
    pBtnGroup->button(0)->click();

}

void FullFunctionWidget::btngroup_clicked_slot(QAbstractButton *btn)
{
    foreach (QAbstractButton* button, buttonList) {

        FunctionClassifyButton* fcbutton=qobject_cast<FunctionClassifyButton*>(button);
        QLayoutItem* iconitem=fcbutton->layout()->itemAt(0);
        QLabel* iconlabel=qobject_cast<QLabel*>(iconitem->widget());
        QLayoutItem* textitem=fcbutton->layout()->itemAt(1);
        QLabel* textlabel=qobject_cast<QLabel*>(textitem->widget());

        if(pBtnGroup->id(btn)==buttonList.indexOf(button))
        {
            int num=classificationbtnlist.indexOf(textlabel->text());
            if(num!=-1)
            {
                int pos=classificationbtnrowlist.at(num).toInt();
                scrollarea->verticalScrollBar()->setSliderPosition(pos);
            }

            QSvgRenderer* svgRender = new QSvgRenderer;
            svgRender->load(iconlightlist.at(functionnamelist.indexOf(textlabel->text())));
            QPixmap* pixmap = new QPixmap(Style::LeftIconSize,Style::LeftIconSize);
            pixmap->fill(Qt::transparent);
            QPainter p(pixmap);
            svgRender->render(&p);
            iconlabel->setPixmap(*pixmap);
            textlabel->setStyleSheet("background:transparent;color:rgba(255, 255, 255);");
            fcbutton->is_pressed=true;

        }
        else{
            QSvgRenderer* svgRender = new QSvgRenderer;
            svgRender->load(iconlist.at(functionnamelist.indexOf(textlabel->text())));
            QPixmap* pixmap = new QPixmap(Style::LeftIconSize,Style::LeftIconSize);
            pixmap->fill(Qt::transparent);
            QPainter p(pixmap);
            svgRender->render(&p);
            iconlabel->setPixmap(*pixmap);
            textlabel->setStyleSheet("background:transparent;color:rgba(255, 255, 255,50%);");
            fcbutton->is_pressed=false;
        }
    }
}

void FullFunctionWidget::widget_make_zero()
{
    foreach (QAbstractButton* button, buttonList) {
        int num=classificationbtnlist.indexOf(functionnamelist.at(buttonList.indexOf(button)));
        if(num!=-1)
        {
            pBtnGroup->button(num)->click();
            break;
        }
    }
}
