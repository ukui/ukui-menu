#ifndef RIGHTITEMDELEGATE_H
#define RIGHTITEMDELEGATE_H
#include <QStyledItemDelegate>
#include "src/Style/style.h"
#include <QPainter>
#include <QPainterPath>
#include <QSettings>
#include <QDir>
#include <QToolTip>
#include <QHelpEvent>
#include "src/Interface/ukuimenuinterface.h"
#include "src/Style/style.h"
#include "kitemdelegate.h"

class RightItemDelegate : public KItemDelegate
{
    Q_OBJECT
public:
    RightItemDelegate(QObject* parent);
    ~RightItemDelegate();
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // FULLITEMDELEGATE_H
