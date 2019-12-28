#include "functionclassifybutton.h"
#include <QDebug>

FunctionClassifyButton::FunctionClassifyButton(QWidget *parent,
                       int width,
                       int height,
                       QString iconstr,
                       QString piconstr,
                       QString hoverbg,
                       QString pressedbg,
                       int module,
                       QString text):
    QPushButton (parent)
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
    pixmap=new QPixmap(19,19);
    pixmap->fill(Qt::transparent);
    QPainter p(pixmap);
    svgRender->render(&p);
    iconlabel=new QLabel(this);
    iconlabel->setFixedSize(pixmap->size());
    iconlabel->setPixmap(*pixmap);
    iconlabel->setStyleSheet("background:transparent;");

    textlabel=new QLabel(this);
    textlabel->setText(this->text);
    textlabel->setStyleSheet("background:transparent; color:rgba(255, 255, 255, 50%);font-size:14px;");
    textlabel->adjustSize();

    mainlayout=new QHBoxLayout;
    mainlayout->setContentsMargins(16,0,0,0);
    mainlayout->setSpacing(10);
    this->setLayout(mainlayout);
    mainlayout->addWidget(iconlabel);
    mainlayout->addWidget(textlabel);

//    if(module==2)
//    {
//        this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
//        this->setText(tr(text.toLocal8Bit().data()));
//        this->setStyleSheet("border:0px;padding-left:16px;background:transparent;color:rgba(255, 255, 255, 50%);font-size:14px;");
//    }
//    else
//        this->setStyleSheet("border:0px;padding-left:0px;background:transparent;");
}

void FunctionClassifyButton::enterEvent(QEvent *e)
{
    Q_UNUSED(e);
    QByteArray byte=hoverbg.toLocal8Bit();
    char* hover=byte.data();
    char style[100];
//    if(module==2)
//        sprintf(style,"border:0px;padding-left:16px;background-color:%s;color:#ffffff;font-size:14px;",hover);
//    else
    sprintf(style,"border:0px;padding-left:0px;background-color:%s;",hover);
    this->setStyleSheet(QString::fromLocal8Bit(style));
    delete svgRender;
    svgRender=new QSvgRenderer(this);
    svgRender->load(piconstr);
    pixmap=new QPixmap(19,19);
    pixmap->fill(Qt::transparent);
    QPainter p(pixmap);
    svgRender->render(&p);
    iconlabel->setPixmap(*pixmap);
    textlabel->setStyleSheet("background:transparent;color:#ffffff;font-size:14px;");
}

void FunctionClassifyButton::leaveEvent(QEvent *e)
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
    this->setStyleSheet("border:0px;padding-left:0px;background:transparent;");
    delete svgRender;
    svgRender=new QSvgRenderer(this);
    if(module==2 && is_pressed)
        svgRender->load(piconstr);
    else
        svgRender->load(iconstr);
    pixmap=new QPixmap(19,19);
    pixmap->fill(Qt::transparent);
    QPainter p(pixmap);
    svgRender->render(&p);
    iconlabel->setPixmap(*pixmap);
    if(is_pressed)
        textlabel->setStyleSheet("background:transparent;color:#ffffff;font-size:14px;");
    else
        textlabel->setStyleSheet("background:transparent; color:rgba(255, 255, 255, 50%);font-size:14px;");
}

void FunctionClassifyButton::mousePressEvent(QMouseEvent *event)
{
    QByteArray byte=pressedbg.toLocal8Bit();
    char* pressed=byte.data();
    char style[100];
//    if(module==2)
//        sprintf(style,"border:0px;padding-left:16px;background-color:%s;color:#ffffff;font-size:14px;",pressed);
//    else
    sprintf(style,"border:0px;padding-left:0px;background-color:%s;",pressed);
    if(event->button()==Qt::LeftButton)
    {
        this->setStyleSheet(QString::fromLocal8Bit(style));
        delete svgRender;
        svgRender=new QSvgRenderer(this);
        svgRender->load(piconstr);
        pixmap=new QPixmap(19,19);
        pixmap->fill(Qt::transparent);
        QPainter p(pixmap);
        svgRender->render(&p);
        iconlabel->setPixmap(*pixmap);
        textlabel->setStyleSheet("background:transparent;color:#ffffff;font-size:14px;");
        is_pressed=true;
    }
}

void FunctionClassifyButton::mouseReleaseEvent(QMouseEvent *event)
{
    QByteArray byte=hoverbg.toLocal8Bit();
    char* hover=byte.data();
    char style[100];
//    if(module==2)
//        sprintf(style,"border:0px;padding-left:16px;background-color:%s;color:#ffffff;font-size:14px;",hover);
//    else
    sprintf(style,"border:0px;padding-left:0px;background-color:%s;",hover);
    if(event->button()==Qt::LeftButton)
    {
        this->setStyleSheet(QString::fromLocal8Bit(style));
        delete svgRender;
        svgRender=new QSvgRenderer(this);
        svgRender->load(piconstr);
        pixmap=new QPixmap(19,19);
        pixmap->fill(Qt::transparent);
        QPainter p(pixmap);
        svgRender->render(&p);
        iconlabel->setPixmap(*pixmap);
        textlabel->setStyleSheet("background:transparent;color:#ffffff;font-size:14px;");
        emit buttonClicked(this);
    }
}



