#include "src/MainWindow/mainwindow.h"
#include <QtSingleApplication>
#include <QDesktopWidget>
#include <QFile>
#include "ukuimenuinterface.h"

int main(int argc, char *argv[])
{
    QtSingleApplication app("ukui-start-menu",argc,argv);
    app.setQuitOnLastWindowClosed(false);

    if(app.isRunning())
    {
        app.sendMessage("raise_window_noop");
        return EXIT_SUCCESS;
    }

    QFile qss(":/data/qss/kylin-start-menu.qss");
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());

    MainWindow w;
    w.setGeometry(QRect(0,QtSingleApplication::desktop()->availableGeometry().height()-590-10,376+10,590+10));
    app.setActivationWindow(&w);

    w.show();
    w.raise();
    w.activateWindow();
    return app.exec();
}
