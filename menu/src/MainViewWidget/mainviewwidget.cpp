#include "mainviewwidget.h"
#include "ui_mainviewwidget.h"
#include "src/color.h"
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
    delete fullsearchresultwid;
    delete pUkuiMenuInterface;
}

void MainViewWidget::init_widget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setStyleSheet("border:0px;background:transparent;");

    mainLayout=new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    topWidget=new QWidget(this);
    char topWidgetcolor[100];
    sprintf(topWidgetcolor, "border:0px;background-color:%s;",MAINVIEWWIDGETCOLOR);
    topWidget->setStyleSheet(QString::fromLocal8Bit(topWidgetcolor));
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

    //记录字母排序模块与功能分类模块默认态与全屏态界面状态
    connect(letterwid,SIGNAL(send_letterwid_state(int,QString)),this,SLOT(recv_letterwid_state(int,QString)));
    connect(functionwid,SIGNAL(send_functionwid_state(int,QString)),this,SLOT(recv_functionwid_state(int,QString)));
    connect(fullletterwid,SIGNAL(send_fullletterwid_state(int,QString)),this,SLOT(recv_letterwid_state(int,QString)));
    connect(fullfunctionwid,SIGNAL(send_fullfunctionwid_state(int,QString)),this,SLOT(recv_functionwid_state(int,QString)));

    //监控.desktop文件目录
    fileWatcher=new QFileSystemWatcher(this);
    fileWatcher->addPath("/usr/share/applications");
//    connect(fileWatcher,SIGNAL(directoryChanged(const QString &)),this,SLOT(directoryChangedSlot()));
//    connect(this,SIGNAL(directoryChangedSignal(QString,QString,int)),letterwid,SLOT(update_app_listview(QString,QString,int)));
//    connect(this,SIGNAL(directoryChangedSignal(QString,QString,int)),fullletterwid,SLOT(update_app_listview(QString,QString,int)));
//    connect(this,SIGNAL(directoryChangedSignal(QString,QString,int)),functionwid,SLOT(update_app_listview(QString,QString,int)));
//    connect(this,SIGNAL(directoryChangedSignal(QString,QString,int)),fullfunctionwid,SLOT(update_app_listview(QString,QString,int)));
//    connect(this,SIGNAL(directoryChangedSignal(QString,QString,int)),commonusewid,SLOT(update_listview_slot()));
//    connect(this,SIGNAL(directoryChangedSignal(QString,QString,int)),fullcommonusewid,SLOT(update_listview_slot()));

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

    //监控应用进程开启
    QDBusConnection::sessionBus().connect("org.ayatana.bamf","/org/ayatana/bamf/matcher","org.ayatana.bamf.matcher",
                                         QString("ViewOpened"),this,SLOT(ViewOpenedSlot(QDBusMessage)));

    QString path=QDir::homePath()+"/.config/ukui-menu/ukui-menu.ini";
    setting=new QSettings(path,QSettings::IniFormat);

//    setting->beginGroup("allapps");
//    setting->remove("");
//    QStringList dflist=pUkuiMenuInterface->get_desktop_file_path();
//    for(int i=0;i<dflist.count();i++)
//    {
//        QFileInfo fileinfo(dflist.at(i));
//        QString desktopfile=fileinfo.fileName();
//        QString appname=pUkuiMenuInterface->get_app_name(dflist.at(i));
//        setting->setValue(desktopfile,appname);
//        setting->sync();
//    }
//    setting->endGroup();

}

/**
 * 添加顶部控件
 */
void MainViewWidget::add_top_control()
{
    topLayout=new QVBoxLayout(topWidget);
    topLayout->setSpacing(0);

    minmaxWidget=new QWidget(topWidget);
    minmaxWidget->setStyleSheet("QWidget{background:transparent;}");
    minmaxLayout=new QHBoxLayout(minmaxWidget);
    minmaxbtn=new QToolButton(minmaxWidget);
    minmaxbtn->setShortcut(QKeySequence::InsertParagraphSeparator);
//    minmaxbtn->setShortcut(Qt::Key_Return);
    horizontalSpacer=new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);
    minmaxLayout->addItem(horizontalSpacer);
    minmaxLayout->addWidget(minmaxbtn);
    minmaxWidget->setLayout(minmaxLayout);

    queryWidget=new QWidget(topWidget);
    queryWidget->setStyleSheet("QWidget{background-color:rgb(136,151,163,20%);}");

    topLayout->addWidget(minmaxWidget);
    topLayout->addWidget(queryWidget);
    topWidget->setLayout(topLayout);

    add_query_lineedit();

}

/**
 * 添加搜索框
 */
void MainViewWidget::add_query_lineedit()
{
    queryLayout=new QHBoxLayout(queryWidget);
    queryLayout->setContentsMargins(5,0,0,0);
    queryLayout->setSpacing(5);
    querybtn=new QToolButton(queryWidget);
//    QPixmap pixmap(QString(":/data/img/mainviewwidget/搜索.svg"));
//    querybtn->setFixedSize(pixmap.size());
    QSvgRenderer* svgRender = new QSvgRenderer(querybtn);
    svgRender->load(QString(":/data/img/mainviewwidget/search.svg"));
    QPixmap* pixmap = new QPixmap(19,19);
    pixmap->fill(Qt::transparent);//设置背景透明
    QPainter p(pixmap);
    svgRender->render(&p);
    querybtn->setIcon(QIcon(*pixmap));
    querybtn->setFixedSize(pixmap->size());
    querybtn->setStyleSheet("QToolButton{background:transparent;border:0px;padding-left:0px;}");
    querylineEdit=new QLineEdit(queryWidget);
    querylineEdit->setPlaceholderText("搜索程序");
    querylineEdit->setStyleSheet("QLineEdit{background-color:rgb(255,255,255,0);color:#626c6e;font-size:14px;border: 0px groove #bebebe;}");
    queryLayout->addWidget(querybtn);
    queryLayout->addWidget(querylineEdit);
    queryWidget->setLayout(queryLayout);

    querylineEdit->setFocusPolicy(Qt::ClickFocus);
    querylineEdit->installEventFilter(this);

    searchappthread=new SearchAppThread;
    connect(this,SIGNAL(send_search_keyword(QString)),
            searchappthread,SLOT(recv_search_keyword(QString)));
//    emit send_search_keyword(arg);
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
             queryWidget->setStyleSheet("QWidget{background-color:#ffffff;}");

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
             queryWidget->setStyleSheet("QWidget{background-color:rgb(136,151,163,20%);}");
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
//    qDebug()<<desktopfplist;
//    searchappthread->quit();
//    searchappthread->wait();
//    delete searchappthread;

    searchappthread->quit();
    fullsearchresultwid->update_app_listview(desktopfplist);
    searchresultwid->update_app_listview(desktopfplist);
}

/**
 * 加载默认主视图
 */
void MainViewWidget::load_min_mainview()
{
    this->setGeometry(60,QApplication::desktop()->availableGeometry().height()-532,330,532);
    set_max_btn();
    topLayout->setContentsMargins(15,0,0,20);
    topWidget->setFixedSize(330,70);
    minmaxWidget->setFixedSize(330-15,20);
    minmaxLayout->setContentsMargins(0,0,0,0);
    disconnect(minmaxbtn,SIGNAL(clicked()),this, SLOT(default_btn_slot()));
    connect(minmaxbtn, SIGNAL(clicked()),this,SLOT(fullscreen_btn_slot()));

    queryWidget->setFixedSize(290,30);
    querylineEdit->setFixedSize(queryWidget->width()-querybtn->width(),30);

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
}

/**
 * 设置全屏按钮
 */
void MainViewWidget::set_max_btn()
{
    minmaxbtn->setFixedSize(20,20);
    QSvgRenderer* svgRender = new QSvgRenderer(minmaxbtn);
    svgRender->load(QString(":/data/img/mainviewwidget/max.svg"));
    QPixmap* pixmap = new QPixmap(14,14);
    pixmap->fill(Qt::transparent);//设置背景透明
    QPainter p(pixmap);
    svgRender->render(&p);
    minmaxbtn->setIcon(QIcon(*pixmap));
    char btncolor[300];
    sprintf(btncolor,"QToolButton{background:transparent;border:0px;padding-left:0px;}\
            QToolButton:hover{background-color:%s;}\
            QToolButton:pressed{background-color:%s;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);
    minmaxbtn->setStyleSheet(QString::fromLocal8Bit(btncolor));

}

/**
 * 加载全屏主视图
 */
void MainViewWidget::load_max_mainview()
{
    this->setGeometry(160,0,QApplication::desktop()->availableGeometry().width()-160,QApplication::desktop()->availableGeometry().height());
    set_min_btn();
    topLayout->setContentsMargins(30,0,0,20);
    topWidget->setFixedSize(QApplication::desktop()->availableGeometry().width()-160,70);
    minmaxWidget->setFixedSize(QApplication::desktop()->availableGeometry().width()-160-30,20);
    minmaxLayout->setContentsMargins(0,5,5,0);

    disconnect(minmaxbtn, SIGNAL(clicked()),this,SLOT(fullscreen_btn_slot()));
    connect(minmaxbtn, SIGNAL(clicked()),this,SLOT(default_btn_slot()));

    queryWidget->setFixedSize(600,30);
    querylineEdit->setFixedSize(queryWidget->width()-querybtn->width(),30);

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
}

/**
 * 设置还原按钮
 */
void MainViewWidget::set_min_btn()
{
    minmaxbtn->setFixedSize(15,15);
    QSvgRenderer* svgRender = new QSvgRenderer(minmaxbtn);
    svgRender->load(QString(":/data/img/mainviewwidget/min.svg"));
    QPixmap* pixmap = new QPixmap(14,14);
    pixmap->fill(Qt::transparent);//设置背景透明
    QPainter p(pixmap);
    svgRender->render(&p);
    minmaxbtn->setIcon(QIcon(*pixmap));
    char btncolor[300];
    sprintf(btncolor,"QToolButton{background:transparent;border:0px;padding-left:0px;}\
            QToolButton:hover{background-color:%s;}\
            QToolButton:pressed{background-color:%s;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);
    minmaxbtn->setStyleSheet(QString::fromLocal8Bit(btncolor));
}

/**
 * 接收默认或全屏字母排序界面状态信号
 */
void MainViewWidget::recv_letterwid_state(int arg,QString btnname)
{
    this->letterwid_state=arg;
    this->letterbtnname=btnname;
}

/**
 * 接收默认与全屏功能分类界面状态信号
 */
void MainViewWidget::recv_functionwid_state(int arg,QString btnname)
{
    this->functionwid_state=arg;
    this->functionbtnname=btnname;

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

}

/**
 * 加载字母分类界面
 */
void MainViewWidget::load_letter_widget()
{
    if(letterbtnname.isEmpty())
        letterbtnname="A";
    letterwid->set_letterwidge_state(letterwid_state,letterbtnname);
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
    if(functionbtnname.isEmpty())
        functionbtnname="最近添加";
    functionwid->set_functionwidge_state(functionwid_state,functionbtnname);
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
    widgetState=1;

}

/**
 * 加载全屏字母分类界面
 */
void MainViewWidget::load_fullletter_widget()
{
    if(letterbtnname.isEmpty())
        letterbtnname="A";
    fullletterwid->set_fullletterwidge_state(letterwid_state,letterbtnname);
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
    if(functionbtnname.isEmpty())
        functionbtnname="最近添加";
    fullfunctionwid->set_fullfunctionwidge_state(functionwid_state,functionbtnname);
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
 * 全屏按钮槽函数
 */
void MainViewWidget::fullscreen_btn_slot()
{
    emit send_fullscreenbtn_signal(widgetState);
}

/**
 * 默认态按钮槽函数
 */
void MainViewWidget::default_btn_slot()
{
    emit send_defaultbtn_signal(widgetState);
}

/**
 * 设置主界面从全屏回到默认状态时应加载的分类窗口
 */
void MainViewWidget::load_classification_widget(int arg)
{
    if(arg==1)
        load_commonuse_widget();
    else if(arg==2)
        load_letter_widget();
    else if(arg==3)
        load_function_widget();
}

/**
 * 主界面全屏时应加载的分类窗口
 */
void MainViewWidget::load_full_classification_widget(int arg)
{
    if(arg==1)
        load_fullcommonuse_widget();
    else if(arg==2)
        load_fullletter_widget();
    else if(arg==3)
        load_fullfunction_widget();
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
        QStringList desktopfnList;
        for(int i=0;i<desktopfpList.count();i++)
        {
    //        QString appname=KylinStartMenuInterface::get_app_name(desktopList.at(i));
    //        appnameList.append(appname);
            QFileInfo fileinfo(desktopfpList.at(i));
            QString desktopfn=fileinfo.fileName();
            desktopfnList.append(desktopfn);
        }
        QString path=QDir::homePath()+"/.config/ukui-menu/ukui-menu.ini";
        setting=new QSettings(path,QSettings::IniFormat);
        setting->beginGroup("allapps");
        QStringList keyList=setting->childKeys();
        if(desktopfnList.count()>keyList.count())
        {
            for(int i=0;i<desktopfnList.count();i++)//有新的应用安装
            {
                if(keyList.indexOf(desktopfnList.at(i))==-1)
                {
                    QString appname=pUkuiMenuInterface->get_app_name(QString("/usr/share/applications/"+desktopfnList.at(i)));
                    setting->setValue(desktopfnList.at(i),appname);
                    setting->sync();
                    emit directoryChangedSignal(desktopfnList.at(i),appname,0);
                    break;
                }
            }
        }
        QString appname;
        if(desktopfnList.count()<keyList.count())//有软件被卸载
        {
            for(int i=0;i<keyList.count();i++)
            {
                if(desktopfnList.indexOf(keyList.at(i))==-1)
                {
                    appname=setting->value(keyList.at(i)).toString();
                    emit directoryChangedSignal(keyList.at(i),appname,-1);
                    setting->remove(keyList.at(i));
                    setting->sync();
                    setting->endGroup();//setting->beginGroup("allapps");
                    break;
                }
            }
            setting->beginGroup("application");
            setting->remove(appname);
            setting->sync();
            setting->endGroup();
        }
}
