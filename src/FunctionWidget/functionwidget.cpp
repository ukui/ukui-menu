#include "functionwidget.h"
#include "ui_functionwidget.h"
#include "src/color.h"
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
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setFixedSize(330,462);

    mainLayout=new QHBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    applistWid=new QWidget;
    applistWid->setStyleSheet("border:0px;background: transparent;");
    applistWid->setFixedSize(this->width(),this->height());
    mainLayout->addWidget(applistWid);
    this->setLayout(mainLayout);

    init_applist_widget();

}

/**
 * 初始化应用列表界面
 */
void FunctionWidget::init_applist_widget()
{
    //以下为测试项
    //此处需实现加载具体的应用列表，btn实际为应用列表最右端的功能分类按钮
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
 * 应用列表功能分类按钮槽函数
 */
void FunctionWidget::app_classificationbtn_clicked_slot()
{
    //加载FunctionButtonWidget界面
    functionbtnwid=new FunctionButtonWidget;
    mainLayout->removeWidget(applistWid);
    applistWid->setParent(NULL);
    mainLayout->addWidget(functionbtnwid);
    emit send_functionwid_state(1,"");

    connect(functionbtnwid, SIGNAL(send_functionbtn_signal(QString)),this,SLOT(recv_functionbtn_signal(QString)));
}

/**
 * 接收FunctionButtonWidget界面按钮信号
 */
void FunctionWidget::recv_functionbtn_signal(QString btnname)
{
    mainLayout->removeWidget(functionbtnwid);
    functionbtnwid->setParent(NULL);
    if(functionbtnwid!=NULL)
    {

        delete functionbtnwid;
        functionbtnwid=NULL;
    }
    mainLayout->addWidget(applistWid);

    //btn为测试项
    //此处需实现将功能为btnname的应用列表移动到applistWid界面最顶端
    btn->setText(btnname);

    emit send_functionwid_state(0,btnname);
}

/**
 * 设置功能分类界面状态
 */
void FunctionWidget::set_functionwidge_state(int state, QString btnname)
{
    if(state==1)
    {
        QLayoutItem *child;
        if((child = mainLayout->takeAt(0)) != 0) {
            QWidget* childwid=child->widget();
            if(childwid!=NULL)
            {
                mainLayout->removeWidget(childwid);
                childwid->setParent(NULL);
            }
        }
        if(functionbtnwid!=NULL)
        {

            delete functionbtnwid;
            functionbtnwid=NULL;
        }
        functionbtnwid=new FunctionButtonWidget;
        connect(functionbtnwid, SIGNAL(send_functionbtn_signal(QString)),this,SLOT(recv_functionbtn_signal(QString)));

        mainLayout->addWidget(functionbtnwid);

    }
    else{
        QLayoutItem *child;
        if((child = mainLayout->takeAt(0)) != 0) {
            QWidget* childwid=child->widget();
            if(childwid!=NULL)
            {
                mainLayout->removeWidget(childwid);
                childwid->setParent(NULL);
            }
        }
        if(functionbtnwid!=NULL)
        {

            delete functionbtnwid;
            functionbtnwid=NULL;
        }
        mainLayout->addWidget(applistWid);

        //btn为测试项
        //此处需实现将功能为btnname的应用列表移动到applistWid界面最顶端
        btn->setText(btnname);
    }
}
