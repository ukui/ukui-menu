#ifndef RIGHTITEMDELEGATE_H
#define RIGHTITEMDELEGATE_H
#include <QStyledItemDelegate>
#include "style.h"
#include <QPainter>
#include <QPainterPath>
#include <QSettings>
#include <QDir>
#include <QToolTip>
#include <QHelpEvent>
#include "ukuimenuinterface.h"
#include "style.h"
#include "kitemdelegate.h"

class RightItemDelegate : public KItemDelegate
{
    Q_OBJECT
public:
    RightItemDelegate(QObject *parent);
    ~RightItemDelegate();
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // FULLITEMDELEGATE_H
