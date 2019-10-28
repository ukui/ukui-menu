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
    delete menu;
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

    mainLayout=new QHBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    applistWid=new QWidget(this);
    applistWid->setStyleSheet("border:0px;background: transparent;");
//    applistWid->setFixedSize(this->width(),this->height());
    mainLayout->addWidget(applistWid);
    this->setLayout(mainLayout);

    init_applist_widget();

}

/**
 * 初始化应用列表界面
 */
void FunctionWidget::init_applist_widget()
{
    QHBoxLayout* layout=new QHBoxLayout(applistWid);
    layout->setContentsMargins(2,0,2,0);
    apptablewid=new QTableWidget(applistWid);
//    apptablewid->setFixedSize(applistWid->width()-4,applistWid->height());
    layout->addWidget(apptablewid);
    applistWid->setLayout(layout);
    init_app_tablewidget();
    fill_app_tablewidget();
    load_min_wid();

}

/**
 * 初始化应用列表
 */
void FunctionWidget::init_app_tablewidget()
{
    QStringList header;
    header.append("");
    apptablewid->setHorizontalHeaderLabels(header);
    apptablewid->setColumnCount(1);
    apptablewid->verticalHeader()->setDefaultSectionSize(42);
    apptablewid->verticalHeader()->setHidden(true);
    apptablewid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    apptablewid->setFocusPolicy(Qt::NoFocus);
    apptablewid->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    apptablewid->horizontalHeader()->setFixedHeight(0);
    apptablewid->setEditTriggers(QAbstractItemView::NoEditTriggers);
    apptablewid->setSelectionMode(QAbstractItemView::NoSelection);
    apptablewid->setShowGrid(false);
    apptablewid->setStyleSheet("QTableWidget{border:0px;background:transparent;}");
    apptablewid->verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
    apptablewid->verticalScrollBar()->setStyleSheet(
                "QScrollBar{width:12px;padding-top:15px;padding-bottom:15px;background-color:#283138;}"
                "QScrollBar::handle{background-color:#414e59; width:12px;}"
                "QScrollBar::handle:hover{background-color:#697883; }"
                "QScrollBar::handle:pressed{background-color:#8897a3;}"
                "QScrollBar::sub-line{background-color:#283138;height:15px;width:12px;border-image:url(:/data/img/mainviewwidget/uparrow.svg);subcontrol-position:top;}"
                "QScrollBar::sub-line:hover{background-color:#697883;}"
                "QScrollBar::sub-line:pressed{background-color:#8897a3;border-image:url(:/data/img/mainviewwidget/uparrow-pressed.svg);}"
                "QScrollBar::add-line{background-color:#283138;height:15px;width:12px;border-image:url(:/data/img/mainviewwidget/downarrow.svg);subcontrol-position:bottom;}"
                "QScrollBar::add-line:hover{background-color:#697883;}"
                "QScrollBar::add-line:pressed{background-color:#8897a3;border-image:url(:/data/img/mainviewwidget/downarrow-pressed.svg);}"
                );

}

/**
 * 填充应用列表
 */
void FunctionWidget::fill_app_tablewidget()
{
    classificationbtnlist.clear();
    classificationbtnrowlist.clear();

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
    char btncolor[300];
    sprintf(btncolor,"QPushButton{background:transparent;border:0px;color:#ffffff;font-size:14px;}\
            QPushButton:hover{background-color:%s;}\
            QPushButton:pressed{background-color:%s;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);

    QPushButton* classificationbtn=new QPushButton;
    classificationbtn->setStyleSheet(btncolor);
//    classificationbtn->setFixedSize(apptablewid->width()-14,20);
    classificationbtn->setFocusPolicy(Qt::NoFocus);
    QHBoxLayout* classificationlayout=new QHBoxLayout(classificationbtn);
    classificationlayout->setContentsMargins(15,0,0,0);
    classificationlayout->setSpacing(6);
    QFrame* line=new QFrame(classificationbtn);
    line->setFrameShape(QFrame::HLine);
//    line->setFixedSize(classificationbtn->width()-77,1);
    line->setStyleSheet("background-color:#626c6e");
    QLabel* classificationlabel=new QLabel(classificationbtn);
    classificationlabel->setFixedSize(56,20);
    classificationlabel->setAlignment(Qt::AlignCenter);
    classificationlabel->setStyleSheet("color:#ffffff;font-size:14px;");
    classificationlabel->setText(btnname);
    classificationlayout->addWidget(line);
    classificationlayout->addWidget(classificationlabel);
    classificationbtn->setLayout(classificationlayout);

    apptablewid->insertRow(apptablewid->rowCount());
//    apptablewid->setRowHeight(apptablewid->rowCount()-1,20);
    apptablewid->setCellWidget(apptablewid->rowCount()-1,0,classificationbtn);

    classificationbtnlist.append(btnname);
    classificationbtnrowlist.append(QString::number(apptablewid->rowCount()-1));

    connect(classificationbtn, SIGNAL(clicked()), this,SLOT(app_classificationbtn_clicked_slot()));

}

void FunctionWidget::insert_app_list(QStringList appnamelist)
{
    char btncolor[300];
    sprintf(btncolor,"QPushButton{background:transparent;border:0px;color:#ffffff;font-size:14px;padding-left:0px;text-align: left center;}\
            QPushButton:hover{background-color:%s;}\
            QPushButton:pressed{background-color:%s;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);

    apptablewid->insertRow(apptablewid->rowCount());
    applistnum.append(QString::number(appnamelist.count()));
    QFlowLayout* flowlayout=new QFlowLayout(0,0,0);
    QWidget* wid=new QWidget;
    wid->setLayout(flowlayout);
    apptablewid->setCellWidget(apptablewid->rowCount()-1,0,wid);
    for(int i=0;i<appnamelist.count();i++)
    {
//        apptablewid->insertRow(apptablewid->rowCount());
        QPushButton* btn=new QPushButton;
        btn->setFixedSize(330-14,42);
        btn->setStyleSheet(QString::fromLocal8Bit(btncolor));
        QHBoxLayout* layout=new QHBoxLayout(btn);
        layout->setContentsMargins(15,0,0,0);
        layout->setSpacing(15);

        QString iconstr=KylinStartMenuInterface::get_app_icon(KylinStartMenuInterface::get_desktop_path_by_app_name(appnamelist.at(i)));
        QIcon::setThemeName("ukui-icon-theme");
        QIcon icon=QIcon::fromTheme(iconstr);
        QLabel* labelicon=new QLabel(btn);
        labelicon->setFixedSize(32,32);
        QPixmap pixmapicon(icon.pixmap(QSize(32,32)));
        labelicon->setPixmap(pixmapicon);
        labelicon->setStyleSheet("QLabel{background:transparent;}");
        QLabel* labeltext=new QLabel(btn);
        labeltext->setText(appnamelist.at(i));
        labeltext->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
        layout->addWidget(labelicon);
        layout->addWidget(labeltext);
        btn->setLayout(layout);
        btn->setFocusPolicy(Qt::NoFocus);
        flowlayout->addWidget(btn);

//        apptablewid->setCellWidget(apptablewid->rowCount()-1,0,btn);

        connect(btn,SIGNAL(clicked()),this,SLOT(exec_app_name()));
        btn->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(btn,SIGNAL(customContextMenuRequested(const QPoint&)),this,
                SLOT(right_click_slot()));
    }

}

void FunctionWidget::load_min_wid()
{
    this->setGeometry(QRect(60,QApplication::desktop()->availableGeometry().height()-462,
                                 330,462));
    applistWid->setFixedSize(this->width(),this->height());
    applistWid->layout()->setContentsMargins(2,0,2,0);
    apptablewid->setFixedSize(applistWid->width()-4,applistWid->height());
    int row=0;
    while(row<apptablewid->rowCount()/2)
    {
        //分类按钮界面
        QWidget* classificationbtnwid=apptablewid->cellWidget(row*2,0);
        QPushButton* classificationbtn=qobject_cast<QPushButton*>(classificationbtnwid);
        classificationbtn->setFixedSize(apptablewid->width()-14,20);
        classificationbtn->layout()->setContentsMargins(15,0,0,0);
        classificationbtn->layout()->setSpacing(6);
        apptablewid->setRowHeight(row*2,20);
        QLayoutItem* item=classificationbtn->layout()->itemAt(0);
        QWidget* linewid=item->widget();
        QFrame* line=qobject_cast<QFrame*>(linewid);
        line->setFixedSize(classificationbtn->width()-77,1);
        //应用按钮界面
        QWidget* wid=apptablewid->cellWidget(row*2+1,0);
        QString numstr=applistnum.at(row);
        int num=numstr.toInt();
        wid->setFixedSize(apptablewid->width()-14,num*42);
        apptablewid->setRowHeight(row*2+1,num*42);
        row++;

    }
}

void FunctionWidget::load_max_wid()
{
    this->setGeometry(QRect(160,
                            70,
                            QApplication::desktop()->availableGeometry().width()-160,
                            QApplication::desktop()->availableGeometry().height()-70));

    applistWid->setFixedSize(this->width(),this->height());
    applistWid->layout()->setContentsMargins(30,0,2,0);
    apptablewid->setFixedSize(applistWid->width()-32-12,applistWid->height());
    int row=0;
    while(row<apptablewid->rowCount()/2)
    {
        //分类按钮
        QWidget* classificationbtnlayoutwid=apptablewid->cellWidget(row*2,0);
        QPushButton* classificationbtn=qobject_cast<QPushButton*>(classificationbtnlayoutwid);
        classificationbtn->setFixedSize(apptablewid->width()-32,20);
        classificationbtn->layout()->setContentsMargins(0,0,0,0);
        classificationbtn->layout()->setSpacing(12);
        apptablewid->setRowHeight(row*2,20);
        QLayoutItem* item=classificationbtn->layout()->itemAt(0);
        QWidget* linewid=item->widget();
        QFrame* line=qobject_cast<QFrame*>(linewid);
        line->setFixedSize(classificationbtn->width()-62,1);
        //应用按钮界面
        int dividend=apptablewid->width()/312;
        QWidget* wid=apptablewid->cellWidget(row*2+1,0);
        QString numstr=applistnum.at(row);
        int num=numstr.toInt();
        if(num%dividend>0)
            num=num/dividend+1;
        else num=num/dividend;
        wid->setFixedSize(apptablewid->width()-14,num*42);
        apptablewid->setRowHeight(row*2+1,num*42);
        row++;

    }

}

/**
 * 加载右键菜单
 */
void FunctionWidget::right_click_slot()
{
    char style[200];
    sprintf(style,"QPushButton{background-color:%s;border:0px;color:#ffffff;font-size:14px;padding-left:15px;text-align: left center;}", MAINVIEWBTNHOVER);

    char btnstyle[300];
    sprintf(btnstyle,"QPushButton{background:transparent;border:0px;color:#ffffff;font-size:14px;padding-left:15px;text-align: left center;}\
            QPushButton:hover{background-color:%s;}\
            QPushButton:pressed{background-color:%s;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);

    QPushButton* btn=dynamic_cast<QPushButton*>(QObject::sender());
    QLayoutItem* item=btn->layout()->itemAt(1);
    QWidget* wid=item->widget();
    QLabel* label=qobject_cast<QLabel*>(wid);
    QString appname=label->text();
    btn->setStyleSheet(style);
    menu=new RightClickMenu;
    int ret=menu->show_appbtn_menu(appname);
    if(ret==1 || ret==2)
        emit send_update_applist_signal();
    btn->setStyleSheet(btnstyle);
}

/**
 * 执行应用程序
 */
void FunctionWidget::exec_app_name()
{
    emit send_hide_mainwindow_signal();
    QPushButton* btn=dynamic_cast<QPushButton*>(QObject::sender());
    QLayoutItem* item=btn->layout()->itemAt(1);
    QWidget* wid=item->widget();
    QLabel* label=qobject_cast<QLabel*>(wid);
    QString appname=label->text();
    QString execpath=KylinStartMenuInterface::get_app_exec(KylinStartMenuInterface::get_desktop_path_by_app_name(appname));
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
void FunctionWidget::update_app_tablewidget()
{
    apptablewid->setRowCount(0);
    fill_app_tablewidget();

}

/**
 * 应用列表功能分类按钮槽函数
 */
void FunctionWidget::app_classificationbtn_clicked_slot()
{
    //加载FunctionButtonWidget界面
    functionbtnwid=new FunctionButtonWidget(this);
    connect(this,SIGNAL(send_classificationbtn_list(QStringList)),functionbtnwid,SLOT(recv_classificationbtn_list(QStringList)));
    emit send_classificationbtn_list(classificationbtnlist);
    mainLayout->removeWidget(applistWid);
    applistWid->setParent(nullptr);
    mainLayout->addWidget(functionbtnwid);

    connect(functionbtnwid, SIGNAL(send_functionbtn_signal(QString)),this,SLOT(recv_functionbtn_signal(QString)));
}

/**
 * 接收FunctionButtonWidget界面按钮信号
 */
void FunctionWidget::recv_functionbtn_signal(QString btnname)
{
    mainLayout->removeWidget(functionbtnwid);
    functionbtnwid->setParent(nullptr);
    if(functionbtnwid!=nullptr)
    {

        delete functionbtnwid;
        functionbtnwid=nullptr;
    }
    mainLayout->addWidget(applistWid);

    //此处需实现将功能为btnname的应用列表移动到applistWid界面最顶端
    int index=classificationbtnlist.indexOf(btnname);
    if(index!=-1)
    {
        int row=classificationbtnrowlist.at(index).toInt();
        apptablewid->verticalScrollBar()->setValue(row);
    }

}
