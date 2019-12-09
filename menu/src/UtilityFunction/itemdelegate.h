#ifndef ITEMDELEGATE_H
#define ITEMDELEGATE_H
#include <QStyledItemDelegate>
#include "src/color.h"
#include <QPainter>
#include <QSettings>
#include <ukuimenuinterface.h>

class ItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    /**
     * @param module为0时为常用模块，1为字母模块、2为功能模块
     */
    ItemDelegate(QObject* parent,int module);
    ~ItemDelegate();
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    int module=0;
    QSettings *setting=nullptr;
    UkuiMenuInterface* pUkuiMenuInterface=nullptr;

};

#endif // ITEMDELEGATE_H
