#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H
#include <QToolButton>
#include <QEvent>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>


class ToolButton : public QToolButton
{
    Q_OBJECT
public:
    ToolButton(QString iconstr,QString piconstr,QString hoverbg, QString pressedbg);

private:
    QString iconstr;
    QString piconstr;
    QString hoverbg;
    QString pressedbg;
    QLabel* label;
    QHBoxLayout* layout;
    QPixmap* pixmap;

protected:
    void enterEvent(QEvent* e);
    void leaveEvent(QEvent* e);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

};

#endif // TOOLBUTTON_H
