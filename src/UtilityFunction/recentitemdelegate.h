#ifndef RECENTITEMDELEGATE_H
#define RECENTITEMDELEGATE_H
#include "itemdelegate.h"
#include <QPushButton>
#include <syslog.h>
#include <QToolTip>
#include "src/UtilityFunction/utility.h"

class recentitemdelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    /**
     * @param module为0时为常用模块，1为字母模块、2为功能模块、3为搜索模块
     */
    recentitemdelegate(QObject* parent,int module);
    ~recentitemdelegate();
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    int module=0;
    UkuiMenuInterface* pUkuiMenuInterface=nullptr;
};

#endif // RECENTITEMDELEGATE_H