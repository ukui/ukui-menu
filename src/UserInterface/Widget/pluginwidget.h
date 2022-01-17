#ifndef PLUGINWIDGET_H
#define PLUGINWIDGET_H

#include <QObject>
#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPluginLoader>
#include <AbstractInterface.h>
#include <abstractInterface.h>
#include <QLibrary>
#include <QEvent>
#include <QLabel>
#include <QGSettings>
#include <QSettings>
#include <QDBusInterface>
#include <QDBusReply>
class pluginwidget : public QWidget
{
    Q_OBJECT
public:
    explicit pluginwidget(QWidget *parent = nullptr);
    void init();
};

#endif // PLUGINWIDGET_H
