#ifndef CLASSIFYBUTTON_H
#define CLASSIFYBUTTON_H
#include <QToolButton>
#include <QSvgRenderer>
#include <QPainter>
#include "src/color.h"

class ClassifyButton : public QToolButton
{
public:
    /**
     * @param name分类名称
     * @param classify为0时为字母分类按钮、1时为功能分类按钮
     */
    ClassifyButton(QString name,int classify,QString iconstr);

private:
    QString name;
    int classify;
    QString iconstr;

protected:
    void init_btn();
};

#endif // CLASSIFYBUTTON_H
