#include "fullletterwidget.h"
#include "ui_fullletterwidget.h"
#include <QDesktopWidget>
#include "src/color.h"
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

    mainLayout=new QVBoxLayout;
    applistWid=new QWidget;
    applistWid->setStyleSheet("border:0px;background: transparent;");
    applistWid->setFixedSize(this->width(),this->height()-70);
    letterlistWid=new QWidget;
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
    //以下为测试项
    //此处需实现加载具体的应用列表，btn实际为应用列表最右端的字母分类按钮
    QHBoxLayout* layout=new QHBoxLayout;
    btn=new QToolButton;
    btn->setFixedSize(100,20);
    layout->addWidget(btn);
    btn->setText("A");
    btn->setStyleSheet("border:1px groove #ffffff;color:#ffffff;");
    applistWid->setLayout(layout);

    connect(btn, SIGNAL(clicked()), this,SLOT(app_classificationbtn_clicked_slot()));
}

/**
 * 应用列表字母分类按钮槽函数
 */
void FullLetterWidget::app_classificationbtn_clicked_slot()
{
    //加载FullLetterButtonWidget界面
    fullletterbtnwid=new FullLetterButtonWidget;
    mainLayout->removeWidget(applistWid);
    applistWid->setParent(NULL);
    mainLayout->removeWidget(letterlistWid);
    letterlistWid->setParent(NULL);
    mainLayout->addWidget(fullletterbtnwid);
    emit send_fullletterwid_state(1,"");

    connect(fullletterbtnwid, SIGNAL(send_letterbtn_signal(QString)),this,SLOT(recv_letterbtn_signal(QString)));
}

/**
 * 接收FullLetterButtonWidget界面按钮信号
 */
void FullLetterWidget::recv_letterbtn_signal(QString btnname)
{
    mainLayout->removeWidget(fullletterbtnwid);
    fullletterbtnwid->setParent(NULL);
    if(fullletterbtnwid!=NULL)
    {
        delete fullletterbtnwid;
        fullletterbtnwid=NULL;
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
        col=(int)*bn-65;
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

        if(fullletterbtnwid!=NULL)
        {
            delete fullletterbtnwid;
            fullletterbtnwid=NULL;
        }
        fullletterbtnwid=new FullLetterButtonWidget;
        connect(fullletterbtnwid, SIGNAL(send_letterbtn_signal(QString)),this,SLOT(recv_letterbtn_signal(QString)));
        mainLayout->addWidget(fullletterbtnwid);

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

        if(fullletterbtnwid!=NULL)
        {
            delete fullletterbtnwid;
            fullletterbtnwid=NULL;
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
            col=(int)*bn-65;
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

    letterlistLayout=new QHBoxLayout;
    letterlistLayout->setContentsMargins(0,0,0,0);
//    letterlistLayout->setSpacing(5);
    letterlistWid->setLayout(letterlistLayout);

    leftbtn=new QToolButton;
    leftbtn->setFixedSize(30,25);
//    QPixmap leftpixmap(":/data/img/mainviewwidget/向左-箭头.svg");
//    leftbtn->setIconSize(leftpixmap.size());
//    leftbtn->setIcon(QIcon(leftpixmap));
    leftbtn->setStyleSheet("QToolButton{background: transparent;border:0px;padding-left:0px;image:url(:/data/img/mainviewwidget/向左-箭头.svg);}\
                               QToolButton:hover{background-color:#414e59;}\
                               QToolButton:pressed{background-color:#2a343e;}");

    letterlisttableWid=new QTableWidget;
    rightbtn=new QToolButton;
    rightbtn->setFixedSize(30,25);
    QPixmap rightpixmap(":/data/img/mainviewwidget/向右-箭头.svg");
    rightbtn->setIconSize(rightpixmap.size());
    rightbtn->setIcon(QIcon(rightpixmap));
    rightbtn->setStyleSheet("QToolButton{background: transparent;border:0px;padding-left:0px;}\
                               QToolButton:hover{background-color:#414e59;}\
                               QToolButton:pressed{background-color:#2a343e;}");

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
    label->setStyleSheet("color:#ffffff");

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
    letterlisttableWid->setFixedSize(28*30,25);
    letterlisttableWid->setColumnCount(28);
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
//        letterlisttableWid->setColumnWidth(i,40);
        letterlisttableWid->setColumnWidth(i,30);

    }
    letterlisttableWid->setHorizontalHeaderLabels(header);
    letterlisttableWid->verticalHeader()->setHidden(true);
    letterlisttableWid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    QStringList letterlist;
    letterlist.clear();
    for(int i=0;i<26;i++)
    {
        char letter=65+i;
        letterlist.append(QString(QChar(letter)));
    }
    letterlist.append("&");
    letterlist.append("#");

    for(int i=0;i<28;i++)
    {
        QLabel* label=new QLabel;
        label->setFixedSize(30,25);
        QFont ft;
        ft.setWeight(50);
        label->setFont(ft);
        label->setStyleSheet("QLabel{background: transparent;color:#8b8b8b;font-size:14px;}\
                             QLabel:hover{background-color:#414e59;color:#ffffff;font-size:14px;}\
                             QLabel:pressed{background-color:#2a343e;color:#8b8b8b;font-size:14px;}");
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
    label->setStyleSheet("color:#ffffff;QFont::Bold;");
    QFont ft;
    ft.setWeight(81);
    label->setFont(ft);


    for(int i=0;i<col;i++)
    {
        QWidget* wid=letterlisttableWid->cellWidget(0,i);
        QLabel* label=qobject_cast<QLabel*>(wid);
        QFont ft;
        ft.setWeight(50);
        label->setFont(ft);
        label->setStyleSheet("QLabel{background: transparent;color:#8b8b8b;font-size:14px;}\
                             QLabel:hover{background-color:#414e59;color:#ffffff;font-size:14px;}\
                             QLabel:pressed{background-color:#2a343e;color:#8b8b8b;font-size:14px;}");
    }
    for(int i=col+1;i<28;i++)
    {
        QWidget* wid=letterlisttableWid->cellWidget(0,i);
        QLabel* label=qobject_cast<QLabel*>(wid);
        QFont ft;
        ft.setWeight(50);
        label->setFont(ft);
        label->setStyleSheet("QLabel{background: transparent;color:#8b8b8b;font-size:14px;}\
                             QLabel:hover{background-color:#414e59;color:#ffffff;font-size:14px;}\
                             QLabel:pressed{background-color:#2a343e;color:#8b8b8b;font-size:14px;}");
    }

    QString arg;
    if(col==26)
    {
        arg="&&";
    }
    else arg=label->text();
    emit send_fullletterwid_state(0,arg);

    //btn为测试项
    //此处需实现将被选定的字母包含的应用列表移动到applistWid界面最顶端
    if(col==26)
        btn->setText("&&");
    else{
        btn->setText(label->text());
    }

}
