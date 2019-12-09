#ifndef SEARCHAPPTHREAD_H
#define SEARCHAPPTHREAD_H
#include <QThread>
#include <ukuimenuinterface.h>
#include <QObject>

class SearchAppThread : public QThread
{
    Q_OBJECT
public:
    SearchAppThread();
    ~SearchAppThread();
    void run();

private:
    QString keyword;
    QStringList appnamepyList;
    QStringList appnameList;
    QStringList searchResultList;
    UkuiMenuInterface* pUkuiMenuInterface=nullptr;

private slots:
    void recv_search_keyword(QString arg);

signals:
    void send_search_result(QStringList list);

};

#endif // SEARCHAPPTHREAD_H
