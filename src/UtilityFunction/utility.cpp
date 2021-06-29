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

#include "utility.h"
#include <QSvgRenderer>
#include <QPainter>
#include <QPixmap>
#include <QDBusMessage>
#include <QDBusConnection>
#include <QGSettings>
#include <QDBusReply>
#include <QDesktopWidget>
#include <QApplication>
#include <QDBusInterface>

#define DATABASENAME QDir::homePath()+"/.config/ukui/"+"UkuiMenu.db"

const QPixmap loadSvg(const QString &fileName, const int size)
{
    QPixmap pixmap(size, size);
    QSvgRenderer renderer(fileName);
    pixmap.fill(Qt::transparent);

    QPainter painter;
    painter.begin(&pixmap);
    renderer.render(&painter);
    painter.end();

    return pixmap;
}

QPixmap drawSymbolicColoredPixmap(const QPixmap &source)
{
    QColor gray(128,128,128);
    QColor standard (31,32,34);
    QImage img = source.toImage();
    for (int x = 0; x < img.width(); x++) {
        for (int y = 0; y < img.height(); y++) {
            auto color = img.pixelColor(x, y);
            if (color.alpha() > 0) {
                if (qAbs(color.red()-gray.red())<20 && qAbs(color.green()-gray.green())<20 && qAbs(color.blue()-gray.blue())<20) {
                    color.setRed(255);
                    color.setGreen(255);
                    color.setBlue(255);
                    img.setPixelColor(x, y, color);
                }
                else if(qAbs(color.red()-standard.red())<20 && qAbs(color.green()-standard.green())<20 && qAbs(color.blue()-standard.blue())<20)
                {
                    color.setRed(255);
                    color.setGreen(255);
                    color.setBlue(255);
                    img.setPixelColor(x, y, color);
                }
                else
                {
                    img.setPixelColor(x, y, color);
                }
            }
        }
    }
    return QPixmap::fromImage(img);
}

QPixmap drawSymbolicBlackColoredPixmap(const QPixmap &source)
{
    QImage img = source.toImage();
    for (int x = 0; x < img.width(); x++) {
        for (int y = 0; y < img.height(); y++) {
            auto color = img.pixelColor(x, y);
            if (color.alpha() > 0) {
                if (qAbs(color.red())>=200 && qAbs(color.green())>=200 && qAbs(color.blue())>=200) {
                    color.setRed(56);
                    color.setGreen(56);
                    color.setBlue(56);
                    img.setPixelColor(x, y, color);
                }
            }
        }
    }
    return QPixmap::fromImage(img);
}

QVariantList getScreenGeometry()
{
    QVariantList list;
    list.clear();
    QDBusInterface iface(DBUS_NAME,
                         DBUS_PATH,
                         DBUS_INTERFACE,
                         QDBusConnection::sessionBus());
    QDBusReply<QVariantList> reply=iface.call("GetPrimaryScreenPhysicalGeometry");
    if(iface.isValid() && reply.isValid())
    {
        list=reply.value();
    }
    else
    {
        QRect rect=QApplication::desktop()->screenGeometry(0);
        list.append(QString::number(rect.x()));
        list.append(QString::number(rect.y()));
        list.append(QString::number(rect.width()));
        list.append(QString::number(rect.height()));

        int position=0;
        int panelSize=0;
        if(QGSettings::isSchemaInstalled(QString("org.ukui.panel.settings").toLocal8Bit()))
        {
            QGSettings* gsetting=new QGSettings(QString("org.ukui.panel.settings").toLocal8Bit());
            if(gsetting->keys().contains(QString("panelposition")))
                position=gsetting->get("panelposition").toInt();
            else
                position=0;
            if(gsetting->keys().contains(QString("panelsize")))
                panelSize=gsetting->get("panelsize").toInt();
            else
                panelSize=46;
            delete gsetting;
        }
        else
        {
            position=0;
            panelSize=46;
        }
        list.append(QString::number(panelSize));
        list.append(QString::number(position));
    }
    return list;
}

double getTransparency()
{
    double transparency=0.0;
    if(QGSettings::isSchemaInstalled(QString("org.ukui.control-center.personalise").toLocal8Bit()))
    {
        QGSettings gsetting(QString("org.ukui.control-center.personalise").toLocal8Bit());
        if(gsetting.keys().contains(QString("transparency")))
            transparency=gsetting.get("transparency").toDouble();
    }
    return transparency;
}

void debugLog(QString strMsg)
{
    QString path=QDir::homePath()+"/.config/ukui/ukui-menu.log";
    QFile confFile(path);
    if(confFile.open(QIODevice::Text | QIODevice::ReadWrite | QIODevice::Append))
    {
        QString text=QString("%1 %2")
                .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                .arg(strMsg);
        QTextStream textStream(&confFile);
        textStream<<text<<endl;
        textStream.flush();
        confFile.close();
    }
}

QString getEnvOverriddenDesktopFile(int pid)
{
    QString ret;
    QString path=QString("/proc/%1/environ")
            .arg(pid);
    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        QList<QByteArray> list=file.readLine().split('\x00');
        Q_FOREACH(QByteArray array, list)
        {
            if(array.contains("GIO_LAUNCHED_DESKTOP_FILE="))
            {
                ret=QString(array.split('=').at(1));
                break;
            }
        }
        file.close();
    }
    return ret;

}

void openDataBase(QString connectionName)
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    db.setDatabaseName(DATABASENAME);

    if (false == db.open())
    {
        qDebug() << db.lastError().text();
    }
}

void closeDataBase(QString connectionName)
{
    QSqlDatabase db=QSqlDatabase::database("connectionName");
    db.close();
    QSqlDatabase::removeDatabase(connectionName);
}

void initDatabase()
{
    QStringList desktopfnList;
    desktopfnList.clear();
    QSqlDatabase db = QSqlDatabase::database("MainThread");
    QSqlQuery sql(db);
    sql.exec("select count(*) from sqlite_master where type='table' and name='appInfo'");
    if(sql.next())
    {
        if(sql.value(0).toInt()==0)
        {
            QSettings* setting=new QSettings("/var/lib/ukui-menu/ukui-menu.ini",QSettings::IniFormat);
            setting->beginGroup("application");
            QString desktopfp;
            Q_FOREACH(QString desktopfn, setting->allKeys())
            {
                if(setting->value(desktopfn).toInt()==1)
                {
                    desktopfp.clear();
                    if(UkuiMenuInterface::androidDesktopfnList.contains(desktopfn))
                        desktopfp=QString(QDir::homePath()+"/.local/share/applications/"+desktopfn);
                    else
                        desktopfp=QString("/usr/share/applications/"+desktopfn);
                    QFileInfo info(desktopfp);
                    if(!info.isFile() || !UkuiMenuInterface::desktopfpVector.contains(desktopfp))
                        continue;
                    desktopfnList.append(desktopfn);
                }
            }
            setting->endGroup();
            delete setting;
        }
    }
    sql.exec("create table if not exists appInfo(desktop char primary key, times int, time int, type int, recent int)");
    Q_FOREACH(QString desktopfn,desktopfnList)
    {
        QDateTime dt=QDateTime::currentDateTime();
        int datetime=dt.toTime_t();
        QString cmd=QString("insert into appInfo values(\"%0\",%1,%2,%3,%4)")
                .arg(desktopfn)
                .arg(0)
                .arg(datetime)
                .arg(1)
                .arg(0);
        sql.exec(cmd);
    }
}

bool updateDataBaseTableTimes(QString desktopfn)
{
    bool ret=false;
    QSqlDatabase db = QSqlDatabase::database("MainThread");
    QSqlQuery sql(db);
    QString cmd;
    cmd=QString("select times from appInfo where desktop=\"%1\"").arg(desktopfn);
    if(sql.exec(cmd))
    {
        QDateTime dt=QDateTime::currentDateTime();
        int datetime=dt.toTime_t();
        if(sql.next())
        {
            int times=sql.value(0).toInt()+1;
            cmd=QString("update appInfo set times=%1, time=%2, recent=%3 where desktop=\"%4\"")
                    .arg(times)
                    .arg(datetime)
                    .arg(0)
                    .arg(desktopfn);
        }
        else
        {
            cmd=QString("insert into appInfo values(\"%0\",%1,%2,%3,%4)")
                    .arg(desktopfn)
                    .arg(1)
                    .arg(datetime)
                    .arg(0)
                    .arg(0);
        }
        ret=sql.exec(cmd);
    }
    return ret;
}

bool updateDataBaseTableType(QString desktopfn, int type)
{
    bool ret=false;
    QSqlDatabase db = QSqlDatabase::database("MainThread");
    QSqlQuery sql(db);
    QString cmd;
    switch (type) {
    case 0://解除锁定
    {
        cmd=QString("delete from appInfo where desktop=\"%1\"")
                .arg(desktopfn);
        ret=sql.exec(cmd);
    }
        break;
    case 1://锁定
    {
        cmd=QString("select type from appInfo where desktop=\"%1\"")
                .arg(desktopfn);
        if(sql.exec(cmd))
        {
            if(sql.next())//更新记录
            {
                cmd=QString("update appInfo set type=%1 where desktop=\"%2\"")
                        .arg(type)
                        .arg(desktopfn);
            }
            else//添加记录
            {
                QDateTime dt=QDateTime::currentDateTime();
                int datetime=dt.toTime_t();
                cmd=QString("insert into appInfo values(\"%0\",%1,%2,%3,%4)")
                        .arg(desktopfn)
                        .arg(0)
                        .arg(datetime)
                        .arg(type)
                        .arg(0);
            }
            ret=sql.exec(cmd);
        }
    }
        break;
    default:
        break;
    }
    return ret;
}

bool updateDataBaseTableRecent(QString desktopfn)
{
    bool ret=false;
    QSqlDatabase db = QSqlDatabase::database("DirectoryChangedThread");
    QSqlQuery sql(db);
    QString cmd;
    QDateTime dt=QDateTime::currentDateTime();
    int datetime=dt.toTime_t();
    cmd=QString("insert into appInfo values(\"%0\",%1,%2,%3,%4)")
            .arg(desktopfn)
            .arg(0)
            .arg(datetime)
            .arg(0)
            .arg(1);
    ret=sql.exec(cmd);
    return ret;
}

bool checkIfLocked(QString desktopfn)
{
    QSqlDatabase db = QSqlDatabase::database("MainThread");
    QSqlQuery sql(db);
    QString cmd=QString("select type from appInfo where desktop=\"%1\"")
            .arg(desktopfn);
    if(sql.exec(cmd))
    {
        if(!sql.next())
            return false;
        else
        {
            if(sql.value(0).toInt()==0)
                return false;
            else
                return true;
        }
    }
    return false;
}

bool checkIfRecent(QString desktopfn)
{
    QSqlDatabase db = QSqlDatabase::database("MainThread");
    QSqlQuery sql(db);
    QString cmd=QString("select recent from appInfo where desktop=\"%1\"")
            .arg(desktopfn);
    if(sql.exec(cmd))
    {
        if(!sql.next())
            return false;
        else
        {
            if(sql.value(0).toInt()==0)
                return false;
            else
                return true;
        }
    }
    return false;
}

QStringList getLockAppList()
{
    QStringList list;
    QSqlDatabase db = QSqlDatabase::database("MainThread");
    QSqlQuery sql(db);
    QString cmd=QString("select desktop from appInfo where type=1");
    if(sql.exec(cmd))
    {
        while(sql.next())
            list.append(sql.value(0).toString());
    }
    return list;
}

QStringList getUnlockAllList()
{
    QStringList list;
    QSqlDatabase db = QSqlDatabase::database("MainThread");
    QSqlQuery sql(db);
    QString cmd=QString("select desktop from appInfo where type=0 and times>=1 order by times desc");
    if(sql.exec(cmd))
    {
        while(sql.next())
            list.append(sql.value(0).toString());
    }
    return list;
}

void cleanTimeoutApp()
{
    QSqlDatabase db = QSqlDatabase::database("MainThread");
    QDateTime dt=QDateTime::currentDateTime();
    int datetime=dt.toTime_t()-24*60*60*3;
    QString cmd=QString("select desktop from appInfo where time < %1")
            .arg(datetime);
    QStringList list;
    QSqlQuery sql(db);
    if(sql.exec(cmd))
    {
        while(sql.next())
        {
            if(!checkIfLocked(sql.value(0).toString()))
                list.append(sql.value(0).toString());
        }
    }
    Q_FOREACH(QString desktopfn, list)
    {
        cmd=QString("delete from appInfo where desktop=\"%1\"")
                .arg(desktopfn);
        sql.exec(cmd);
    }
}

bool deleteAppRecord(QString desktopfn)
{
    bool ret=false;
    QSqlDatabase db = QSqlDatabase::database("DirectoryChangedThread");
    QString cmd=QString("delete from appInfo where desktop=\"%1\"")
            .arg(desktopfn);
    QSqlQuery sql(db);
    ret=sql.exec(cmd);
    return ret;
}

void execApp(QString desktopfp)
{
    UkuiMenuInterface interface;
    if(interface.checkKreApp(desktopfp))
    {
        QProcess::startDetached(interface.getAppExec(desktopfp));
    }
    else
    {
        QString appName = interface.getAppExec(desktopfp);
        QStringList strList = (appName.replace("\"","")).split(" ");
        for(int i = 1; i < strList.size(); i++)
        {
            if(QString(strList.at(i)).size() > 2)
            {
                QProcess::startDetached(strList.at(0));
                return;
            }
        }
        GDesktopAppInfo * desktopAppInfo=g_desktop_app_info_new_from_filename(desktopfp.toLocal8Bit().data());
        g_app_info_launch(G_APP_INFO(desktopAppInfo),nullptr, nullptr, nullptr);
        g_object_unref(desktopAppInfo);
    }
}

bool checkOsRelease()
{
    QFile file("/etc/os-release");
    if(file.open(QFile::ReadOnly))
    {
        QByteArray line=file.readLine();
        file.close();
        if(QString(line).contains("Ubuntu"))
            return true;
    }
    return false;
}

//获取用户图像
QString getUserIcon()
{
    qint64 uid=static_cast<qint64>(getuid());
    QDBusInterface iface("org.freedesktop.Accounts",
                         "/org/freedesktop/Accounts",
                         "org.freedesktop.Accounts",
                         QDBusConnection::systemBus());
    QDBusReply<QDBusObjectPath>objPath=iface.call("FindUserById",uid);

    QDBusInterface useriface("org.freedesktop.Accounts",
                             objPath.value().path(),
                             "org.freedesktop.DBus.Properties",
                             QDBusConnection::systemBus());
    QDBusReply<QVariant> var=useriface.call("Get","org.freedesktop.Accounts.User","IconFile");
    QString iconstr=var.value().toString();
    return iconstr;
}

QString getUserName()
{
    QString name;
    qint64 uid=static_cast<qint64>(getuid());
    QDBusInterface iface("org.freedesktop.Accounts",
                         "/org/freedesktop/Accounts",
                         "org.freedesktop.Accounts",
                         QDBusConnection::systemBus());
    QDBusReply<QDBusObjectPath> objPath=iface.call("FindUserById",uid);
    QDBusInterface useriface("org.freedesktop.Accounts",
                             objPath.value().path(),
                             "org.freedesktop.DBus.Properties",
                             QDBusConnection::systemBus());
    QDBusReply<QVariant> var=useriface.call("Get","org.freedesktop.Accounts.User","RealName");
    name=var.value().toString();
    if(name.isEmpty())
    {
        var=useriface.call("Get","org.freedesktop.Accounts.User","UserName");
        name=var.value().toString();
    }
    return name;
}
