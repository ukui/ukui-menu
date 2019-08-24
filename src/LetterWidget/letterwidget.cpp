#include "letterwidget.h"
#include "ui_letterwidget.h"
#include "src/color.h"
#include <QDebug>

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

    mainLayout=new  QHBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
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
void LetterWidget::init_applist_widget()
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
void LetterWidget::app_classificationbtn_clicked_slot()
{
    //加载LetterBUttonWidget界面
    letterbtnwid=new LetterButtonWidget;
    mainLayout->removeWidget(applistWid);
    applistWid->setParent(NULL);
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
    letterbtnwid->setParent(NULL);
    if(letterbtnwid!=NULL)
    {
        delete letterbtnwid;
        letterbtnwid=NULL;
    }
    mainLayout->addWidget(applistWid);

    //btn为测试项
    //此处需实现将字母为btnname的应用列表移动到applistWid界面最顶端
    btn->setText(btnname);

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
        if((child = mainLayout->takeAt(0)) != 0) {
            QWidget* childwid=child->widget();
            if(childwid!=NULL)
            {
                mainLayout->removeWidget(childwid);
                childwid->setParent(NULL);
            }
        }
        if(letterbtnwid!=NULL)
        {
            delete letterbtnwid;
            letterbtnwid=NULL;
        }
        letterbtnwid=new LetterButtonWidget;
        connect(letterbtnwid, SIGNAL(send_letterbtn_signal(QString)),this,SLOT(recv_letterbtn_signal(QString)));
        mainLayout->addWidget(letterbtnwid);
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
        if(letterbtnwid!=NULL)
        {
            delete letterbtnwid;
            letterbtnwid=NULL;
        }
        mainLayout->addWidget(applistWid);

        //btn为测试项
        //此处需实现将字母为btnname的应用列表移动到applistWid界面最顶端
        btn->setText(btnname);
    }

}
