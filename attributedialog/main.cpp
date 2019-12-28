#include "attributedialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile qss(":/ukui-menu-attr.qss");
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());

    AttributeDialog w;
    w.set_attribute_value(argv[1]);
    w.show();

    return a.exec();
}
