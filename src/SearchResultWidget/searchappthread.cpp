/*
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/&gt;.
 *
 */

#include "searchappthread.h"
#include <syslog.h>

SearchAppThread::SearchAppThread()
{
    pUkuiMenuInterface=new UkuiMenuInterface;
}

SearchAppThread::~SearchAppThread()
{
    delete pUkuiMenuInterface;
}

void SearchAppThread::run()
{
    appInfoVector.clear();
    appInfoVector=UkuiMenuInterface::appInfoVector;
    searchResultVector.clear();
    if(!this->keyword.isEmpty())
    {
        QString str=pUkuiMenuInterface->getAppNamePinyin(keyword);
        int index=0;
        if(str.size()==1)
        {
            int num=static_cast<int>(str.toLocal8Bit().at(0));
            if(str.at(0)>=65 && str.at(0)<=90)
            {
                searchResultVector.append(UkuiMenuInterface::alphabeticVector.at(num-65));

//                while(index<appInfoVector.size())
//                {
//                    QString appNamePys=appInfoVector.at(index).at(5);
//                    if(appNamePys.contains(str))
//                    {
//                        searchResultList.append(appInfoVector.at(index).at(0));
//                    }
//                    index++;
//                }
            }
            else if(str.at(0)<48 || (str.at(0)>57 && str.at(0)<65) || str.at(0)>90)
                searchResultVector.append(UkuiMenuInterface::alphabeticVector.at(26));
            else
                searchResultVector.append(UkuiMenuInterface::alphabeticVector.at(27));
        }
//        else if(QString::compare("Mobile",keyword,Qt::CaseInsensitive)==0 || QString::compare("移动",keyword,Qt::CaseInsensitive)==0)
//            searchResultList=UkuiMenuInterface::functionalVector.at(0);
//        else if(QString::compare("Internet",keyword,Qt::CaseInsensitive)==0 || QString::compare("网络",keyword,Qt::CaseInsensitive)==0)
//            searchResultList=UkuiMenuInterface::functionalVector.at(1);
//        else if(QString::compare("Social",keyword,Qt::CaseInsensitive)==0 || QString::compare("社交",keyword,Qt::CaseInsensitive)==0)
//            searchResultList=UkuiMenuInterface::functionalVector.at(2);
//        else if(QString::compare("Video",keyword,Qt::CaseInsensitive)==0 || QString::compare("影音",keyword,Qt::CaseInsensitive)==0)
//            searchResultList=UkuiMenuInterface::functionalVector.at(3);
//        else if(QString::compare("Development",keyword,Qt::CaseInsensitive)==0 || QString::compare("开发",keyword,Qt::CaseInsensitive)==0)
//            searchResultList=UkuiMenuInterface::functionalVector.at(4);
//        else if(QString::compare("Image",keyword,Qt::CaseInsensitive)==0 || QString::compare("图像",keyword,Qt::CaseInsensitive)==0)
//            searchResultList=UkuiMenuInterface::functionalVector.at(5);
//        else if(QString::compare("Game",keyword,Qt::CaseInsensitive)==0 || QString::compare("游戏",keyword,Qt::CaseInsensitive)==0)
//            searchResultList=UkuiMenuInterface::functionalVector.at(6);
//        else if(QString::compare("Office",keyword,Qt::CaseInsensitive)==0 || QString::compare("办公",keyword,Qt::CaseInsensitive)==0)
//            searchResultList=UkuiMenuInterface::functionalVector.at(7);
//        else if(QString::compare("Education",keyword,Qt::CaseInsensitive)==0 || QString::compare("教育",keyword,Qt::CaseInsensitive)==0)
//            searchResultList=UkuiMenuInterface::functionalVector.at(8);
//        else if(QString::compare("System",keyword,Qt::CaseInsensitive)==0 || QString::compare("系统",keyword,Qt::CaseInsensitive)==0)
//            searchResultList=UkuiMenuInterface::functionalVector.at(9);
//        else if(QString::compare("Others",keyword,Qt::CaseInsensitive)==0 || QString::compare("其它",keyword,Qt::CaseInsensitive)==0)
//            searchResultList=UkuiMenuInterface::functionalVector.at(10);
        else
        {
            while(index<appInfoVector.size())
            {
                QString appNamePy=pUkuiMenuInterface->getAppNamePinyin(appInfoVector.at(index).at(2));
                QString appEnglishName=appInfoVector.at(index).at(3);
                QString appNameFls=appInfoVector.at(index).at(6);
                if(appNamePy.contains(str,Qt::CaseInsensitive) ||
                        appNameFls.contains(str,Qt::CaseInsensitive) ||
                        appEnglishName.contains(str,Qt::CaseInsensitive))
                {
                    searchResultVector.append(appInfoVector.at(index));
                }
                index++;
            }
        }
    }

    qSort(searchResultVector.begin(),searchResultVector.end(),UkuiMenuInterface::cmpApp);
    Q_EMIT sendSearchResult(searchResultVector);
}

void SearchAppThread::recvSearchKeyword(QString arg)
{
    this->keyword.clear();
    this->keyword=arg;
}
