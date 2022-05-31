#ifndef BURIEDPOINTDATASEND_H
#define BURIEDPOINTDATASEND_H

#include <QObject>
#include <QDateTime>
#include <QDir>
#include "src/UtilityFunction/utility.h"
#include "unistd.h"

#  if __has_include(<uploadPluginInterface.h>)
#    define  hasUploadInterface
#    include <uploadPluginInterface.h>
#  endif

class BuriedPointDataSend : public QObject
{
    Q_OBJECT
public:
    BuriedPointDataSend();
    ~BuriedPointDataSend();

    static BuriedPointDataSend *getInstance();

    void setPoint(const pointDataStruct &data);

private:
    QString getCurrentTime();
    QHash<QString, int> m_functionCount;
    QHash<QString, QStringList> m_applist;
    UploadPluginInterface *m_sendDataInterface = nullptr;
    bool m_pluginAvailable = false;
};

#endif // BURIEDPOINTDATASEND_H
