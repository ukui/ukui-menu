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
    this->setFixedSize(320,500);

    gridLayout=new QGridLayout(this);
    gridLayout->setContentsMargins(15,0,6,this->height()-48*6);
    gridLayout->setSpacing(5);
    this->setLayout(gridLayout);

    iconlist.clear();
    iconlist.append(":/data/img/mainviewwidget/recent-gray.svg");
    iconlist.append(":/data/img/mainviewwidget/net-gray.svg");
    iconlist.append(":/data/img/mainviewwidget/social-gray.svg");
    iconlist.append(":/data/img/mainviewwidget/video-gray.svg");
    iconlist.append(":/data/img/mainviewwidget/develop-gray.svg");
    iconlist.append(":/data/img/mainviewwidget/img-gray.svg");
    iconlist.append(":/data/img/mainviewwidget/game-gray.svg");
    iconlist.append(":/data/img/mainviewwidget/office-gray.svg");
    iconlist.append(":/data/img/mainviewwidget/reading-gray.svg");
    iconlist.append(":/data/img/mainviewwidget/system-gray.svg");
    iconlist.append(":/data/img/mainviewwidget/other-gray.svg");

    iconlightlist.clear();
    iconlightlist.append(":/data/img/mainviewwidget/recent.svg");
    iconlightlist.append(":/data/img/mainviewwidget/net.svg");
    iconlightlist.append(":/data/img/mainviewwidget/social.svg");
    iconlightlist.append(":/data/img/mainviewwidget/video.svg");
    iconlightlist.append(":/data/img/mainviewwidget/develop.svg");
    iconlightlist.append(":/data/img/mainviewwidget/img.svg");
    iconlightlist.append(":/data/img/mainviewwidget/game.svg");
    iconlightlist.append(":/data/img/mainviewwidget/office.svg");
    iconlightlist.append(":/data/img/mainviewwidget/reading.svg");
    iconlightlist.append(":/data/img/mainviewwidget/system.svg");
    iconlightlist.append(":/data/img/mainviewwidget/other.svg");

    functionnamelist.clear();
    functionnamelist.append("最近");
    functionnamelist.append("网络");
    functionnamelist.append("社交");
    functionnamelist.append("影音");
    functionnamelist.append("开发");
    functionnamelist.append("图像");
    functionnamelist.append("游戏");
    functionnamelist.append("办公");
    functionnamelist.append("教育");
    functionnamelist.append("系统");
    functionnamelist.append("其它");
}

/**
 * 功能分类按钮槽函数
 */
void FunctionButtonWidget::functionbtn_clicked_slot()
{
    FunctionClassifyButton* btn=dynamic_cast<FunctionClassifyButton *>(QObject::sender());
    QWidget* wid=btn->layout()->itemAt(1)->widget();
    QLabel* label=qobject_cast<QLabel *>(wid);
    QString btnname=label->text();
    emit send_functionbtn_signal(btnname);
}

/**
 * 接收FunctionWidget界面分类按钮列表
 */
void FunctionButtonWidget::recv_classificationbtn_list(QStringList list)
{
    for(int i=0;i<list.size();i++)
    {
        FunctionClassifyButton* iconbtn=new FunctionClassifyButton(this,
                                                                   106,
                                                                   48,
                                                                   iconlist.at(functionnamelist.indexOf(list.at(i))),
                                                                   iconlightlist.at(functionnamelist.indexOf(list.at(i))),
                                                                   ClassifyBtnHoverBackground,
                                                                   ClassifyBtnHoverBackground,
                                                                   2,
                                                                   list.at(i));
        vector.append(iconbtn);
        connect(iconbtn,SIGNAL(buttonClicked(QAbstractButton*)),this, SLOT(functionbtn_clicked_slot()));
    }

    for(int row=0;row<6;row++)
        for(int col=0;col<2;col++)
        {
            if(row*2+col < vector.size())
                gridLayout->addWidget(vector.at(row*2+col),row,col);
            else
                break;
        }
}
