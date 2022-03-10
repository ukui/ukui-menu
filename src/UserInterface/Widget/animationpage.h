#ifndef ANIMATIONPAGE_H
#define ANIMATIONPAGE_H
#include <QLabel>

class AnimationPage : public QWidget
{
public:
    AnimationPage();
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // ANIMATIONPAGE_H
