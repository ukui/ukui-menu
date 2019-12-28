#include "fullitemdelegate.h"
#include <QDebug>

FullItemDelegate::FullItemDelegate(QObject *parent, int module):
    QStyledItemDelegate(parent)
{
    this->module=module;
    QString path=QDir::homePath()+"/.config/ukui-menu/ukui-menu.ini";
    setting=new QSettings(path,QSettings::IniFormat);


}

FullItemDelegate::~FullItemDelegate()
{

}

void FullItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.isValid())
    {
        painter->save();
        QRectF rect;
        rect.setX(option.rect.x());
        rect.setY(option.rect.y());
        rect.setWidth( option.rect.width());
        rect.setHeight(option.rect.height());

        //QPainterPath画圆角矩形
        const qreal radius = 7;
        QPainterPath path;
        path.moveTo(rect.topRight() - QPointF(radius, 0));
        path.lineTo(rect.topLeft() + QPointF(radius, 0));
        path.quadTo(rect.topLeft(), rect.topLeft() + QPointF(0, radius));
        path.lineTo(rect.bottomLeft() + QPointF(0, -radius));
        path.quadTo(rect.bottomLeft(), rect.bottomLeft() + QPointF(radius, 0));
        path.lineTo(rect.bottomRight() - QPointF(radius, 0));
        path.quadTo(rect.bottomRight(), rect.bottomRight() + QPointF(0, -radius));
        path.lineTo(rect.topRight() + QPointF(0, radius));
        path.quadTo(rect.topRight(), rect.topRight() + QPointF(-radius, -0));

//        if(option.state.testFlag(QStyle::State_Selected))
//        {
//            painter->setPen(QPen(Qt::blue));
//            painter->setBrush(QColor(229, 241, 255));
//            painter->drawPath(path);
//        }
//        else if(option.state.testFlag(QStyle::State_MouseOver))
//        {
//            painter->setPen(QPen(Qt::green));
//            painter->setBrush(Qt::NoBrush);
//            painter->drawPath(path);
//        }
//        else{
//            painter->setPen(QPen(Qt::gray));
//            painter->setBrush(Qt::NoBrush);
//            painter->drawPath(path);
//        }

        painter->setRenderHint(QPainter::Antialiasing);
        if(option.state & QStyle::State_MouseOver)
        {
            painter->setPen(QPen(Qt::NoPen));
            painter->setBrush(QColor(AppBtnHover));
            painter->setOpacity(0.4);
            painter->drawPath(path);

//            painter->setOpacity(1);
//            painter->setPen(QPen(QColor("#4877F2"),2));
//            painter->setBrush(Qt::NoBrush);
//            painter->drawPath(path);
        }

        painter->setOpacity(1);
        QIcon icon=index.data(Qt::DecorationRole).value<QIcon>();
        QString appname=index.data(Qt::DisplayRole).value<QString>();

        QFont font;
        QRect iconRect;
        if(QApplication::desktop()->width()*QApplication::desktop()->height() <= 1600*900)
        {
            font.setPixelSize(12);
            iconRect=QRect(rect.x()+32,rect.y()+15,64,64);
        }
        else
        {
            font.setPixelSize(14);
            iconRect=QRect(rect.x()+40,rect.y()+20,80,80);
        }
        painter->setFont(font);
        icon.paint(painter,iconRect);
        if(module==0)
        {
            setting->beginGroup("application");
            if(setting->value(appname).toInt()==0)
            {
                QIcon icon(QString(":/data/img/mainviewwidget/lock.svg"));
                painter->drawImage(QRect(iconRect.topRight().x()-8,iconRect.topRight().y(),16,16),
                                   icon.pixmap(icon.actualSize(QSize(16, 16))).toImage());
            }
            setting->endGroup();

        }

        painter->setPen(QPen(Qt::white));
        QRect textRect;
        if(QApplication::desktop()->width()*QApplication::desktop()->height() <= 1600*900)
        {
            textRect=QRect(rect.x(),iconRect.bottom()+14,
                           rect.width(),rect.height()-iconRect.height()-14-22);
        }
        else{
            textRect=QRect(rect.x(),iconRect.bottom()+22,
                           rect.width(),rect.height()-iconRect.height()-22-27);
        }
        painter->drawText(textRect,Qt::TextWordWrap |Qt::AlignCenter,appname);

        painter->restore();

    }
}
