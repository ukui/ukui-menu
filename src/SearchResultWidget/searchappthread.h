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

#ifndef SEARCHAPPTHREAD_H
#define SEARCHAPPTHREAD_H
#include <QThread>
#include "src/Interface/ukuimenuinterface.h"
#include <QObject>

class SearchAppThread : public QThread
{
    Q_OBJECT
public:
    SearchAppThread();
    ~SearchAppThread();
    void run();

private:
    QString m_keyWord;
    QVector<QStringList> m_searchResultVector;
    QVector<QStringList> m_searchFirstVector;
    QVector<QStringList> m_searchRestVector;

    UkuiMenuInterface* m_ukuiMenuInterface=nullptr;
    QVector<QStringList> m_appInfoVector;

public Q_SLOTS:
    /**
     * @brief Receive search keywords
     * @param arg: Search keywords
     */
    void recvSearchKeyword(QString arg);

Q_SIGNALS:
    /**
     * @brief Send search results
     * @param vector: Search results
     */
    void sendSearchResult(QVector<QStringList> vector);

};

#endif // SEARCHAPPTHREAD_H
