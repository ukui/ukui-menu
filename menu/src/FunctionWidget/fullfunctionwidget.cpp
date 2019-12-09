#include "fullfunctionwidget.h"
#include "ui_fullfunctionwidget.h"
#include <QDesktopWidget>
#include <QHeaderView>
#include "src/color.h"
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
    char widgetcolor[100];
    sprintf(widgetcolor, "border:0px;background-color:%s;",MAINVIEWWIDGETCOLOR);
    this->setStyleSheet(QString::fromLocal8Bit(widgetcolor));
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setFixedSize(QApplication::desktop()->availableGeometry().width()-160,
                       QApplication::desktop()->availableGeometry().height()-70);

    mainLayout=new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    applistWid=new QWidget(this);
    applistWid->setStyleSheet("border:0px;background:transparent;");
    applistWid->setFixedSize(this->width(),this->height()-70);
    iconlistWid=new QWidget(this);
    iconlistWid->setStyleSheet("border:0px;background:transparent");
    iconlistWid->setFixedSize(this->width(),70);
    mainLayout->addWidget(applistWid);
    mainLayout->addWidget(iconlistWid);
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
}

/**
 * 初始化应用列表界面
 */
void FullFunctionWidget::init_applist_widget()
{
    QHBoxLayout* layout=new QHBoxLayout(applistWid);
    layout->setContentsMargins(30,0,2,0);
    applistWid->setLayout(layout);

    scrollarea=new ScrollArea;
    scrollareawid=new QWidget;
    scrollarea->setFixedSize(applistWid->width()-32-12,applistWid->height());
    scrollarea->setWidget(scrollareawid);
    scrollarea->setWidgetResizable(true);
    scrollareawidLayout=new QVBoxLayout;
    scrollareawidLayout->setContentsMargins(0,0,0,0);
    scrollareawidLayout->setSpacing(0);
    scrollareawid->setLayout(scrollareawidLayout);
    layout->addWidget(scrollarea);
    fill_app_tablewidget();
}

/**
 * 填充应用列表
 */
void FullFunctionWidget::fill_app_tablewidget()
{
    classificationbtnlist.clear();
    classificationbtnrowlist.clear();

    QVector<QStringList> vector=pUkuiMenuInterface->get_functional_classification();

    QStringList recentlist=vector.at(0);
    if(!recentlist.isEmpty())
    {
        insert_classification_btn("最近添加");
        insert_app_list(recentlist);
    }

    QStringList netlist=vector.at(1);
    if(!netlist.isEmpty())
    {
        insert_classification_btn("网络应用");
        insert_app_list(netlist);
    }
    QStringList sociallist=vector.at(2);
    if(!sociallist.isEmpty())
    {
        insert_classification_btn("社交沟通");
        insert_app_list(sociallist);
    }
    QStringList avlist=vector.at(3);
    if(!avlist.isEmpty())
    {
        insert_classification_btn("影音播放");
        insert_app_list(avlist);
    }
    QStringList developlist=vector.at(4);
    if(!developlist.isEmpty())
    {
        insert_classification_btn("开发编程");
        insert_app_list(developlist);
    }
    QStringList graphicslist=vector.at(5);
    if(!graphicslist.isEmpty())
    {
        insert_classification_btn("图形图像");
        insert_app_list(graphicslist);
    }
    QStringList gamelist=vector.at(6);
    if(!gamelist.isEmpty())
    {
        insert_classification_btn("游戏娱乐");
        insert_app_list(gamelist);
    }
    QStringList officelist=vector.at(7);
    if(!officelist.isEmpty())
    {
        insert_classification_btn("办公学习");
        insert_app_list(officelist);
    }
    QStringList educationlist=vector.at(8);
    if(!educationlist.isEmpty())
    {
        insert_classification_btn("阅读翻译");

        insert_app_list(educationlist);
    }

    QStringList systemadminlist=vector.at(9);
    if(!systemadminlist.isEmpty())
    {
        insert_classification_btn("系统管理");
        insert_app_list(systemadminlist);
    }
    QStringList otherlist=vector.at(10);
    if(!otherlist.isEmpty())
    {
        insert_classification_btn("其他应用");
        insert_app_list(otherlist);
    }

//    scrollarea->widget()->adjustSize();
    resize_scrollarea_controls();
}

void FullFunctionWidget::insert_classification_btn(QString btnname)
{

    PushButton* classificationbtn=new PushButton(btnname,0,2);
    classificationbtn->setFixedSize(scrollarea->width()-32,20);
    scrollareawidLayout->addWidget(classificationbtn);
    classificationbtnlist.append(btnname);
//    classificationbtnrowlist.append(QString::number(pos));
    connect(classificationbtn, SIGNAL(clicked()), this,SLOT(app_classificationbtn_clicked_slot()));

}

void FullFunctionWidget::insert_app_list(QStringList appnamelist)
{
    FullListView* listview=new FullListView(this,applistWid->width(),100,2);
    scrollareawidLayout->addWidget(listview);
    data.clear();
    for(int i=0;i<appnamelist.count();i++)
    {

        QString desktopfp=pUkuiMenuInterface->get_desktop_path_by_app_name(appnamelist.at(i));
        data.append(desktopfp);

//        data.append(appnamelist.at(i));

    }

    FullItemDelegate* itemdelegate=new FullItemDelegate(this,2);
    listview->setItemDelegate(itemdelegate);
    listview->addData(data);
//    listview->adjustSize();
//    int dividend=scrollarea->width()/120;
//    int rowcount=0;
//    if(appnamelist.count()%dividend>0)
//        rowcount=appnamelist.count()/dividend+1;
//    else rowcount=appnamelist.count()/dividend;
//    QModelIndex index=listview->model()->index(rowcount-1,0);
//    QRect rect=listview->visualRect(index);
//    listview->setFixedSize(scrollarea->width()-32,rect.y()+rect.height()+20*rowcount);
    connect(listview,SIGNAL(sendItemClickedSignal(QString)),this,SLOT(exec_app_name(QString)));
    connect(listview,SIGNAL(sendFixedOrUnfixedSignal()),this,SIGNAL(send_update_applist_signal()));
//    pos+=(20+listview->height());
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
void FullFunctionWidget::update_app_listview(QString desktopfn, QString appname, int arg)
{
    //放到最近添加模块
    QLayoutItem* item=scrollareawidLayout->itemAt(1);
    QWidget* wid=item->widget();
    FullListView* listview=qobject_cast<FullListView*>(wid);
    if(arg==0)//添加到最近模块
    {
        listview->insertRow(QString("/usr/share/applications/"+desktopfn));

    }
    else {//从最近模块删除
        listview->removeRow(QString("/usr/share/applications/"+desktopfn));
    }

    if(arg==0)//有新软件安装
    {
//        QString appname=pUkuiMenuInterface->get_app_name(QString("/usr/share/applications/"+desktopfn));
        QStringList networkapplist=pUkuiMenuInterface->get_network_app_list();
        QStringList socilaapplist=pUkuiMenuInterface->get_social_app_list();
        QStringList avapplist=pUkuiMenuInterface->get_av_app_list();
        QStringList devapplist=pUkuiMenuInterface->get_develop_app_list();
        QStringList graphicsapplist=pUkuiMenuInterface->get_graphics_app_list();
        QStringList gameapplist=pUkuiMenuInterface->get_game_app_list();
        QStringList officeapplist=pUkuiMenuInterface->get_office_app_list();
        QStringList educationapplist=pUkuiMenuInterface->get_education_app_list();
        QStringList systemadminlist=pUkuiMenuInterface->get_systemadmin_app_list();
        QStringList otherapplist=pUkuiMenuInterface->get_other_app_list();
        if(networkapplist.contains(appname))
        {
            int num=networkapplist.count();
            add_app("网络应用",desktopfn,num);
        }
        if(socilaapplist.contains(appname))
        {
            int num=socilaapplist.count();
            add_app("社交沟通",desktopfn,num);
        }
        if(avapplist.contains(appname))
        {
            int num=avapplist.count();
            add_app("影音播放",desktopfn,num);
        }
        if(devapplist.contains(appname))
        {
            int num=devapplist.count();
            add_app("开发编程",desktopfn,num);
        }
        if(graphicsapplist.contains(appname))
        {
            int num=graphicsapplist.count();
            add_app("图形图像",desktopfn,num);
        }
        if(gameapplist.contains(appname))
        {
            int num=gameapplist.count();
            add_app("游戏娱乐",desktopfn,num);
        }
        if(officeapplist.contains(appname))
        {
            int num=officeapplist.count();
            add_app("办公学习",desktopfn,num);
        }
        if(educationapplist.contains(appname))
        {
            int num=educationapplist.count();
            add_app("阅读翻译",desktopfn,num);
        }
        if(systemadminlist.contains(appname))
        {
            int num=systemadminlist.count();
            add_app("系统管理",desktopfn,num);
        }
        if(otherapplist.contains(appname))
        {
            int num=otherapplist.count();
            add_app("其他应用",desktopfn,num);
        }
    }
    else{//有软件被卸载
        int pos=0;
        while(pos<scrollareawidLayout->count()/2)
        {
            QLayoutItem* item=scrollareawidLayout->itemAt(2*pos+1);
            QWidget* wid=item->widget();
            FullListView* listview=qobject_cast<FullListView*>(wid);
            listview->removeRow("/usr/share/applications/"+desktopfn);

            if(listview->model()->rowCount()==0)//该分类只有一个应用，卸载后，listview列表个数为零
            {
                QLayoutItem* classifybtnitem=scrollareawidLayout->itemAt(2*pos);
                QWidget* classifybtnwid=classifybtnitem->widget();
                QPushButton* classifybtn=qobject_cast<QPushButton*>(classifybtnwid);
                QLayoutItem* labelitem=classifybtn->layout()->itemAt(1);
                QWidget* labelwid=labelitem->widget();
                QLabel* label=qobject_cast<QLabel*>(labelwid);
                QString classify=label->text();
                for(int i=1;i>=0;i--)//删除listview和对应的分类按钮
                {
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

    resize_scrollarea_controls();
}

void FullFunctionWidget::add_app(QString classify, QString desktopfn, int num)
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
        insert_app_listview(desktopfn,2*(pos+1)+1);
    }
    else {
        QLayoutItem* item=scrollareawidLayout->itemAt(2*pos+1);
        QWidget* wid=item->widget();
        FullListView* listview=qobject_cast<FullListView*>(wid);
        listview->insertRow("/usr/share/applications/"+desktopfn);
    }
}

void FullFunctionWidget::insert_classification_btn(QString classify, int pos)
{
    PushButton* classificationbtn=new PushButton(classify,0,2);
    classificationbtnlist.insert(pos/2,classify);
    scrollareawidLayout->insertWidget(pos,classificationbtn);
    scrollareawidLayout->update();

    connect(classificationbtn, SIGNAL(clicked()), this,SLOT(app_classificationbtn_clicked_slot()));
}

void FullFunctionWidget::insert_app_listview(QString desktopfn, int pos)
{
    FullListView* listview=new FullListView(this,applistWid->width(),100,1);
    data.clear();
    data.append("/usr/share/applications/"+desktopfn);
    FullItemDelegate* itemdelegate=new FullItemDelegate(this,1);
    listview->setItemDelegate(itemdelegate);
    listview->addData(data);
    listview->adjustSize();
    scrollareawidLayout->insertWidget(pos,listview);
    connect(listview,SIGNAL(sendItemClickedSignal(QString)),this,SLOT(exec_app_name(QString)));
    connect(listview,SIGNAL(sendFixedOrUnfixedSignal()),this,SIGNAL(send_update_applist_signal()));
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
        QLayoutItem* classificationbtnwidItem=scrollareawidLayout->itemAt(row*2);
        QWidget* classificationbtnwid=classificationbtnwidItem->widget();
        QPushButton* classificationbtn=qobject_cast<QPushButton*>(classificationbtnwid);
        classificationbtn->setFixedSize(scrollarea->width()-32,20);
        //应用界面
        QLayoutItem* widItem=scrollareawidLayout->itemAt(row*2+1);
        QWidget* wid=widItem->widget();
        FullListView* listview=qobject_cast<FullListView*>(wid);
        listview->adjustSize();
        int dividend=scrollarea->width()/120;
        int rowcount=0;
//        qDebug()<<dividend<<listview->model()->rowCount();
        if(listview->model()->rowCount()%dividend>0)
        {
            rowcount=listview->model()->rowCount()/dividend+1;
        }
        else
        {
            rowcount=listview->model()->rowCount()/dividend;

        }
        QModelIndex index=listview->model()->index(rowcount-1,0);
        QRect rect=listview->visualRect(index);
        listview->setFixedSize(scrollarea->width()-32,rect.y()+rect.height()+20);
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
 * 全屏应用列表功能分类按钮槽函数
 */
void FullFunctionWidget::app_classificationbtn_clicked_slot()
{
    //加载FullFunctionButtonWidget界面
    fullfunbtnwid=new FullFunctionButtonWidget(this);
    connect(this,SIGNAL(send_classificationbtn_list(QStringList)),fullfunbtnwid,SLOT(recv_classificationbtn_list(QStringList)));
    emit send_classificationbtn_list(classificationbtnlist);
    mainLayout->removeWidget(applistWid);
    applistWid->setParent(nullptr);
    mainLayout->removeWidget(iconlistWid);
    iconlistWid->setParent(nullptr);
    mainLayout->addWidget(fullfunbtnwid);
    emit send_fullfunctionwid_state(1,"");

    connect(fullfunbtnwid, SIGNAL(send_functionbtn_signal(QString)),this,SLOT(recv_functionbtn_signal(QString)));
}

/**
 * 接收FullFunctionButtonWidget界面按钮信号
 */
void FullFunctionWidget::recv_functionbtn_signal(QString btnname)
{
    mainLayout->removeWidget(fullfunbtnwid);
    fullfunbtnwid->setParent(nullptr);
    if(fullfunbtnwid!=nullptr)
    {
        delete fullfunbtnwid;
        fullfunbtnwid=nullptr;
    }
    mainLayout->addWidget(applistWid);
    mainLayout->addWidget(iconlistWid);

    int col=functionnamelist.indexOf(btnname);
    QLayoutItem* item=iconlistscrollarea->widget()->layout()->itemAt(col);
    QWidget* wid=item->widget();
    QToolButton* btn=qobject_cast<QToolButton*>(wid);
    btn->click();

    emit send_fullfunctionwid_state(0,btnname);
}

/**
 * 设置全屏功能分类界面状态
 */
void FullFunctionWidget::set_fullfunctionwidge_state(int state, QString btnname)
{
    if(state==1)
    {
        QLayoutItem *firstchild;
        if((firstchild = mainLayout->itemAt(1)) != nullptr) {
            QWidget* childwid=firstchild->widget();
            if(childwid!=nullptr)
            {
                mainLayout->removeWidget(childwid);
                childwid->setParent(nullptr);
            }
        }
        QLayoutItem *secondchild;
        if((secondchild = mainLayout->itemAt(0)) != nullptr) {
            QWidget* childwid=secondchild->widget();
            if(childwid!=nullptr)
            {
                mainLayout->removeWidget(childwid);
                childwid->setParent(nullptr);
            }
        }

        if(fullfunbtnwid!=nullptr)
        {
            delete fullfunbtnwid;
            fullfunbtnwid=nullptr;
        }
        fullfunbtnwid=new FullFunctionButtonWidget(this);
        connect(this,SIGNAL(send_classificationbtn_list(QStringList)),fullfunbtnwid,SLOT(recv_classificationbtn_list(QStringList)));
        emit send_classificationbtn_list(classificationbtnlist);
        connect(fullfunbtnwid, SIGNAL(send_functionbtn_signal(QString)),this,SLOT(recv_functionbtn_signal(QString)));
        mainLayout->addWidget(fullfunbtnwid);
    }
    else{
        QLayoutItem *firstchild;
        if((firstchild = mainLayout->itemAt(1)) != nullptr) {
            QWidget* childwid=firstchild->widget();
            if(childwid!=nullptr)
            {
                mainLayout->removeWidget(childwid);
                childwid->setParent(nullptr);
            }
        }
        QLayoutItem *secondchild;
        if((secondchild = mainLayout->itemAt(0)) != nullptr) {
            QWidget* childwid=secondchild->widget();
            if(childwid!=nullptr)
            {
                mainLayout->removeWidget(childwid);
                childwid->setParent(nullptr);
            }
        }

        if(fullfunbtnwid!=nullptr)
        {
            delete fullfunbtnwid;
            fullfunbtnwid=nullptr;
        }
        mainLayout->addWidget(applistWid);
        mainLayout->addWidget(iconlistWid);

        int col=functionnamelist.indexOf(btnname);
        QLayoutItem* item=iconlistscrollarea->widget()->layout()->itemAt(col);
        QWidget* wid=item->widget();
        QToolButton* btn=qobject_cast<QToolButton*>(wid);
        btn->click();
    }
}


/**
 * 初始化图标列表界面
 */
void FullFunctionWidget::init_iconlist_widget()
{
    iconlistleftSpacer=new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);
    iconlistrightSpacer=new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);

    iconlistLayout=new QHBoxLayout(iconlistWid);
    iconlistLayout->setContentsMargins(0,0,0,0);
    iconlistLayout->setSpacing(20);
    iconlistWid->setLayout(iconlistLayout);

    leftbtn=new ToolButton(40,30,":/data/img/mainviewwidget/leftarrow.svg",":/data/img/mainviewwidget/leftarrow-hover.svg",
                           MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED,2);
    iconlistscrollarea=new ClassifyScrollArea();
    iconlistscrollarea->setFixedSize(11*40+10*20,30);
    iconlistscrollareaWid=new QWidget;
    iconlistscrollareawidLayout=new QHBoxLayout;
    iconlistscrollareawidLayout->setContentsMargins(0,0,0,0);
    iconlistscrollareawidLayout->setSpacing(20);
    iconlistscrollareaWid->setLayout(iconlistscrollareawidLayout);
    iconlistscrollarea->setWidget(iconlistscrollareaWid);

    rightbtn=new ToolButton(40,30,":/data/img/mainviewwidget/rightarrow.svg",":/data/img/mainviewwidget/rightarrow-hover.svg",
                            MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED,2);
    rightbtn->setFixedSize(40,30);

    iconlistLayout->addItem(iconlistleftSpacer);
    iconlistLayout->addWidget(leftbtn);
    iconlistLayout->addWidget(iconlistscrollarea);
    iconlistLayout->addWidget(rightbtn);
    iconlistLayout->addItem(iconlistrightSpacer);
    init_iconlist_scrollarea();

    QLayoutItem* item=iconlistscrollarea->widget()->layout()->itemAt(0);
    QWidget* wid=item->widget();
    QToolButton* btn=qobject_cast<QToolButton*>(wid);
    btn->setChecked(true);

    connect(leftbtn, SIGNAL(clicked()), this, SLOT(leftbtn_clicked_slot()));
    connect(rightbtn, SIGNAL(clicked()), this, SLOT(rightbtn_clicked_slot()));
}

/**
 * 初始化图标列表界面数据表格iconlisttableWid
 */
void FullFunctionWidget::init_iconlist_scrollarea()
{
    for(int i=0;i<11;i++)
    {
//        ClassifyButton* iconbtn=new ClassifyButton("",1,iconlist.at(i));
        ToolButton* iconbtn=new ToolButton(40,30,iconlist.at(i),iconlightlist.at(i),MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED,2);
        iconlistscrollareawidLayout->addWidget(iconbtn);
        iconbtn->setCheckable(true);
        iconbtn->setChecked(false);
        connect(iconbtn,SIGNAL(clicked()),this,SLOT(iconbtn_clicked_slot()));
        connect(iconbtn,SIGNAL(toggled(bool)),this,SLOT(iconbtn_checked_slot(bool)));
    }

    iconlistscrollarea->widget()->adjustSize();
}

/**
 * 向左按钮槽函数
 */
void FullFunctionWidget::leftbtn_clicked_slot()
{
    if(btnPos>0)
    {
        btnPos--;
        QLayoutItem* item=iconlistscrollarea->widget()->layout()->itemAt(btnPos);
        QWidget* wid=item->widget();
        QToolButton* btn=qobject_cast<QToolButton*>(wid);
        btn->click();
        if((btn->pos().x()+iconlistscrollarea->widget()->pos().x()) <= 0)
        {   if(btnPos>0)
            {
                int val=iconlistscrollarea->horizontalScrollBar()->value();
                iconlistscrollarea->horizontalScrollBar()->setValue(val-40);
            }
            else{
//                qDebug()<<iconlistscrollarea->horizontalScrollBar()->minimum();
                iconlistscrollarea->horizontalScrollBar()->setValue(iconlistscrollarea->horizontalScrollBar()->minimum());
            }

        }

    }
}

/**
 * 向右按钮槽函数
 */
void FullFunctionWidget::rightbtn_clicked_slot()
{
    if(btnPos<iconlistscrollarea->widget()->layout()->count()-1)
    {
        btnPos++;
        QLayoutItem* item=iconlistscrollarea->widget()->layout()->itemAt(btnPos);
        QWidget* wid=item->widget();
        QToolButton* btn=qobject_cast<QToolButton*>(wid);
        btn->click();
//        qDebug()<<"---"<<btn->pos().x();
//        qDebug()<<"---111---"<<letterlistscrollarea->widget()->pos().x();

        if((btn->pos().x()+iconlistscrollarea->widget()->pos().x()) >= iconlistscrollarea->width())
        {   if(btnPos<iconlistscrollarea->widget()->layout()->count()-1)
            {
                int val=iconlistscrollarea->horizontalScrollBar()->value();
                iconlistscrollarea->horizontalScrollBar()->setValue(val+40);
            }
            else{
//                qDebug()<<scrollarea->horizontalScrollBar()->maximum();
                iconlistscrollarea->horizontalScrollBar()->setValue(iconlistscrollarea->horizontalScrollBar()->maximum());
            }

        }
    }
}

/**
 * 图标列表数据项被点击槽函数
 */
void FullFunctionWidget::iconbtn_clicked_slot()
{
    QLayoutItem* item=iconlistscrollarea->widget()->layout()->itemAt(beforebtnPos);
    QWidget* wid=item->widget();
    QToolButton* beforebtn=qobject_cast<QToolButton*>(wid);
    beforebtn->setChecked(false);

    QToolButton* btn=dynamic_cast<QToolButton*>(QObject::sender());
    btnPos=iconlistscrollarea->widget()->layout()->indexOf(btn);
    beforebtnPos=btnPos;
    //此处需实现将被选定的功能图标所包含的应用列表移动到applistWid界面最顶端
    int num=classificationbtnlist.indexOf(functionnamelist.at(btnPos));
    btn->setChecked(true);
    if(num!=-1)
    {
        int pos=classificationbtnrowlist.at(num).toInt();
        scrollarea->verticalScrollBar()->setSliderPosition(pos);
    }

    emit send_fullfunctionwid_state(0,functionnamelist.at(btnPos));

}

void FullFunctionWidget::iconbtn_checked_slot(bool check)
{
    QToolButton* btn=dynamic_cast<QToolButton*>(QObject::sender());
    int pos=iconlistscrollarea->widget()->layout()->indexOf(btn);
    QSvgRenderer* svgRender = new QSvgRenderer;
    if(check)
        svgRender->load(iconlightlist.at(pos));
    else
        svgRender->load(iconlist.at(pos));
    QPixmap* pixmap = new QPixmap(19,19);
    pixmap->fill(Qt::transparent);
    QPainter p(pixmap);
    svgRender->render(&p);
    btn->setIcon(*pixmap);
}
