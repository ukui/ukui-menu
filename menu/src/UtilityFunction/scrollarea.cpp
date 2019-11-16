#include "scrollarea.h"
#include <QDebug>

ScrollArea::ScrollArea()
{
    this->verticalScrollBar()->setVisible(false);
    this->verticalScrollBar()->setStyleSheet("QScrollBar{width:12px;padding-top:0px;padding-bottom:0px;background-color:#283138;border-radius:6px;}"
                                             "QScrollBar::handle{background-color:#414e59; width:12px;border-radius:6px;}"
                                             "QScrollBar::handle:hover{background-color:#697883;border-radius:6px;}"
                                             "QScrollBar::handle:pressed{background-color:#8897a3;border-radius:6px;}"
                                             "QScrollBar::sub-line{background-color:transparent;height:0px;width:0px;}"
                                             "QScrollBar::add-line{background-color:transparent;height:0px;width:0px;}"
                                             );
    installEventFilter(this);

}

void ScrollArea::enterEvent(QEvent *e)
{
    Q_UNUSED(e);
    this->verticalScrollBar()->setVisible(true);
}

void ScrollArea::leaveEvent(QEvent *e)
{
    Q_UNUSED(e);
    this->verticalScrollBar()->setVisible(false);
}
