#include "searchappthread.h"

SearchAppThread::SearchAppThread()
{
    pUkuiMenuInterface=new UkuiMenuInterface;
    appInfoVector=UkuiMenuInterface::appInfoVector;

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
        QString str=pUkuiMenuInterface->get_app_name_pinyin(keyword);
        int index=0;
        while(index<appInfoVector.size())
        {
            QString appNamePy=pUkuiMenuInterface->get_app_name_pinyin(appInfoVector.at(index).at(2));
            if(appNamePy.contains(str,Qt::CaseInsensitive))
            {
                searchResultList.append(appInfoVector.at(index).at(0));
            }
            index++;
        }
    }

    emit send_search_result(searchResultList);
}

void SearchAppThread::recv_search_keyword(QString arg)
{
    this->keyword.clear();
    this->keyword=arg;
//    qDebug()<<this->keyword;
}
