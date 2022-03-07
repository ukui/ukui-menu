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
//    QGraphicsBlurEffect *ef = new QGraphicsBlurEffect;
//    ef->setBlurRadius(1);
//    ef->setBlurHints(QGraphicsBlurEffect::AnimationHint);
//    this->setGraphicsEffect(ef);
//    this->setStyleSheet("backdrop-filter:blur(8px);");
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
    QLabel::paintEvent(event);
}
