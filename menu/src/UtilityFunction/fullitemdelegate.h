#ifndef FULLITEMDELEGATE_H
#define FULLITEMDELEGATE_H
#include <QStyledItemDelegate>
#include "src/color.h"
#include <QPainter>
#include <QSettings>
#include <QDir>

class FullItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    FullItemDelegate(QObject* parent,int module);
    ~FullItemDelegate();
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    int module=0;
    QSettings* setting=nullptr;
};

#endif // FULLITEMDELEGATE_H
