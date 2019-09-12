#include "functionbuttonwidget.h"
#include "ui_functionbuttonwidget.h"
#include "src/color.h"
#include <QSvgRenderer>
#include <QPainter>
#include <QDebug>

FunctionButtonWidget::FunctionButtonWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FunctionButtonWidget)
{
    ui->setupUi(this);
    init_widget();
}

FunctionButtonWidget::~FunctionButtonWidget()
{
    delete ui;
}

void FunctionButtonWidget::init_widget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setStyleSheet("border:0px;background:transparent;");
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setFixedSize(330,532-70);

    mainLayout=new QVBoxLayout(this);
    mainLayout->setContentsMargins(15,0,6,0);
    mainLayout->setSpacing(5);
    this->setLayout(mainLayout);

    QToolButton* recentbtn=new QToolButton;
    set_functionbtn_style(recentbtn,":/data/img/mainviewwidget/recent.svg", "最近添加");
    QToolButton* netbtn=new QToolButton;
    set_functionbtn_style(netbtn,":/data/img/mainviewwidget/net.svg","网络应用");
    QToolButton* socialbtn=new QToolButton;
    set_functionbtn_style(socialbtn,":/data/img/mainviewwidget/social.svg","社交沟通");
    QToolButton* avbtn=new QToolButton;
    set_functionbtn_style(avbtn,":/data/img/mainviewwidget/video.svg","影音播放");
    QToolButton* developbtn=new QToolButton;
    set_functionbtn_style(developbtn,":/data/img/mainviewwidget/develop.svg","开发编程");
    QToolButton* imgbtn=new QToolButton;
    set_functionbtn_style(imgbtn,":/data/img/mainviewwidget/img.svg","图形图像");
    QToolButton* gamebtn=new QToolButton;
    set_functionbtn_style(gamebtn,":/data/img/mainviewwidget/game.svg","游戏娱乐");
    QToolButton* officebtn=new QToolButton;
    set_functionbtn_style(officebtn,":/data/img/mainviewwidget/office.svg","办公学习");
    QToolButton* readingbtn=new QToolButton;
    set_functionbtn_style(readingbtn,":/data/img/mainviewwidget/reading.svg","阅读翻译");
    QToolButton* systembtn=new QToolButton;
    set_functionbtn_style(systembtn,":/data/img/mainviewwidget/system.svg","系统管理");
    QToolButton* otherbtn=new QToolButton;
    set_functionbtn_style(otherbtn,":/data/img/mainviewwidget/other.svg","其他应用");

    add_functionbtn_control(recentbtn,netbtn);
    add_functionbtn_control(socialbtn,avbtn);
    add_functionbtn_control(developbtn,imgbtn);
    add_functionbtn_control(gamebtn,officebtn);
    add_functionbtn_control(readingbtn,systembtn);

    QWidget* wid=new QWidget(this);
    wid->setStyleSheet("QWidget{border:0px;}");
    wid->setFixedSize(295,48);
    QHBoxLayout* layout=new QHBoxLayout(wid);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(5);
    otherbtn->setParent(wid);
    layout->addWidget(otherbtn);
    horizontalSpacer=new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);
    layout->addItem(horizontalSpacer);
    wid->setLayout(layout);
    mainLayout->addWidget(wid);

    verticalSpacer=new QSpacerItem(20,40,QSizePolicy::Fixed,QSizePolicy::Expanding);
    mainLayout->addItem(verticalSpacer);
}

/**
 * 添加功能分类按钮
 */
void FunctionButtonWidget::add_functionbtn_control(QToolButton* firstbtn, QToolButton* secondbtn)
{
    QWidget* wid=new QWidget(this);
    firstbtn->setParent(wid);
    secondbtn->setParent(wid);
    wid->setStyleSheet("QWidget{border:0px;}");
    wid->setFixedSize(295,48);
    QHBoxLayout* layout=new QHBoxLayout(wid);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(5);
    layout->addWidget(firstbtn);
    layout->addWidget(secondbtn);
    wid->setLayout(layout);
    mainLayout->addWidget(wid);
}

/**
 * 设置功能分类按钮样式
 */
void FunctionButtonWidget::set_functionbtn_style(QToolButton *btn, QString btnicon, QString btnname)
{
    char btncolor[300];
    sprintf(btncolor,"QToolButton{background:transparent;border:0px;}\
            QToolButton:hover{background-color:%s;}\
            QToolButton:pressed{background-color:%s;}",MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);
    btn->setFixedSize(145,48);
    btn->setStyleSheet(QString::fromLocal8Bit(btncolor));
    QHBoxLayout* btnlayout=new QHBoxLayout(btn);

    QSvgRenderer* svgRender = new QSvgRenderer(btn);
    svgRender->load(btnicon);
    QPixmap* pixmap = new QPixmap(19,19);
    pixmap->fill(Qt::transparent);//设置背景透明
    QPainter p(pixmap);
    svgRender->render(&p);
    QLabel* labelicon=new QLabel(btn);
    labelicon->setAlignment(Qt::AlignCenter);
//    QPixmap pixmapicon(btnicon);
    labelicon->setStyleSheet("QLabel{background:transparent;}");
    labelicon->setFixedSize(pixmap->size());
    labelicon->setPixmap(*pixmap);
    btnlayout->addWidget(labelicon);

    QLabel* labeltext=new QLabel(btn);
    QByteArray btnnamebyte=btnname.toLocal8Bit();
    char* name=btnnamebyte.data();
    labeltext->setText(tr(name));
    labeltext->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
    labeltext->adjustSize();
    btnlayout->addWidget(labeltext);
    btn->setLayout(btnlayout);

    btnlayout->setContentsMargins(10,0,btn->width()-10*2-labelicon->width()-labeltext->width(),0);
    btnlayout->setSpacing(10);

    connect(btn, SIGNAL(clicked()), this, SLOT(functionbtn_clicked_slot()));
}

/**
 * 功能分类按钮槽函数
 */
void FunctionButtonWidget::functionbtn_clicked_slot()
{
    QToolButton* btn=dynamic_cast<QToolButton *>(QObject::sender());
    QWidget* wid=btn->layout()->itemAt(1)->widget();
    QLabel* label=qobject_cast<QLabel *>(wid);
    QString btnname=label->text();
    emit send_functionbtn_signal(btnname);
}
