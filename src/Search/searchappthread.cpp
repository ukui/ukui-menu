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
#include "file-utils.h"

SearchAppThread::SearchAppThread()
{
    m_ukuiMenuInterface = new UkuiMenuInterface;
}

SearchAppThread::~SearchAppThread()
{
    delete m_ukuiMenuInterface;
}

void SearchAppThread::run()
{

    m_appInfoVector.clear();
    m_appInfoVector = UkuiMenuInterface::appInfoVector;
    m_searchResultVector.clear();
    m_searchFirstVector.clear();
    m_searchRestVector.clear();

    if (!this->m_keyWord.isEmpty()) {
        QString str = m_ukuiMenuInterface->getAppNamePinyin(m_keyWord);
        int index = 0;
        while (index < m_appInfoVector.size()) {

            QString appName = m_appInfoVector.at(index).at(1);
            QString appEnglishName = m_appInfoVector.at(index).at(2);

            QStringList appNameLs;  //中文音标全拼列表
            QStringList appNameFls; //中文拼音首字母简拼列表
            QStringList appNamePyLst = Zeeker::FileUtils::findMultiToneWords(appName); //获取中文拼音列表

            for (int i = 0; i < appNamePyLst.size() / 2; i++) {
                appNameLs.append(appNamePyLst.at(i * 2));
                appNameFls.append(appNamePyLst.at(i * 2 + 1));
            }

            if (m_keyWord.contains(QRegExp("[\\x4e00-\\x9fa5]+"))) { //中文正则表达式
                if (appName.toUpper().contains(m_keyWord.toUpper())) {
                    m_searchResultVector.append(m_appInfoVector.at(index));
                }
            } else {
                for (int var = 0; var < appNameLs.size(); ++var) {
                    if (appNameLs[var].left(str.length()).contains(str,Qt::CaseInsensitive) ||
                            appNameFls[var].left(str.length()).contains(str,Qt::CaseInsensitive)) {
                        //按照顺序从首字母开始严格匹配查找
                        m_searchFirstVector.append(m_appInfoVector.at(index));
                        break;
                    } else if (appNameLs[var].contains(str,Qt::CaseInsensitive) ||
                            appNameFls[var].contains(str,Qt::CaseInsensitive) ||
                            appEnglishName.contains(str,Qt::CaseInsensitive)) {
                        //只要应用名存在包含输入信息就匹配
                        m_searchRestVector.append(m_appInfoVector.at(index));
                        break;
                    }
                }
            }
            index++;
        }
    }
    qSort(m_searchFirstVector.begin(), m_searchFirstVector.end(), UkuiMenuInterface::cmpApp);
    qSort(m_searchRestVector.begin(), m_searchRestVector.end(), UkuiMenuInterface::cmpApp);
    //对中文搜索结果进行排序
    if (m_searchResultVector.size() != 0){
         qSort(m_searchResultVector.begin(),m_searchResultVector.end(),UkuiMenuInterface::cmpApp);
    }
    //优先将严格匹配结果加入列表
    for (int i = 0; i < m_searchFirstVector.size(); i++) {
        m_searchResultVector.append(m_searchFirstVector.at(i));
    }
    //将模糊匹配结果加入列表
    for (int i = 0; i < m_searchRestVector.size(); i++) {
        m_searchResultVector.append(m_searchRestVector.at(i));
    }
    Q_EMIT sendSearchResult(m_searchResultVector);
}

void SearchAppThread::recvSearchKeyword(QString arg)
{
    this->m_keyWord.clear();
    this->m_keyWord = arg;
}
