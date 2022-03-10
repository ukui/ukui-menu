#include "animationpage.h"
#include "utility.h"
#include <QPainter>
#include <KWindowEffects>
#include <QGraphicsBlurEffect>

AnimationPage::AnimationPage()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);
    this->setAutoFillBackground(false);
    this->setAttribute(Qt::WA_TranslucentBackground);
}
void AnimationPage::paintEvent(QPaintEvent *event)
{
    double transparency = getTransparency();
    QRect rect = this->rect();
    QPainter painter(this);
    painter.setBrush(this->palette().base());
    painter.setPen(Qt::transparent);
    painter.setOpacity(transparency);
    painter.drawRect(rect);
    KWindowEffects::enableBlurBehind(this->winId(), true, QRect(rect));
    QWidget::paintEvent(event);
}
