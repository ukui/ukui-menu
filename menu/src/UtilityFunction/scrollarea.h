#ifndef SCROLLAREA_H
#define SCROLLAREA_H
#include <QScrollArea>
#include <QEvent>
#include <QScrollBar>
#include <QMouseEvent>

class ScrollArea : public QScrollArea
{
public:
    ScrollArea();

protected:
    void enterEvent(QEvent* e) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent* e) Q_DECL_OVERRIDE;

};

#endif // SCROLLAREA_H
