#include "pagemanager.h"
#include "src/Interface/ukuimenuinterface.h"
#include "src/Style/style.h"
PageManager::PageManager()
{
    //获取数据
    m_ukuiMenuInterface = new UkuiMenuInterface();
    UkuiMenuInterface::appInfoVector = m_ukuiMenuInterface->createAppInfoVector();
    m_ukuiMenuInterface->initAppIni();
}

int PageManager::getPageNum(int appnum)
{
    int pageNum = 0;
    Style::appColumn = 6;
    Style::appLine = 4;
    if(appnum%(Style::appColumn * Style::appLine)==0)
        pageNum = appnum / (Style::appColumn * Style::appLine);
    else
        pageNum = appnum / (Style::appColumn * Style::appLine) + 1;

    return pageNum;
}

QVector<QStringList> PageManager::sortAppInPage(QVector<QString> appvector)
{
    QVector<QString> m_data;
    QVector<QStringList> pageData;
    QStringList onePageData;
    Q_FOREACH(QString desktopfp,appvector)
    {
        m_data.append(desktopfp);
    }

    for(int i = 0; i < getPageNum(m_data.size()); i++)
    {
        onePageData.clear();
        for(int j = 0; j < Style::appColumn * Style::appLine; j++)
        {
            if(m_data.size() > 0)
            {
                onePageData.append(m_data.at(0));
                m_data.pop_front();
            }
            else
            {
              //  pageData.append(onePageData);
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
    Q_FOREACH(QStringList desktopfp,sortAppInPage(UkuiMenuInterface::tencentInitVector))
    {
        appPagelist.append(desktopfp);
    }

    Q_FOREACH(QStringList desktopfp,sortAppInPage(UkuiMenuInterface::customizedVector))
    {
        appPagelist.append(desktopfp);
    }

    Q_FOREACH(QStringList desktopfp,sortAppInPage(UkuiMenuInterface::thirdPartyVector))
    {
        appPagelist.append(desktopfp);
    }

    Q_FOREACH(QStringList desktopfp,sortAppInPage(UkuiMenuInterface::applicationVector))
    {
        appPagelist.append(desktopfp);
    }

    return appPagelist;
}
