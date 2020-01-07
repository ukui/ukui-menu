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
}
