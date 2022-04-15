#ifndef UPLOADMESSAGE_H
#define UPLOADMESSAGE_H
#include <QCoreApplication>

#if defined(UPLOADDATA_LIBRARY)
    #define UPLOADDATA_EXPORT Q_DECL_EXPORT
#else
    #define UPLOADDATA_EXPORT Q_DECL_IMPORT
#endif

class KUploadMessage;

class UPLOADDATA_EXPORT UploadMessageInterface
{
public:
    /**
     * @brief UploadMessageInterface
     * @param ppid 当前进程id
     * @param packageName 包名
     * @param messageType 消息类型
     * @param packageInfoIdFolderPath 路径
     */
    UploadMessageInterface(int ppid, QString packageName, QString messageType, QString packageInfoIdFolderPath);

    ~UploadMessageInterface();

    /**
     * @brief UploadMessage
     * @param uploadedMessage 数据
     * @return -1 异常 0 正常
     */
    int UploadMessage(const QString &uploadedMessage);
private:
    KUploadMessage *d = nullptr;
};

#endif // UPLOADMESSAGE_H
