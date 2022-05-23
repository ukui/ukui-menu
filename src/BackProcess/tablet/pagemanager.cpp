#include "pagemanager.h"
#include "ukuimenuinterface.h"
#include "style.h"
PageManager::PageManager()
{
    //获取数据
    m_ukuiMenuInterface = new UkuiMenuInterface();
    UkuiMenuInterface::appInfoVector = m_ukuiMenuInterface->createAppInfoVector();
    m_ukuiMenuInterface->initAppIni();
}

int PageManager::getPageNum(const int &appNum)
{
    int pageNum = 0;

    if (appNum / (Style::appColumnFirst * Style::appLineFirst) == 0) {
        pageNum = 1;
    } else {
        int  appNumOtherPage = appNum - (Style::appColumnFirst * Style::appLineFirst);

        if (appNumOtherPage % (Style::appColumn * Style::appLine) == 0) {
            pageNum = appNumOtherPage / (Style::appColumn * Style::appLine) + 1;
        } else {
            pageNum = appNumOtherPage / (Style::appColumn * Style::appLine) + 2;
        }
    }

    return pageNum;
}

QVector<QStringList> PageManager::sortAppInPage(const QVector<QString> &appVector)
{
    QVector<QString> m_data;
    QVector<QStringList> pageData;
    QStringList onePageData;

    Q_FOREACH (QString desktopfp, appVector) {
        m_data.append(desktopfp);
    }

    int pageSize = getPageNum(m_data.size());
    onePageData.clear();

    for (int j = 0; j < Style::appColumnFirst * Style::appLineFirst; j++) {
        if (m_data.size() > 0) {
            onePageData.append(m_data.at(0));
            m_data.pop_front();
        } else {
            break;
        }
    }

    pageData.append(onePageData);

    for (int i = 0; i < pageSize - 1; i++) {
        onePageData.clear();

        for (int j = 0; j < Style::appColumn * Style::appLine; j++) {
            if (m_data.size() > 0) {
                onePageData.append(m_data.at(0));
                m_data.pop_front();
            } else {
                break;
            }
        }

        pageData.append(onePageData);
    }

    return pageData;
}

QVector<QStringList> PageManager::getAppPageVector()
{
    UkuiMenuInterface::tencentInitVector = m_ukuiMenuInterface->sortDesktopList("tencent");
    UkuiMenuInterface::customizedVector = m_ukuiMenuInterface->sortDesktopList("customized");
    UkuiMenuInterface::thirdPartyVector = m_ukuiMenuInterface->sortDesktopList("thirdParty");
    UkuiMenuInterface::applicationVector = m_ukuiMenuInterface->sortDesktopList("application");
    QVector<QStringList> appPagelist;
    QVector<QString> allAppList = UkuiMenuInterface::tencentInitVector + UkuiMenuInterface::customizedVector + UkuiMenuInterface::thirdPartyVector + UkuiMenuInterface::applicationVector;

    Q_FOREACH (QStringList desktopfp, sortAppInPage(/*UkuiMenuInterface::tencentInitVector*/allAppList)) {
        appPagelist.append(desktopfp);
    }

//    Q_FOREACH(QStringList desktopfp,sortAppInPage(UkuiMenuInterface::customizedVector))
//    {
//        appPagelist.append(desktopfp);
//    }
//    Q_FOREACH(QStringList desktopfp,sortAppInPage(UkuiMenuInterface::thirdPartyVector))
//    {
//        appPagelist.append(desktopfp);
//    }
//    Q_FOREACH(QStringList desktopfp,sortAppInPage(UkuiMenuInterface::applicationVector))
//    {
//        appPagelist.append(desktopfp);
//    }
    return appPagelist;
}
