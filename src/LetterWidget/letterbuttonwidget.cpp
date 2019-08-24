#include "letterbuttonwidget.h"
#include "ui_letterbuttonwidget.h"

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
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setFixedSize(330,532-70);

    mainLayout=new QVBoxLayout;
    mainLayout->setContentsMargins(15,0,6,0);
    mainLayout->setSpacing(5);
    this->setLayout(mainLayout);

    add_letterbtn_control();
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
        char letter=65+i;
        letterlist.append(QString(QChar(letter)));
    }
    letterlist.append("&&");
    letterlist.append("#");

    for(int i=0;i<6;i++)
    {
        QWidget* wid=new QWidget;
        wid->setStyleSheet("QWidget{border:0px;background:transparent;}");
        QHBoxLayout* layout=new QHBoxLayout;
        layout->setContentsMargins(0,0,0,0);
        layout->setSpacing(5);
        wid->setLayout(layout);

        for(int j=0;j<5;j++)
        {
            QToolButton* btn=new QToolButton;
            btn->setFixedSize(55,48);
            btn->setStyleSheet("QToolButton{background:transparent;;color:#ffffff;font-size:20px;padding-left:0px;}\
                               QToolButton:hover{background-color:#414e59;color:#ffffff;font-size:20px;}\
                               QToolButton:pressed{background-color:#2a343e;color:#8b8b8b;font-size:20px;}");

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
