#include "right_item_delegate.h"
#include "src/UtilityFunction/utility.h"
#include <QDebug>

RightItemDelegate::RightItemDelegate(QObject *parent):
    KItemDelegate(parent)
{
    pUkuiMenuInterface = new UkuiMenuInterface;
}

RightItemDelegate::~RightItemDelegate()
{
    delete pUkuiMenuInterface;
}

void RightItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid()) {
        QStyleOptionViewItem viewOption(option);//用来在视图中画一个item
        QRectF rect;
        rect.setX(option.rect.x());
        rect.setY(option.rect.y());
        rect.setWidth(option.rect.width());
        rect.setHeight(option.rect.height());
        //QPainterPath画圆角矩形
        const qreal radius = 10;
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
        painter->setRenderHint(QPainter::Antialiasing);
        painter->save();
        painter->setPen(QPen(Qt::NoPen));
        painter->setBrush(Qt::black);

        if ((option.state & QStyle::State_MouseOver) || (option.state & QStyle::State_HasFocus)) {
            painter->setOpacity(0.6);
        }
        //        else if(option.state & QStyle::State_DownArrow)
        //        {
        //            painter->setOpacity(0.8);
        //        }
        else {
            painter->setOpacity(0.4);
        }

        painter->drawPath(path);
        painter->restore();
        painter->save();
        painter->setOpacity(1);
        QString desktopfp = index.data(Qt::DisplayRole).value<QString>();
        QString iconstr = pUkuiMenuInterface->getAppIcon(desktopfp);
        QIcon icon;
        QFileInfo iconFileInfo(iconstr);

        if (iconFileInfo.isFile() && (iconstr.endsWith(".png") || iconstr.endsWith(".svg"))) {
            icon = QIcon(iconstr);
        } else {
            iconstr.remove(".png");
            iconstr.remove(".svg");
            icon = QIcon::fromTheme(iconstr);

            if (icon.isNull()) {
                if (QFile::exists(QString("/usr/share/icons/hicolor/scalable/apps/%1.%2").arg(iconstr).arg("svg"))) {
                    icon = QIcon(QString("/usr/share/icons/hicolor/scalable/apps/%1.%2").arg(iconstr).arg("svg"));
                } else if (QFile::exists(QString("/usr/share/icons/hicolor/scalable/apps/%1.%2").arg(iconstr).arg("png"))) {
                    icon = QIcon(QString("/usr/share/icons/hicolor/scalable/apps/%1.%2").arg(iconstr).arg("png"));
                } else if (QFile::exists(QString("/usr/share/icons/hicolor/128x128/apps/%1.%2").arg(iconstr).arg("png"))) {
                    icon = QIcon(QString("/usr/share/icons/hicolor/128x128/apps/%1.%2").arg(iconstr).arg("png"));
                } else if (QFile::exists(QString("/usr/share/icons/hicolor/128x128/apps/%1.%2").arg(iconstr).arg("svg"))) {
                    icon = QIcon(QString("/usr/share/icons/hicolor/128x128/apps/%1.%2").arg(iconstr).arg("svg"));
                } else if (QFile::exists(QString("/usr/share/icons/hicolor/96x96/apps/%1.%2").arg(iconstr).arg("png"))) {
                    icon = QIcon(QString("/usr/share/icons/hicolor/96x96/apps/%1.%2").arg(iconstr).arg("png"));
                } else if (QFile::exists(QString("/usr/share/icons/hicolor/96x96/apps/%1.%2").arg(iconstr).arg("svg"))) {
                    icon = QIcon(QString("/usr/share/icons/hicolor/96x96/apps/%1.%2").arg(iconstr).arg("svg"));
                } else if (QFile::exists(QString("/usr/share/icons/hicolor/64x64/apps/%1.%2").arg(iconstr).arg("png"))) {
                    icon = QIcon(QString("/usr/share/icons/hicolor/64x64/apps/%1.%2").arg(iconstr).arg("png"));
                } else if (QFile::exists(QString("/usr/share/icons/hicolor/64x64/apps/%1.%2").arg(iconstr).arg("svg"))) {
                    icon = QIcon(QString("/usr/share/icons/hicolor/64x64/apps/%1.%2").arg(iconstr).arg("svg"));
                } else if (QFile::exists(QString("/usr/share/icons/hicolor/48x48/apps/%1.%2").arg(iconstr).arg("png"))) {
                    icon = QIcon(QString("/usr/share/icons/hicolor/48x48/apps/%1.%2").arg(iconstr).arg("png"));
                } else if (QFile::exists(QString("/usr/share/icons/hicolor/48x48/apps/%1.%2").arg(iconstr).arg("svg"))) {
                    icon = QIcon(QString("/usr/share/icons/hicolor/48x48/apps/%1.%2").arg(iconstr).arg("svg"));
                } else if (QFile::exists(QString("/usr/share/icons/hicolor/32x32/apps/%1.%2").arg(iconstr).arg("png"))) {
                    icon = QIcon(QString("/usr/share/icons/hicolor/32x32/apps/%1.%2").arg(iconstr).arg("png"));
                } else if (QFile::exists(QString("/usr/share/icons/hicolor/32x32/apps/%1.%2").arg(iconstr).arg("svg"))) {
                    icon = QIcon(QString("/usr/share/icons/hicolor/32x32/apps/%1.%2").arg(iconstr).arg("svg"));
                } else if (QFile::exists(QString("/usr/share/pixmaps/%1.%2").arg(iconstr).arg("png"))) {
                    icon = QIcon(QString("/usr/share/pixmaps/%1.%2").arg(iconstr).arg("png"));
                } else if (QFile::exists(QString("/usr/share/pixmaps/%1.%2").arg(iconstr).arg("svg"))) {
                    icon = QIcon(QString("/usr/share/pixmaps/%1.%2").arg(iconstr).arg("svg"));
                } else {
                    icon = QIcon::fromTheme(QString("application-x-desktop"));
                }
            }
        }

        QString appname = pUkuiMenuInterface->getAppName(desktopfp);
        QRect iconRect;
        QRect textRect;
        iconRect = QRect(rect.x() + 28,
                         rect.y() + 16,
                         48,
                         48);
        icon.paint(painter, iconRect);
        painter->restore();
        painter->save();
        bool is_locked = false;
        QFileInfo fileInfo(desktopfp);
        QString desktopfn = fileInfo.fileName();
        painter->restore();
        textRect = QRect(rect.x(),
                         iconRect.bottom(),
                         rect.width(),
                         rect.height() - iconRect.height());
        painter->save();
        //添加最近安装蓝色标签
        bool is_recentapp = false;
        QFontMetrics fm = painter->fontMetrics();
        QString appnameElidedText = fm.elidedText(appname, Qt::ElideRight, rect.width() - 10, Qt::TextShowMnemonic);

        if (checkIfRecent(desktopfn) && !checkIfLocked(desktopfn)) {
            is_recentapp = true;
            appnameElidedText = fm.elidedText(appname, Qt::ElideRight, rect.width() - 23, Qt::TextShowMnemonic);
            textRect = QRect(rect.x() + 18,
                             iconRect.bottom() + Style::AppSpaceBetweenIconText,
                             rect.width() - 23,
                             rect.height() - iconRect.height() - Style::AppSpaceBetweenIconText);
            painter->setPen(QPen(Qt::NoPen));
            painter->setBrush(QColor("#4d94ff"));
            int x = 0;

            if (rect.width() < (23 + fm.boundingRect(appname).width())) {
                x = rect.x() + 9;
            } else {
                x = rect.x() + (rect.width() - 13 - fm.boundingRect(appname).width()) / 2 + 4;
            }

            painter->drawEllipse(QPoint(x,
                                        textRect.y() + (fm.boundingRect(appname).height() - 8) / 2 + 4),
                                 4,
                                 4);
        }

        painter->restore();
        painter->save();
        //        painter->setPen(QPen(option.palette.text().color()));
        painter->setPen(QPen(Qt::white));
        painter->setBrush(Qt::NoBrush);
        painter->drawText(textRect, Qt::AlignHCenter | Qt::AlignTop, appnameElidedText);
        //        painter->drawText(textRect,Qt::TextWordWrap |Qt::AlignHCenter,appname);
        painter->restore();

        if ((option.state & QStyle::State_MouseOver) || (option.state & QStyle::State_HasFocus)) {
            int len = 0;

            if (!is_locked && is_recentapp) {
                len = fm.boundingRect(appname).width() + 23;
            } else {
                len = fm.boundingRect(appname).width() + 10;
            }

            if (len > rect.width()) {
                if (option.state & QStyle::State_MouseOver) {
                    QToolTip::showText(QCursor::pos(), appname);
                } else {
                    QToolTip::showText(option.widget->mapToGlobal(QPoint(rect.center().x(), rect.center().y() + 15)), appname);
                }
            } else {
                QToolTip::hideText();
            }
        }
    }
}

QSize RightItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(104, 104);
}
