#ifndef ANIMATIONPAGE_H
#define ANIMATIONPAGE_H
#include <QLabel>

class AnimationPage : public QWidget
{
    Q_OBJECT
public:
    AnimationPage(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // ANIMATIONPAGE_H
