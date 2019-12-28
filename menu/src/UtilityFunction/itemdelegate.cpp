#include "itemdelegate.h"
#include <QPushButton>

ItemDelegate::ItemDelegate(QObject* parent, int module):
    QStyledItemDelegate(parent)
{
    this->module=module;
    QString path=QDir::homePath()+"/.config/ukui-menu/ukui-menu.ini";
    setting=new QSettings(path,QSettings::IniFormat);
    pUkuiMenuInterface=new UkuiMenuInterface;

}

ItemDelegate::~ItemDelegate()
{
    delete pUkuiMenuInterface;
}

void ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.isValid())
    {
        painter->save();

        QRectF rect;
        rect.setX(option.rect.x());
        rect.setY(option.rect.y());
        rect.setWidth(option.rect.width());
        rect.setHeight(option.rect.height());

        QFont font;
//            font.setFamily("Microsoft YaHei");
        if(QApplication::desktop()->width()*QApplication::desktop()->height() <= 1600*900)
            font.setPixelSize(12);
        else
            font.setPixelSize(14);
        painter->setFont(font);

        QIcon icon=index.model()->data(index,Qt::DecorationRole).value<QIcon>();
        QStringList strlist=index.model()->data(index,Qt::DisplayRole).toStringList();
//        if(!icon.isNull())
//            painter->fillRect(QRect(rect.x(),rect.y(),40,40),QColor(Qt::blue));//图片加背景

        if(option.state & QStyle::State_MouseOver)
        {
            if(strlist.at(1).toInt()==1)
            {
                painter->setPen(QPen(Qt::NoPen));
                painter->setBrush(QColor(AppBtnHover));
                painter->setOpacity(0.4);
                painter->drawRect(rect);
            }
            else
            {
                painter->setPen(QPen(Qt::NoPen));
                painter->setBrush(QColor(AppBtnHover));
                painter->setOpacity(0.4);
                painter->drawRect(rect);
            }

        }
        painter->setOpacity(1);

        if(module>0)
        {
            if(strlist.at(1).toInt()==1)
            {
                QRect iconRect=QRect(rect.left()+15,rect.y()+(rect.height()-32)/2,32,32);
                icon.paint(painter,iconRect);
                painter->setPen(QPen(Qt::white));
                QString appname=pUkuiMenuInterface->get_app_name(strlist.at(0));
                painter->drawText(QRect(iconRect.right()+15,rect.y(),
                                        rect.width()-62,rect.height()),Qt::AlignVCenter,appname);

            }
            else
            {
                painter->setPen(QPen(Qt::white));
                QRect textRect=QRect(rect.x()+15,rect.y()+(rect.height()-14)/2,strlist.at(0).size()*14,14);
                painter->drawText(textRect,Qt::AlignVCenter,strlist.at(0));
                painter->setRenderHint(QPainter::Antialiasing, true);
                painter->setPen(QPen(QColor("#FFFFFF"),1));
                painter->setOpacity(0.06);
                painter->drawLine(QPoint(5,rect.bottom()),QPoint(rect.right(),rect.bottom()));
            }

        }
        else
        {
            setting->beginGroup("application");
            QRect iconRect=QRect(rect.left()+15,rect.y()+(rect.height()-28)/2,28,28);
            icon.paint(painter,iconRect);

            QString appname=pUkuiMenuInterface->get_app_name(strlist.at(0));
            if(setting->value(appname).toInt()==0)
            {
                QIcon icon(QString(":/data/img/mainviewwidget/lock.svg"));
                icon.paint(painter,QRect(iconRect.topRight().x()-8,iconRect.topRight().y(),16,16));
            }
            painter->setOpacity(1);
            painter->setPen(QPen(Qt::white));
            painter->drawText(QRect(iconRect.right()+15,rect.y(),
                                    rect.width()-62,rect.height()),Qt::AlignVCenter,appname);

            setting->endGroup();
        }
        painter->restore();
    }

}
