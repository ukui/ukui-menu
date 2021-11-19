#include "menubox.h"
#include <QDebug>
#include <QMouseEvent>
#include <QEventLoop>
#include <QTimer>

MenuBox::MenuBox(QWidget *parent):
    QMenu(parent)
{
}

void MenuBox::mousePressEvent(QMouseEvent *event)
{
    if (!this->actionAt(event->pos())) {
        Q_EMIT sendMainWinActiveSignal(true);
    }

    qDebug() << "void MenuBox::mousePressEvent(QMouseEvent *event)";
    return QMenu::mousePressEvent(event);
}

