#include "menubox.h"
#include <QDebug>
#include <QMouseEvent>
#include <QEventLoop>
#include <QTimer>

MenuBox::MenuBox(QWidget *parent):
    QMenu(parent)
{
    this->activateWindow();
}

void MenuBox::mousePressEvent(QMouseEvent *event)
{
    this->activateWindow();
    Q_EMIT sendMainWinActiveSignal(true);
    return QMenu::mousePressEvent(event);
}

//void MenuBox::mouseReleaseEvent(QMouseEvent *event)
//{
//    Q_UNUSED(event);
//    this->activateWindow();
//     Q_EMIT sendMainWinActiveSignal(true);
//    if(this->geometry().contains(QCursor::pos()))
//    {
//        return QMenu::mousePressEvent(event);
//    }
//    else
//    {
//        this->raise();
//        this->activateWindow();
//    }

//    return QMenu::mouseReleaseEvent(event);
//}
