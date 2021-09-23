#include "pluginwidget.h"
#include "KySmallPluginInterface.h"
#include <QDebug>
pluginwidget::pluginwidget(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setFocusPolicy(Qt::NoFocus);

    mainvbox=new QVBoxLayout(this);
    this->setLayout(mainvbox);
    mainvbox->setContentsMargins(0,0,0,0);
    usrInterface = new QDBusInterface("com.kylin.statusmanager.interface",
                              "/",
                              "com.kylin.statusmanager.interface",
                              QDBusConnection::sessionBus());

     QDBusConnection::sessionBus().connect("com.kylin.statusmanager.interface",
                                         "/",
                                         "com.kylin.statusmanager.interface",
                                         "stylename_change_signal",
                                         this,
                                         SLOT(changeSearchBoxBackground(QString))
                                         );

    init();
}
void pluginwidget::init()
{
    QPluginLoader loader("/opt/small-plugin/bin/libsmall-plugin-manage.so");
    QObject * plugin=loader.instance();
    if(plugin)
    {
        KySmallPluginInterface * app=qobject_cast<KySmallPluginInterface *>(plugin);
        widget = app->createWidget(this);
        widget->setFixedSize(400,638);
        mainvbox->addWidget(widget);
    }
}

//搜索框适配主题
void pluginwidget::changeSearchBoxBackground(QString styleName)
{
//    themeName=themeSetting->get("style-name").toString();
    if(styleName == "ukui-dark")
    {
        toolbutton_widget->setStyleSheet("border-radius:40px;background:#FF2C3239;color:white;");
    }
    else
    {
        toolbutton_widget->setStyleSheet("border-radius:40px;background:white;color:rgba(58,67,78,0.25);");
    }
}

pluginwidget::~pluginwidget()
{
    if(widget)
        delete widget;
    if(toolbutton_widget)
        delete toolbutton_widget;
    if(themeSetting)
        delete themeSetting;
    widget=nullptr;
    toolbutton_widget=nullptr;
    themeSetting=nullptr;
}
