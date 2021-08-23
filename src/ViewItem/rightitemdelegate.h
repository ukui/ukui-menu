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

class RightItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    RightItemDelegate(QObject* parent);
    ~RightItemDelegate();
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    int module=0;
    UkuiMenuInterface* pUkuiMenuInterface=nullptr;

protected:
//    bool helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index);
};

#endif // FULLITEMDELEGATE_H
