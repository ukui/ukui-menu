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

#include "mainviewwidget.h"
#include "ui_mainviewwidget.h"
#include "src/Style/style.h"
#include <QSvgRenderer>
#include <QPainter>
#include <libbamf/bamf-matcher.h>
#include <syslog.h>
#include <QDebug>

MainViewWidget::MainViewWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainViewWidget)
{
    ui->setupUi(this);
    initWidget();
}

MainViewWidget::~MainViewWidget()
{
    delete ui;
    delete commonusewid;
    delete fullcommonusewid;
    delete letterwid;
    delete fullletterwid;
    delete functionwid;
    delete fullfunctionwid;
    delete searchresultwid;
    delete fullsearchresultwid;
    delete searchappthread;
    delete pUkuiMenuInterface;
    delete directoryChangedThread;
}

void MainViewWidget::initWidget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setStyleSheet("border:0px;background:transparent;");

    mainLayout=new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    topWidget=new QWidget(this);
    topWidget->setStyleSheet("border:0px;background:transparent;");

    verticalSpacer=new QSpacerItem(20,40, QSizePolicy::Fixed, QSizePolicy::Expanding);
    mainLayout->addWidget(topWidget);
    mainLayout->addItem(verticalSpacer);
    this->setLayout(mainLayout);

    this->setFocusPolicy(Qt::NoFocus);

    commonusewid=new CommonUseWidget;
    fullcommonusewid=new FullCommonUseWidget;
    letterwid=new LetterWidget;
    functionwid=new FunctionWidget;
    fullletterwid=new FullLetterWidget;
    fullfunctionwid=new FullFunctionWidget;

    fullsearchresultwid=new FullSearchResultWidget;
    searchresultwid=new SearchResultWidget;

    pUkuiMenuInterface=new UkuiMenuInterface;

    //监控.desktop文件目录
    fileWatcher=new QFileSystemWatcher(this);
    fileWatcher->addPath("/usr/share/applications");
    connect(fileWatcher,&QFileSystemWatcher::directoryChanged,this,&MainViewWidget::directoryChangedSlot);
    directoryChangedThread=new DirectoryChangedThread;
    connect(this,&MainViewWidget::sendDirectoryPath,directoryChangedThread,&DirectoryChangedThread::recvDirectoryPath);
    connect(directoryChangedThread,&DirectoryChangedThread::requestUpdateSignal,this,&MainViewWidget::requestUpdateSlot);
    connect(this,&MainViewWidget::directoryChangedSignal,letterwid,&LetterWidget::updateAppListView);
    connect(this,&MainViewWidget::directoryChangedSignal,fullletterwid,&FullLetterWidget::updateAppListView);
    connect(this,&MainViewWidget::directoryChangedSignal,functionwid,&FunctionWidget::updateAppListView);
    connect(this,&MainViewWidget::directoryChangedSignal,fullfunctionwid,&FullFunctionWidget::updateAppListView);
    connect(this,&MainViewWidget::directoryChangedSignal,commonusewid,&CommonUseWidget::updateListViewSlot);
    connect(this,&MainViewWidget::directoryChangedSignal,fullcommonusewid,&FullCommonUseWidget::updateListViewSlot);

    //发送隐藏主界面信号
    connect(commonusewid,&CommonUseWidget::sendHideMainWindowSignal,this,&MainViewWidget::sendHideMainWindowSignal);
    connect(fullcommonusewid,&FullCommonUseWidget::sendHideMainWindowSignal,this,&MainViewWidget::sendHideMainWindowSignal);
    connect(letterwid,&LetterWidget::sendHideMainWindowSignal,this,&MainViewWidget::sendHideMainWindowSignal);
    connect(fullletterwid,&FullLetterWidget::sendHideMainWindowSignal,this,&MainViewWidget::sendHideMainWindowSignal);
    connect(functionwid,&FunctionWidget::sendHideMainWindowSignal,this,&MainViewWidget::sendHideMainWindowSignal);
    connect(fullfunctionwid,&FullFunctionWidget::sendHideMainWindowSignal,this,&MainViewWidget::sendHideMainWindowSignal);
    connect(searchresultwid,&SearchResultWidget::sendHideMainWindowSignal,this,&MainViewWidget::sendHideMainWindowSignal);
    connect(fullsearchresultwid,&FullSearchResultWidget::sendHideMainWindowSignal,this,&MainViewWidget::sendHideMainWindowSignal);

    addTopControl();
    loadMinMainView();

    //监控应用进程开启
    bamf_matcher_get_default();
    QDBusConnection::sessionBus().connect("org.ayatana.bamf","/org/ayatana/bamf/matcher","org.ayatana.bamf.matcher",
                                         QString("ViewOpened"),this,SLOT(ViewOpenedSlot(QDBusMessage)));

    QString path=QDir::homePath()+"/.config/ukui/ukui-menu.ini";
    setting=new QSettings(path,QSettings::IniFormat);

    if(QGSettings::isSchemaInstalled(QString("org.ukui.style").toLocal8Bit()))
    {
        gsetting=new QGSettings(QString("org.ukui.style").toLocal8Bit());
        connect(gsetting,&QGSettings::changed,this,&MainViewWidget::iconThemeChangeSlot);
    }
}

/**
 * 添加顶部控件
 */
void MainViewWidget::addTopControl()
{
    topLayout=new QHBoxLayout(topWidget);
    topLayout->setSpacing(0);
    querylineEdit=new QLineEdit(topWidget);
    queryLayout=new QHBoxLayout;
    queryLayout->setContentsMargins(0,0,0,0);
    queryLayout->setSpacing(0);
    querylineEdit->setLayout(queryLayout);
    char style[100];
    sprintf(style, "QLineEdit{border:0px;background-color:%s;border-radius:4px;}",QueryLineEditBackground);
    querylineEdit->setStyleSheet(style);
    topLayout->addWidget(querylineEdit);
//    topLayout->setAlignment(querylineEdit,Qt::AlignCenter);
    topWidget->setLayout(topLayout);

    initQueryLineEdit();

}

/**
 * 添加搜索框
 */
void MainViewWidget::initQueryLineEdit()
{
    pIconTextWid=new QWidget(querylineEdit);
    pIconTextWid->setFocusPolicy(Qt::NoFocus);
    pIconTextWid->setStyleSheet("background:transparent");
    pIconTextWidLayout=new QHBoxLayout;
    pIconTextWidLayout->setContentsMargins(5,0,0,0);
    pIconTextWidLayout->setSpacing(5);
    pIconTextWid->setLayout(pIconTextWidLayout);
    QSvgRenderer* svgRender = new QSvgRenderer();
    svgRender->load(QString(":/data/img/mainviewwidget/search.svg"));
    QPixmap* pixmap = new QPixmap(Style::QueryLineEditIconSize,Style::QueryLineEditIconSize);
    pixmap->fill(Qt::transparent);//设置背景透明
    QPainter p(pixmap);
    svgRender->render(&p);
    pQueryIcon=new QLabel(pIconTextWid);
    pQueryIcon->setStyleSheet("background:transparent");
    pQueryIcon->setFixedSize(pixmap->size());
    pQueryIcon->setPixmap(*pixmap);
//    QFont font;
//    font.setPixelSize(Style::QueryLineEditFontSize);
    pQueryText=new QLabel(pIconTextWid);
//    pQueryText->setFont(font);
    pQueryText->setText(tr("Search"));
    pQueryText->setStyleSheet("background:transparent;color:#626c6e;");
    pQueryText->adjustSize();
    pIconTextWidLayout->addWidget(pQueryIcon);
    pIconTextWidLayout->addWidget(pQueryText);
//    pIconTextWid->setFixedSize(pQueryIcon->width()+pQueryText->width()+5,Style::QueryLineEditHeight);
    queryLayout->addWidget(pIconTextWid);
    queryLayout->setAlignment(pIconTextWid,Qt::AlignCenter);
    querylineEdit->setFocusPolicy(Qt::ClickFocus);
    querylineEdit->installEventFilter(this);
    querylineEdit->setContextMenuPolicy(Qt::NoContextMenu);

    searchappthread=new SearchAppThread;
    connect(this,&MainViewWidget::sendSearchKeyword,
            searchappthread,&SearchAppThread::recvSearchKeyword);
    connect(searchappthread,&SearchAppThread::sendSearchResult,
            this,&MainViewWidget::recvSearchResult);
    connect(querylineEdit, &QLineEdit::textChanged, this, &MainViewWidget::searchAppSlot);
}

bool MainViewWidget::eventFilter(QObject *watched, QEvent *event)
{
//    QFont font;
//    font.setPixelSize(Style::QueryLineEditFontSize);
    if(watched==querylineEdit)
    {
        if(event->type()==QEvent::FocusIn)
        {   
            pIconTextWidLayout->removeWidget(pQueryText);
            pQueryText->setParent(nullptr);
            pIconTextWid->setFixedSize(pQueryIcon->width()+5,Style::QueryLineEditHeight);
            queryLayout->setAlignment(pIconTextWid,Qt::AlignLeft);

             char style[200];
             sprintf(style, "QLineEdit{border:1px solid %s;background-color:%s;border-radius:4px;color:#ffffff;}",
                     QueryLineEditClickedBorder,QueryLineEditClickedBackground);
             querylineEdit->setStyleSheet(style);
             querylineEdit->setTextMargins(20,1,0,1);
//             querylineEdit->setFont(font);
             if(!querylineEdit->text().isEmpty())
                 searchAppSlot(querylineEdit->text());
        }
        else if(event->type()==QEvent::FocusOut)
        {
            if(querylineEdit->text().isEmpty())
            {
                char style[100];
                sprintf(style, "QLineEdit{border:0px;background-color:%s;border-radius:4px;}",QueryLineEditBackground);
                querylineEdit->setStyleSheet(style);
                querylineEdit->setTextMargins(0,1,0,1);
                pIconTextWidLayout->addWidget(pQueryText);
                pIconTextWid->setFixedSize(pQueryIcon->width()+pQueryText->width()+10,Style::QueryLineEditHeight);
                queryLayout->setAlignment(pIconTextWid,Qt::AlignCenter);
            }
            else {
                char style[100];
                sprintf(style, "QLineEdit{border:0px;background-color:%s;border-radius:4px;color:#ffffff;}",
                        QueryLineEditBackground);
                querylineEdit->setStyleSheet(style);
            }
        }
    }

     return QWidget::eventFilter(watched,event);     // 最后将事件交给上层对话框
}

void MainViewWidget::setLineEditFocus(QString arg)
{
    if(!querylineEdit->hasFocus())
    {
        querylineEdit->setFocus();
        querylineEdit->setText(arg);
    }
}

/**
 * 搜索程序和文件槽函数
 */
void MainViewWidget::searchAppSlot(QString arg)
{
    if(!is_hiden)
    {
        if(!arg.isEmpty())
        {
            if(widgetState!=0)
            {
                QLayoutItem* child;
                if((child=mainLayout->takeAt(1))!=nullptr)
                {
                    QWidget* childWid=child->widget();
                    if(childWid!=nullptr)
                    {
                        mainLayout->removeWidget(childWid);
                        childWid->setParent(nullptr);
                    }
                }
                widgetState=0;
                if(is_fullscreen==false)
                {
                    mainLayout->addWidget(searchresultwid);
                }
                else{
                    mainLayout->addWidget(fullsearchresultwid);
                }
            }
        }
        else{
            QLayoutItem* child;
            if((child=mainLayout->takeAt(1))!=nullptr)
            {
                QWidget* childWid=child->widget();
                if(childWid!=nullptr)
                {
                    mainLayout->removeWidget(childWid);
                    childWid->setParent(nullptr);
                }
            }
            if(is_fullscreen)
            {
                switch (saveCurrentWidState) {
                case 1:
                    loadFullCommonUseWidget();
                    break;
                case 2:
                    loadFullLetterWidget();
                    break;
                case 3:
                    loadFullFunctionWidget();
                default:
                    break;
                }
            }
            else {
                switch (saveCurrentWidState) {
                case 1:
                    loadCommonUseWidget();
                    break;
                case 2:
                    loadLetterWidget();
                    break;
                case 3:
                    loadFunctionWidget();
                    break;
                default:
                    break;
                }
            }
        }

        Q_EMIT sendSearchKeyword(arg);
        searchappthread->start();
    }
}

void MainViewWidget::recvSearchResult(QStringList desktopfplist)
{
    searchappthread->quit();
    fullsearchresultwid->updateAppListView(desktopfplist);
    searchresultwid->updateAppListView(desktopfplist);
}

/**
 * 加载默认主视图
 */
void MainViewWidget::loadMinMainView()
{
    is_hiden=false;
    this->setFixedSize(320,590);
    topWidget->setFixedSize(320,54);
    topLayout->setContentsMargins(0,0,0,0);
    topLayout->setAlignment(querylineEdit,Qt::AlignCenter);
    querylineEdit->setFixedSize(288,30);

    if(widgetState==0)
    {
        QLayoutItem* child;
        if((child=mainLayout->takeAt(1))!=nullptr)
        {
            QWidget* childWid=child->widget();
            if(childWid!=nullptr)
            {
                mainLayout->removeWidget(childWid);
                childWid->setParent(nullptr);
            }
        }
        mainLayout->addWidget(searchresultwid);
    }
    else if(widgetState==1)
        loadCommonUseWidget();
    else if(widgetState==2)
        loadLetterWidget();
    else if(widgetState==3)
        loadFunctionWidget();
    is_fullscreen=false;
}

/**
 * 加载全屏主视图
 */
void MainViewWidget::loadMaxMainView()
{
    is_hiden=false;
    this->setFixedSize(Style::MainViewWidWidth,
                       Style::heightavailable);
    topWidget->setFixedSize(this->width(),Style::TopWidgetHeight);
    querylineEdit->setFixedSize(Style::QueryLineEditWidth,Style::QueryLineEditHeight);

    topLayout->setContentsMargins((topWidget->width()-Style::LeftWidWidth-querylineEdit->width())/2+Style::LeftWidWidth,
                                  0,
                                  (topWidget->width()-Style::LeftWidWidth-querylineEdit->width())/2,
                                  0);
    if(widgetState==0)
    {
        QLayoutItem* child;
        if((child=mainLayout->takeAt(1))!=nullptr)
        {
            QWidget* childWid=child->widget();
            if(childWid!=nullptr)
            {
                mainLayout->removeWidget(childWid);
                childWid->setParent(nullptr);
            }
        }
        mainLayout->addWidget(fullsearchresultwid);
    }
    else if(widgetState==1)
        loadFullCommonUseWidget();
    else if(widgetState==2)
        loadFullLetterWidget();
    else if(widgetState==3)
        loadFullFunctionWidget();
    is_fullscreen=true;
}

/**
 * 加载常用分类界面
 */
void MainViewWidget::loadCommonUseWidget()
{
    fullcommonusewid->widgetMakeZero();
    letterwid->widgetMakeZero();
    fullletterwid->widgetMakeZero();
    functionwid->widgetMakeZero();
    fullfunctionwid->widgetMakeZero();
    QLayoutItem *child;
    if((child = mainLayout->takeAt(1)) != nullptr) {
        QWidget* childwid=child->widget();
        if(childwid!=nullptr)
        {
            mainLayout->removeWidget(childwid);
            childwid->setParent(nullptr);
        }

    }
    mainLayout->addWidget(commonusewid);
    commonusewid->updateListView();
    widgetState=1;
    saveCurrentWidState=1;
}

/**
 * 加载字母分类界面
 */
void MainViewWidget::loadLetterWidget()
{
    commonusewid->widgetMakeZero();
    fullcommonusewid->widgetMakeZero();
    fullletterwid->widgetMakeZero();
    functionwid->widgetMakeZero();
    fullfunctionwid->widgetMakeZero();
    QLayoutItem *child;
    if((child = mainLayout->takeAt(1)) != nullptr) {
        QWidget* childwid=child->widget();
        if(childwid!=nullptr)
        {
            mainLayout->removeWidget(childwid);
            childwid->setParent(nullptr);
        }

    }
    mainLayout->addWidget(letterwid);
    widgetState=2;
    saveCurrentWidState=2;
}

/**
 * 加载功能分类界面
 */
void MainViewWidget::loadFunctionWidget()
{
    commonusewid->widgetMakeZero();
    fullcommonusewid->widgetMakeZero();
    letterwid->widgetMakeZero();
    fullletterwid->widgetMakeZero();
    fullfunctionwid->widgetMakeZero();
    QLayoutItem *child;
    if((child = mainLayout->takeAt(1)) != nullptr) {
        QWidget* childwid=child->widget();
        if(childwid!=nullptr)
        {
            mainLayout->removeWidget(childwid);
            childwid->setParent(nullptr);
        }

    }
    mainLayout->addWidget(functionwid);
    widgetState=3;
    saveCurrentWidState=3;
}


/**
 * 加载全屏常用分类界面
 */
void MainViewWidget::loadFullCommonUseWidget()
{
    commonusewid->widgetMakeZero();
    letterwid->widgetMakeZero();
    fullletterwid->widgetMakeZero();
    functionwid->widgetMakeZero();
    fullfunctionwid->widgetMakeZero();
    QLayoutItem *child;
    if((child = mainLayout->takeAt(1)) != nullptr) {
        QWidget* childwid=child->widget();
        if(childwid!=nullptr)
        {
            mainLayout->removeWidget(childwid);
            childwid->setParent(nullptr);
        }

    }
    mainLayout->addWidget(fullcommonusewid);
    fullcommonusewid->updateListView();
    widgetState=1;
    saveCurrentWidState=1;
}

/**
 * 加载全屏字母分类界面
 */
void MainViewWidget::loadFullLetterWidget()
{
    commonusewid->widgetMakeZero();
    fullcommonusewid->widgetMakeZero();
    letterwid->widgetMakeZero();
    functionwid->widgetMakeZero();
    fullfunctionwid->widgetMakeZero();
    QLayoutItem *child;
    if((child = mainLayout->takeAt(1)) != nullptr) {
        QWidget* childwid=child->widget();
        if(childwid!=nullptr)
        {
            mainLayout->removeWidget(childwid);
            childwid->setParent(nullptr);
        }
    }
    mainLayout->addWidget(fullletterwid);
    if(!is_fullscreen || (is_fullscreen && saveCurrentWidState!=2))
        fullletterwid->enterAnimation();
    widgetState=2;
    saveCurrentWidState=2;
}

/**
 * 加载全屏功能分类界面
 */
void MainViewWidget::loadFullFunctionWidget()
{
    commonusewid->widgetMakeZero();
    fullcommonusewid->widgetMakeZero();
    letterwid->widgetMakeZero();
    fullletterwid->widgetMakeZero();
    functionwid->widgetMakeZero();
    QLayoutItem *child;
    if((child = mainLayout->takeAt(1)) != nullptr) {
        QWidget* childwid=child->widget();
        if(childwid!=nullptr)
        {
            mainLayout->removeWidget(childwid);
            childwid->setParent(nullptr);
        }

    }
    mainLayout->addWidget(fullfunctionwid);
    if(!is_fullscreen || (is_fullscreen && saveCurrentWidState!=3))
        fullfunctionwid->enterAnimation();
    widgetState=3;
    saveCurrentWidState=3;
}

/**
 * 进程开启监控槽函数
 */
void MainViewWidget::ViewOpenedSlot(QDBusMessage msg)
{
    QString path=msg.arguments().at(0).value<QString>();
    QString type=msg.arguments().at(1).value<QString>();
    if(QString::compare(type,"application")==0)
    {
        QDBusInterface ifaceapp("org.ayatana.bamf",path,
                                "org.ayatana.bamf.application",QDBusConnection::sessionBus());
        QDBusReply<QString> replyapp =ifaceapp.call("DesktopFile");
        QString desktopfp=replyapp.value();
        QStringList desktopfpList=pUkuiMenuInterface->getDesktopFilePath();
        if(desktopfpList.contains(desktopfp))
        {
            QFileInfo fileInfo(desktopfp);
            QString desktopfn=fileInfo.fileName();

            QString dateTimeKey;
            dateTimeKey.clear();
            if(!desktopfn.isEmpty())
            {
                setting->beginGroup("lockapplication");
                bool ret=setting->contains(desktopfn);
                setting->endGroup();
                if(!ret)
                {
                    setting->beginGroup("application");
                    setting->setValue(desktopfn,setting->value(desktopfn).toInt()+1);
                    dateTimeKey=desktopfn;
                    setting->sync();
                    setting->endGroup();
                }
            }

            if(!dateTimeKey.isEmpty())
            {
                QDateTime dt=QDateTime::currentDateTime();
                int datetime=dt.toTime_t();
                setting->beginGroup("datetime");
                setting->setValue(dateTimeKey,datetime);
                setting->sync();
                setting->endGroup();
            }
        }
    }
}

/**
 * desktop文件目录改变信号槽
 */
void MainViewWidget::directoryChangedSlot()
{
    Q_EMIT sendDirectoryPath(QString("/usr/share/applications"));
    directoryChangedThread->start();

//    QStringList desktopfpList=pUkuiMenuInterface->getDesktopFilePath();
//    if(desktopfpList.size() > UkuiMenuInterface::desktopfpVector.size())//有新的应用安装
//    {
//        setting->beginGroup("recentapp");
//        for(int i=0;i<desktopfpList.count();i++)
//        {
//            if(!UkuiMenuInterface::desktopfpVector.contains(desktopfpList.at(i)))
//            {
//                //获取当前时间戳
//                QDateTime dt=QDateTime::currentDateTime();
//                int datetime=dt.toTime_t();
////                QString appname=pUkuiMenuInterface->getAppName(desktopfpList.at(i));
//                QFileInfo fileInfo(desktopfpList.at(i));
//                QString desktopfn=fileInfo.fileName();
//                setting->setValue(desktopfn,datetime);
//                qDebug()<<"安装:"<<desktopfn;
//                break;
//            }

//        }
//        setting->endGroup();
//        UkuiMenuInterface::appInfoVector.clear();
//        UkuiMenuInterface::alphabeticVector.clear();
//        UkuiMenuInterface::functionalVector.clear();
//        UkuiMenuInterface::commonUseVector.clear();
//        UkuiMenuInterface::appInfoVector=pUkuiMenuInterface->createAppInfoVector();
//        UkuiMenuInterface::alphabeticVector=pUkuiMenuInterface->getAlphabeticClassification();
//        UkuiMenuInterface::functionalVector=pUkuiMenuInterface->getFunctionalClassification();
//        UkuiMenuInterface::commonUseVector=pUkuiMenuInterface->getCommonUseApp();
//        Q_EMIT directoryChangedSignal();
//    }
//    else//软件卸载
//    {
//        for(int i=0;i<UkuiMenuInterface::desktopfpVector.size();i++)
//        {
//            if(!desktopfpList.contains(UkuiMenuInterface::desktopfpVector.at(i)))
//            {
//                QString desktopfp=UkuiMenuInterface::appInfoVector.at(i).at(0);
//                QFileInfo fileInfo(desktopfp);
//                QString desktopfn=fileInfo.fileName();
//                setting->beginGroup("lockapplication");
//                setting->remove(desktopfn);
//                setting->sync();
//                setting->endGroup();
//                setting->beginGroup("application");
//                setting->remove(desktopfn);
//                setting->sync();
//                setting->endGroup();
//                setting->beginGroup("datetime");
//                setting->remove(desktopfn);
//                setting->sync();
//                setting->endGroup();
//                setting->beginGroup("recentapp");
//                setting->remove(desktopfn);
//                setting->sync();
//                setting->endGroup();
//                qDebug()<<"卸载:"<<desktopfn;
//                break;
//            }
//        }
//        UkuiMenuInterface::appInfoVector.clear();
//        UkuiMenuInterface::alphabeticVector.clear();
//        UkuiMenuInterface::functionalVector.clear();
//        UkuiMenuInterface::commonUseVector.clear();
//        UkuiMenuInterface::appInfoVector=pUkuiMenuInterface->createAppInfoVector();
//        UkuiMenuInterface::alphabeticVector=pUkuiMenuInterface->getAlphabeticClassification();
//        UkuiMenuInterface::functionalVector=pUkuiMenuInterface->getFunctionalClassification();
//        UkuiMenuInterface::commonUseVector=pUkuiMenuInterface->getCommonUseApp();
//        Q_EMIT directoryChangedSignal();
//    }
}

void MainViewWidget::requestUpdateSlot()
{
    directoryChangedThread->quit();
    Q_EMIT directoryChangedSignal();
}

void MainViewWidget::iconThemeChangeSlot(QString key)
{
    if(key=="iconThemeName" || key=="icon-theme-name")
        Q_EMIT directoryChangedSignal();
}

void MainViewWidget::repaintWidget()
{
    fullcommonusewid->repaintWidget();
    fullletterwid->repaintWidget();
    fullfunctionwid->repaintWidget();
    fullsearchresultwid->repaintWidget();
}

void MainViewWidget::widgetMakeZero()
{
    is_hiden=true;
    commonusewid->widgetMakeZero();
    fullcommonusewid->widgetMakeZero();
    letterwid->widgetMakeZero();
    fullletterwid->widgetMakeZero();
    functionwid->widgetMakeZero();
    fullfunctionwid->widgetMakeZero();
    querylineEdit->clear();
    querylineEdit->clearFocus();
    char style[100];
    sprintf(style, "QLineEdit{border:0px;background-color:%s;border-radius:2px;}",QueryLineEditBackground);
    querylineEdit->setStyleSheet(style);
    querylineEdit->setTextMargins(0,1,0,1);
    pIconTextWidLayout->addWidget(pQueryText);
    pIconTextWid->setFixedSize(pQueryIcon->width()+pQueryText->width()+10,Style::QueryLineEditHeight);
    queryLayout->setAlignment(pIconTextWid,Qt::AlignCenter);
    is_fullscreen=false;
    widgetState=1;
}

void MainViewWidget::moveScrollBar(int type)
{
    if(widgetState==0)
    {
        if(is_fullscreen)
            fullsearchresultwid->moveScrollBar(type);
        else
            searchresultwid->moveScrollBar(type);
    }
    if(widgetState==1)
    {
        if(is_fullscreen)
            fullcommonusewid->moveScrollBar(type);
        else
            commonusewid->moveScrollBar(type);
    }
    if(widgetState==2)
    {
        if(is_fullscreen)
            fullletterwid->moveScrollBar(type);
        else
            letterwid->moveScrollBar(type);
    }
    if(widgetState==3)
    {
        if(is_fullscreen)
            fullfunctionwid->moveScrollBar(type);
        else
            functionwid->moveScrollBar(type);
    }
}

//void MainViewWidget::mousePressEvent(QMouseEvent *event)
//{
//    if(is_fullscreen && event->button()==Qt::LeftButton)
//        Q_EMIT sendHideMainWindowSignal();
//}
