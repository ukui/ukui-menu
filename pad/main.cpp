#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QTranslator>
#include <QLocale>
#include <QtSingleApplication>
#include <QSettings>
//#define TABLED_SCHEMA "org.ukui.SettingsDaemon.plugins.tablet-mode"

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);



    //单例
    QtSingleApplication a("ukui-menu-tablet", argc, argv);
    if(a.isRunning())
    {
        a.sendMessage("raise_window_noop");
        return EXIT_SUCCESS;
    }

    MainWindow w;
   // w.removeEventFilter(QMoveEvent);
    a.setActivationWindow(&w);//单例


    int x=QApplication::primaryScreen()->geometry().x();
    int y=QApplication::primaryScreen()->geometry().y();
    w.setGeometry(x,y,QApplication::primaryScreen()->geometry().width(),QApplication::primaryScreen()->geometry().height()-46);

     QTranslator translator;
    if(QLocale().language()==QLocale::Chinese)
    {
        translator.load(":/translation/tr_zh.qm");
        a.installTranslator(&translator);
    }

//    QGSettings* tabletMode= new QGSettings(TABLED_SCHEMA);
//    bool res = tabletMode->get("tablet-mode").toBool();
//    qDebug()<<res;
//    if(!res)
//    {
//        w.hide();
//    }


//    else
//    {
//        w.show();
//        w.raise();
//        w.activateWindow();
//    }

    w.show();

    return a.exec();
}
