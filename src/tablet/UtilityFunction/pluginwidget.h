#ifndef PLUGINWIDGET_H
#define PLUGINWIDGET_H

#include <QObject>
#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPluginLoader>
#include <src/tablet/UtilityFunction/AbstractInterface.h>
#include <src/tablet/UtilityFunction/abstractInterface.h>
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
    ~pluginwidget();
    QWidget *widget = nullptr;
    void init();
    QVBoxLayout *mainvbox = nullptr;

private:
    QGSettings *themeSetting = nullptr;
    QString themeName;
    QWidget *toolbutton_widget;
    QDBusInterface *usrInterface = nullptr;

public Q_SLOTS:

    void changeSearchBoxBackground(QString styleName);

};

#endif // PLUGINWIDGET_H
