#ifndef ROTATIONLABEL_H
#define ROTATIONLABEL_H
#include <QLabel>

class RotationLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(int rotation READ rotation WRITE setRotation) //旋转
public:
    RotationLabel(QWidget *parent = nullptr);
    void setIcon(const QPixmap &pixmap);
protected:
    int rotation();
    void setRotation(int rot);
    void paintEvent(QPaintEvent *event);
private:
    int m_rotation = 0;
    QPixmap m_pixmap;
};

#endif // ROTATIONLABEL_H
