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
#include <QScreen>

#define DATABASENAME QDir::homePath()+"/.config/ukui/"+"UkuiMenu.db"
QString projectCodeName = "V10SP1";

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
    QColor gray(128, 128, 128);
    QColor standard(31, 32, 34);
    QImage img = source.toImage();

    for (int x = 0; x < img.width(); x++) {
        for (int y = 0; y < img.height(); y++) {
            auto color = img.pixelColor(x, y);

            if (color.alpha() > 0) {
                if (qAbs(color.red() - gray.red()) < 20 && qAbs(color.green() - gray.green()) < 20 && qAbs(color.blue() - gray.blue()) < 20) {
                    color.setRed(255);
                    color.setGreen(255);
                    color.setBlue(255);
                    img.setPixelColor(x, y, color);
                } else if (qAbs(color.red() - standard.red()) < 20 && qAbs(color.green() - standard.green()) < 20 && qAbs(color.blue() - standard.blue()) < 20) {
                    color.setRed(255);
                    color.setGreen(255);
                    color.setBlue(255);
                    img.setPixelColor(x, y, color);
                } else {
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
                if (qAbs(color.red()) >= 200 && qAbs(color.green()) >= 200 && qAbs(color.blue()) >= 200) {
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

//不通过任务栏获取屏幕可用区域数据
QVariantList getScreenGeometryList()
{
    QRect rect;
    rect = qApp->primaryScreen()->geometry();

    int panelSize = 0;
    int position = 0;
    QVariantList list;
    list.clear();
    QDBusInterface iface(DBUS_NAME,
                         DBUS_PATH,
                         DBUS_INTERFACE,
                         QDBusConnection::sessionBus());
    QDBusReply<QVariantList> reply = iface.call("GetPrimaryScreenPhysicalGeometry");
    if (reply.isValid()) {
        panelSize = reply.value().at(4).toInt();
        position = reply.value().at(5).toInt();
    }
//    if (rect == qApp->primaryScreen()->geometry()) {
    if (position == 0) {
        list.append(rect.x());
        list.append(rect.y());
        list.append(rect.width());
        list.append(rect.height() - panelSize);
    } else if (position == 1) {
        list.append(rect.x());
        list.append(rect.y() + panelSize);
        list.append(rect.width());
        list.append(rect.height() - panelSize);
    } else if (position == 2) {
        list.append(rect.x() + panelSize);
        list.append(rect.y());
        list.append(rect.width() - panelSize);
        list.append(rect.height());
    } else {
        list.append(rect.x());
        list.append(rect.y());
        list.append(rect.width() - panelSize);
        list.append(rect.height());
    }
//    } else {
//        list.append(rect.x());
//        list.append(rect.y());
//        list.append(rect.width());
//        list.append(rect.height());
//    }

    list.append(panelSize);
    list.append(position);
    return list;
}

double getTransparency()
{
    double transparency = 0.0;

    if (QGSettings::isSchemaInstalled(QString("org.ukui.control-center.personalise").toLocal8Bit())) {
        QGSettings gsetting(QString("org.ukui.control-center.personalise").toLocal8Bit());

        if (gsetting.keys().contains(QString("transparency"))) {
            transparency = gsetting.get("transparency").toDouble();
        }
    }

    return transparency;
}

void centerToScreen(QWidget *widget)
{
    if (!widget) {
        return;
    }

    QDesktopWidget *deskTopWidget = QApplication::desktop();
    QRect deskRect = deskTopWidget->screenGeometry(deskTopWidget->screenNumber(QCursor::pos()));
    int deskWidth = deskRect.width();
    int deskHeight = deskRect.height();
    int x = QApplication::primaryScreen()->geometry().width();
    int y = QApplication::primaryScreen()->geometry().height();
    widget->move(deskWidth / 2 - x / 2 + deskRect.left(), deskHeight / 2 - y / 2 + deskRect.top());
}

void debugLog(QString strMsg)
{
    QString path = QDir::homePath() + "/.config/ukui/ukui-menu.log";
    QFile confFile(path);

    if (confFile.open(QIODevice::Text | QIODevice::ReadWrite | QIODevice::Append)) {
        QString text = QString("%1 %2")
                       .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                       .arg(strMsg);
        QTextStream textStream(&confFile);
        textStream << text << endl;
        textStream.flush();
        confFile.close();
    }
}

QString getEnvOverriddenDesktopFile(int pid)
{
    QString ret;
    QString path = QString("/proc/%1/environ")
                   .arg(pid);
    QFile file(path);

    if (file.open(QIODevice::ReadOnly)) {
        QList<QByteArray> list = file.readLine().split('\x00');

        Q_FOREACH (QByteArray array, list) {
            if (array.contains("GIO_LAUNCHED_DESKTOP_FILE=")) {
                ret = QString(array.split('=').at(1));
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

    if (false == db.open()) {
        qDebug() << db.lastError().text();
    }
}

void closeDataBase(QString connectionName)
{
    QSqlDatabase db = QSqlDatabase::database("connectionName");
    db.close();
    QSqlDatabase::removeDatabase(connectionName);
}

void initDatabase()
{
    QStringList desktopfnList;
    desktopfnList.clear();
    QSqlDatabase db = QSqlDatabase::database("MainThreadDataBase");
    QSqlQuery sql(db);
    sql.exec("select count(*) from sqlite_master where type='table' and name='appInfo'");

    if (sql.next()) {
        if (sql.value(0).toInt() == 0) {
            QSettings *setting = new QSettings("/var/lib/ukui-menu/ukui-menu.ini", QSettings::IniFormat);
            setting->beginGroup("application");
            QString desktopfp;

            Q_FOREACH (QString desktopfn, setting->allKeys()) {
                if (setting->value(desktopfn).toInt() == 1) {
                    desktopfp.clear();

                    if (UkuiMenuInterface::androidDesktopfnList.contains(desktopfn)) {
                        desktopfp = QString(QDir::homePath() + "/.local/share/applications/" + desktopfn);
                    } else {
                        desktopfp = QString("/usr/share/applications/" + desktopfn);
                    }

                    QFileInfo info(desktopfp);

                    if (!info.isFile() || !UkuiMenuInterface::desktopfpVector.contains(desktopfp)) {
                        continue;
                    }

                    desktopfnList.append(desktopfn);
                }
            }

            setting->endGroup();
            delete setting;
        }
    }

    bool b = sql.exec("create table if not exists appInfo(desktop char primary key, times int, time int, type int, recent int, num int, collect int)");

    Q_FOREACH (QString desktopfn, desktopfnList) {
        myDebug() << "void initDatabase()" << desktopfn;
        QDateTime dt = QDateTime::currentDateTime();
        int datetime = dt.toTime_t();
        QString cmd = QString("insert into appInfo values(\"%0\",%1,%2,%3,%4,%5,%6)")
                      .arg(desktopfn)
                      .arg(0)
                      .arg(datetime)
                      .arg(1)
                      .arg(0)
                      .arg(0)
                      .arg(0);
        bool a = sql.exec(cmd);
        myDebug() << "数据库执行是否成功" << a;
    }

    sql.exec("create table appCategory(app_name char primary key, name_en char, name_zh char)");
    sql.exec("insert into appCategory values('indicator-china-weather', '其他', 'others')");
    sql.exec("insert into appCategory values('peony', '其他', 'others')");
    sql.exec("insert into appCategory values('kylin-usb-creator', '其他', 'others')");
    sql.exec("insert into appCategory values('kylin-burner', '其他', 'others')");
    sql.exec("insert into appCategory values('sc-reader', '办公', 'office')");
    sql.exec("insert into appCategory values('kylin-scanner', '办公', 'office')");
    sql.exec("insert into appCategory values('onboard', '系统', 'system')");
    sql.exec("insert into appCategory values('evolution', '系统', 'system')");
    sql.exec("insert into appCategory values('kylin-screenshot', '系统', 'system')");
    sql.exec("insert into appCategory values('kylin-ipmsg', '网络', 'network')");
    sql.exec("insert into appCategory values('ksc-defender', '安全', 'safe')");
    sql.exec("insert into appCategory values('yhkylin-backup-tools', '安全', 'safe')");
    sql.exec("insert into appCategory values('box-manager', '安全', 'safe')");
    sql.exec("insert into appCategory values('ukui-system-monitor', '安全', 'safe')");
}

bool updateDataBaseTableTimes(QString desktopfn)
{
    bool ret = false;
    QSqlDatabase db = QSqlDatabase::database("MainThreadDataBase");
    QSqlQuery sql(db);
    QString cmd;
    cmd = QString("select times from appInfo where desktop=\"%1\"").arg(desktopfn);

    if (sql.exec(cmd)) {
        QDateTime dt = QDateTime::currentDateTime();
        int datetime = dt.toTime_t();

        if (sql.next()) {
            int times = sql.value(0).toInt() + 1;
            cmd = QString("update appInfo set times=%1, time=%2, recent=%3 where desktop=\"%4\"")
                  .arg(times)
                  .arg(datetime)
                  .arg(0)
                  .arg(desktopfn);
        } else {
            cmd = QString("insert into appInfo values(\"%0\",%1,%2,%3,%4,%5,%6)")
                  .arg(desktopfn)
                  .arg(1)
                  .arg(datetime)
                  .arg(0)
                  .arg(0)
                  .arg(0)
                  .arg(0);
        }

        ret = sql.exec(cmd);
    }

    return ret;
}

bool updateDataBaseCollect(QString desktopfn, int type)
{
    bool ret = false;
    QSqlDatabase db = QSqlDatabase::database("MainThreadDataBase");
    QSqlQuery sql(db);
    QString cmd;

    switch (type) {
        case 0: { //取消收藏
            cmd = QString("update appInfo set collect=%1 where desktop=\"%2\"")
                  .arg(type)
                  .arg(desktopfn);
            ret = sql.exec(cmd);
        }
        break;

        default: { //收藏
            cmd = QString("select type from appInfo where desktop=\"%1\"")
                  .arg(desktopfn);

            if (sql.exec(cmd)) {
                if (sql.next()) { //更新记录
                    cmd = QString("update appInfo set collect=%1 where desktop=\"%2\"")
                          .arg(type)
                          .arg(desktopfn);
                } else { //添加记录
                    QDateTime dt = QDateTime::currentDateTime();
                    int datetime = dt.toTime_t();
                    cmd = QString("insert into appInfo values(\"%0\",%1,%2,%3,%4,%5,%6)")
                          .arg(desktopfn)
                          .arg(0)
                          .arg(datetime)
                          .arg(0)
                          .arg(0)
                          .arg(0)
                          .arg(type);
                }

                ret = sql.exec(cmd);
            }
        }
        break;
    }

    return ret;
}

QStringList getCollectAppList()
{
    QStringList list;
    int count = 0;
    QSqlDatabase db = QSqlDatabase::database("MainThreadDataBase");
    QSqlQuery sql(db);
    QSqlQuery sqlque(db);
    QString cmd = QString("select desktop from appInfo where collect!=0 order by collect");

    if (sql.exec(cmd)) {
        while (sql.next()) {
            list.append(sql.value(0).toString());
            sqlque.exec(QString("update appInfo set collect=%1 where desktop=\"%2\"")
                        .arg(++count)
                        .arg(sql.value(0).toString()));
        }
    }

    collectCount = list.size() + 1;
    return list;
}

int getCollectAppCount(QString desktopfn)
{
    int appCount = 0;
    QSqlDatabase db = QSqlDatabase::database("MainThreadDataBase");
    QSqlQuery sql(db);
    QString cmd = QString("select collect from appInfo where desktop=\"%1\"")
                  .arg(desktopfn);

    if (sql.exec(cmd)) {
        if (sql.next()) {
            appCount = sql.value(0).toInt();
        }
    }

    return appCount;
}

void changeCollectSort(QString dragDesktopfn, QString dropDesktopfn)
{
    int endNum = getCollectAppCount(dropDesktopfn);
    int startNum = getCollectAppCount(dragDesktopfn);
    QStringList applist = getCollectAppList();

    if (startNum < endNum) {
        updateDataBaseCollect(dragDesktopfn, endNum + 1);

        for (int i = endNum; i < applist.count(); i++) {
            updateDataBaseCollect(applist.at(i), i + 2);
        }
    } else {
        updateDataBaseCollect(dragDesktopfn, endNum);

        for (int i = endNum - 1; i < applist.count(); i++) {
            if (dragDesktopfn != applist.at(i)) {
                updateDataBaseCollect(applist.at(i), i + 2);
            }
        }
    }
}

bool checkIfCollected(QString desktopfn)
{
    QSqlDatabase db = QSqlDatabase::database("MainThreadDataBase");
    QSqlQuery sql(db);
    QString cmd = QString("select collect from appInfo where desktop=\"%1\"")
                  .arg(desktopfn);

    if (sql.exec(cmd)) {
        if (!sql.next()) {
            return false;
        } else {
            if (sql.value(0).toInt() == 0) {
                return false;
            } else {
                return true;
            }
        }
    }

    return false;
}

bool updateDataBaseTableType(QString desktopfn, int type)
{
    bool ret = false;
    QSqlDatabase db = QSqlDatabase::database("MainThreadDataBase");
    QSqlQuery sql(db);
    QString cmd;

    switch (type) {
        case 0: { //解除锁定
            cmd = QString("update appInfo set times=%1,type=%2 where desktop=\"%3\"")
                  .arg(0)
                  .arg(type)
                  .arg(desktopfn);
            ret = sql.exec(cmd);
        }
        break;

        case 1: { //锁定
            cmd = QString("select type from appInfo where desktop=\"%1\"")
                  .arg(desktopfn);

            if (sql.exec(cmd)) {
                if (sql.next()) { //更新记录
                    cmd = QString("update appInfo set type=%1 where desktop=\"%2\"")
                          .arg(type)
                          .arg(desktopfn);
                } else { //添加记录
                    QDateTime dt = QDateTime::currentDateTime();
                    int datetime = dt.toTime_t();
                    cmd = QString("insert into appInfo values(\"%0\",%1,%2,%3,%4,%5,%6)")
                          .arg(desktopfn)
                          .arg(0)
                          .arg(datetime)
                          .arg(type)
                          .arg(0)
                          .arg(0)
                          .arg(0);
                }

                ret = sql.exec(cmd);
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
    bool ret = false;
    QSqlDatabase db = QSqlDatabase::database("DirectoryChangedThread");
    QSqlQuery sql(db);
    QString cmd;
    QDateTime dt = QDateTime::currentDateTime();
    int datetime = dt.toTime_t();
    cmd = QString("insert into appInfo values(\"%0\",%1,%2,%3,%4,%5)")
          .arg(desktopfn)
          .arg(0)
          .arg(datetime)
          .arg(0)
          .arg(1)
          .arg(0);
    ret = sql.exec(cmd);
    return ret;
}

bool checkIfLocked(QString desktopfn)
{
    QSqlDatabase db = QSqlDatabase::database("MainThreadDataBase");
    QSqlQuery sql(db);
    QString cmd = QString("select type from appInfo where desktop=\"%1\"")
                  .arg(desktopfn);

    if (sql.exec(cmd)) {
        if (!sql.next()) {
            return false;
        } else {
            if (sql.value(0).toInt() == 0) {
                return false;
            } else {
                return true;
            }
        }
    }

    return false;
}

bool checkIfRecent(QString desktopfn)
{
    QSqlDatabase db = QSqlDatabase::database("MainThreadDataBase");
    QSqlQuery sql(db);
    QString cmd = QString("select recent from appInfo where desktop=\"%1\"")
                  .arg(desktopfn);

    if (sql.exec(cmd)) {
        if (!sql.next()) {
            return false;
        } else {
            if (sql.value(0).toInt() == 0) {
                return false;
            } else {
                return true;
            }
        }
    }

    return false;
}

QStringList getLockAppList()
{
    QStringList list;
    QSqlDatabase db = QSqlDatabase::database("MainThreadDataBase");
    QSqlQuery sql(db);
    QString cmd = QString("select desktop from appInfo where type=1");

    if (sql.exec(cmd)) {
        while (sql.next()) {
            list.append(sql.value(0).toString());
        }
    }

    return list;
}

QStringList getUnlockAllList()
{
    QStringList list;
    QSqlDatabase db = QSqlDatabase::database("MainThreadDataBase");
    QSqlQuery sql(db);
    QString cmd = QString("select desktop from appInfo where type=0 and times>=1 order by times desc");

    if (sql.exec(cmd)) {
        while (sql.next()) {
            list.append(sql.value(0).toString());
        }
    }

    return list;
}

void cleanTimeoutApp()
{
    QSqlDatabase db = QSqlDatabase::database("MainThreadDataBase");
    QDateTime dt = QDateTime::currentDateTime();
    int datetime = dt.toTime_t() - 24 * 60 * 60 * 3;
    QString cmd = QString("select desktop from appInfo where time < %1")
                  .arg(datetime);
    QStringList list;
    QSqlQuery sql(db);

    if (sql.exec(cmd)) {
        while (sql.next()) {
            if (!checkIfLocked(sql.value(0).toString())) {
                list.append(sql.value(0).toString());
            }
        }
    }

    Q_FOREACH (QString desktopfn, list) {
        cmd = QString("delete from appInfo where desktop=\"%1\"")
              .arg(desktopfn);
        sql.exec(cmd);
    }
}

bool deleteAppRecord(QString desktopfn)
{
    bool ret = false;
    QSqlDatabase db = QSqlDatabase::database("DirectoryChangedThread");
    QString cmd = QString("delete from appInfo where desktop=\"%1\"")
                  .arg(desktopfn);
    QSqlQuery sql(db);
    ret = sql.exec(cmd);
    return ret;
}

void execApp(QString desktopfp)
{
    UkuiMenuInterface interface;

    if (interface.checkKreApp(desktopfp)) {
        QProcess::startDetached(interface.getAppExec(desktopfp));
    } else {
        QString appName = interface.getAppExec(desktopfp);
        QStringList strList = (appName.replace("\"", "")).split(" ");

        if (QString(strList.at(0)) == "kmplayer") {
            QProcess::startDetached(strList.at(0));
            return;
        }

        GDesktopAppInfo *desktopAppInfo = g_desktop_app_info_new_from_filename(desktopfp.toLocal8Bit().data());
        g_app_info_launch(G_APP_INFO(desktopAppInfo), nullptr, nullptr, nullptr);
        g_object_unref(desktopAppInfo);
    }

    QFileInfo fileInfo(desktopfp);
    QString desktopfn = fileInfo.fileName();
    if (desktopfn.startsWith("app.web.")) {
        updateDataBaseTableTimes(desktopfn);
    }
}

bool checkOsRelease()
{
    QFile file("/etc/os-release");

    if (file.open(QFile::ReadOnly)) {
        QByteArray line = file.readLine();
        file.close();

        if (QString(line).contains("Ubuntu")) {
            return true;
        }
    }

    return false;
}

//获取用户图像
QString getUserIcon()
{
    qint64 uid = static_cast<qint64>(getuid());
    QDBusInterface iface("org.freedesktop.Accounts",
                         "/org/freedesktop/Accounts",
                         "org.freedesktop.Accounts",
                         QDBusConnection::systemBus());
    QDBusReply<QDBusObjectPath>objPath = iface.call("FindUserById", uid);
    QDBusInterface useriface("org.freedesktop.Accounts",
                             objPath.value().path(),
                             "org.freedesktop.DBus.Properties",
                             QDBusConnection::systemBus());
    QDBusReply<QVariant> var = useriface.call("Get", "org.freedesktop.Accounts.User", "IconFile");
    QString iconstr = var.value().toString();
    return iconstr;
}

QString getUserName()
{
    QString name;
    qint64 uid = static_cast<qint64>(getuid());
    QDBusInterface iface("org.freedesktop.Accounts",
                         "/org/freedesktop/Accounts",
                         "org.freedesktop.Accounts",
                         QDBusConnection::systemBus());
    QDBusReply<QDBusObjectPath> objPath = iface.call("FindUserById", uid);
    QDBusInterface useriface("org.freedesktop.Accounts",
                             objPath.value().path(),
                             "org.freedesktop.DBus.Properties",
                             QDBusConnection::systemBus());
    //    QDBusReply<QVariant> var=useriface.call("Get","org.freedesktop.Accounts.User","RealName");
    //    name=var.value().toString();
    //    if(name.isEmpty())
    //    {
    QDBusReply<QVariant> var = useriface.call("Get", "org.freedesktop.Accounts.User", "UserName");
    name = var.value().toString();
    //    }
    return name;
}
