#include "letterbuttonwidget.h"
#include "ui_letterbuttonwidget.h"
#include "src/color.h"
#include "letterwidget.h"
#include <QDebug>

LetterButtonWidget::LetterButtonWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LetterButtonWidget)
{
    ui->setupUi(this);
    init_widget();
}

LetterButtonWidget::~LetterButtonWidget()
{
    delete ui;
}

void LetterButtonWidget::init_widget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setStyleSheet("border:0px;background:transparent;");
//    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
//    this->setFixedSize(330,532-70);

    mainLayout=new QVBoxLayout(this);
//    mainLayout->setContentsMargins(15,0,6,0);
//    mainLayout->setSpacing(5);
    this->setLayout(mainLayout);

    add_letterbtn_control();
//    load_min_wid();
    QSpacerItem* verticalSpacer=new QSpacerItem(20,40,QSizePolicy::Fixed,QSizePolicy::Expanding);
    mainLayout->addItem(verticalSpacer);

}

//添加字母分类按钮
void LetterButtonWidget::add_letterbtn_control()
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
//        layout->setSpacing(5);
        wid->setLayout(layout);

        for(int j=0;j<5;j++)
        {
            QToolButton* btn=new QToolButton(wid);
//            btn->setFixedSize(55,48);
            char btncolor[400];
            sprintf(btncolor,"QToolButton{background:transparent;;color:#ffffff;font-size:20px;padding-left:0px;}\
                    QToolButton:hover{background-color:%s;color:#ffffff;font-size:20px;}\
                    QToolButton:pressed{background-color:%s;color:#8b8b8b;font-size:20px;}\
                    QToolButton:disabled{color:#33ffffff;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);
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
void LetterButtonWidget::letterbtn_clicked_slot()
{
    QToolButton* btn=dynamic_cast<QToolButton *>(QObject::sender());
    QString btnname=btn->text();
    emit send_letterbtn_signal(btnname);
}

/**
 * 接收LetterWidget字母按钮列表
 */
void LetterButtonWidget::recv_letterbtn_list(QStringList list)
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
            else{
                btn->setEnabled(true);
            }

            if(i*5+j==27)break;

        }
    }
}

void LetterButtonWidget::load_min_wid()
{
    this->setGeometry(QRect(60,QApplication::desktop()->availableGeometry().height()-462,
                                 330,462));
    mainLayout->setContentsMargins(15,0,6,0);
    mainLayout->setSpacing(5);
    for(int i=0;i<6;i++)
    {
        QLayoutItem* item=mainLayout->itemAt(i);
        QWidget* wid=item->widget();
        QLayout* layout=wid->layout();
        layout->setSpacing(5);
        for(int j=0;j<5;j++)
        {
            QLayoutItem* item=layout->itemAt(j);
            QWidget* wid=item->widget();
            QToolButton* btn=qobject_cast<QToolButton*>(wid);
            btn->setFixedSize(55,48);
            if(i*5+j==27)break;
        }
    }
}

void LetterButtonWidget::load_max_wid()
{
    this->setGeometry(QRect(160,
                              70,
                              QApplication::desktop()->availableGeometry().width()-160,
                              QApplication::desktop()->availableGeometry().height()-70));
    mainLayout->setContentsMargins((this->width()-5*80-4*40)/2,70,(this->width()-5*80-4*40)/2,0);
    mainLayout->setSpacing(20);
    for(int i=0;i<6;i++)
    {
        QLayoutItem* item=mainLayout->itemAt(i);
        QWidget* wid=item->widget();
        QLayout* layout=wid->layout();
        layout->setSpacing(40);
        for(int j=0;j<5;j++)
        {
            QLayoutItem* item=layout->itemAt(j);
            QWidget* wid=item->widget();
            QToolButton* btn=qobject_cast<QToolButton*>(wid);
            btn->setFixedSize(80,50);
            if(i*5+j==27)break;
        }
    }
}
