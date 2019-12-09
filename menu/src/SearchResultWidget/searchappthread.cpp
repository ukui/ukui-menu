#include "searchappthread.h"

SearchAppThread::SearchAppThread()
{
    pUkuiMenuInterface=new UkuiMenuInterface;
    QStringList desktopfpList=pUkuiMenuInterface->get_desktop_file_path();
    QString desktopfp;
    foreach (desktopfp, desktopfpList) {
        QString appnamepy=pUkuiMenuInterface->get_app_name_pinyin(pUkuiMenuInterface->get_app_name(desktopfp));
//        QString appname=pUkuiMenuInterface->get_app_english_name(deskfp);
        appnamepyList.append(appnamepy);
        QString appname=pUkuiMenuInterface->get_app_name(desktopfp);
        appnameList.append(appname);

    }

}

SearchAppThread::~SearchAppThread()
{
    delete pUkuiMenuInterface;
}

void SearchAppThread::run()
{
    searchResultList.clear();
    if(!this->keyword.isEmpty())
    {
//        qDebug()<<"---111---";
        QString str=pUkuiMenuInterface->get_app_name_pinyin(keyword);
        QString appnamepy;
        int index=0;
        foreach (appnamepy, appnamepyList) {
            if(appnamepy.contains(str,Qt::CaseInsensitive))
            {
                QString desktopfp=pUkuiMenuInterface->get_desktop_path_by_app_name(appnameList.at(index));
//                QString deskfp=pUkuiMenuInterface->get_desktop_path_by_app_english_name(appname);
//                QString appname=pUkuiMenuInterface->get_app_name(deskfp);
                searchResultList.append(desktopfp);

            }
            index++;
        }
    }

    emit send_search_result(searchResultList);
//    qDebug()<<"---"<<searchResultList;

}

void SearchAppThread::recv_search_keyword(QString arg)
{
    this->keyword.clear();
    this->keyword=arg;
//    qDebug()<<this->keyword;
}
