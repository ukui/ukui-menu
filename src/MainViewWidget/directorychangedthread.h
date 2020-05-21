#ifndef DIRECTORYCHANGEDTHREAD_H
#define DIRECTORYCHANGEDTHREAD_H
#include <QThread>
#include <QObject>
#include <QSettings>
#include "src/Interface/ukuimenuinterface.h"

class DirectoryChangedThread: public QThread
{
    Q_OBJECT
public:
    DirectoryChangedThread();
    ~DirectoryChangedThread();
    void run();

private:
    UkuiMenuInterface* pUkuiMenuInterface=nullptr;
    QString path;
    QSettings* setting=nullptr;

public Q_SLOTS:
    void recvDirectoryPath(QString arg);

Q_SIGNALS:
    void requestUpdateSignal();//desktop文件目录改变信号

};

#endif // DIRECTORYCHANGEDTHREAD_H
