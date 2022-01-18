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

#include "software_database_update_thread.h"
#include <QDBusMessage>
#include <QDBusConnection>
#include <QGSettings>
#include <QDBusReply>
#include <QDBusInterface>
#include <QDir>
#include "src/UtilityFunction/utility.h"

#define DATABASENAME QDir::homePath()+"/.config/ukui/"+"UkuiMenu.db"

SoftwareDatabaseUpdateThread::SoftwareDatabaseUpdateThread()
{

}

SoftwareDatabaseUpdateThread::~SoftwareDatabaseUpdateThread()
{

}

void SoftwareDatabaseUpdateThread::run()
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE", "SoftwareThreadDatabase");
    db.setDatabaseName(DATABASENAME);
    if (false == db.open()){
        myDebug() << db.lastError().text();
    }

    QSqlQuery sql(db);

    QJsonDocument jsonDocument = QJsonDocument::fromJson(m_category.toLocal8Bit().data());
    QJsonObject jsonObject = jsonDocument.object();
    QJsonArray jsonArray = jsonObject[jsonObject.keys()[0]].toArray();

    Q_FOREACH(QJsonValue jsonValue, jsonArray){
        QJsonObject arrObject = jsonValue.toObject();
        QString execline = QString("replace into appCategory values(\"%0\", \"%1\", \"%2\")")
                .arg(arrObject[arrObject.keys()[0]].toString())
                .arg(arrObject[arrObject.keys()[1]].toString())
                .arg(arrObject[arrObject.keys()[2]].toString());
        sql.prepare(execline);
        if(sql.exec()){
            msleep(15);
        }
    }
    myDebug() << "数据库已经更新";
    Q_EMIT updateDatabaseSignal();
    msleep(10);
}

void SoftwareDatabaseUpdateThread::getDatabaseList(QString category)
{
    this->m_category = category;
}
