#include "dbus.h"
#include <QFileInfo>
#include <QDir>

DBus::DBus(QObject *parent) : QObject(parent)
{

}

void DBus::ReloadSecurityConfig()
{
    Q_EMIT sendReloadSignal();
}

QString DBus::GetSecurityConfigPath()
{
    QString path=QDir::homePath()+"/.config/ukui-menu-security-config.json";
    return path;
}
