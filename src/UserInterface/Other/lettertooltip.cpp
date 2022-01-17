#include "lettertooltip.h"
#include <QPainter>
#include <QAction>
#include <QHBoxLayout>

LetterToolTip::LetterToolTip(QWidget *parent):
    QDialog(parent)
{
    this->setWindowFlags(/*Qt::FramelessWindowHint *//*| */Qt::X11BypassWindowManagerHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setContentsMargins(0, 0, 0, 0);
    this->setAutoFillBackground(false);
    this->setFixedSize(83, 46);
    QHBoxLayout *mainlayout = new QHBoxLayout;
    this->setLayout(mainlayout);
    mainlayout->setAlignment(Qt::AlignCenter);
    mainlayout->addWidget(&textLabel);
    //    this->raise();
    //    this->activateWindow();
}

LetterToolTip::~LetterToolTip()
{
}

void LetterToolTip::setText(QString str)
{
    QFont font;
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::black);
    font.setPixelSize(30);
    font.setBold(true);
    textLabel.setContentsMargins(0, 0, 0, 0);
    textLabel.setAlignment(Qt::AlignCenter);
    textLabel.setFont(font);
    textLabel.setPalette(pa);
    textLabel.setText(str);
}

void LetterToolTip::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#F2FAFF"));
    painter.drawRoundedRect(5, 0, width() - 5, height(), 4, 4);
    QPointF points[3] = {
        QPointF(0, height() / 2),
        QPointF(5, height() / 2 - 5),
        QPointF(5, height() / 2 + 5),
    };
    painter.drawPolygon(points, 3);
    QWidget::paintEvent(event);
}
