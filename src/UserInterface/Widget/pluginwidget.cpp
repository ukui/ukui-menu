#include "pluginwidget.h"
#include "KySmallPluginInterface.h"
#include <QDebug>
pluginwidget::pluginwidget(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setFocusPolicy(Qt::NoFocus);
    init();
}
void pluginwidget::init()
{
    QPluginLoader loader("/opt/small-plugin/bin/libsmall-plugin-manage.so");
    QObject *plugin = loader.instance();

    if (plugin) {
        KySmallPluginInterface *app = qobject_cast<KySmallPluginInterface *>(plugin);
        QWidget *widget = app->createWidget(this);
        widget->setFixedSize(400, 638);
    }
}
