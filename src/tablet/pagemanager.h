#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H

#include <QObject>
#include "src/Interface/ukuimenuinterface.h"

class PageManager : public QObject
{
    Q_OBJECT
public:
    PageManager();

    int getPageNum(int appnum);
    QVector<QStringList> sortAppInPage(QVector<QString> appvector);
    QVector<QStringList> getAppPageVector();
private:
    UkuiMenuInterface *m_ukuiMenuInterface = nullptr;
};

#endif // PAGEMANAGER_H
