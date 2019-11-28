#include "classifybutton.h"
#include <QDebug>

ClassifyButton::ClassifyButton(QString name,int classify,QString iconstr)
{
    this->name=name;
    this->classify=classify;
    this->iconstr=iconstr;
    init_btn();
}

void ClassifyButton::init_btn()
{
    if(classify==0)//字母分类按钮
    {
        this->setFixedSize(30,25);
        this->setText(name);

        this->setCheckable(true);
        char btnstyle[500];
        sprintf(btnstyle,"QToolButton{background:transparent;color:#8b8b8b;font-size:14px;padding-left:0px;}\
                QToolButton:hover{background-color:%s;color:#ffffff;font-size:14px;}\
                QToolButton:pressed{background-color:%s;color:#8b8b8b;font-size:14px;}\
                QToolButton:checked{background:transparent;color:#ffffff;font-size:14px;}",MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);
        this->setStyleSheet(btnstyle);
    }
    else {//功能分类按钮
//        char btnstyle[400];
//        sprintf(btnstyle,"QToolButton{background:transparent;padding-left:0px;}\
//                QToolButton:hover{background-color:%s;}\
//                QToolButton:pressed{background-color:%s;}",MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);
//        this->setFixedSize(40,30);
//        QSvgRenderer* svgRender = new QSvgRenderer;
//        svgRender->load(iconstr);
//        QPixmap* pixmap = new QPixmap(19,19);
//        pixmap->fill(Qt::transparent);//设置背景透明
//        QPainter p(pixmap);
//        svgRender->render(&p);
//        this->setIcon(QIcon(*pixmap));
//        this->setStyleSheet(btnstyle);
    }

}
