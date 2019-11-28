#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H
#include <QToolButton>
#include <QEvent>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QSvgRenderer>
#include <QPainter>

class ToolButton : public QToolButton
{
    Q_OBJECT
public:
    /**
     * @param iconstr正常图片
     * @param piconstr点击图片
     * @param hoverbg悬浮背景色
     * @param pressedbg点击后背景色
     * @param module为0时为常用模块，1为字母模块、2为功能模块,3为属性模块
     */
    ToolButton(int width, int height,QString iconstr,QString piconstr,QString hoverbg, QString pressedbg, int module);

private:
    QString iconstr;
    QString piconstr;
    QString hoverbg;
    QString pressedbg;
    int module;
    QLabel* label;
    QHBoxLayout* layout;
    QPixmap* pixmap;
    QSvgRenderer* svgRender;

protected:
    void enterEvent(QEvent* e);
    void leaveEvent(QEvent* e);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

};

#endif // TOOLBUTTON_H
