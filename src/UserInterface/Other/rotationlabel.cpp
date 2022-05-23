#include "rotationlabel.h"
#include <QPainter>
#include <QDebug>

RotationLabel::RotationLabel(QWidget *parent)
    : QLabel(parent)
{
    this->setFrameShape(QFrame::NoFrame);
    this->setContentsMargins(0, 0, 0, 0);
}

void RotationLabel::setRotation(int rot)
{
    m_rotation = rot;
    this->update();
    qDebug() << m_rotation;
}

int RotationLabel::rotation()
{
    return m_rotation;
}

void RotationLabel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
    painter.translate(rect().width() / 2, rect().height() / 2);
    painter.rotate(m_rotation);
    painter.translate(-rect().width() / 2, -rect().height() / 2);
    painter.drawPixmap(rect().adjusted(0, 9, 0, -9), m_pixmap);
    return QLabel::paintEvent(event);
}

void RotationLabel::setIcon(const QPixmap &pixmap)
{
    m_pixmap = pixmap;
}
