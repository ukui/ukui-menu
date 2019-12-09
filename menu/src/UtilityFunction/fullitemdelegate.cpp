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
        rect.setWidth( option.rect.width()-1);
        rect.setHeight(option.rect.height()-1);

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

//        if(option.state & QStyle::State_MouseOver)
//        {
//            painter->setPen(QPen(Qt::NoPen));
//            painter->setBrush(QColor(MAINVIEWBTNHOVER));
//            painter->drawPath(path);
//        }

        QIcon icon=index.data(Qt::DecorationRole).value<QIcon>();
        QString appname=index.data(Qt::DisplayRole).value<QString>();

        QFont font;
        font.setPixelSize(14);
        painter->setFont(font);
        QRect iconRect=QRect(rect.x()+(rect.width()-50)/2,rect.y()+10,50,50);
        painter->drawImage(iconRect,icon.pixmap(icon.actualSize(QSize(32, 32))).toImage());
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
        painter->drawText(QRect(rect.x(),iconRect.bottom()+10,
                                rect.width(),rect.height()-iconRect.height()-10-10),Qt::TextWordWrap |Qt::AlignCenter,appname);

        painter->restore();

    }
}
