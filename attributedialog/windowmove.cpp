#include "windowmove.h"
#include "ui_windowmove.h"

WindowMove::WindowMove(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WindowMove)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

//    this->resize(930-110,36);

}

WindowMove::~WindowMove()
{
    delete ui;
}

//设置窗口移动
void WindowMove::mousePressEvent(QMouseEvent *e)
{

    press_point = e->pos();
    is_move = true;
}

void WindowMove::mouseMoveEvent(QMouseEvent *e)
{
    if((e->buttons() == Qt::LeftButton) && is_move)
    {
        //        static QWidget* parent_widget = this->parentWidget();
        QPoint point = this->parentWidget()->pos();
        point.setX(point.x() + e->x() - press_point.x());
        point.setY(point.y() + e->y() - press_point.y());


        //设置临界值以阻止父窗口移出屏幕外
        int x=point.x() + e->x() - press_point.x();
        int y=point.y() + e->y() - press_point.y();
        if(x<0)
        {
            point.setX(0);
        }

        if(y<0)
        {
            point.setY(0);
        }

        QDesktopWidget *desktopWidget=QApplication::desktop();
        QRect screenRect = desktopWidget->screenGeometry();
        //        qDebug()<<screenRect.width()<<":"<<screenRect.height();
        int width=screenRect.width()-this->parentWidget()->width();
        int height=screenRect.height()-this->parentWidget()->height();

        //        qDebug()<<"屏幕分辨率："<<screenRect.width()<<":"<<screenRect.height();
        //        qDebug()<<"窗口移动"<<x<<":"<<y;
        //        qDebug()<<"窗口大小"<<this->width()<<":"<<this->height();
        //        qDebug()<<"屏幕减去父窗口大小"<<width<<":"<<height;
        if(x>width)
        {
            point.setX(width);
        }

        if(y>height)
        {
            point.setY(height);
        }

        this->parentWidget()->move(point);
    }

}

void WindowMove::mouseReleaseEvent(QMouseEvent *)
{
    if(is_move)
    {
        is_move = false;
    }
}
