#include "toolbutton.h"

ToolButton::ToolButton(QString iconstr,QString piconstr,QString hoverbg, QString pressedbg)
{
    this->iconstr=iconstr;
    this->piconstr=piconstr;
    this->hoverbg=hoverbg;
    this->pressedbg=pressedbg;

    this->setFocusPolicy(Qt::NoFocus);
    layout=new QHBoxLayout;
    label=new QLabel;
    pixmap=new QPixmap(iconstr);
    label->resize(pixmap->size());
    label->setStyleSheet("border:0px;background:transparent;");
    label->setPixmap(*pixmap);
    layout->addWidget(label);
    layout->setAlignment(label,Qt::AlignCenter);
    this->setLayout(layout);
    this->setStyleSheet("border:0px;background:transparent;");

}

void ToolButton::enterEvent(QEvent *e)
{
    QByteArray byte=hoverbg.toLocal8Bit();
    char* hover=byte.data();
    char style[100];
    sprintf(style,"border:0px;background-color:%s;",hover);
    this->setStyleSheet(QString::fromLocal8Bit(style));
    pixmap=new QPixmap(piconstr);
    label->setPixmap(*pixmap);
}

void ToolButton::leaveEvent(QEvent *e)
{
    this->setStyleSheet("border:0px;background:transparent;");
    pixmap=new QPixmap(iconstr);
    label->setPixmap(*pixmap);
}

void ToolButton::mousePressEvent(QMouseEvent *event)
{
    QByteArray byte=pressedbg.toLocal8Bit();
    char* pressed=byte.data();
    char style[100];
    sprintf(style,"border:0px;background-color:%s;",pressed);
    if(event->button()==Qt::LeftButton)
        this->setStyleSheet(QString::fromLocal8Bit(style));
    pixmap=new QPixmap(piconstr);
    label->setPixmap(*pixmap);
}

void ToolButton::mouseReleaseEvent(QMouseEvent *event)
{
    QByteArray byte=hoverbg.toLocal8Bit();
    char* hover=byte.data();
    char style[100];
    sprintf(style,"border:0px;background-color:%s;",hover);
    if(event->button()==Qt::LeftButton)
        this->setStyleSheet(QString::fromLocal8Bit(style));
    pixmap=new QPixmap(piconstr);
    label->setPixmap(*pixmap);
    emit clicked();
}



