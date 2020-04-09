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
}

void MainViewWidget::initWidget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setStyleSheet("border:0px;background:transparent;");
//    char style[100];
//    sprintf(style, "border:0px;background-color:%s;",DefaultBackground);
//    this->setStyleSheet(QString::fromLocal8Bit(style));

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

    //进程开启，刷新常用软件界面
    connect(this,SIGNAL(viewOpenedSignal()),fullcommonusewid,SLOT(updateListViewAllSlot()));
    connect(this,SIGNAL(viewOpenedSignal()),commonusewid,SLOT(updateListViewAllSlot()));

    //常用软件界面删除操作，刷新界面
    connect(commonusewid,SIGNAL(sendUpdateAppListSignal(QString,int)),fullcommonusewid,SLOT(updateListViewSlot(QString,int)));
    connect(fullcommonusewid,SIGNAL(sendUpdateAppListSignal(QString,int)),commonusewid,SLOT(updateListViewSlot(QString,int)));
    connect(commonusewid,SIGNAL(removeListItemSignal(QString)),fullcommonusewid,SLOT(removeListItemSlot(QString)));
    connect(fullcommonusewid,SIGNAL(removeListItemSignal(QString)),commonusewid,SLOT(removeListItemSlot(QString)));
    connect(commonusewid,SIGNAL(removeListAllItemSignal()),fullcommonusewid,SLOT(removeListAllItemSlot()));
    connect(fullcommonusewid,SIGNAL(removeListAllItemSignal()),commonusewid,SLOT(removeListAllItemSlot()));

    //字母排序、功能分类界面、搜索界面固定或取消固定到常用软件，刷新常用软件界面
    connect(letterwid,SIGNAL(sendUpdateAppListSignal(QString,int)),commonusewid,SLOT(updateListViewSlot(QString,int)));
    connect(fullletterwid,SIGNAL(sendUpdateAppListSignal(QString,int)),commonusewid,SLOT(updateListViewSlot(QString,int)));
    connect(functionwid,SIGNAL(sendUpdateAppListSignal(QString,int)),commonusewid,SLOT(updateListViewSlot(QString,int)));
    connect(fullfunctionwid,SIGNAL(sendUpdateAppListSignal(QString,int)),commonusewid,SLOT(updateListViewSlot(QString,int)));
    connect(letterwid,SIGNAL(sendUpdateAppListSignal(QString,int)),fullcommonusewid,SLOT(updateListViewSlot(QString,int)));
    connect(fullletterwid,SIGNAL(sendUpdateAppListSignal(QString,int)),fullcommonusewid,SLOT(updateListViewSlot(QString,int)));
    connect(functionwid,SIGNAL(sendUpdateAppListSignal(QString,int)),fullcommonusewid,SLOT(updateListViewSlot(QString,int)));
    connect(fullfunctionwid,SIGNAL(sendUpdateAppListSignal(QString,int)),fullcommonusewid,SLOT(updateListViewSlot(QString,int)));
    connect(searchresultwid,SIGNAL(sendUpdateAppListSignal(QString,int)),commonusewid,SLOT(updateListViewSlot(QString,int)));
    connect(fullsearchresultwid,SIGNAL(sendUpdateAppListSignal(QString,int)),fullcommonusewid,SLOT(updateListViewSlot(QString,int)));

    //监控.desktop文件目录
    fileWatcher=new QFileSystemWatcher(this);
    fileWatcher->addPath("/usr/share/applications");
    connect(fileWatcher,SIGNAL(directoryChanged(const QString &)),this,SLOT(directoryChangedSlot()));
    connect(this,SIGNAL(directoryChangedSignal()),letterwid,SLOT(updateAppListView()));
    connect(this,SIGNAL(directoryChangedSignal()),fullletterwid,SLOT(updateAppListView()));
    connect(this,SIGNAL(directoryChangedSignal()),functionwid,SLOT(updateAppListView()));
    connect(this,SIGNAL(directoryChangedSignal()),fullfunctionwid,SLOT(updateAppListView()));
    connect(this,SIGNAL(directoryChangedSignal()),commonusewid,SLOT(updateListViewAllSlot()));
    connect(this,SIGNAL(directoryChangedSignal()),fullcommonusewid,SLOT(updateListViewAllSlot()));

    //发送隐藏主界面信号
    connect(commonusewid,SIGNAL(sendHideMainWindowSignal()),this,SIGNAL(sendHideMainWindowSignal()));
    connect(fullcommonusewid,SIGNAL(sendHideMainWindowSignal()),this,SIGNAL(sendHideMainWindowSignal()));
    connect(letterwid,SIGNAL(sendHideMainWindowSignal()),this,SIGNAL(sendHideMainWindowSignal()));
    connect(fullletterwid,SIGNAL(sendHideMainWindowSignal()),this,SIGNAL(sendHideMainWindowSignal()));
    connect(functionwid,SIGNAL(sendHideMainWindowSignal()),this,SIGNAL(sendHideMainWindowSignal()));
    connect(fullfunctionwid,SIGNAL(sendHideMainWindowSignal()),this,SIGNAL(sendHideMainWindowSignal()));
    connect(searchresultwid,SIGNAL(sendHideMainWindowSignal()),this,SIGNAL(sendHideMainWindowSignal()));
    connect(fullsearchresultwid,SIGNAL(sendHideMainWindowSignal()),this,SIGNAL(sendHideMainWindowSignal()));

    addTopControl();
    loadMinMainView();
    loadLetterWidget();

    //监控应用进程开启
    QDBusConnection::sessionBus().connect("org.ayatana.bamf","/org/ayatana/bamf/matcher","org.ayatana.bamf.matcher",
                                         QString("ViewOpened"),this,SLOT(ViewOpenedSlot(QDBusMessage)));

    QString path=QDir::homePath()+"/.config/ukui/ukui-menu.ini";
    setting=new QSettings(path,QSettings::IniFormat);
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
    sprintf(style, "QLineEdit{border:0px;background-color:%s;border-radius:2px;}",QueryLineEditBackground);
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
    connect(this,SIGNAL(sendSearchKeyword(QString)),
            searchappthread,SLOT(recvSearchKeyword(QString)));
    connect(searchappthread,SIGNAL(sendSearchResult(QStringList)),
            this,SLOT(recvSearchResult(QStringList)));
    connect(querylineEdit, SIGNAL(textChanged(QString)), this, SLOT(searchAppSlot(QString)));
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
             sprintf(style, "QLineEdit{border:1px solid %s;background-color:%s;border-radius:2px;color:#ffffff;}",
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
                sprintf(style, "QLineEdit{border:0px;background-color:%s;border-radius:2px;}",QueryLineEditBackground);
                querylineEdit->setStyleSheet(style);
                querylineEdit->setTextMargins(0,1,0,1);
                pIconTextWidLayout->addWidget(pQueryText);
                pIconTextWid->setFixedSize(pQueryIcon->width()+pQueryText->width()+10,Style::QueryLineEditHeight);
                queryLayout->setAlignment(pIconTextWid,Qt::AlignCenter);
            }
            else {
                char style[100];
                sprintf(style, "QLineEdit{border:0px;background-color:%s;border-radius:2px;color:#ffffff;}",
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
            if(is_fullWid==false)
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
        if(is_fullWid)
        {
            if(saveCurrentWidState==1)
                loadFullCommonUseWidget();
            else if(saveCurrentWidState==2)
                loadFullLetterWidget();
            else if(saveCurrentWidState==3)
                loadFullFunctionWidget();
        }
        else {
            if(saveCurrentWidState==1)
                loadCommonUseWidget();
            else if(saveCurrentWidState==2)
                loadLetterWidget();
            else
                loadFunctionWidget();
        }
    }

    Q_EMIT sendSearchKeyword(arg);
    searchappthread->start();
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
//    this->setGeometry(60,QApplication::desktop()->availableGeometry().height()-532,330,532);
//    this->setGeometry(0,QApplication::desktop()->availableGeometry().height()-532,330,532);
    this->setFixedSize(320,590);
    topWidget->setFixedSize(320,54);
    topLayout->setContentsMargins(0,0,0,0);
    topLayout->setAlignment(querylineEdit,Qt::AlignCenter);
    querylineEdit->setFixedSize(288,30);

    is_fullWid=false;
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
    else
        loadFunctionWidget();
}

/**
 * 加载全屏主视图
 */
void MainViewWidget::loadMaxMainView()
{
    this->setFixedSize(Style::MainViewWidWidth,
                       Style::heightavailable);
    topWidget->setFixedSize(this->width(),Style::TopWidgetHeight);
    querylineEdit->setFixedSize(Style::QueryLineEditWidth,Style::QueryLineEditHeight);

    topLayout->setContentsMargins((topWidget->width()-Style::LeftWidWidth-querylineEdit->width())/2+Style::LeftWidWidth,
                                  0,
                                  (topWidget->width()-Style::LeftWidWidth-querylineEdit->width())/2,
                                  0);
    is_fullWid=true;
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
                    Q_EMIT viewOpenedSignal();
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
    QStringList desktopfpList=pUkuiMenuInterface->getDesktopFilePath();
    if(desktopfpList.size() > UkuiMenuInterface::desktopfpVector.size())//有新的应用安装
    {
        setting->beginGroup("recentapp");
        for(int i=0;i<desktopfpList.count();i++)
        {
            if(!UkuiMenuInterface::desktopfpVector.contains(desktopfpList.at(i)))
            {
                //获取当前时间戳
                QDateTime dt=QDateTime::currentDateTime();
                int datetime=dt.toTime_t();
//                QString appname=pUkuiMenuInterface->getAppName(desktopfpList.at(i));
                QFileInfo fileInfo(desktopfpList.at(i));
                QString desktopfn=fileInfo.fileName();
                setting->setValue(desktopfn,datetime);
                qDebug()<<"安装:"<<desktopfn;
            }

        }
        setting->endGroup();
        UkuiMenuInterface::appInfoVector.clear();
        UkuiMenuInterface::appInfoVector=pUkuiMenuInterface->create_appinfo_vector();
        Q_EMIT directoryChangedSignal();
    }
    else//软件卸载
    {
        for(int i=0;i<UkuiMenuInterface::desktopfpVector.size();i++)
        {
            if(!desktopfpList.contains(UkuiMenuInterface::desktopfpVector.at(i)))
            {
//                QString appname=pUkuiMenuInterface->getAppName(UkuiMenuInterface::desktopfpVector.at(i));
//                QString appname=UkuiMenuInterface::appInfoVector.at(i).at(2);
                QString desktopfp=UkuiMenuInterface::appInfoVector.at(i).at(0);
                QFileInfo fileInfo(desktopfp);
                QString desktopfn=fileInfo.fileName();
                setting->beginGroup("lockapplication");
                setting->remove(desktopfn);
                setting->sync();
                setting->endGroup();
                setting->beginGroup("application");
                setting->remove(desktopfn);
                setting->sync();
                setting->endGroup();
                setting->beginGroup("datetime");
                setting->remove(desktopfn);
                setting->sync();
                setting->endGroup();
                setting->beginGroup("recentapp");
                setting->remove(desktopfn);
                setting->sync();
                setting->endGroup();
                qDebug()<<"卸载:"<<desktopfn;
            }
        }
        UkuiMenuInterface::appInfoVector=pUkuiMenuInterface->create_appinfo_vector();
        Q_EMIT directoryChangedSignal();
    }
}

void MainViewWidget::widgetMakeZero()
{
    commonusewid->widgetMakeZero();
    fullcommonusewid->widgetMakeZero();
    letterwid->widgetMakeZero();
    fullletterwid->widgetMakeZero();
    functionwid->widgetMakeZero();
    fullfunctionwid->widgetMakeZero();
    querylineEdit->clear();
    querylineEdit->clearFocus();
}
