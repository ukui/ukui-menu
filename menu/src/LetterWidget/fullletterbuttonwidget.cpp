#include "fullletterbuttonwidget.h"
#include "ui_fullletterbuttonwidget.h"
#include <QDebug>
#include "src/color.h"

FullLetterButtonWidget::FullLetterButtonWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FullLetterButtonWidget)
{
    ui->setupUi(this);
    init_widget();
}

FullLetterButtonWidget::~FullLetterButtonWidget()
{
    delete ui;
}

void FullLetterButtonWidget::init_widget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setStyleSheet("border:0px;background:transparent");
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setFixedSize(QApplication::desktop()->availableGeometry().width()-160,
                       QApplication::desktop()->availableGeometry().height()-70);

    mainLayout=new QVBoxLayout(this);
    mainLayout->setContentsMargins((this->width()-5*80-4*40)/2,70,(this->width()-5*80-4*40)/2,0);
    mainLayout->setSpacing(20);
    this->setLayout(mainLayout);

    add_letterbtn_control();
    QSpacerItem* verticalSpacer=new QSpacerItem(20,40,QSizePolicy::Fixed,QSizePolicy::Expanding);
    mainLayout->addItem(verticalSpacer);

}

/**
 * 添加字母分类按钮
 */
void FullLetterButtonWidget::add_letterbtn_control()
{
    QStringList letterlist;
    letterlist.clear();
    for(int i=0;i<26;i++)
    {
        char letter=static_cast<char>(65+i);
        letterlist.append(QString(QChar(letter)));
    }
    letterlist.append("&&");
    letterlist.append("#");

    for(int i=0;i<6;i++)
    {
        QWidget* wid=new QWidget(this);
        wid->setStyleSheet("QWidget{border:0px;background:transparent;}");
        QHBoxLayout* layout=new QHBoxLayout(wid);
        layout->setContentsMargins(0,0,0,0);
        layout->setSpacing(40);
        wid->setLayout(layout);

        for(int j=0;j<5;j++)
        {
            QToolButton* btn=new QToolButton(wid);
            btn->setFixedSize(80,50);
            QFont ft;
            ft.setWeight(57);
            btn->setFont(ft);
            char btncolor[400];
            sprintf(btncolor,"QToolButton{background:transparent;color:#ffffff;font-size:20px;padding-left:0px;}\
                    QToolButton:hover{background-color:%s;color:#ffffff;font-size:20px;}\
                    QToolButton:pressed{background-color:%s;color:#8b8b8b;font-size:20px;}\
                    QToolButton:disabled{color:#33ffffff;}",MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);
            btn->setStyleSheet(QString::fromLocal8Bit(btncolor));

            btn->setText(letterlist.at(i*5+j));
            layout->addWidget(btn);
            connect(btn, SIGNAL(clicked()), this, SLOT(letterbtn_clicked_slot()));
            if(i*5+j==27)break;

        }
        QSpacerItem* righthorizontalSpacer=new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);
        layout->addItem(righthorizontalSpacer);
        mainLayout->addWidget(wid);
    }

}

/**
 * 字母分类按钮槽函数
 */
void FullLetterButtonWidget::letterbtn_clicked_slot()
{
    QToolButton* btn=dynamic_cast<QToolButton *>(QObject::sender());
    QString btnname=btn->text();
    emit send_letterbtn_signal(btnname);
}

/**
 * 接收LetterWidget字母按钮列表
 */
void FullLetterButtonWidget::recv_letterbtn_list(QStringList list)
{
    for(int i=0;i<6;i++)
    {
        QLayoutItem* item=mainLayout->itemAt(i);
        QWidget* wid=item->widget();

        for(int j=0;j<5;j++)
        {
            QLayoutItem* item=wid->layout()->itemAt(j);
            QWidget* wid=item->widget();
            QToolButton* btn=static_cast<QToolButton*>(wid);
            QString letter=btn->text();
            if(list.indexOf(QString(QChar(letter.at(0))))==-1)
                btn->setEnabled(false);

            if(i*5+j==27)break;

        }
    }
}
