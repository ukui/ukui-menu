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
    delete letterwid;
    delete functionwid;
    delete searchResultWid;
    delete fullsearchresultwid;
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
    letterwid=new LetterWidget;
    functionwid=new FunctionWidget;

    connect(letterwid,SIGNAL(send_update_applist_signal(QString)),commonusewid,SLOT(update_tablewid_slot(QString)));
    connect(functionwid,SIGNAL(send_update_applist_signal(QString)),commonusewid,SLOT(update_tablewid_slot(QString)));

    fileWatcher=new QFileSystemWatcher(this);
    fileWatcher->addPath("/usr/share/applications");
    connect(fileWatcher,SIGNAL(directoryChanged(const QString &)),this,SLOT(directoryChangedSlot()));
    connect(this,SIGNAL(directoryChangedSignal(QString,int)),letterwid,SLOT(update_app_tablewidget(QString,int)));
    connect(this,SIGNAL(directoryChangedSignal(QString,int)),functionwid,SLOT(update_app_tablewidget(QString,int)));

    connect(commonusewid,SIGNAL(send_hide_mainwindow_signal()),this,SIGNAL(send_hide_mainwindow_signal()));
    connect(letterwid,SIGNAL(send_hide_mainwindow_signal()),this,SIGNAL(send_hide_mainwindow_signal()));
    connect(functionwid,SIGNAL(send_hide_mainwindow_signal()),this,SIGNAL(send_hide_mainwindow_signal()));

    add_top_control();
    load_min_mainview();

    QString path=QDir::homePath()+"/.config/ukui-start-menu/ukui-start-menu.ini";
    setting=new QSettings(path,QSettings::IniFormat);
    QStringList grouplist=setting->childGroups();
    if(grouplist.indexOf("apps")==-1)
    {
        setting->beginGroup("apps");
        QStringList dflist=KylinStartMenuInterface::get_desktop_file_path();
        for(int i=0;i<dflist.count();i++)
        {
            QFileInfo fileinfo(dflist.at(i));
            QString desktopfile=fileinfo.fileName();
            QString appname=KylinStartMenuInterface::get_app_name(dflist.at(i));
            setting->setValue(desktopfile,appname);
            setting->sync();
        }
        setting->endGroup();
    }
}

/**
 * 添加顶部控件
 */
void MainViewWidget::add_top_control()
{
    topLayout=new QVBoxLayout(topWidget);
    topLayout->setSpacing(0);

    minmaxWidget=new QWidget(topWidget);
    minmaxWidget->setStyleSheet("QWidget{background:transparent;border:0px;}");
    minmaxLayout=new QHBoxLayout(minmaxWidget);
    minmaxbtn=new QToolButton(minmaxWidget);
    minmaxbtn->setShortcut(QKeySequence::InsertParagraphSeparator);
//    minmaxbtn->setShortcut(Qt::Key_Return);
    horizontalSpacer=new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);
    minmaxLayout->addItem(horizontalSpacer);
    minmaxLayout->addWidget(minmaxbtn);
    minmaxWidget->setLayout(minmaxLayout);

    queryWidget=new QWidget(topWidget);
    queryWidget->setStyleSheet("QWidget{background-color:rgb(136,151,163,20%);border:0px;}");

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

    querylineEdit->setEnabled(false);

    querylineEdit->setFocusPolicy(Qt::ClickFocus);
    querylineEdit->installEventFilter(this);

    fullsearchresultwid=new FullSearchResultWidget;
    searchresultwid=new SearchResultWidget;
    connect(this, SIGNAL(send_search_keyword(QString)), fullsearchresultwid, SLOT(recv_search_keyword(QString)));
    connect(this, SIGNAL(send_search_keyword(QString)), searchresultwid, SLOT(recv_search_keyword(QString)));

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
 * 全屏按钮槽函数
 */
void MainViewWidget::fullscreen_btn_slot()
{
    emit send_fullscreenbtn_signal();
}

/**
 * 默认态按钮槽函数
 */
void MainViewWidget::default_btn_slot()
{
    emit send_defaultbtn_signal();
}

/**
 * 设置主界面从全屏回到默认状态时应加载的分类窗口
 */
void MainViewWidget::load_classification_widget()
{
        commonusewid->load_min_wid();
        letterwid->load_min_wid();
        functionwid->load_min_wid();
}

/**
 * 主界面全屏时应加载的分类窗口
 */
void MainViewWidget::load_full_classification_widget()
{
        commonusewid->load_max_wid();
        letterwid->load_max_wid();
        functionwid->load_max_wid();
}

/**
 * desktop文件目录改变信号槽
 */
void MainViewWidget::directoryChangedSlot()
{
        QStringList desktopfpList=KylinStartMenuInterface::get_desktop_file_path();
        QStringList desktopfnList;
        for(int i=0;i<desktopfpList.count();i++)
        {
    //        QString appname=KylinStartMenuInterface::get_app_name(desktopList.at(i));
    //        appnameList.append(appname);
            QFileInfo fileinfo(desktopfpList.at(i));
            QString desktopfn=fileinfo.fileName();
            desktopfnList.append(desktopfn);
        }
        QString path=QDir::homePath()+"/.config/ukui-start-menu/ukui-start-menu.ini";
        setting=new QSettings(path,QSettings::IniFormat);
        setting->beginGroup("apps");
        QStringList keyList=setting->childKeys();
        if(desktopfnList.count()>keyList.count())
        {
            for(int i=0;i<desktopfnList.count();i++)//有新的应用安装
            {
                if(keyList.indexOf(desktopfnList.at(i))==-1)
                {
                    QString appname=KylinStartMenuInterface::get_app_name(QString("/usr/share/applications/"+desktopfnList.at(i)));
                    setting->setValue(desktopfnList.at(i),appname);
                    setting->sync();
                    emit directoryChangedSignal(desktopfnList.at(i),0);
                    break;
                }
            }
        }
        if(desktopfnList.count()<keyList.count())//有软件被卸载
        {
            for(int i=0;i<keyList.count();i++)
            {
                if(desktopfnList.indexOf(keyList.at(i))==-1)
                {
                    QString appname=setting->value(keyList.at(i)).toString();
                    emit directoryChangedSignal(appname,-1);
                    setting->remove(keyList.at(i));
                    setting->sync();
                    break;
                }
            }
        }
        setting->endGroup();

}

