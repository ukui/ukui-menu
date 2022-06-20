#include "buriedpointdatasend.h"
#include <QHostInfo>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDateTime>
#include <QTimeZone>

Q_GLOBAL_STATIC(BuriedPointDataSend, buriedPointDataSend)

BuriedPointDataSend::BuriedPointDataSend()
{
#ifdef hasUploadInterface
    QString path = QDir::homePath() + "/menuUploadMessage/";
    QPluginLoader pluginLoder("/usr/lib/libupload-plugin-interface.so");
    QObject *plugin = pluginLoder.instance();
    if (plugin) {
        qDebug() << "插件加载成功";
        m_sendDataInterface = qobject_cast<UploadPluginInterface *>(plugin);
        m_sendDataInterface->initInterface(getpid(), "ukui-menu", "menuData", path);
        m_pluginAvailable = true;
    }
#else
    myDebug() << "埋点数据上传依赖不存在，不执行后续操作";
#endif
}

BuriedPointDataSend::~BuriedPointDataSend()
{
#ifdef hasUploadInterface
    if (m_sendDataInterface != nullptr) {
        delete  m_sendDataInterface;
        m_sendDataInterface = nullptr;
    }
#endif
}

BuriedPointDataSend *BuriedPointDataSend::getInstance()
{
    return buriedPointDataSend();
}

QString BuriedPointDataSend::getCurrentTime()
{
    QString tempDateTimeStr;
    QDateTime tempDateTime = QDateTime::currentDateTime().toTimeZone(QTimeZone(8 * 3600));
    tempDateTimeStr = tempDateTime.toString("yyyy-MM-dd HH:mm:ss.zzz");
    return tempDateTimeStr;
}

void BuriedPointDataSend::setPoint(const pointDataStruct &data)
{
#ifdef hasUploadInterface
    if (m_pluginAvailable) {
        int curNum = 1;
        QStringList applist = QStringList();
        QString keyValue = data.module + data.function;

        if (m_functionCount.keys().contains(keyValue)) {
            curNum = m_functionCount.value(keyValue);
            curNum ++;
        }

        m_functionCount.insert(keyValue, curNum);

        if (!data.otherFunction.isEmpty()) {
            if (m_applist.keys().contains(keyValue)) {
                applist = m_applist.value(keyValue);
            }

            applist.append(data.otherFunction.at(0));
            m_applist.insert(keyValue, applist);
        }

        QString time = getCurrentTime();
        QJsonObject jsonObj;
        QJsonArray otherFunction = QJsonArray::fromStringList(data.otherFunction);
        jsonObj.insert("module", QJsonValue(data.module));
        jsonObj.insert("function", QJsonValue(data.function));
        jsonObj.insert("functionNum", QJsonValue(QString::number(curNum)));
        jsonObj.insert("otherFunction", otherFunction);
        jsonObj.insert("errorLevel", QJsonValue(data.errorLevel));
        jsonObj.insert("errorOutput", QJsonValue(data.errorOutput));
        jsonObj.insert("createTimeStamp", QJsonValue(time));//注意该字段名称不能修改，否则会报invalid
        //  将数据转化为QString
        QString informationData(QJsonDocument(jsonObj).toJson(QJsonDocument::Compact));
        qDebug() << "jsonObj:" << jsonObj;
        m_sendDataInterface->uploadMessage(informationData);
    }
#endif
}


