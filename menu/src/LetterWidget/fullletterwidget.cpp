#include "fullletterwidget.h"
#include "ui_fullletterwidget.h"
#include <QDesktopWidget>
#include <QHeaderView>
#include "src/color.h"
#include <QScrollBar>
#include "kylin-start-menu-interface.h"
#include <QPushButton>
#include <QTableWidgetItem>
#include <QFrame>
#include <QSvgRenderer>
#include <QPainter>
#include <QDebug>

FullLetterWidget::FullLetterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FullLetterWidget)
{
    ui->setupUi(this);

    init_widget();
}

FullLetterWidget::~FullLetterWidget()
{
    delete ui;
    delete menu;
}

/**
 * 主界面初始化
 */
void FullLetterWidget::init_widget()
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
    applistWid=new QWidget(this);
    applistWid->setStyleSheet("border:0px;background: transparent;");
    applistWid->setFixedSize(this->width(),this->height()-70);
    letterlistWid=new QWidget(this);
    letterlistWid->setStyleSheet("border:0px;background: transparent;");
    letterlistWid->setFixedSize(this->width(),70);
    mainLayout->addWidget(applistWid);
    mainLayout->addWidget(letterlistWid);
    this->setLayout(mainLayout);

    init_applist_widget();
    init_letterlist_widget();
}

/**
 * 初始化应用列表界面
 */
void FullLetterWidget::init_applist_widget()
{
    QHBoxLayout* layout=new QHBoxLayout(applistWid);
    layout->setContentsMargins(30,0,2,0);

    apptablewid=new QTableWidget(applistWid);
    apptablewid->setFixedSize(applistWid->width()-32-12,applistWid->height());
    layout->addWidget(apptablewid);
    applistWid->setLayout(layout);
    init_app_tablewidget();
    fill_app_tablewidget();

}

/**
 * 初始化应用列表
 */
void FullLetterWidget::init_app_tablewidget()
{

    apptablewid->setColumnCount(4);
    QStringList header;
    for(int i=0;i<4;i++)
    {
        apptablewid->setColumnWidth(i,(apptablewid->width()-46)/4);
        header.append("");
    }
    apptablewid->setHorizontalHeaderLabels(header);

    apptablewid->verticalHeader()->setDefaultSectionSize(50);
    apptablewid->verticalHeader()->setHidden(true);
    apptablewid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    apptablewid->setFocusPolicy(Qt::NoFocus);

    apptablewid->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
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
void FullLetterWidget::fill_app_tablewidget()
{
    char pushbtncolor[300];
    sprintf(pushbtncolor,"QPushButton{background:transparent;border:0px;color:#ffffff;font-size:14px;padding-left:0px;text-align: left center;}\
            QPushButton:hover{background-color:%s;}\
            QPushButton:pressed{background-color:%s;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);

    QStringList applist=KylinStartMenuInterface::sort_app_name();
    int index=0;
    int row=0;
    while(index<applist.count())
    {
        apptablewid->insertRow(apptablewid->rowCount());
        apptablewid->insertRow(apptablewid->rowCount());

        QString indexstr=applist.at(index);
        QString indexpinyin=KylinStartMenuInterface::get_app_name_pinyin(indexstr);
        QChar indexch=indexpinyin.at(0);
        if(indexch<65 || indexch>90)
        {
            indexch='&';
        }
        letterbtnlist.append(QString(indexch));
        letterbtnrowlist.append(QString::number(apptablewid->rowCount()-2));

        apptablewid->setSpan(apptablewid->rowCount()-2,0,1,4);
        QPushButton* letterbtn=new QPushButton;
        letterbtn->setFixedSize(apptablewid->width()-32,20);
        letterbtn->setStyleSheet(pushbtncolor);
        letterbtn->setFocusPolicy(Qt::NoFocus);
        QHBoxLayout* letterlayout=new QHBoxLayout(letterbtn);
        letterlayout->setContentsMargins(0,0,0,0);
        letterlayout->setSpacing(12);
        QFrame* line=new QFrame(letterbtn);
        line->setFrameShape(QFrame::HLine);
        line->setFixedSize(letterbtn->width()-26,1);
        line->setStyleSheet("background-color:#626c6e");
        QLabel* letterlabel=new QLabel(letterbtn);
        letterlabel->setFixedSize(20,20);
        letterlabel->setAlignment(Qt::AlignCenter);
        letterlabel->setStyleSheet("color:#ffffff;font-size:14px;");
        letterlabel->setText(QString(indexch));
        letterlayout->addWidget(line);
        letterlayout->addWidget(letterlabel);
        letterbtn->setLayout(letterlayout);

        apptablewid->setRowHeight(apptablewid->rowCount()-2,20);
        apptablewid->setCellWidget(apptablewid->rowCount()-2,0,letterbtn);
        connect(letterbtn, SIGNAL(clicked()), this,SLOT(app_classificationbtn_clicked_slot()));


        int count=0;
        for(row=index;row<applist.count();row++)
        {
//            QPushButton* btn=new QPushButton;
//            btn->setFixedSize(apptablewid->columnWidth(0)-12,50);
//            btn->setStyleSheet(QString::fromLocal8Bit(pushbtncolor));
//            QString iconstr=KylinStartMenuInterface::get_app_icon(KylinStartMenuInterface::get_desktop_path_by_app_name(applist.at(row)));
//            QIcon::setThemeName("ukui-icon-theme");
//            QIcon icon=QIcon::fromTheme(iconstr);
//            btn->setIcon(icon);
//            btn->setIconSize(QSize(32,32));
//            btn->setText(applist.at(row));
//            btn->setFocusPolicy(Qt::NoFocus);

            QPushButton* btn=new QPushButton;
            btn->setFixedSize(apptablewid->columnWidth(0)-12,50);
            btn->setStyleSheet(QString::fromLocal8Bit(pushbtncolor));
            QHBoxLayout* layout=new QHBoxLayout(btn);
            layout->setContentsMargins(10,0,0,0);
            layout->setSpacing(15);

            QString iconstr=KylinStartMenuInterface::get_app_icon(KylinStartMenuInterface::get_desktop_path_by_app_name(applist.at(row)));
            QIcon::setThemeName("ukui-icon-theme");
            QIcon icon=QIcon::fromTheme(iconstr);
            QLabel* labelicon=new QLabel(btn);
            labelicon->setFixedSize(32,32);
            QPixmap pixmapicon(icon.pixmap(QSize(32,32)));
            labelicon->setPixmap(pixmapicon);
            labelicon->setStyleSheet("QLabel{background:transparent;}");
            QLabel* labeltext=new QLabel(btn);
            labeltext->setText(applist.at(row));
            labeltext->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
            layout->addWidget(labelicon);
            layout->addWidget(labeltext);
            btn->setLayout(layout);
            btn->setFocusPolicy(Qt::NoFocus);

            connect(btn,SIGNAL(clicked()),this,SLOT(exec_app_name()));
            btn->setContextMenuPolicy(Qt::CustomContextMenu);
            connect(btn,SIGNAL(customContextMenuRequested(const QPoint&)),
                    SLOT(right_click_slot()));

            if(indexch>=65 && indexch <=90)
            {
                QString appnamestr=applist.at(row);
                QChar appnamech=KylinStartMenuInterface::get_app_name_pinyin(appnamestr).at(0);
                if(indexch==appnamech)
                {
                    if(count==4)
                    {
                        apptablewid->insertRow(apptablewid->rowCount());
                        count=0;
                    }
                    apptablewid->setCellWidget(apptablewid->rowCount()-1,count,btn);
                    count++;
                }
                else {
                    index=row;
                    break;
                }
            }
            else{
                if(count==4)
                {
                    apptablewid->insertRow(apptablewid->rowCount());
                    count=0;
                }
                apptablewid->setCellWidget(apptablewid->rowCount()-1,count,btn);
                count++;
            }


        }
        if(row==applist.count())
            break;
    }
}

/**
 * 执行应用程序
 */
void FullLetterWidget::exec_app_name()
{
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
 * 应用列表字母分类按钮槽函数
 */
void FullLetterWidget::app_classificationbtn_clicked_slot()
{
    //加载FullLetterButtonWidget界面
    fullletterbtnwid=new FullLetterButtonWidget(this);
    mainLayout->removeWidget(applistWid);
    applistWid->setParent(nullptr);
    mainLayout->removeWidget(letterlistWid);
    letterlistWid->setParent(nullptr);
    mainLayout->addWidget(fullletterbtnwid);
    emit send_fullletterwid_state(1,"");

    connect(fullletterbtnwid, SIGNAL(send_letterbtn_signal(QString)),this,SLOT(recv_letterbtn_signal(QString)));
}

/**
 * 加载右键菜单
 */
void FullLetterWidget::right_click_slot()
{
    char style[200];
    sprintf(style,"QPushButton{background-color:%s;border:0px;color:#ffffff;font-size:14px;padding-left:10px;text-align: left center;}", MAINVIEWBTNHOVER);

    char btnstyle[300];
    sprintf(btnstyle,"QPushButton{background:transparent;border:0px;color:#ffffff;font-size:14px;padding-left:10px;text-align: left center;}\
            QPushButton:hover{background-color:%s;}\
            QPushButton:pressed{background-color:%s;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);

    QPushButton* btn=dynamic_cast<QPushButton*>(QObject::sender());
    btn->setStyleSheet(style);
    menu=new RightClickMenu;
    menu->show_appbtn_menu();
    btn->setStyleSheet(btnstyle);
}

/**
 * 接收FullLetterButtonWidget界面按钮信号
 */
void FullLetterWidget::recv_letterbtn_signal(QString btnname)
{
    mainLayout->removeWidget(fullletterbtnwid);
    fullletterbtnwid->setParent(nullptr);
    if(fullletterbtnwid!=nullptr)
    {
        delete fullletterbtnwid;
        fullletterbtnwid=nullptr;
    }
    mainLayout->addWidget(applistWid);
    mainLayout->addWidget(letterlistWid);

    int col=0;
    if(QString::compare(btnname,"&&")==0)
        col=26;
    else if(QString::compare(btnname,"#")==0)
        col=27;
    else{
        QByteArray btnnamearr=btnname.toLocal8Bit();
        char* bn=btnnamearr.data();
        col=static_cast<int>(*bn)-65;
    }
    letterlisttableWid->selectColumn(col);

    emit send_fullletterwid_state(0,btnname);
}

/**
 * 设置全屏字母分类界面状态
 */
void FullLetterWidget::set_fullletterwidge_state(int state, QString btnname)
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

        if(fullletterbtnwid!=nullptr)
        {
            delete fullletterbtnwid;
            fullletterbtnwid=nullptr;
        }
        fullletterbtnwid=new FullLetterButtonWidget(this);
        connect(fullletterbtnwid, SIGNAL(send_letterbtn_signal(QString)),this,SLOT(recv_letterbtn_signal(QString)));
        mainLayout->addWidget(fullletterbtnwid);

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

        if(fullletterbtnwid!=nullptr)
        {
            delete fullletterbtnwid;
            fullletterbtnwid=nullptr;
        }
        mainLayout->addWidget(applistWid);
        mainLayout->addWidget(letterlistWid);

        int col=0;
        if(QString::compare(btnname,"&&")==0)
            col=26;
        else if(QString::compare(btnname,"#")==0)
            col=27;
        else{
            QByteArray btnnamearr=btnname.toLocal8Bit();
            char* bn=btnnamearr.data();
            col=static_cast<int>(*bn)-65;
        }
        letterlisttableWid->selectColumn(col);

    }
}

/**
 * 初始化字母列表界面
 */
void FullLetterWidget::init_letterlist_widget()
{
    letterlistleftSpacer=new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);
    letterlistrightSpacer=new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);

    letterlistLayout=new QHBoxLayout(letterlistWid);
    letterlistLayout->setContentsMargins(0,0,0,0);
    //    letterlistLayout->setSpacing(5);
    letterlistWid->setLayout(letterlistLayout);

    char btncolor[300];
    sprintf(btncolor,"QToolButton{background: transparent;border:0px;padding-left:0px;}\
            QToolButton:hover{background-color:%s;}\
            QToolButton:pressed{background-color:%s;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);
    QSvgRenderer* leftsvg=new QSvgRenderer(letterlistWid);
    leftsvg->load(QString(":/data/img/mainviewwidget/leftarrow.svg"));
    QPixmap* leftpixmap=new QPixmap(19,19);
    leftpixmap->fill(Qt::transparent);
    QPainter leftp(leftpixmap);
    leftsvg->render(&leftp);
    leftbtn=new QToolButton(letterlistWid);
    leftbtn->setFixedSize(30,25);
    leftbtn->setIcon(QIcon(*leftpixmap));
    leftbtn->setStyleSheet(QString::fromLocal8Bit(btncolor));

    QSvgRenderer* rightsvg=new QSvgRenderer(letterlistWid);
    rightsvg->load(QString(":/data/img/mainviewwidget/rightarrow.svg"));
    QPixmap* rightpixmap=new QPixmap(19,19);
    rightpixmap->fill(Qt::transparent);
    QPainter rightp(rightpixmap);
    rightsvg->render(&rightp);
    letterlisttableWid=new QTableWidget(letterlistWid);
    rightbtn=new QToolButton(letterlistWid);
    rightbtn->setFixedSize(30,25);
    rightbtn->setIcon(QIcon(*rightpixmap));
    rightbtn->setStyleSheet(QString::fromLocal8Bit(btncolor));

    letterlistLayout->addItem(letterlistleftSpacer);
    letterlistLayout->addWidget(leftbtn);
    letterlistLayout->addWidget(letterlisttableWid);
    letterlistLayout->addWidget(rightbtn);
    letterlistLayout->addItem(letterlistrightSpacer);
    init_letterlist_table();

    letterlisttableWid->selectColumn(0);
    QWidget* wid=letterlisttableWid->cellWidget(0,0);
    QLabel* label=qobject_cast<QLabel*>(wid);
    QFont ft;
    ft.setWeight(81);
    label->setFont(ft);
    label->setStyleSheet("color:#ffffff;font-size:14px;");

    connect(leftbtn, SIGNAL(clicked()), this, SLOT(leftbtn_clicked_slot()));
    connect(rightbtn, SIGNAL(clicked()), this, SLOT(rightbtn_clicked_slot()));
    connect(letterlisttableWid,SIGNAL(itemSelectionChanged()),this,SLOT(letterlistitem_selected_slot()));

}

/**
 * 初始化字母列表界面数据表格letterlisttableWid
 */
void FullLetterWidget::init_letterlist_table()
{
    letterlisttableWid->setFocusPolicy(Qt::NoFocus);
    //    letterlisttableWid->setFixedSize(28*30+28*10,25);
    letterlisttableWid->setFixedSize(28*40,25);//Qt5.12中QTableWidget列宽至少为40
    letterlisttableWid->setColumnCount(28);
    letterlisttableWid->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    letterlisttableWid->setRowCount(1);
    letterlisttableWid->horizontalHeader()->setFixedHeight(0);
    letterlisttableWid->verticalHeader()->setDefaultSectionSize(25);
    letterlisttableWid->setEditTriggers(QAbstractItemView::NoEditTriggers);
    letterlisttableWid->setSelectionMode(QAbstractItemView::SingleSelection);
    letterlisttableWid->setSelectionBehavior(QAbstractItemView::SelectColumns);
    letterlisttableWid->setShowGrid(false);
    letterlisttableWid->setStyleSheet("QTableWidget{border:0px;background: transparent;}"
                                      "QTableWidget::Item{background: transparent;}\
                                      QTableWidget::Item:selected{background-color:rgba(14,19,22,92%);}");
                                      //    letterlisttableWid->setStyleSheet("QTableWidget{border:0px;background: transparent;}"
                                      //                                    "QTableWidget::Item{background: transparent;padding-left:5px;}\
                                      //                                     QTableWidget::Item:selected{background-color:rgba(14,19,22,76%);}");

    QStringList header;
    for(int i=0;i<28;i++)
    {
        header.append("");
        letterlisttableWid->setColumnWidth(i,40);

    }
    letterlisttableWid->setHorizontalHeaderLabels(header);
    letterlisttableWid->verticalHeader()->setHidden(true);
    letterlisttableWid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    QStringList letterlist;
    letterlist.clear();
    for(int i=0;i<26;i++)
    {
        char letter=static_cast<char>(65+i);
        letterlist.append(QString(QChar(letter)));
    }
    letterlist.append("&");
    letterlist.append("#");

    for(int i=0;i<28;i++)
    {
        QLabel* label=new QLabel;
        label->setFixedSize(40,25);
        QFont ft;
        ft.setWeight(50);
        label->setFont(ft);
        char labelcolor[300];
        sprintf(labelcolor,"QLabel{background: transparent;color:#8b8b8b;font-size:14px;}\
                QLabel:hover{background-color:%s;color:#ffffff;font-size:14px;}\
                QLabel:pressed{background-color:%s;color:#8b8b8b;font-size:14px;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);
        label->setStyleSheet(QString::fromLocal8Bit(labelcolor));
        label->setAlignment(Qt::AlignCenter);
        label->setText(letterlist.at(i));
        letterlisttableWid->setCellWidget(0,i,label);
    }
}

/**
 * 向左按钮槽函数
 */
void FullLetterWidget::leftbtn_clicked_slot()
{
    int col=letterlisttableWid->currentColumn();
    if(col>0)
        letterlisttableWid->selectColumn(col-1);
}

/**
 * 向右按钮槽函数
 */
void FullLetterWidget::rightbtn_clicked_slot()
{
    int col=letterlisttableWid->currentColumn();
    if(col<27)
        letterlisttableWid->selectColumn(col+1);
}

/**
 * 字母列表数据项被选定槽函数
 */
void FullLetterWidget::letterlistitem_selected_slot()
{
    int col=letterlisttableWid->currentColumn();
    QWidget* wid=letterlisttableWid->cellWidget(0,col);
    QLabel* label=qobject_cast<QLabel*>(wid);
    label->setStyleSheet("color:#ffffff;font-size:14px;");
    QFont ft;
    ft.setWeight(81);
    label->setFont(ft);

    char labelcolor[300];
    sprintf(labelcolor,"QLabel{background: transparent;color:#8b8b8b;font-size:14px;}\
            QLabel:hover{background-color:%s;color:#ffffff;font-size:14px;}\
            QLabel:pressed{background-color:%s;color:#8b8b8b;font-size:14px;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);

    for(int i=0;i<col;i++)
    {
        QWidget* wid=letterlisttableWid->cellWidget(0,i);
        QLabel* label=qobject_cast<QLabel*>(wid);
        QFont ft;
        ft.setWeight(50);
        label->setFont(ft);
        label->setStyleSheet(QString::fromLocal8Bit(labelcolor));
    }
    for(int i=col+1;i<28;i++)
    {
        QWidget* wid=letterlisttableWid->cellWidget(0,i);
        QLabel* label=qobject_cast<QLabel*>(wid);
        QFont ft;
        ft.setWeight(50);
        label->setFont(ft);
        label->setStyleSheet(QString::fromLocal8Bit(labelcolor));
    }

    QString arg;
    if(col==26)
    {
        arg="&&";
    }
    else arg=label->text();
    emit send_fullletterwid_state(0,arg);

    //此处需实现将被选定的字母包含的应用列表移动到applistWid界面最顶端
    QString letterstr=label->text();
    int num=letterbtnlist.indexOf(letterstr);
    if(num!=-1)
    {
        int row=letterbtnrowlist.at(num).toInt();
        apptablewid->verticalScrollBar()->setValue(row);
    }

}
