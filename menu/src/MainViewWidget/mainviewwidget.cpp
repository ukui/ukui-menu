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
    init_widget();
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

void MainViewWidget::init_widget()
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

    this->setFocusPolicy(Qt::StrongFocus);

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
    connect(this,SIGNAL(view_opened_signal()),fullcommonusewid,SLOT(update_listview_slot()));
    connect(this,SIGNAL(view_opened_signal()),commonusewid,SLOT(update_listview_slot()));

    //常用软件界面删除操作，刷新界面
    connect(commonusewid,SIGNAL(send_update_applist_signal()),fullcommonusewid,SLOT(update_listview_slot()));
    connect(fullcommonusewid,SIGNAL(send_update_applist_signal()),commonusewid,SLOT(update_listview_slot()));

    //字母排序、功能分类界面、搜索界面固定或取消固定到常用软件，刷新常用软件界面
    connect(letterwid,SIGNAL(send_update_applist_signal()),commonusewid,SLOT(update_listview_slot()));
    connect(fullletterwid,SIGNAL(send_update_applist_signal()),commonusewid,SLOT(update_listview_slot()));
    connect(functionwid,SIGNAL(send_update_applist_signal()),commonusewid,SLOT(update_listview_slot()));
    connect(fullfunctionwid,SIGNAL(send_update_applist_signal()),commonusewid,SLOT(update_listview_slot()));
    connect(letterwid,SIGNAL(send_update_applist_signal()),fullcommonusewid,SLOT(update_listview_slot()));
    connect(fullletterwid,SIGNAL(send_update_applist_signal()),fullcommonusewid,SLOT(update_listview_slot()));
    connect(functionwid,SIGNAL(send_update_applist_signal()),fullcommonusewid,SLOT(update_listview_slot()));
    connect(fullfunctionwid,SIGNAL(send_update_applist_signal()),fullcommonusewid,SLOT(update_listview_slot()));
    connect(searchresultwid,SIGNAL(send_update_applist_signal()),commonusewid,SLOT(update_listview_slot()));
    connect(fullsearchresultwid,SIGNAL(send_update_applist_signal()),fullcommonusewid,SLOT(update_listview_slot()));

    //监控.desktop文件目录
    fileWatcher=new QFileSystemWatcher(this);
    fileWatcher->addPath("/usr/share/applications");
    connect(fileWatcher,SIGNAL(directoryChanged(const QString &)),this,SLOT(directoryChangedSlot()));
    connect(this,SIGNAL(directoryChangedSignal()),letterwid,SLOT(update_app_listview()));
    connect(this,SIGNAL(directoryChangedSignal()),fullletterwid,SLOT(update_app_listview()));
    connect(this,SIGNAL(directoryChangedSignal()),functionwid,SLOT(update_app_listview()));
    connect(this,SIGNAL(directoryChangedSignal()),fullfunctionwid,SLOT(update_app_listview()));
    connect(this,SIGNAL(directoryChangedSignal()),commonusewid,SLOT(update_listview_slot()));
    connect(this,SIGNAL(directoryChangedSignal()),fullcommonusewid,SLOT(update_listview_slot()));

    //发送隐藏主界面信号
    connect(commonusewid,SIGNAL(send_hide_mainwindow_signal()),this,SIGNAL(send_hide_mainwindow_signal()));
    connect(fullcommonusewid,SIGNAL(send_hide_mainwindow_signal()),this,SIGNAL(send_hide_mainwindow_signal()));
    connect(letterwid,SIGNAL(send_hide_mainwindow_signal()),this,SIGNAL(send_hide_mainwindow_signal()));
    connect(fullletterwid,SIGNAL(send_hide_mainwindow_signal()),this,SIGNAL(send_hide_mainwindow_signal()));
    connect(functionwid,SIGNAL(send_hide_mainwindow_signal()),this,SIGNAL(send_hide_mainwindow_signal()));
    connect(fullfunctionwid,SIGNAL(send_hide_mainwindow_signal()),this,SIGNAL(send_hide_mainwindow_signal()));
    connect(searchresultwid,SIGNAL(send_hide_mainwindow_signal()),this,SIGNAL(send_hide_mainwindow_signal()));
    connect(fullsearchresultwid,SIGNAL(send_hide_mainwindow_signal()),this,SIGNAL(send_hide_mainwindow_signal()));

    add_top_control();
    load_min_mainview();
    load_letter_widget();

    //监控应用进程开启
    QDBusConnection::sessionBus().connect("org.ayatana.bamf","/org/ayatana/bamf/matcher","org.ayatana.bamf.matcher",
                                         QString("ViewOpened"),this,SLOT(ViewOpenedSlot(QDBusMessage)));

    QString path=QDir::homePath()+"/.config/ukui-menu/ukui-menu.ini";
    setting=new QSettings(path,QSettings::IniFormat);
}

/**
 * 添加顶部控件
 */
void MainViewWidget::add_top_control()
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

    init_query_lineedit();

}

/**
 * 添加搜索框
 */
void MainViewWidget::init_query_lineedit()
{
    pIconTextWid=new QWidget(querylineEdit);
    pIconTextWid->setStyleSheet("background:transparent");
    pIconTextWidLayout=new QHBoxLayout;
    pIconTextWidLayout->setContentsMargins(0,0,0,0);
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
    QFont font;
    font.setPixelSize(Style::QueryLineEditFontSize);
    pQueryText=new QLabel(pIconTextWid);
    pQueryText->setFont(font);
    pQueryText->setText(tr("Search"));
    pQueryText->setStyleSheet("background:transparent;color:#626c6e;");
    pQueryText->adjustSize();
    pIconTextWidLayout->addWidget(pQueryIcon);
    pIconTextWidLayout->addWidget(pQueryText);
    pIconTextWid->setFixedSize(pQueryIcon->width()+pQueryText->width()+5,Style::QueryLineEditHeight);
    queryLayout->addWidget(pIconTextWid);
    queryLayout->setAlignment(pIconTextWid,Qt::AlignCenter);
    querylineEdit->setFocusPolicy(Qt::ClickFocus);
    querylineEdit->installEventFilter(this);

    searchAction = new QAction(querylineEdit);
    searchAction->setIcon(QIcon(":/data/img/mainviewwidget/search.svg"));

    searchappthread=new SearchAppThread;
    connect(this,SIGNAL(send_search_keyword(QString)),
            searchappthread,SLOT(recv_search_keyword(QString)));
    connect(searchappthread,SIGNAL(send_search_result(QStringList)),
            this,SLOT(recv_search_result(QStringList)));
    connect(querylineEdit, SIGNAL(textChanged(QString)), this, SLOT(search_app_slot(QString)));
}

bool MainViewWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==querylineEdit)
    {
        if(event->type()==QEvent::FocusIn)
        {   
             emit send_querylineEdit_focusin_signal();
             querylineEdit->addAction(searchAction,QLineEdit::LeadingPosition);
             char style[200];
             sprintf(style, "QLineEdit{border:1px solid %s;background-color:%s;border-radius:2px;font-size:14px;color:#ffffff;}",
                     QueryLineEditClickedBorder,QueryLineEditClickedBackground);
             querylineEdit->setStyleSheet(style);
             queryLayout->removeWidget(pIconTextWid);
             pIconTextWid->setParent(nullptr);

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
        else if(event->type()==QEvent::FocusOut)
        {
             querylineEdit->removeAction(searchAction);
             char style[100];
             sprintf(style, "QLineEdit{border:0px;background-color:%s;border-radius:2px;}",QueryLineEditBackground);
             querylineEdit->setStyleSheet(style);
             queryLayout->addWidget(pIconTextWid);
             queryLayout->setAlignment(pIconTextWid,Qt::AlignCenter);
        }
    }

     return QWidget::eventFilter(watched,event);     // 最后将事件交给上层对话框
}

void MainViewWidget::keyPressEvent(QKeyEvent *e)
{
    querylineEdit->setFocus();
    if(e->type()==QEvent::KeyPress)
    {

        QKeyEvent* ke=static_cast<QKeyEvent*>(e);
        if(ke->key()>=0x30 && ke->key()<=0x39 && ke->key()>=0x41 && ke->key()<=0x5a)
        {
            querylineEdit->setText(QKeySequence(ke->key()).toString());
        }
    }

}

/**
 * 搜索程序和文件槽函数
 */
void MainViewWidget::search_app_slot(QString arg)
{
    emit send_search_keyword(arg);
    searchappthread->start();
}

void MainViewWidget::recv_search_result(QStringList desktopfplist)
{
    searchappthread->quit();
    fullsearchresultwid->update_app_listview(desktopfplist);
    searchresultwid->update_app_listview(desktopfplist);
}

/**
 * 加载默认主视图
 */
void MainViewWidget::load_min_mainview()
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
        load_commonuse_widget();
    else if(widgetState==2)
        load_letter_widget();
    else
        load_function_widget();
}

/**
 * 加载全屏主视图
 */
void MainViewWidget::load_max_mainview()
{
//    this->setGeometry(160,0,QApplication::desktop()->availableGeometry().width()-160,QApplication::desktop()->availableGeometry().height());
//    this->setGeometry(0,0,QApplication::desktop()->availableGeometry().width()-60,QApplication::desktop()->availableGeometry().height());

    qDebug()<<"MainViewWidget111:"<<Style::AppListWidHeight<<QApplication::desktop()->availableGeometry().height();
    this->setFixedSize(Style::MainViewWidWidth,
                       QApplication::desktop()->availableGeometry().height());
    topWidget->setFixedSize(this->width(),Style::TopWidgetHeight);
    querylineEdit->setFixedSize(Style::QueryLineEditWidth,Style::QueryLineEditHeight);

    topLayout->setContentsMargins((topWidget->width()-Style::LeftWidWidth-querylineEdit->width())/2+Style::LeftWidWidth,
                                  0,
                                  (topWidget->width()-Style::LeftWidWidth-querylineEdit->width())/2,
                                  0);

    qDebug()<<"MainViewWidget:"<<Style::AppListWidHeight<<QApplication::desktop()->availableGeometry().height();
//    qDebug()<<topWidget->height()<<Style::AppListWidHeight<<QApplication::desktop()->availableGeometry().height();

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
        load_fullcommonuse_widget();
    else if(widgetState==2)
        load_fullletter_widget();
    else if(widgetState==3)
        load_fullfunction_widget();
}

/**
 * 加载常用分类界面
 */
void MainViewWidget::load_commonuse_widget()
{
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
//    emit send_mainview_state(1);

}

/**
 * 加载字母分类界面
 */
void MainViewWidget::load_letter_widget()
{
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

}

/**
 * 加载功能分类界面
 */
void MainViewWidget::load_function_widget()
{
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

}


/**
 * 加载全屏常用分类界面
 */
void MainViewWidget::load_fullcommonuse_widget()
{
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
//    emit send_mainview_state(1);
    widgetState=1;


}

/**
 * 加载全屏字母分类界面
 */
void MainViewWidget::load_fullletter_widget()
{
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

}

/**
 * 加载全屏功能分类界面
 */
void MainViewWidget::load_fullfunction_widget()
{
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
        QString deskfp=replyapp.value();

        if(!deskfp.isEmpty())
        {
//            setting->setIniCodec("UTF8");
            setting->beginGroup("application");
            QString appname=pUkuiMenuInterface->get_app_name(deskfp);
            if(!setting->contains(appname))
            {
                setting->setValue(appname,1);
                setting->sync();
            }
            else if(setting->value(appname).toInt()==1)
            {
                setting->setValue(appname,2);
                setting->sync();
                emit view_opened_signal();
            }
            setting->endGroup();
        }
    }
}

/**
 * desktop文件目录改变信号槽
 */
void MainViewWidget::directoryChangedSlot()
{
    QStringList desktopfpList=pUkuiMenuInterface->get_desktop_file_path();
    if(desktopfpList.size() > UkuiMenuInterface::desktopfpVector.size())//有新的应用安装
    {
        setting->beginGroup("recentapp");
        for(int i=0;i<desktopfpList.count();i++)
        {
            if(!UkuiMenuInterface::desktopfpVector.contains(desktopfpList.at(i)))
            {
                QString appname=pUkuiMenuInterface->get_app_name(desktopfpList.at(i));
                setting->setValue(appname,0);
                qDebug()<<"anzhuang:"<<appname;
            }

        }
        setting->endGroup();
        UkuiMenuInterface::appInfoVector.clear();
        UkuiMenuInterface::appInfoVector=pUkuiMenuInterface->create_appinfo_vector();
        emit directoryChangedSignal();
    }
    else//软件卸载
    {
        for(int i=0;i<UkuiMenuInterface::desktopfpVector.size();i++)
        {
            if(!desktopfpList.contains(UkuiMenuInterface::desktopfpVector.at(i)))
            {
//                QString appname=pUkuiMenuInterface->get_app_name(UkuiMenuInterface::desktopfpVector.at(i));
                QString appname=UkuiMenuInterface::appInfoVector.at(i).at(2);
                setting->beginGroup("application");
                setting->remove(appname);
                setting->endGroup();
                setting->beginGroup("recentapp");
                setting->remove(appname);
                setting->endGroup();
                qDebug()<<"xiezai:"<<appname;
            }
        }
        UkuiMenuInterface::appInfoVector=pUkuiMenuInterface->create_appinfo_vector();
        emit directoryChangedSignal();
    }
}

void MainViewWidget::widget_make_zero()
{
    commonusewid->widget_make_zero();
    fullcommonusewid->widget_make_zero();
    letterwid->widget_make_zero();
    fullletterwid->widget_make_zero();
    functionwid->widget_make_zero();
    fullfunctionwid->widget_make_zero();
    querylineEdit->clear();
    querylineEdit->clearFocus();
//    load_letter_widget();
}
