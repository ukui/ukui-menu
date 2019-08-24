#include "src/MainWindow/mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile qss(":/data/qss/kylin-start-menu.qss");
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());

    MainWindow w;
//    w.move(0,a.desktop()->height()-w.height());
    w.setGeometry(QRect(0,QApplication::desktop()->availableGeometry().height()-532,390,532));

    w.show();
    w.raise();
    w.activateWindow();

    return a.exec();
}
