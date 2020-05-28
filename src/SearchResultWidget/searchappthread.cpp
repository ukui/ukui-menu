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
    m_ukuiMenuInterface=new UkuiMenuInterface;
}

SearchAppThread::~SearchAppThread()
{
    delete m_ukuiMenuInterface;
}

void SearchAppThread::run()
{
    m_appInfoVector.clear();
    m_appInfoVector=UkuiMenuInterface::appInfoVector;
    m_searchResultVector.clear();
    if(!this->m_keyWord.isEmpty())
    {
        QString str=m_ukuiMenuInterface->getAppNamePinyin(m_keyWord);
        int index=0;
        if(str.size()==1)
        {
            int num=static_cast<int>(str.toLocal8Bit().at(0));
            QStringList searchDesktopList;
            searchDesktopList.clear();
            if(num>=65 && num<=90)
            {
                for(int i=0;i<UkuiMenuInterface::alphabeticVector.at(num-65).size();i++)
                    searchDesktopList.append(UkuiMenuInterface::alphabeticVector.at(num-65).at(i));
            }
            else if(num<48 || (num>57 && num<65) || num>90)
                for(int i=0;i<UkuiMenuInterface::alphabeticVector.at(26).size();i++)
                    searchDesktopList.append(UkuiMenuInterface::alphabeticVector.at(26).at(i));
            else
                for(int i=0;i<UkuiMenuInterface::alphabeticVector.at(27).size();i++)
                    searchDesktopList.append(UkuiMenuInterface::alphabeticVector.at(27).at(i));

            if(!searchDesktopList.isEmpty())
            {
                for(int i=0;i<searchDesktopList.size();i++)
                    m_searchResultVector.append(QStringList()<<searchDesktopList.at(i)<<m_ukuiMenuInterface->getAppName(searchDesktopList.at(i)));
            }
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
            while(index<m_appInfoVector.size())
            {
                QString appNamePy=m_ukuiMenuInterface->getAppNamePinyin(m_appInfoVector.at(index).at(1));
                QString appEnglishName=m_appInfoVector.at(index).at(2);
                QString appNameFls=m_appInfoVector.at(index).at(4);
                if(appNamePy.contains(str,Qt::CaseInsensitive) ||
                        appNameFls.contains(str,Qt::CaseInsensitive) ||
                        appEnglishName.contains(str,Qt::CaseInsensitive))
                {
                    m_searchResultVector.append(m_appInfoVector.at(index));
                }
                index++;
            }
        }
    }

    qSort(m_searchResultVector.begin(),m_searchResultVector.end(),UkuiMenuInterface::cmpApp);
    Q_EMIT sendSearchResult(m_searchResultVector);
}

void SearchAppThread::recvSearchKeyword(QString arg)
{
    this->m_keyWord.clear();
    this->m_keyWord=arg;
}
