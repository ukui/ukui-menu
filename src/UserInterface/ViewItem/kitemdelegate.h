#ifndef KITEMDELEGATE_H
#define KITEMDELEGATE_H
#include <QStyledItemDelegate>
#include "ukuimenuinterface.h"

class KItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    KItemDelegate(QObject *parent);

    int module = 0;
    UkuiMenuInterface *pUkuiMenuInterface = nullptr;
};

#endif // KITEMDELEGATE_H
