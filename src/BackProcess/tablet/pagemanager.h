#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H

#include <QObject>
#include "ukuimenuinterface.h"

class PageManager : public QObject
{
    Q_OBJECT
public:
    PageManager();

    int getPageNum(const int &appNum);
    QVector<QStringList> sortAppInPage(const QVector<QString> &appVector);
    QVector<QStringList> getAppPageVector();
private:
    UkuiMenuInterface *m_ukuiMenuInterface = nullptr;
};

#endif // PAGEMANAGER_H
