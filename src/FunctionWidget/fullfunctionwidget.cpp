#include "fullfunctionwidget.h"
#include "ui_fullfunctionwidget.h"
#include <QDesktopWidget>
#include "src/color.h"
#include <QDebug>

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

    mainLayout=new QVBoxLayout;
    applistWid=new QWidget;
    applistWid->setStyleSheet("border:0px;background:transparent;");
    applistWid->setFixedSize(this->width(),this->height()-70);
    iconlistWid=new QWidget;
    iconlistWid->setStyleSheet("border:0px;background:transparent");
    iconlistWid->setFixedSize(this->width(),70);
    mainLayout->addWidget(applistWid);
    mainLayout->addWidget(iconlistWid);
    this->setLayout(mainLayout);

    iconlist.clear();
    QString recentstr=":/data/img/mainviewwidget/gn-最新添加-灰色.svg";
    QString netstr=":/data/img/mainviewwidget/gn-网络应用-灰色.svg";
    QString socialstr=":/data/img/mainviewwidget/gn-社交沟通-灰色.svg";
    QString musicstr=":/data/img/mainviewwidget/gn-音乐欣赏-灰色.svg";
    QString videostr=":/data/img/mainviewwidget/gn-视频播放-灰色.svg";
    QString imgstr=":/data/img/mainviewwidget/gn-图形图像-灰色.svg";
    QString gamestr=":/data/img/mainviewwidget/gn-游戏娱乐-灰色.svg";
    QString officestr=":/data/img/mainviewwidget/gn-办公学习-灰色.svg";
    QString readingstr=":/data/img/mainviewwidget/gn-阅读翻译-灰色.svg";
    QString systemstr=":/data/img/mainviewwidget/gn-系统管理-灰色.svg";
    QString otherstr=":/data/img/mainviewwidget/gn-其他应用-灰色.svg";
    iconlist.append(recentstr);
    iconlist.append(netstr);
    iconlist.append(socialstr);
    iconlist.append(musicstr);
    iconlist.append(videostr);
    iconlist.append(imgstr);
    iconlist.append(gamestr);
    iconlist.append(officestr);
    iconlist.append(readingstr);
    iconlist.append(systemstr);
    iconlist.append(otherstr);

    iconlightlist.clear();
    QString recentlightstr=":/data/img/mainviewwidget/gn-最新添加.svg";
    QString netlightstr=":/data/img/mainviewwidget/gn-网络应用.svg";
    QString sociallightstr=":/data/img/mainviewwidget/gn-社交沟通.svg";
    QString musiclightstr=":/data/img/mainviewwidget/gn-音乐欣赏.svg";
    QString videolightstr=":/data/img/mainviewwidget/gn-视频播放.svg";
    QString imglightstr=":/data/img/mainviewwidget/gn-图形图像.svg";
    QString gamelightstr=":/data/img/mainviewwidget/gn-游戏娱乐.svg";
    QString officelightstr=":/data/img/mainviewwidget/gn-办公学习.svg";
    QString readinglightstr=":/data/img/mainviewwidget/gn-阅读翻译.svg";
    QString systemlightstr=":/data/img/mainviewwidget/gn-系统管理.svg";
    QString otherlightstr=":/data/img/mainviewwidget/gn-其他应用.svg";
    iconlightlist.append(recentlightstr);
    iconlightlist.append(netlightstr);
    iconlightlist.append(sociallightstr);
    iconlightlist.append(musiclightstr);
    iconlightlist.append(videolightstr);
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
    functionnamelist.append("音乐欣赏");
    functionnamelist.append("视频播放");
    functionnamelist.append("图形图像");
    functionnamelist.append("游戏娱乐");
    functionnamelist.append("办公学习");
    functionnamelist.append("阅读翻译");
    functionnamelist.append("系统管理");
    functionnamelist.append("其它应用");

    init_applist_widget();
    init_iconlist_widget();
}

/**
 * 初始化应用列表界面
 */
void FullFunctionWidget::init_applist_widget()
{
    //以下为测试项
    //此处需实现加载具体的应用列表，btn为应用分类列表右端的功能分类按钮
    QHBoxLayout* layout=new QHBoxLayout;
    btn=new QToolButton;
    btn->setFixedSize(100,20);
    layout->addWidget(btn);
    btn->setText("最近添加");
    btn->setStyleSheet("border:1px groove #ffffff;color:#ffffff;");
    applistWid->setLayout(layout);

    connect(btn, SIGNAL(clicked()), this,SLOT(app_classificationbtn_clicked_slot()));
}

/**
 * 全屏应用列表功能分类按钮槽函数
 */
void FullFunctionWidget::app_classificationbtn_clicked_slot()
{
    //加载FullFunctionButtonWidget界面
    fullfunbtnwid=new FullFunctionButtonWidget;
    mainLayout->removeWidget(applistWid);
    applistWid->setParent(NULL);
    mainLayout->removeWidget(iconlistWid);
    iconlistWid->setParent(NULL);
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
    fullfunbtnwid->setParent(NULL);
    if(fullfunbtnwid!=NULL)
    {
        delete fullfunbtnwid;
        fullfunbtnwid=NULL;
    }
    mainLayout->addWidget(applistWid);
    mainLayout->addWidget(iconlistWid);

    int col=functionnamelist.indexOf(btnname);
    iconlisttableWid->selectColumn(col);

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
        if((firstchild = mainLayout->takeAt(1)) != 0) {
            QWidget* childwid=firstchild->widget();
            if(childwid!=NULL)
            {
                mainLayout->removeWidget(childwid);
                childwid->setParent(NULL);
            }
        }
        QLayoutItem *secondchild;
        if((secondchild = mainLayout->takeAt(0)) != 0) {
            QWidget* childwid=secondchild->widget();
            if(childwid!=NULL)
            {
                mainLayout->removeWidget(childwid);
                childwid->setParent(NULL);
            }
        }

        if(fullfunbtnwid!=NULL)
        {
            delete fullfunbtnwid;
            fullfunbtnwid=NULL;
        }
        fullfunbtnwid=new FullFunctionButtonWidget;
        connect(fullfunbtnwid, SIGNAL(send_functionbtn_signal(QString)),this,SLOT(recv_functionbtn_signal(QString)));
        mainLayout->addWidget(fullfunbtnwid);
    }
    else{
        QLayoutItem *firstchild;
        if((firstchild = mainLayout->takeAt(1)) != 0) {
            QWidget* childwid=firstchild->widget();
            if(childwid!=NULL)
            {
                mainLayout->removeWidget(childwid);
                childwid->setParent(NULL);
            }
        }
        QLayoutItem *secondchild;
        if((secondchild = mainLayout->takeAt(0)) != 0) {
            QWidget* childwid=secondchild->widget();
            if(childwid!=NULL)
            {
                mainLayout->removeWidget(childwid);
                childwid->setParent(NULL);
            }
        }

        if(fullfunbtnwid!=NULL)
        {
            delete fullfunbtnwid;
            fullfunbtnwid=NULL;
        }
        mainLayout->addWidget(applistWid);
        mainLayout->addWidget(iconlistWid);

        int col=functionnamelist.indexOf(btnname);
        iconlisttableWid->selectColumn(col);
    }
}


/**
 * 初始化图标列表界面
 */
void FullFunctionWidget::init_iconlist_widget()
{
    iconlistleftSpacer=new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);
    iconlistrightSpacer=new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);

    iconlistLayout=new QHBoxLayout;
    iconlistLayout->setContentsMargins(0,0,0,0);
//    iconlistLayout->setSpacing(10);
    iconlistWid->setLayout(iconlistLayout);

    leftbtn=new QToolButton;
    leftbtn->setFixedSize(40,30);
    QPixmap leftpixmap(":/data/img/mainviewwidget/向左-箭头.svg");
    leftbtn->setIconSize(leftpixmap.size());
    leftbtn->setIcon(QIcon(leftpixmap));
    leftbtn->setStyleSheet("QToolButton{background: transparent;border:0px;padding-left:0px;}\
                               QToolButton:hover{background-color:#414e59;}\
                               QToolButton:pressed{background-color:#2a343e;}");
    iconlisttableWid=new QTableWidget;
    rightbtn=new QToolButton;
    rightbtn->setFixedSize(40,30);
    QPixmap rightpixmap(":/data/img/mainviewwidget/向右-箭头.svg");
    rightbtn->setIconSize(rightpixmap.size());
    rightbtn->setIcon(QIcon(rightpixmap));
    rightbtn->setStyleSheet("QToolButton{background: transparent;border:0px;padding-left:0px;}\
                               QToolButton:hover{background-color:#414e59;}\
                               QToolButton:pressed{background-color:#2a343e;}");

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
void FullFunctionWidget::init_iconlist_table()
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
        label->setStyleSheet("QLabel{background: transparent;}\
                             QLabel:hover{background-color:#414e59;}\
                             QLabel:pressed{background-color:#2a343e;}");
        QPixmap pixmap(iconlist.at(i));
        label->setPixmap(pixmap);
        label->setAlignment(Qt::AlignCenter);
        iconlisttableWid->setCellWidget(0,i,label);
    }

}

/**
 * 向左按钮槽函数
 */
void FullFunctionWidget::leftbtn_clicked_slot()
{
    int col=iconlisttableWid->currentColumn();
    if(col>0)
        iconlisttableWid->selectColumn(col-1);
}

/**
 * 向右按钮槽函数
 */
void FullFunctionWidget::rightbtn_clicked_slot()
{
    int col=iconlisttableWid->currentColumn();
    if(col<27)
        iconlisttableWid->selectColumn(col+1);
}

/**
 * 图标列表数据项被选定槽函数
 */
void FullFunctionWidget::iconlistitem_selected_slot()
{
    int col=iconlisttableWid->currentColumn();
    QWidget* wid=iconlisttableWid->cellWidget(0,col);
    QLabel* label=qobject_cast<QLabel*>(wid);
    QPixmap pixmap(iconlightlist.at(col));
    label->setPixmap(pixmap);

    for(int i=0;i<col;i++)
    {
        QWidget* wid=iconlisttableWid->cellWidget(0,i);
        QLabel* label=qobject_cast<QLabel*>(wid);
        QPixmap pixmap(iconlist.at(i));
        label->setPixmap(pixmap);
    }
    for(int i=col+1;i<11;i++)
    {
        QWidget* wid=iconlisttableWid->cellWidget(0,i);
        QLabel* label=qobject_cast<QLabel*>(wid);
        QPixmap pixmap(iconlist.at(i));
        label->setPixmap(pixmap);
    }

    emit send_fullfunctionwid_state(0,functionnamelist.at(col));
    //btn为测试项
    //此处需实现将被选定的功能图标所包含的应用列表移动到applistWid界面最顶端
    btn->setText(functionnamelist.at(col));

}
