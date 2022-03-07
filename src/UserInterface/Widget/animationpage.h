#ifndef ANIMATIONPAGE_H
#define ANIMATIONPAGE_H
#include <QLabel>

class AnimationPage : public QLabel
{
public:
    AnimationPage();
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // ANIMATIONPAGE_H
