#ifndef WINDOWMOVE_H
#define WINDOWMOVE_H

#include <QWidget>
#include <QMouseEvent>
#include <QDesktopWidget>

namespace Ui {
class WindowMove;
}

class WindowMove : public QWidget
{
    Q_OBJECT

public:
    explicit WindowMove(QWidget *parent = 0);
    ~WindowMove();

private:
    Ui::WindowMove *ui;
    QPoint press_point;
    bool is_move;

protected:
    /*
     * 设置窗口移动事件
     */
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
};

#endif // WINDOWMOVE_H
