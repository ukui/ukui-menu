#include "functionwidget.h"
#include "ui_functionwidget.h"
#include "src/color.h"
#include "kylin-start-menu-interface.h"
#include <QDebug>

FunctionWidget::FunctionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FunctionWidget)
{
    ui->setupUi(this);

    init_widget();
}

FunctionWidget::~FunctionWidget()
{
    delete ui;
    if(menu!=nullptr)
        delete menu;
    if(functionbtnwid!=nullptr)
        delete functionbtnwid;
}

/**
 * 主界面初始化
 */
void FunctionWidget::init_widget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    char widgetcolor[100];
    sprintf(widgetcolor, "border:0px;background-color:%s;",MAINVIEWWIDGETCOLOR);
    this->setStyleSheet(QString::fromLocal8Bit(widgetcolor));
//    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
//    this->setFixedSize(330,462);

    mainLayout=new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    applistWid=new QWidget(this);
    applistWid->setStyleSheet("border:0px;background: transparent;");
//    applistWid->setFixedSize(this->width(),this->height());
    iconlistWid=new QWidget(this);
    iconlistWid->setStyleSheet("border:0px;background:transparent");
    mainLayout->addWidget(applistWid);
    mainLayout->addWidget(iconlistWid);
    this->setLayout(mainLayout);

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
    functionnamelist.append("最近添加");
    functionnamelist.append("网络应用");
    functionnamelist.append("社交沟通");
    functionnamelist.append("影音播放");
    functionnamelist.append("开发编程");
    functionnamelist.append("图形图像");
    functionnamelist.append("游戏娱乐");
    functionnamelist.append("办公学习");
    functionnamelist.append("阅读翻译");
    functionnamelist.append("系统管理");
    functionnamelist.append("其他应用");

    init_applist_widget();
    init_iconlist_widget();

    functionbtnwid=new FunctionButtonWidget();
    connect(this,SIGNAL(send_classificationbtn_list(QStringList)),functionbtnwid,SLOT(recv_classificationbtn_list(QStringList)));
    connect(functionbtnwid, SIGNAL(send_functionbtn_signal(QString)),this,SLOT(recv_functionbtn_signal(QString)));

}

/**
 * 初始化应用列表界面
 */
void FunctionWidget::init_applist_widget()
{
    QHBoxLayout* layout=new QHBoxLayout(applistWid);
    layout->setContentsMargins(2,0,2,0);
    scrollarea=new ScrollArea;
    scrollareawid=new QWidget;
    scrollarea->setWidget(scrollareawid);
    scrollarea->setWidgetResizable(true);
    scrollareawidLayout=new QVBoxLayout;
    scrollareawidLayout->setContentsMargins(0,0,0,0);
    scrollareawidLayout->setSpacing(0);
    scrollareawid->setLayout(scrollareawidLayout);
    layout->addWidget(scrollarea);
    applistWid->setLayout(layout);
    fill_app_list();
    load_min_wid();

}

/**
 * 填充应用列表
 */
void FunctionWidget::fill_app_list()
{
    classificationbtnlist.clear();

    QStringList recentlist=KylinStartMenuInterface::get_recent_app_list();
    if(!recentlist.isEmpty())
    {
        QStringList newrecentlist;
        newrecentlist.clear();
        insert_classification_btn("最近添加");
        for(int i=0;i<10;i++)
        {
            newrecentlist.append(recentlist.at(i));
        }
        insert_app_list(newrecentlist);
    }

    QStringList netlist=KylinStartMenuInterface::get_network_app_list();
    if(!netlist.isEmpty())
    {
        insert_classification_btn("网络应用");
        insert_app_list(netlist);
    }
    QStringList sociallist=KylinStartMenuInterface::get_social_app_list();
    if(!sociallist.isEmpty())
    {
        insert_classification_btn("社交沟通");
        insert_app_list(sociallist);
    }
    QStringList avlist=KylinStartMenuInterface::get_av_app_list();
    if(!avlist.isEmpty())
    {
        insert_classification_btn("影音播放");
        insert_app_list(avlist);
    }
    QStringList developlist=KylinStartMenuInterface::get_develop_app_list();
    if(!developlist.isEmpty())
    {
        insert_classification_btn("开发编程");
        insert_app_list(developlist);
    }
    QStringList graphicslist=KylinStartMenuInterface::get_graphics_app_list();
    if(!graphicslist.isEmpty())
    {
        insert_classification_btn("图形图像");
        insert_app_list(graphicslist);
    }
    QStringList gamelist=KylinStartMenuInterface::get_game_app_list();
    if(!gamelist.isEmpty())
    {
        insert_classification_btn("游戏娱乐");
        insert_app_list(gamelist);
    }
    QStringList officelist=KylinStartMenuInterface::get_office_app_list();
    if(!officelist.isEmpty())
    {
        insert_classification_btn("办公学习");
        insert_app_list(officelist);
    }
    QStringList educationlist=KylinStartMenuInterface::get_education_app_list();
    if(!educationlist.isEmpty())
    {
        insert_classification_btn("阅读翻译");
        insert_app_list(educationlist);
    }

    QStringList systemadminlist=KylinStartMenuInterface::get_systemadmin_app_list();
    if(!systemadminlist.isEmpty())
    {
        insert_classification_btn("系统管理");
        insert_app_list(systemadminlist);
    }
    QStringList otherlist=KylinStartMenuInterface::get_other_app_list();
    if(!otherlist.isEmpty())
    {
        insert_classification_btn("其他应用");
        insert_app_list(otherlist);
    }


}

void FunctionWidget::insert_classification_btn(QString btnname)
{
    PushButton* classificationbtn=new PushButton(btnname,0,2);
    scrollareawidLayout->addWidget(classificationbtn);

    classificationbtnlist.append(btnname);

    connect(classificationbtn, SIGNAL(clicked()), this,SLOT(app_classificationbtn_clicked_slot()));

}

void FunctionWidget::insert_app_list(QStringList appnamelist)
{
    QFlowLayout* flowlayout=new QFlowLayout(0,0,0);
    QWidget* wid=new QWidget;
    wid->setLayout(flowlayout);
    scrollareawidLayout->addWidget(wid);
    for(int i=0;i<appnamelist.count();i++)
    {
        PushButton* btn=new PushButton(appnamelist.at(i),1,2);
        flowlayout->addWidget(btn);
        connect(btn,SIGNAL(clicked()),
                this,SIGNAL(send_hide_mainwindow_signal()));
        connect(btn,SIGNAL(send_update_applist_signal(QString)),
                this,SIGNAL(send_update_applist_signal(QString)));
    }

}

void FunctionWidget::load_min_wid()
{
    classificationbtnrowlist.clear();
    is_fullscreen=false;
    this->setGeometry(QRect(60,QApplication::desktop()->availableGeometry().height()-462,
                                 330,462));
    applistWid->setFixedSize(this->width(),this->height());
    applistWid->layout()->setContentsMargins(2,0,2,0);
    scrollarea->setFixedSize(applistWid->width()-4,applistWid->height());
    mainLayout->removeWidget(iconlistWid);
    iconlistWid->setParent(nullptr);
    if(is_functionbtnwid)
        functionbtnwid->load_min_wid();

    int pos=0;
    classificationbtnrowlist.append(QString::number(pos));
    int index=0;
    while(index<scrollareawidLayout->count()/2)
    {
        //分类按钮界面
        QLayoutItem* cfbitem=scrollareawidLayout->itemAt(index*2);
        QWidget* classificationbtnwid=cfbitem->widget();
        QPushButton* classificationbtn=qobject_cast<QPushButton*>(classificationbtnwid);
        classificationbtn->setFixedSize(scrollarea->width()-14,20);
        classificationbtn->layout()->setContentsMargins(15,0,0,0);
        classificationbtn->layout()->setSpacing(6);
        QLayoutItem* item=classificationbtn->layout()->itemAt(0);
        QWidget* linewid=item->widget();
        QFrame* line=qobject_cast<QFrame*>(linewid);
        line->setFixedSize(classificationbtn->width()-77,1);
        //应用按钮界面
        QLayoutItem* widItem=scrollareawidLayout->itemAt(index*2+1);
        QWidget* wid=widItem->widget();
        int num=wid->layout()->count();
        wid->setFixedSize(scrollarea->width()-14,num*42);

        pos+=(20+wid->height());
        classificationbtnrowlist.append(QString::number(pos));
        index++;

    }
    scrollarea->widget()->adjustSize();
}

void FunctionWidget::load_max_wid()
{
    classificationbtnrowlist.clear();
    is_fullscreen=true;
    this->setGeometry(QRect(160,
                            70,
                            QApplication::desktop()->availableGeometry().width()-160,
                            QApplication::desktop()->availableGeometry().height()-70));

    applistWid->setFixedSize(this->width(),this->height()-70);
    applistWid->layout()->setContentsMargins(30,0,2,0);
    scrollarea->setFixedSize(applistWid->width()-32-12,applistWid->height());
    iconlistWid->setFixedSize(this->width(),70);
    if(!is_functionbtnwid)
        mainLayout->addWidget(iconlistWid);
    else
        functionbtnwid->load_max_wid();

    int pos=0;
    classificationbtnrowlist.append(QString::number(pos));
    int index=0;
    while(index<scrollareawidLayout->count()/2)
    {
        //分类按钮
        QLayoutItem* cfbitem=scrollareawidLayout->itemAt(index*2);
        QWidget* classificationbtnwid=cfbitem->widget();
        QPushButton* classificationbtn=qobject_cast<QPushButton*>(classificationbtnwid);
        classificationbtn->setFixedSize(scrollarea->width()-32,20);
        classificationbtn->layout()->setContentsMargins(0,0,0,0);
        classificationbtn->layout()->setSpacing(12);
        QLayoutItem* item=classificationbtn->layout()->itemAt(0);
        QWidget* linewid=item->widget();
        QFrame* line=qobject_cast<QFrame*>(linewid);
        line->setFixedSize(classificationbtn->width()-62,1);
        //应用按钮界面
        int dividend=scrollarea->width()/312;
        QLayoutItem* widItem=scrollareawidLayout->itemAt(index*2+1);
        QWidget* wid=widItem->widget();
        int num=wid->layout()->count();
        if(num%dividend>0)
            num=num/dividend+1;
        else num=num/dividend;
        wid->setFixedSize(scrollarea->width()-14,num*42);
        pos+=(20+wid->height());
        classificationbtnrowlist.append(QString::number(pos));
        index++;

    }
    scrollarea->widget()->adjustSize();
}

/**
 * 更新应用列表
 */
void FunctionWidget::update_app_list(QString name, int arg)
{
    //放到最近添加模块
    QLayoutItem* item=scrollareawidLayout->itemAt(1);
    QWidget* wid=item->widget();
    if(arg==0)//添加到最近模块
    {
        QString appname=KylinStartMenuInterface::get_app_name(QString("/usr/share/applications/"+name));
        PushButton* btn=new PushButton(appname,1,2);
        wid->layout()->addWidget(btn);
        wid->layout()->update();
    }
    else {//从最近模块删除
        int count=wid->layout()->count();
        int index=0;
        for(index=0;index<count;index++)
        {
            QLayoutItem* btnitem=wid->layout()->itemAt(index);
            QWidget* btnwid=btnitem->widget();
            QPushButton* btn=qobject_cast<QPushButton*>(btnwid);
            QLayoutItem* labelitem=btn->layout()->itemAt(1);
            QWidget* labelwid=labelitem->widget();
            QLabel* label=qobject_cast<QLabel*>(labelwid);
            QString appname=label->text();
            if(QString::compare(appname,name)==0)
            {
                wid->layout()->removeWidget(btn);
                btn->setParent(nullptr);
                delete btn;
                wid->layout()->update();
                break;
            }
        }
    }

    if(arg==0)//有新软件安装
    {
        QString appname=KylinStartMenuInterface::get_app_name(QString("/usr/share/applications/"+name));
        QStringList networkapplist=KylinStartMenuInterface::get_network_app_list();
        QStringList socilaapplist=KylinStartMenuInterface::get_social_app_list();
        QStringList avapplist=KylinStartMenuInterface::get_av_app_list();
        QStringList devapplist=KylinStartMenuInterface::get_develop_app_list();
        QStringList graphicsapplist=KylinStartMenuInterface::get_graphics_app_list();
        QStringList gameapplist=KylinStartMenuInterface::get_game_app_list();
        QStringList officeapplist=KylinStartMenuInterface::get_office_app_list();
        QStringList educationapplist=KylinStartMenuInterface::get_education_app_list();
        QStringList systemadminlist=KylinStartMenuInterface::get_systemadmin_app_list();
        QStringList otherapplist=KylinStartMenuInterface::get_other_app_list();
        if(networkapplist.contains(appname))
        {
            int num=networkapplist.count();
            add_app("网络应用",name,num);
        }
        if(socilaapplist.contains(appname))
        {
            int num=socilaapplist.count();
            add_app("社交沟通",name,num);
        }
        if(avapplist.contains(appname))
        {
            int num=avapplist.count();
            add_app("影音播放",name,num);
        }
        if(devapplist.contains(appname))
        {
            int num=devapplist.count();
            add_app("开发编程",name,num);
        }
        if(graphicsapplist.contains(appname))
        {
            int num=graphicsapplist.count();
            add_app("图形图像",name,num);
        }
        if(gameapplist.contains(appname))
        {
            int num=gameapplist.count();
            add_app("游戏娱乐",name,num);
        }
        if(officeapplist.contains(appname))
        {
            int num=officeapplist.count();
            add_app("办公学习",name,num);
        }
        if(educationapplist.contains(appname))
        {
            int num=educationapplist.count();
            add_app("阅读翻译",name,num);
        }
        if(systemadminlist.contains(appname))
        {
            int num=systemadminlist.count();
            add_app("系统管理",name,num);
        }
        if(otherapplist.contains(appname))
        {
            int num=otherapplist.count();
            add_app("其他应用",name,num);
        }
    }
    else{//有软件被卸载
        int pos=0;
        while(pos<scrollareawidLayout->count()/2)
        {
            QLayoutItem* item=scrollareawidLayout->itemAt(2*pos+1);
            QWidget* wid=item->widget();
            int count=wid->layout()->count();
            for(int i=0;i<count;i++)
            {
                QLayoutItem* btnitem=wid->layout()->itemAt(i);
                QWidget* btnwid=btnitem->widget();
                QPushButton* btn=qobject_cast<QPushButton*>(btnwid);
                QLayoutItem* labelitem=btn->layout()->itemAt(1);
                QWidget* labelwid=labelitem->widget();
                QLabel* label=qobject_cast<QLabel*>(labelwid);
                QString cmpname=label->text();
                if(QString::compare(name,cmpname)==0)
                {
                    wid->layout()->removeWidget(btn);
                    btn->setParent(nullptr);
                    delete btn;
                    wid->layout()->update();
                    break;
                }
            }
            if(wid->layout()->count()==0)
            {
                QLayoutItem* classifybtnitem=scrollareawidLayout->itemAt(2*pos);
                QWidget* classifybtnwid=classifybtnitem->widget();
                QPushButton* classifybtn=qobject_cast<QPushButton*>(classifybtnwid);
                QLayoutItem* labelitem=classifybtn->layout()->itemAt(1);
                QWidget* labelwid=labelitem->widget();
                QLabel* label=qobject_cast<QLabel*>(labelwid);
                QString classify=label->text();
                for (int i=1;i>=0;i--) {
                    QLayoutItem* item=scrollareawidLayout->itemAt(2*pos+i);
                    QWidget* wid=item->widget();
                    scrollareawidLayout->removeWidget(wid);
                    wid->setParent(nullptr);
                    delete wid;
                }
                classificationbtnlist.removeAll(classify);
            }
            else {
               pos++;
            }
        }
    }

    if(is_fullscreen)
    {
        int pos=0;
        classificationbtnrowlist.append(QString::number(pos));
        int index=0;
        while(index<scrollareawidLayout->count()/2)
        {
            //分类按钮
            QLayoutItem* cfbitem=scrollareawidLayout->itemAt(index*2);
            QWidget* classificationbtnwid=cfbitem->widget();
            QPushButton* classificationbtn=qobject_cast<QPushButton*>(classificationbtnwid);
            classificationbtn->setFixedSize(scrollarea->width()-32,20);
            classificationbtn->layout()->setContentsMargins(0,0,0,0);
            classificationbtn->layout()->setSpacing(12);
            QLayoutItem* item=classificationbtn->layout()->itemAt(0);
            QWidget* linewid=item->widget();
            QFrame* line=qobject_cast<QFrame*>(linewid);
            line->setFixedSize(classificationbtn->width()-62,1);
            //应用按钮界面
            int dividend=scrollarea->width()/312;
            QLayoutItem* widItem=scrollareawidLayout->itemAt(index*2+1);
            QWidget* wid=widItem->widget();
            int num=wid->layout()->count();
            if(num%dividend>0)
                num=num/dividend+1;
            else num=num/dividend;
            wid->setFixedSize(scrollarea->width()-14,num*42);
            if(index<scrollareawidLayout->count()/2-1)
            {
                pos+=(20+wid->height());
                classificationbtnrowlist.append(QString::number(pos));
            }
            index++;

        }
        scrollarea->widget()->adjustSize();
    }
    else {
        int pos=0;
        classificationbtnrowlist.append(QString::number(pos));
        int index=0;
        while(index<scrollareawidLayout->count()/2)
        {
            //分类按钮界面
            QLayoutItem* cfbitem=scrollareawidLayout->itemAt(index*2);
            QWidget* classificationbtnwid=cfbitem->widget();
            QPushButton* classificationbtn=qobject_cast<QPushButton*>(classificationbtnwid);
            classificationbtn->setFixedSize(scrollarea->width()-14,20);
            classificationbtn->layout()->setContentsMargins(15,0,0,0);
            classificationbtn->layout()->setSpacing(6);
            QLayoutItem* item=classificationbtn->layout()->itemAt(0);
            QWidget* linewid=item->widget();
            QFrame* line=qobject_cast<QFrame*>(linewid);
            line->setFixedSize(classificationbtn->width()-77,1);
            //应用按钮界面
            QLayoutItem* widItem=scrollareawidLayout->itemAt(index*2+1);
            QWidget* wid=widItem->widget();
            int num=wid->layout()->count();
            wid->setFixedSize(scrollarea->width()-14,num*42);
            if(index<scrollareawidLayout->count()/2-1)
            {
                pos+=(20+wid->height());
                classificationbtnrowlist.append(QString::number(pos));
            }
            index++;

        }
        scrollarea->widget()->adjustSize();
    }
}

void FunctionWidget::add_app(QString classify, QString desktopfn, int num)
{
    //在functionnamelist中查找当前classify或者其上一个是否存在
    int index=functionnamelist.indexOf(classify);
    int pos=0;
    for(pos=index;pos>0;pos--)
    {
        if(classificationbtnlist.indexOf(functionnamelist.at(pos))!=-1)
            break;
    }
    if(pos==index)//说明functionnamelist存在classify
        pos=classificationbtnlist.indexOf(classify);
    if(num==1)//只有一个新增的应用
    {
        QString classifyname=classify;
        insert_classification_btn(classifyname,2*(pos+1));
        insert_appbtn_wid(desktopfn,2*(pos+1)+1);
    }
    else {
        QLayoutItem* item=scrollareawidLayout->itemAt(2*pos+1);
        QWidget* wid=item->widget();
        QString appname=KylinStartMenuInterface::get_app_name(QString("/usr/share/applications/"+desktopfn));
        PushButton* btn=new PushButton(appname,1,2);
        wid->layout()->addWidget(btn);
        wid->layout()->update();
        connect(btn,SIGNAL(clicked()),
                this,SIGNAL(send_hide_mainwindow_signal()));
        connect(btn,SIGNAL(send_update_applist_signal(QString)),
                this,SIGNAL(send_update_applist_signal(QString)));
    }
}

void FunctionWidget::insert_classification_btn(QString classify, int pos)
{
    PushButton* classificationbtn=new PushButton(classify,0,2);
    classificationbtnlist.insert(pos/2,classify);
    scrollareawidLayout->insertWidget(pos,classificationbtn);
    scrollareawidLayout->update();

    connect(classificationbtn, SIGNAL(clicked()), this,SLOT(app_classificationbtn_clicked_slot()));
}

void FunctionWidget::insert_appbtn_wid(QString desktopfn, int row)
{
    QString appname=KylinStartMenuInterface::get_app_name(QString("/usr/share/applications/"+desktopfn));
    PushButton* btn=new PushButton(appname,1,2);
    QWidget* wid=new QWidget;
    QFlowLayout* flowlayout=new QFlowLayout(0,0,0);
    wid->setLayout(flowlayout);
    flowlayout->addWidget(btn);
    flowlayout->update();
    scrollareawidLayout->insertWidget(row,wid);
    connect(btn,SIGNAL(clicked()),
            this,SIGNAL(send_hide_mainwindow_signal()));
    connect(btn,SIGNAL(send_update_applist_signal(QString)),
            this,SIGNAL(send_update_applist_signal(QString)));
}

/**
 * 应用列表功能分类按钮槽函数
 */
void FunctionWidget::app_classificationbtn_clicked_slot()
{
    //加载FunctionButtonWidget界面
    is_functionbtnwid=true;
    emit send_classificationbtn_list(classificationbtnlist);
    mainLayout->removeWidget(applistWid);
    applistWid->setParent(nullptr);
    if(is_fullscreen)
    {
        mainLayout->removeWidget(iconlistWid);
        iconlistWid->setParent(nullptr);
        functionbtnwid->load_max_wid();
    }
    else
        functionbtnwid->load_min_wid();
    mainLayout->addWidget(functionbtnwid);
}

/**
 * 接收FunctionButtonWidget界面按钮信号
 */
void FunctionWidget::recv_functionbtn_signal(QString btnname)
{
    is_functionbtnwid=false;
    mainLayout->removeWidget(functionbtnwid);
    functionbtnwid->setParent(nullptr);
    mainLayout->addWidget(applistWid);
    if(is_fullscreen)
    {
        mainLayout->addWidget(iconlistWid);
        int col=functionnamelist.indexOf(btnname);
        iconlisttableWid->selectColumn(col);
    }
    else{
        //此处需实现将功能为btnname的应用列表移动到applistWid界面最顶端
        int index=classificationbtnlist.indexOf(btnname);
        if(index!=-1)
        {
            int pos=classificationbtnrowlist.at(index).toInt();
            scrollarea->verticalScrollBar()->setSliderPosition(pos);
        }
    }
}

/**
 * 初始化图标列表界面
 */
void FunctionWidget::init_iconlist_widget()
{
    iconlistleftSpacer=new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);
    iconlistrightSpacer=new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);

    iconlistLayout=new QHBoxLayout(iconlistWid);
    iconlistLayout->setContentsMargins(0,0,0,0);
    iconlistWid->setLayout(iconlistLayout);

    char btncolor[300];
    sprintf(btncolor,"QToolButton{background: transparent;border:0px;padding-left:0px;}\
            QToolButton:hover{background-color:%s;}\
            QToolButton:pressed{background-color:%s;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);
    leftbtn=new QToolButton(iconlistWid);
    leftbtn->setFixedSize(40,30);
    QSvgRenderer* leftsvgRender = new QSvgRenderer(leftbtn);
    leftsvgRender->load(QString(":/data/img/mainviewwidget/leftarrow.svg"));
    QPixmap* leftpixmap = new QPixmap(19,19);
    leftpixmap->fill(Qt::transparent);//设置背景透明
    QPainter leftp(leftpixmap);
    leftsvgRender->render(&leftp);
    leftbtn->setIcon(QIcon(*leftpixmap));
    leftbtn->setStyleSheet(QString::fromLocal8Bit(btncolor));

    iconlisttableWid=new QTableWidget(iconlistWid);

    rightbtn=new QToolButton(iconlistWid);
    rightbtn->setFixedSize(40,30);
    QSvgRenderer* rightsvgRender = new QSvgRenderer(rightbtn);
    rightsvgRender->load(QString(":/data/img/mainviewwidget/rightarrow.svg"));
    QPixmap* rightpixmap = new QPixmap(19,19);
    rightpixmap->fill(Qt::transparent);//设置背景透明
    QPainter rightp(rightpixmap);
    rightsvgRender->render(&rightp);
    rightbtn->setIcon(QIcon(*rightpixmap));
    rightbtn->setStyleSheet(QString::fromLocal8Bit(btncolor));

    iconlistLayout->addItem(iconlistleftSpacer);
    iconlistLayout->addWidget(leftbtn);
    iconlistLayout->addWidget(iconlisttableWid);
    iconlistLayout->addWidget(rightbtn);
    iconlistLayout->addItem(iconlistrightSpacer);
    init_iconlist_table();

    QWidget* wid=iconlisttableWid->cellWidget(0,0);
    QLabel* label=qobject_cast<QLabel*>(wid);
    iconlisttableWid->selectColumn(0);
    QPixmap pixmap(iconlightlist.at(0));
    label->setPixmap(pixmap);

    connect(leftbtn, SIGNAL(clicked()), this, SLOT(leftbtn_clicked_slot()));
    connect(rightbtn, SIGNAL(clicked()), this, SLOT(rightbtn_clicked_slot()));
    connect(iconlisttableWid,SIGNAL(itemSelectionChanged()),this,SLOT(iconlistitem_selected_slot()));

}

/**
 * 初始化图标列表界面数据表格iconlisttableWid
 */
void FunctionWidget::init_iconlist_table()
{
    iconlisttableWid->setFocusPolicy(Qt::NoFocus);
//    iconlisttableWid->setFixedSize(11*40+11*20,30);
    iconlisttableWid->setFixedSize(11*40,30);
    iconlisttableWid->setColumnCount(11);
    iconlisttableWid->setRowCount(1);
    iconlisttableWid->horizontalHeader()->setFixedHeight(0);
    iconlisttableWid->verticalHeader()->setDefaultSectionSize(30);
    iconlisttableWid->setEditTriggers(QAbstractItemView::NoEditTriggers);
    iconlisttableWid->setSelectionMode(QAbstractItemView::SingleSelection);
    iconlisttableWid->setSelectionBehavior(QAbstractItemView::SelectColumns);
    iconlisttableWid->setShowGrid(false);
    iconlisttableWid->setStyleSheet("QTableWidget{border:0px;background: transparent;}"
                                    "QTableWidget::Item{background: transparent;}\
                                     QTableWidget::Item:selected{background-color:rgba(14,19,22,92%);}");
//    iconlisttableWid->setStyleSheet("QTableWidget{border:0px;background: transparent;}"
//                                    "QTableWidget::Item{background: transparent;padding-left:10px;}\
//                                     QTableWidget::Item:selected{background-color:rgba(14,19,22,92%);}");
    QStringList header;
    for(int i=0;i<11;i++)
    {
        header.append("");
//        iconlisttableWid->setColumnWidth(i,60);
        iconlisttableWid->setColumnWidth(i,40);

    }
    iconlisttableWid->setHorizontalHeaderLabels(header);
    iconlisttableWid->verticalHeader()->setHidden(true);
    iconlisttableWid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    for(int i=0;i<11;i++)
    {
        QLabel* label=new QLabel;
        label->setFixedSize(40,30);
        char labelcolor[300];
        sprintf(labelcolor,"QLabel{background: transparent;}\
                QLabel:hover{background-color:%s;}\
                QLabel:pressed{background-color:%s;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);
        label->setStyleSheet(QString::fromLocal8Bit(labelcolor));
//        QPixmap pixmap(iconlist.at(i));
        QSvgRenderer* svgRender = new QSvgRenderer(label);
        svgRender->load(iconlist.at(i));
        QPixmap* pixmap = new QPixmap(19,19);
        pixmap->fill(Qt::transparent);//设置背景透明
        QPainter p(pixmap);
        svgRender->render(&p);
        label->setPixmap(*pixmap);
        label->setAlignment(Qt::AlignCenter);
        iconlisttableWid->setCellWidget(0,i,label);
    }

}

/**
 * 向左按钮槽函数
 */
void FunctionWidget::leftbtn_clicked_slot()
{
    int col=iconlisttableWid->currentColumn();
    if(col>0)
        iconlisttableWid->selectColumn(col-1);
}

/**
 * 向右按钮槽函数
 */
void FunctionWidget::rightbtn_clicked_slot()
{
    int col=iconlisttableWid->currentColumn();
    if(col<27)
        iconlisttableWid->selectColumn(col+1);
}

/**
 * 图标列表数据项被选定槽函数
 */
void FunctionWidget::iconlistitem_selected_slot()
{
    int col=iconlisttableWid->currentColumn();
    QWidget* wid=iconlisttableWid->cellWidget(0,col);
    QLabel* label=qobject_cast<QLabel*>(wid);
//    QPixmap pixmap(iconlightlist.at(col));
    QSvgRenderer* svgRender = new QSvgRenderer(label);
    svgRender->load(iconlightlist.at(col));
    QPixmap* pixmap = new QPixmap(19,19);
    pixmap->fill(Qt::transparent);//设置背景透明
    QPainter p(pixmap);
    svgRender->render(&p);
    label->setPixmap(*pixmap);
    label->setAlignment(Qt::AlignCenter);

    for(int i=0;i<col;i++)
    {
        QWidget* wid=iconlisttableWid->cellWidget(0,i);
        QLabel* label=qobject_cast<QLabel*>(wid);
//        QPixmap pixmap(iconlist.at(i));
        QSvgRenderer* svgRender = new QSvgRenderer(label);
        svgRender->load(iconlist.at(i));
        QPixmap* pixmap = new QPixmap(19,19);
        pixmap->fill(Qt::transparent);//设置背景透明
        QPainter p(pixmap);
        svgRender->render(&p);
        label->setPixmap(*pixmap);
        label->setAlignment(Qt::AlignCenter);
    }
    for(int i=col+1;i<11;i++)
    {
        QWidget* wid=iconlisttableWid->cellWidget(0,i);
        QLabel* label=qobject_cast<QLabel*>(wid);
//        QPixmap pixmap(iconlist.at(i));
        QSvgRenderer* svgRender = new QSvgRenderer(label);
        svgRender->load(iconlist.at(i));
        QPixmap* pixmap = new QPixmap(19,19);
        pixmap->fill(Qt::transparent);//设置背景透明
        QPainter p(pixmap);
        svgRender->render(&p);
        label->setPixmap(*pixmap);
        label->setAlignment(Qt::AlignCenter);
    }

    //此处需实现将被选定的功能图标所包含的应用列表移动到applistWid界面最顶端
    int index=classificationbtnlist.indexOf(functionnamelist.at(col));
    if(index!=-1)
    {
        int pos=classificationbtnrowlist.at(index).toInt();
        scrollarea->verticalScrollBar()->setSliderPosition(pos);
    }

}
