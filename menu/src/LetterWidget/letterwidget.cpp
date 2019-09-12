#include "letterwidget.h"
#include "ui_letterwidget.h"
#include "src/color.h"
#include <QDebug>
#include <QHeaderView>
#include <QScrollBar>
#include "kylin-start-menu-interface.h"
#include <QPushButton>
#include <QTableWidgetItem>
#include <QFrame>
#include <QSlider>

LetterWidget::LetterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LetterWidget)
{
    ui->setupUi(this);

    init_widget();
}

LetterWidget::~LetterWidget()
{
    delete ui;
    delete menu;
}

/**
 * 主界面初始化
 */
void LetterWidget::init_widget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    char widgetcolor[100];
    sprintf(widgetcolor, "border:0px;background-color:%s;",MAINVIEWWIDGETCOLOR);
    this->setStyleSheet(QString::fromLocal8Bit(widgetcolor));
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setFixedSize(330,462);

    mainLayout=new  QHBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    applistWid=new QWidget(this);
    applistWid->setStyleSheet("border:0px;background: transparent;");
    applistWid->setFixedSize(this->width(),this->height());
    mainLayout->addWidget(applistWid);
    this->setLayout(mainLayout);

    init_applist_widget();

}

/**
 * 初始化应用列表界面
 */
void LetterWidget::init_applist_widget()
{
    QHBoxLayout* layout=new QHBoxLayout(applistWid);
    layout->setContentsMargins(2,0,2,0);
    apptablewid=new QTableWidget(applistWid);
    apptablewid->setFixedSize(applistWid->width()-4,applistWid->height());
    layout->addWidget(apptablewid);
    applistWid->setLayout(layout);
    init_app_tablewidget();
    fill_app_tablewidget();

}

/**
 * 初始化应用列表
 */
void LetterWidget::init_app_tablewidget()
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
    apptablewid->verticalScrollBar()->setStyleSheet(
                "QScrollBar{width:12px;padding-top:15px;padding-bottom:15px;background-color:#283138;}"
                "QScrollBar::handle{background-color:#414e59; width:12px;}"
                "QScrollBar::handle:hover{background-color:#697883; }"
                "QScrollBar::handle:pressed{background-color:#8897a3;}"
                "QScrollBar::sub-line{background-color:#283138;height:15px;width:12px;border-image:url(:/data/img/mainviewwidget/滑动条-向上箭头.svg);subcontrol-position:top;}"
                "QScrollBar::sub-line:hover{background-color:#697883;}"
                "QScrollBar::sub-line:pressed{background-color:#8897a3;border-image:url(:/data/img/mainviewwidget/滑动条-向上箭头-点击.svg);}"
                "QScrollBar::add-line{background-color:#283138;height:15px;width:12px;border-image:url(:/data/img/mainviewwidget/滑动条-向下箭头.svg);subcontrol-position:bottom;}"
                "QScrollBar::add-line:hover{background-color:#697883;}"
                "QScrollBar::add-line:pressed{background-color:#8897a3;border-image:url(:/data/img/mainviewwidget/滑动条-向下箭头-点击.svg);}"
                );

}

/**
 * 填充应用列表
 */
void LetterWidget::fill_app_tablewidget()
{
    QStringList applist=KylinStartMenuInterface::sort_app_name();
    apptablewid->setRowCount(applist.count());
    for(int i=0;i<applist.count();i++)
    {
        char btncolor[300];
        sprintf(btncolor,"QPushButton{background:transparent;border:0px;color:#ffffff;font-size:14px;padding-left:0px;text-align: left center;}\
                QPushButton:hover{background-color:%s;}\
                QPushButton:pressed{background-color:%s;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);
        QPushButton* btn=new QPushButton;
        btn->setFixedSize(apptablewid->width()-14,42);
        btn->setStyleSheet(QString::fromLocal8Bit(btncolor));
        QHBoxLayout* layout=new QHBoxLayout(btn);
        layout->setContentsMargins(15,0,0,0);
        layout->setSpacing(15);

        QString iconstr=KylinStartMenuInterface::get_app_icon(KylinStartMenuInterface::get_desktop_path_by_app_name(applist.at(i)));
        QIcon::setThemeName("ukui-icon-theme");
        QIcon icon=QIcon::fromTheme(iconstr);
        QLabel* labelicon=new QLabel(btn);
        labelicon->setFixedSize(32,32);
        QPixmap pixmapicon(icon.pixmap(QSize(32,32)));
        labelicon->setPixmap(pixmapicon);
        labelicon->setStyleSheet("QLabel{background:transparent;}");
        QLabel* labeltext=new QLabel(btn);
        labeltext->setText(applist.at(i));
        labeltext->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
        layout->addWidget(labelicon);
        layout->addWidget(labeltext);
        btn->setLayout(layout);
        btn->setFocusPolicy(Qt::NoFocus);

//        QWidget* btn=new QWidget;
//        btn->setStyleSheet("background:transparent;");
//        btn->setFixedSize(apptablewid->width()-14,42);
//        QHBoxLayout* layout=new QHBoxLayout;
//        layout->setContentsMargins(15,0,0,0);
//        layout->setSpacing(15);

//        KylinStartMenuInterface menu;
//        QString iconstr=menu.get_app_icon(KylinStartMenuInterface::get_desktop_path_by_app_name(applist.at(i)));
//        QIcon::setThemeName("ukui-icon-theme");
//        QIcon icon=QIcon::fromTheme(iconstr);
//        QLabel* labelicon=new QLabel;
//        labelicon->setFixedSize(32,32);
//        QPixmap pixmapicon(icon.pixmap(QSize(32,32)));
//        labelicon->setPixmap(pixmapicon);
//        labelicon->setStyleSheet("QLabel{background:transparent;}");
//        QLabel* labeltext=new QLabel;
//        labeltext->setText(applist.at(i));
//        labeltext->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
//        layout->addWidget(labelicon);
//        layout->addWidget(labeltext);
//        btn->setLayout(layout);
//        btn->setFocusPolicy(Qt::NoFocus);

//        QPushButton* btn=new QPushButton;
//        btn->setFixedSize(apptablewid->width()-14,42);
//        btn->setStyleSheet(QString::fromLocal8Bit(btncolor));
//        QString iconstr=KylinStartMenuInterface::get_app_icon(KylinStartMenuInterface::get_desktop_path_by_app_name(applist.at(i)));

//        QIcon::setThemeName("ukui-icon-theme");
//        QIcon icon=QIcon::fromTheme(iconstr);
//        btn->setIcon(icon);
//        btn->setIconSize(QSize(32,32));
//        btn->setText(applist.at(i));
//        btn->setFocusPolicy(Qt::NoFocus);

        apptablewid->setCellWidget(i,0,btn);

        connect(btn,SIGNAL(clicked()),this,SLOT(exec_app_name()));
//        connect(apptablewid,SIGNAL(cellClicked(int,int)),this,SLOT(exec_app_name(int,int)));
        btn->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(btn,SIGNAL(customContextMenuRequested(const QPoint&)),
                SLOT(right_click_slot()));
    }

    insert_letter_btn();
}

/**
 * 加载右键菜单
 */
void LetterWidget::right_click_slot()
{
    char style[200];
    sprintf(style,"QPushButton{background-color:%s;border:0px;color:#ffffff;font-size:14px;padding-left:15px;text-align: left center;}", MAINVIEWBTNHOVER);

    char btnstyle[300];
    sprintf(btnstyle,"QPushButton{background:transparent;border:0px;color:#ffffff;font-size:14px;padding-left:15px;text-align: left center;}\
            QPushButton:hover{background-color:%s;}\
            QPushButton:pressed{background-color:%s;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);

    QPushButton* btn=dynamic_cast<QPushButton*>(QObject::sender());
    btn->setStyleSheet(style);
    menu=new RightClickMenu;
    menu->show_appbtn_menu();
    btn->setStyleSheet(btnstyle);
}

/**
 * 插入字母按钮
 */
void LetterWidget::insert_letter_btn()
{
    letterbtnlist.clear();
    letterbtnrowlist.clear();
    char btnstyle[300];
    sprintf(btnstyle,"QPushButton{background:transparent;border:0px;color:#ffffff;font-size:14px;}\
            QPushButton:hover{background-color:%s;}\
            QPushButton:pressed{background-color:%s;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);

    int index=0;
    int row;
    while(index<apptablewid->rowCount())
    {
        QWidget* wid=apptablewid->cellWidget(index,0);
        QPushButton* btn=qobject_cast<QPushButton*>(wid);
        QLayoutItem* item=btn->layout()->itemAt(1);
        QWidget* labelWid=item->widget();
        QLabel* label=qobject_cast<QLabel*>(labelWid);
        QString indexstr=label->text();
        QString indexpinyin=KylinStartMenuInterface::get_app_name_pinyin(indexstr);
        QChar indexch=indexpinyin.at(0);

        if(indexch<65 || indexch>90)
        {
            indexch='&';

        }
        letterbtnlist.append(QString(indexch));
        letterbtnrowlist.append(QString::number(index));

        QPushButton* letterbtn=new QPushButton;
        letterbtn->setStyleSheet(btnstyle);
        letterbtn->setFixedSize(apptablewid->width()-14,20);
        letterbtn->setFocusPolicy(Qt::NoFocus);
        QHBoxLayout* letterlayout=new QHBoxLayout(letterbtn);
        letterlayout->setContentsMargins(15,0,0,0);
        letterlayout->setSpacing(6);
        QFrame* line=new QFrame(letterbtn);
        line->setFrameShape(QFrame::HLine);
        line->setFixedSize(letterbtn->width()-41,1);
        line->setStyleSheet("background-color:#626c6e");
        QLabel* letterlabel=new QLabel(letterbtn);
        letterlabel->setFixedSize(20,20);
        letterlabel->setAlignment(Qt::AlignCenter);
        letterlabel->setStyleSheet("color:#ffffff;font-size:14px;");
        letterlabel->setText(QString(indexch));
        letterlayout->addWidget(line);
        letterlayout->addWidget(letterlabel);
        letterbtn->setLayout(letterlayout);

        apptablewid->insertRow(index);
        apptablewid->setRowHeight(index,20);
        apptablewid->setCellWidget(index,0,letterbtn);
        connect(letterbtn, SIGNAL(clicked()), this,SLOT(app_classificationbtn_clicked_slot()));


        if(indexch>=65 && indexch<=90)
        {
            for(row=index+1;row<apptablewid->rowCount();row++)
            {
                QWidget* wid=apptablewid->cellWidget(row,0);
                QPushButton* btn=qobject_cast<QPushButton*>(wid);
//                QString appnamestr=btn->text();
                QLayoutItem* item=btn->layout()->itemAt(1);
                QWidget* labelwid=item->widget();
                QLabel* label=qobject_cast<QLabel*>(labelwid);
                QString appnamestr=label->text();
                QString appnamepinyin=KylinStartMenuInterface::get_app_name_pinyin(appnamestr);
                QChar appnamech=appnamepinyin.at(0);
                if(indexch!=appnamech)
                {
                    index=row;
                    break;
                }

            }
        }
        else {
            break;
        }
        if(row==apptablewid->rowCount())
            break;
    }
}

/**
 * 执行应用程序
 */
void LetterWidget::exec_app_name()
{

        QPushButton* btn=dynamic_cast<QPushButton*>(QObject::sender());
//        QModelIndex index=apptablewid->indexAt(btn->pos());
//    QWidget* btn=apptablewid->cellWidget(row,col);
        QLayoutItem* child=btn->layout()->itemAt(1);
        QWidget* wid=child->widget();
        QLabel* appnamelabel=qobject_cast<QLabel*>(wid);
    //    qDebug()<<appname->text();
//    QPushButton* btn=dynamic_cast<QPushButton*>(QObject::sender());
    QString appname=appnamelabel->text();
//    qDebug()<<appname;
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
 * 应用列表字母分类按钮槽函数
 */
void LetterWidget::app_classificationbtn_clicked_slot()
{
    //加载LetterBUttonWidget界面
    letterbtnwid=new LetterButtonWidget(this);
    mainLayout->removeWidget(applistWid);
    applistWid->setParent(nullptr);
    mainLayout->addWidget(letterbtnwid);
    emit send_letterwid_state(1,"");

    connect(letterbtnwid, SIGNAL(send_letterbtn_signal(QString)),this,SLOT(recv_letterbtn_signal(QString)));
}

/**
 * 接收LetterButtonWidget界面按钮信号
 */
void LetterWidget::recv_letterbtn_signal(QString btnname)
{
    mainLayout->removeWidget(letterbtnwid);
    letterbtnwid->setParent(nullptr);
    if(letterbtnwid!=nullptr)
    {
        delete letterbtnwid;
        letterbtnwid=nullptr;
    }
    mainLayout->addWidget(applistWid);

    //此处需实现将字母为btnname的应用列表移动到applistWid界面最顶端
    int num=letterbtnlist.indexOf(QString(QChar(btnname.at(0))));
    if(num!=-1)
    {
        int row=letterbtnrowlist.at(num).toInt();
        apptablewid->verticalScrollBar()->setValue(row);

    }
    emit send_letterwid_state(0,btnname);
}

/**
 * 设置字母分类界面状态
 */
void LetterWidget::set_letterwidge_state(int state, QString btnname)
{
    if(state==1)
    {
        QLayoutItem *child;
        if((child = mainLayout->itemAt(0)) != nullptr) {
            QWidget* childwid=child->widget();
            if(childwid!=nullptr)
            {
                mainLayout->removeWidget(childwid);
                childwid->setParent(nullptr);
            }
        }
        if(letterbtnwid!=nullptr)
        {
            delete letterbtnwid;
            letterbtnwid=nullptr;
        }
        letterbtnwid=new LetterButtonWidget(this);
        connect(letterbtnwid, SIGNAL(send_letterbtn_signal(QString)),this,SLOT(recv_letterbtn_signal(QString)));
        mainLayout->addWidget(letterbtnwid);
    }
    else{
        QLayoutItem *child;
        if((child = mainLayout->itemAt(0)) != nullptr) {
            QWidget* childwid=child->widget();
            if(childwid!=nullptr)
            {
                mainLayout->removeWidget(childwid);
                childwid->setParent(nullptr);
            }
        }
        if(letterbtnwid!=nullptr)
        {
            delete letterbtnwid;
            letterbtnwid=nullptr;
        }
        mainLayout->addWidget(applistWid);

        //此处需实现将字母为btnname的应用列表移动到applistWid界面最顶端
        int num=letterbtnlist.indexOf(QString(QChar(btnname.at(0))));
        if(num!=-1)
        {
            int row=letterbtnrowlist.at(num).toInt();
            apptablewid->verticalScrollBar()->setValue(row);
        }
    }

}
