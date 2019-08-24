#include "functionbuttonwidget.h"
#include "ui_functionbuttonwidget.h"

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

    mainLayout=new QVBoxLayout;
    mainLayout->setContentsMargins(15,0,6,0);
    mainLayout->setSpacing(5);
    this->setLayout(mainLayout);

    QToolButton* recentbtn=new QToolButton;
    set_functionbtn_style(recentbtn,":/data/img/mainviewwidget/gn-最新添加.svg", "最近添加");
    QToolButton* netbtn=new QToolButton;
    set_functionbtn_style(netbtn,":/data/img/mainviewwidget/gn-网络应用.svg","网络应用");
    QToolButton* socialbtn=new QToolButton;
    set_functionbtn_style(socialbtn,":/data/img/mainviewwidget/gn-社交沟通.svg","社交沟通");
    QToolButton* musicbtn=new QToolButton;
    set_functionbtn_style(musicbtn,":/data/img/mainviewwidget/gn-音乐欣赏.svg","音乐欣赏");
    QToolButton* videobtn=new QToolButton;
    set_functionbtn_style(videobtn,":/data/img/mainviewwidget/gn-视频播放.svg","视频播放");
    QToolButton* imgbtn=new QToolButton;
    set_functionbtn_style(imgbtn,":/data/img/mainviewwidget/gn-图形图像.svg","图形图像");
    QToolButton* gamebtn=new QToolButton;
    set_functionbtn_style(gamebtn,":/data/img/mainviewwidget/gn-游戏娱乐.svg","游戏娱乐");
    QToolButton* officebtn=new QToolButton;
    set_functionbtn_style(officebtn,":/data/img/mainviewwidget/gn-办公学习.svg","办公学习");
    QToolButton* readingbtn=new QToolButton;
    set_functionbtn_style(readingbtn,":/data/img/mainviewwidget/gn-阅读翻译.svg","阅读翻译");
    QToolButton* systembtn=new QToolButton;
    set_functionbtn_style(systembtn,":/data/img/mainviewwidget/gn-系统管理.svg","系统管理");
    QToolButton* otherbtn=new QToolButton;
    set_functionbtn_style(otherbtn,":/data/img/mainviewwidget/gn-其他应用.svg","其它应用");

    add_functionbtn_control(recentbtn,netbtn);
    add_functionbtn_control(socialbtn,musicbtn);
    add_functionbtn_control(videobtn,imgbtn);
    add_functionbtn_control(gamebtn,officebtn);
    add_functionbtn_control(readingbtn,systembtn);

    QWidget* wid=new QWidget;
    wid->setStyleSheet("QWidget{border:0px;}");
    wid->setFixedSize(295,48);
    QHBoxLayout* layout=new QHBoxLayout;
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(5);
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
    QWidget* wid=new QWidget;
    wid->setStyleSheet("QWidget{border:0px;}");
    wid->setFixedSize(295,48);
    QHBoxLayout* layout=new QHBoxLayout;
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
    QString style="QToolButton{background:transparent;border:0px;}\
            QToolButton:hover{background-color:#414e59;}\
            QToolButton:pressed{background-color:#2a343e;}";
    btn->setFixedSize(145,48);
    btn->setStyleSheet(style);
    QHBoxLayout* btnlayout=new QHBoxLayout;
    btnlayout->setContentsMargins(10,0,0,0);
    btnlayout->setSpacing(10);

    QLabel* labelicon=new QLabel;
    QPixmap pixmapicon(btnicon);
    labelicon->setStyleSheet("QLabel{background:transparent;}");
    labelicon->setFixedSize(pixmapicon.size());
    labelicon->setPixmap(pixmapicon);
    btnlayout->addWidget(labelicon);

    QLabel* labeltext=new QLabel;
    QByteArray btnnamebyte=btnname.toLocal8Bit();
    char* name=btnnamebyte.data();
    labeltext->setText(tr(name));
    labeltext->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
    btnlayout->addWidget(labeltext);
    btn->setLayout(btnlayout);

    connect(btn, SIGNAL(clicked()), this, SLOT(functionbtn_clicked_slot()));
}

/**
 * 功能分类按钮槽函数
 */
void FunctionButtonWidget::functionbtn_clicked_slot()
{
    QToolButton* btn=dynamic_cast<QToolButton *>(QObject::sender());
    QWidget* wid=btn->layout()->takeAt(1)->widget();
    QLabel* label=qobject_cast<QLabel *>(wid);
    QString btnname=label->text();
    emit send_functionbtn_signal(btnname);
}
