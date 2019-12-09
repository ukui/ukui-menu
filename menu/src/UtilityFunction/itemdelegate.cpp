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
//    QStyleOptionButton* button=m_btn.value(index);
//    if(!button)
//    {
//        auto opt = option;
////        QStyleOptionViewItem myOption = option;
//         initStyleOption(&opt,index);
//        opt.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;
//        opt.decorationAlignment = Qt::AlignLeft;
//        button = new QStyleOptionButton();
//        button->rect = opt.rect;
////        button->direction=Qt::LayoutDirection::LeftToRight;
//        QStringList strlist=index.model()->data(index,Qt::DisplayRole).toStringList();
//        QString str=strlist.at(1);
//        if(str.toInt()==0)
//            button->text=strlist.at(0);
//        else{
////            QString iconstr=pUkuiMenuInterface->get_app_icon(strlist.at(0));
//            QString appname=pUkuiMenuInterface->get_app_name(strlist.at(0));
////            QIcon icon=QIcon::fromTheme(iconstr);
//            QIcon icon=index.model()->data(index,Qt::DisplayRole).value<QIcon>();
//            button->icon=icon;
//            button->iconSize=QSize(32,32);
//            button->text=appname;
//            button->direction=Qt::LeftToRight;
//            button->palette.setColor(QPalette::Button, Qt::transparent);
//        }


//    }

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
        font.setPixelSize(14);
        painter->setFont(font);

        QIcon icon=index.model()->data(index,Qt::DecorationRole).value<QIcon>();
        QStringList strlist=index.model()->data(index,Qt::DisplayRole).toStringList();
//        if(!icon.isNull())
//            painter->fillRect(QRect(rect.x(),rect.y(),40,40),QColor(Qt::blue));//图片加背景

        if(module>0)
        {
            if(strlist.at(1).toInt()==1)
            {
                QRect iconRect=QRect(rect.left()+15,rect.y()+(rect.height()-32)/2,32,32);
                painter->drawImage(iconRect,icon.pixmap(icon.actualSize(QSize(32, 32))).toImage());
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
                painter->setPen(QColor("#626c6e"));
                painter->drawLine(QPoint(textRect.right()+6,rect.y()+rect.height()/2),
                                  QPoint(rect.width()-2,rect.y()+rect.height()/2));
            }

        }
        else
        {
            setting->beginGroup("application");
            QRect iconRect=QRect(rect.left()+15,rect.y()+(rect.height()-32)/2,32,32);
            painter->drawImage(iconRect,icon.pixmap(icon.actualSize(QSize(32, 32))).toImage());

            QString appname=pUkuiMenuInterface->get_app_name(strlist.at(0));
            if(setting->value(appname).toInt()==0)
            {
                QIcon icon(QString(":/data/img/mainviewwidget/lock.svg"));
                painter->drawImage(QRect(iconRect.topRight().x()-8,iconRect.topRight().y(),16,16),
                                   icon.pixmap(icon.actualSize(QSize(16, 16))).toImage());
            }
            painter->setPen(QPen(Qt::white));
            painter->drawText(QRect(iconRect.right()+15,rect.y(),
                                    rect.width()-62,rect.height()),Qt::AlignVCenter,appname);

            setting->endGroup();
        }
        painter->restore();
    }

}
