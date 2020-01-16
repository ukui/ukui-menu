#include "toolbutton.h"
#include <QDebug>

ToolButton::ToolButton(int width,
                       int height,
                       QString iconstr,
                       QString piconstr,
                       QString hoverbg,
                       QString pressedbg,
                       int module,
                       QString text)
{
    this->setFixedSize(width,height);
    this->iconstr=iconstr;
    this->piconstr=piconstr;
    this->hoverbg=hoverbg;
    this->pressedbg=pressedbg;
    this->module=module;
    this->text=text;

    this->setFocusPolicy(Qt::NoFocus);
    svgRender=new QSvgRenderer(this);
    svgRender->load(iconstr);
//    if(module==1)
//        pixmap=new QPixmap(14,14);
//    else if(module==2)
//        pixmap=new QPixmap(19,19);
//    else
        pixmap=new QPixmap(9,9);
    pixmap->fill(Qt::transparent);
    QPainter p(pixmap);
    svgRender->render(&p);
    this->setIcon(QIcon(*pixmap));

//    if(module==2)
//    {
//        this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
//        this->setText(tr(text.toLocal8Bit().data()));
//        this->setStyleSheet("border:0px;padding-left:16px;background:transparent;color:rgba(255, 255, 255, 50%);font-size:14px;");
//    }
//    else
        this->setStyleSheet("border:0px;border-radius:4px;padding-left:0px;background:transparent;");
}

void ToolButton::enterEvent(QEvent *e)
{
    Q_UNUSED(e);
    QByteArray byte=hoverbg.toLocal8Bit();
    char* hover=byte.data();
    char style[100];
//    if(module==2)
//        sprintf(style,"border:0px;padding-left:16px;background-color:%s;color:#ffffff;font-size:14px;",hover);
//    else
        sprintf(style,"border:0px;border-radius:4px;padding-left:0px;background-color:%s;",hover);
    this->setStyleSheet(QString::fromLocal8Bit(style));
    delete svgRender;
    svgRender=new QSvgRenderer(this);
    svgRender->load(piconstr);
//    if(module==1)
//        pixmap=new QPixmap(14,14);
//    else if(module==2)
//        pixmap=new QPixmap(19,19);
//    else
        pixmap=new QPixmap(9,9);
    pixmap->fill(Qt::transparent);
    QPainter p(pixmap);
    svgRender->render(&p);
    this->setIcon(QIcon(*pixmap));
}

void ToolButton::leaveEvent(QEvent *e)
{
    Q_UNUSED(e);
//    if(module==2)
//    {
//        if(!is_pressed)
//            this->setStyleSheet("border:0px;padding-left:16px;background:transparent;color:rgba(255, 255, 255, 50%);font-size:14px;");
//        else
//            this->setStyleSheet("border:0px;padding-left:16px;background:transparent;color:rgba(255, 255, 255);font-size:14px;");
//    }
//    else
        this->setStyleSheet("border:0px;border-radius:4px;padding-left:0px;background:transparent;");
    delete svgRender;
    svgRender=new QSvgRenderer(this);
//    if(module==2 && is_pressed)
//        svgRender->load(piconstr);
//    else
        svgRender->load(iconstr);
//    if(module==1)
//        pixmap=new QPixmap(14,14);
//    else if(module==2)
//        pixmap=new QPixmap(19,19);
//    else
        pixmap=new QPixmap(9,9);
    pixmap->fill(Qt::transparent);
    QPainter p(pixmap);
    svgRender->render(&p);
    this->setIcon(QIcon(*pixmap));
}

void ToolButton::mousePressEvent(QMouseEvent *event)
{
    QByteArray byte=pressedbg.toLocal8Bit();
    char* pressed=byte.data();
    char style[100];
//    if(module==2)
//        sprintf(style,"border:0px;padding-left:16px;background-color:%s;color:#ffffff;font-size:14px;",pressed);
//    else
        sprintf(style,"border:0px;border-radius:4px;padding-left:0px;background-color:%s;",pressed);
    if(event->button()==Qt::LeftButton)
    {
        this->setStyleSheet(QString::fromLocal8Bit(style));
        delete svgRender;
        svgRender=new QSvgRenderer(this);
        svgRender->load(piconstr);
//        if(module==1)
//            pixmap=new QPixmap(14,14);
//        else if(module==2)
//            pixmap=new QPixmap(19,19);
//        else
            pixmap=new QPixmap(9,9);
        pixmap->fill(Qt::transparent);
        QPainter p(pixmap);
        svgRender->render(&p);
        this->setIcon(QIcon(*pixmap));

        is_pressed=true;
    }
}

void ToolButton::mouseReleaseEvent(QMouseEvent *event)
{
    QByteArray byte=hoverbg.toLocal8Bit();
    char* hover=byte.data();
    char style[100];
//    if(module==2)
//        sprintf(style,"border:0px;padding-left:16px;background-color:%s;color:#ffffff;font-size:14px;",hover);
//    else
        sprintf(style,"border:0px;border-radius:4px;padding-left:0px;background-color:%s;",hover);
    if(event->button()==Qt::LeftButton)
    {
        this->setStyleSheet(QString::fromLocal8Bit(style));
        delete svgRender;
        svgRender=new QSvgRenderer(this);
        svgRender->load(piconstr);
//        if(module==1)
//            pixmap=new QPixmap(14,14);
//        else if(module==2)
//            pixmap=new QPixmap(19,19);
//        else
            pixmap=new QPixmap(9,9);
        pixmap->fill(Qt::transparent);
        QPainter p(pixmap);
        svgRender->render(&p);
        this->setIcon(QIcon(*pixmap));
//        if(module==2)
//            emit buttonClicked(this);
//        else
            emit clicked();
    }
}



