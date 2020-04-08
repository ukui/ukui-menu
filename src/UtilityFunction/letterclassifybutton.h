#ifndef LETTERCLASSIFYBUTTON_H
#define LETTERCLASSIFYBUTTON_H
#include <QToolButton>
#include <QEvent>
#include <QMouseEvent>
#include "src/Style/style.h"

class LetterClassifyButton : public QToolButton
{
    Q_OBJECT
public:
    LetterClassifyButton(QWidget *parent,
                         QString hoverbg,
                         QString pressedbg,
                         QString letter
                         );

    bool is_pressed=false;

private:
    QString hoverbg;
    QString pressedbg;

protected:
    void enterEvent(QEvent* e);
    void leaveEvent(QEvent* e);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

Q_SIGNALS:
    void buttonClicked(QAbstractButton* btn);

private Q_SLOTS:
    void reactToToggle(bool checked);
};

#endif // LETTERCLASSIFYBUTTON_H
