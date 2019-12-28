#include "pushbutton.h"

PushButton::PushButton(QString name, int width, int height)
{
    this->name=name;
    this->width=width;
    this->height=height;
    init_app_btn();
}

PushButton::~PushButton()
{
}

void PushButton::init_app_btn()
{
    char btnstyle[300];
    sprintf(btnstyle,"QPushButton{background:transparent;border:0px;color:#ffffff;font-size:14px;padding-left:0px;text-align: left center;}\
            QPushButton:hover{background-color:%s;}\
            QPushButton:pressed{background-color:%s;}", ClassifyBtnHoverBackground,ClassifyBtnHoverBackground);

    this->setFixedSize(this->width,this->height);
    this->setStyleSheet(btnstyle);
    this->setFocusPolicy(Qt::NoFocus);
    QHBoxLayout* layout=new QHBoxLayout(this);
    layout->setContentsMargins(15,0,0,0);
    layout->setSpacing(6);

    QLabel* textlabel=new QLabel(this);
    textlabel->setAlignment(Qt::AlignCenter);
    textlabel->setStyleSheet("color:#ffffff;font-size:14px;");
    textlabel->setText(name);
    textlabel->adjustSize();

    QFrame* line=new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFixedHeight(1);
    line->setStyleSheet("background-color:rgba(255, 255, 255, 0.06)");
    line->setFixedSize(this->width-textlabel->width()-15,1);

    layout->addWidget(textlabel);
    layout->addWidget(line);
    this->setLayout(layout);
    this->setEnabled(false);
}
