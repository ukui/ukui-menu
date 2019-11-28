#ifndef CLASSIFYSCROLLAREA_H
#define CLASSIFYSCROLLAREA_H
#include <QScrollArea>
#include <QScrollBar>

class ClassifyScrollArea : public QScrollArea
{
public:
    ClassifyScrollArea();

protected:
    void init_wid();
};

#endif // CLASSIFYSCROLLAREA_H
