#ifndef MENUBOX_H
#define MENUBOX_H
#include <QMenu>
#include <QObject>

class MenuBox : public QMenu
{
    Q_OBJECT
public:
    MenuBox(QWidget *parent = nullptr);

protected:

    void mousePressEvent(QMouseEvent *event);

//    void mouseReleaseEvent(QMouseEvent *event);

//    void focusInEvent(QFocusEvent *event);

//    void focusOutEvent(QFocusEvent *event);
public:
Q_SIGNALS:
    void sendMainWinActiveSignal(bool flag);


private:
    bool isFocusIn = false;
};

#endif // MENUBOX_H
