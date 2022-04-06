#ifndef BURIEDPOINTDATASEND_H
#define BURIEDPOINTDATASEND_H

#include <QObject>
#include <QDateTime>
#include <QDir>
#include <uploadmessage_interface.h>
#include "src/UtilityFunction/utility.h"
#include "unistd.h"

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
    UploadMessageInterface *m_sendDataInterface = nullptr;
};

#endif // BURIEDPOINTDATASEND_H
