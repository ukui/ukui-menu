#include "getmodeldata.h"
#include <QTranslator>
#include <QDir>
#include "utility.h"

GetModelData::GetModelData()
{
    m_ukuiMenuInterface = new UkuiMenuInterface;
    loadDesktopVercor();
}

void GetModelData::loadDesktopVercor()
{
    UkuiMenuInterface::appInfoVector.clear();
    UkuiMenuInterface::alphabeticVector.clear();
    UkuiMenuInterface::functionalVector.clear();
    UkuiMenuInterface::allAppVector.clear();
    UkuiMenuInterface::collectAppVector.clear();
    UkuiMenuInterface::appInfoVector = m_ukuiMenuInterface->createAppInfoVector();
    UkuiMenuInterface::alphabeticVector = m_ukuiMenuInterface->getAlphabeticClassification();
    UkuiMenuInterface::functionalVector = m_ukuiMenuInterface->getFunctionalClassification();
    UkuiMenuInterface::allAppVector = m_ukuiMenuInterface->getAllClassification();
    UkuiMenuInterface::collectAppVector = m_ukuiMenuInterface->getCollectApp();
}

QVector<QStringList> GetModelData::getMinAllData()
{
    m_minAllData.clear();

    Q_FOREACH (QString desktopfp, UkuiMenuInterface::allAppVector) {
        m_minAllData.append(QStringList() << desktopfp << "1");
    }

    return m_minAllData;
}

QStringList GetModelData::getPreCollectionApp()
{
    //收藏区预置应用：设置、天气、软件商店、截图、文件管理器
    QStringList preAppList = QStringList();
    QStringList preAppListExist = QStringList();
    preAppList << QString("/usr/share/applications/ukui-control-center.desktop")
               <<QString("/usr/share/applications/kylin-weather.desktop")
               <<QString("/usr/share/applications/kylin-software-center.desktop")
               <<QString("/usr/share/applications/kylin-screenshot.desktop")
               <<QString("/usr/share/applications/peony.desktop");
    Q_FOREACH (QString appDesktop,preAppList) {
        if (UkuiMenuInterface::allAppVector.contains(appDesktop)) {
            preAppListExist.append(appDesktop);
            QFileInfo fileInfo(appDesktop);
            QString desktopfn = fileInfo.fileName();
            updateDataBaseCollect(desktopfn,1);
        }
    }

    return preAppListExist;
}

QStringList GetModelData::getcollectData()
{
    m_collectData.clear();

    Q_FOREACH (QString desktopfp, UkuiMenuInterface::collectAppVector) {
        m_collectData.append(QString(desktopfp));
    }

    return m_collectData;
}

bool GetModelData::cmpApp(QStringList &arg_1, QStringList &arg_2)
{
    if (arg_1.at(4) >= arg_2.at(4)) {
        return true;
    } else {
        return false;
    }
}


QVector<QStringList> GetModelData::getRecentData()
{
    QVector<QStringList> recentDataVector = QVector<QStringList>();
    enumerator = new Peony::FileEnumerator(this);
    enumerator->setEnumerateDirectory("recent:///");
    enumerator->enumerateSync();
    QString uri;

    for (auto fileInfo : enumerator->getChildren()) {
        QStringList recentData;
        Peony::FileInfoJob infoJob(fileInfo);
        infoJob.querySync();
        QString targetUri = fileInfo.get()->targetUri();
        QString displayName = fileInfo.get()->displayName();
        QString symlinkTarget = fileInfo.get()->symlinkTarget();
        QString iconName = fileInfo.get()->iconName();
        QString accessDate = fileInfo.get()->modifiedDate();
        recentData << targetUri << displayName << symlinkTarget << iconName << accessDate;

        if (!displayName.endsWith(".desktop")) {
            recentDataVector.append(recentData);
        }
    }

    qSort(recentDataVector.begin(), recentDataVector.end(), cmpApp);
    return recentDataVector;
}

QStringList GetModelData::getFuncClassificationList()
{
    return m_classificationList;
}

QStringList GetModelData::getLetterClassificationList()
{
    return m_letterList;
}

QStringList GetModelData::getFuncClassificationBtnRowList()
{
    return m_classificationBtnRowList;
}

QStringList GetModelData::getLetterClassificationBtnRowList()
{
    return m_letterBtnRowList;
}

QVector<QStringList> GetModelData::getMinLetterData()
{
    m_minLetterData.clear();
    m_letterList.clear();
    m_letterBtnRowList.clear();
    int row = 0;
    QVector<QStringList> vector = UkuiMenuInterface::alphabeticVector;

    for (int i = 0; i < vector.size(); i++) {
        QStringList appList = vector.at(i);

        if (!appList.isEmpty()) {
            QString letterstr;

            if (i < 26) {
                letterstr = QString(QChar(static_cast<char>(i + 65)));
            } else if (i == 26) {
                letterstr = "&";
            } else {
                letterstr = "#";
            }

            m_letterList.append(letterstr);//存储分类字符
            m_letterBtnRowList.append(QString::number(row));//存储分类字符所在行
            m_minLetterData.append(QStringList() << letterstr << "0");

            for (int i = 0; i < appList.count(); i++) {
                m_minLetterData.append(QStringList() << appList.at(i) << "1");
            }

            row += (appList.count() + 1);
        }
    }

    return m_minLetterData;
}

QVector<QStringList> GetModelData::getMinFuncData()
{
    m_funcRow = 0;
    m_minFuncData.clear();
    m_classificationList.clear();
    m_classificationBtnRowList.clear();
    QVector<QStringList> vector = UkuiMenuInterface::functionalVector;
    QStringList functionList = m_ukuiMenuInterface->getFunctionClassName();

    for (int i = 0; i < vector.size(); i++) {
        if (!(vector.at(i).isEmpty())) {
            QString functionName = functionList.at(i);
            insertClassificationBtn(functionName);
            insertAppList(vector.at(i));
        }
    }

    return m_minFuncData;
}



void GetModelData::insertClassificationBtn(QString btnname)
{
    m_classificationList.append(btnname);
    m_minFuncData.append(QStringList() << btnname << "0");
    m_classificationBtnRowList.append(QString::number(m_funcRow));
}

void GetModelData::insertAppList(QStringList appnamelist)
{
    m_funcRow += (appnamelist.count() + 1);

    for (int i = 0; i < appnamelist.count(); i++) {
        m_minFuncData.append(QStringList() << appnamelist.at(i) << "1");
    }
}

