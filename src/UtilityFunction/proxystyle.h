#ifndef INTERNALSTYLE_H
#define INTERNALSTYLE_H
#include <QProxyStyle>
#include <qgsettings.h>
#include <QVariant>
#include <QPainter>
#include <QPalette>
#include <QStyleOption>

class ProxyStyle : public QProxyStyle
{
    Q_OBJECT
public:
    ProxyStyle();

//    void drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const;
//    void drawControl(QStyle::ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const;
//    void drawComplexControl(QStyle::ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget = nullptr) const;
    void polish(QPalette &pal);
//    void polish(QWidget *widget);
    QPalette standardPalette() const;

private:
    double transparency;

};

#endif // INTERNALSTYLE_H
