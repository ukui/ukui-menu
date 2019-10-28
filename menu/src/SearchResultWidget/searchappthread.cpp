#include "searchappthread.h"

SearchAppThread::SearchAppThread()
{
    QStringList deskfpList=KylinStartMenuInterface::get_desktop_file_path();
    QString deskfp;
    foreach (deskfp, deskfpList) {
        QString appname=KylinStartMenuInterface::get_app_english_name(deskfp);
        appnameList.append(appname);
    }

}

void SearchAppThread::run()
{
    searchResultList.clear();
    if(!this->keyword.isEmpty())
    {
        QString str=KylinStartMenuInterface::get_app_name_pinyin(keyword);
        QString appname;
        foreach (appname, appnameList) {
            if(appname.contains(str,Qt::CaseInsensitive))
            {
                QString deskfp=KylinStartMenuInterface::get_desktop_path_by_app_english_name(appname);
                QString appname=KylinStartMenuInterface::get_app_name(deskfp);
                searchResultList.append(appname);

            }
        }
    }

    emit send_search_result(searchResultList);

}

void SearchAppThread::recv_search_keyword(QString arg)
{
    this->keyword.clear();
    this->keyword=arg;
}
