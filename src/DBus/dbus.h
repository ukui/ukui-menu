#ifndef DBUS_H
#define DBUS_H

#include <QObject>

class DBus : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.ukui.menu")
public:
    explicit DBus(QObject *parent = 0);

Q_SIGNALS:
    void sendReloadSignal();
    void winKeyResponseSignal();

public Q_SLOTS:
    QString GetSecurityConfigPath();
    void ReloadSecurityConfig();
    void WinKeyResponse();
};

#endif // DBUS_H
