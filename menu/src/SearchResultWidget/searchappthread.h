#ifndef SEARCHAPPTHREAD_H
#define SEARCHAPPTHREAD_H
#include <QThread>
#include "kylin-start-menu-interface.h"
#include <QObject>

class SearchAppThread : public QThread
{
    Q_OBJECT
public:
    SearchAppThread();
    void run();

private:
    QString keyword;
    QStringList appnameList;
    QStringList searchResultList;

private slots:
    void recv_search_keyword(QString arg);

signals:
    void send_search_result(QStringList list);

};

#endif // SEARCHAPPTHREAD_H
