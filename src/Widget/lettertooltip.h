#ifndef LETTERTOOLTIP_H
#define LETTERTOOLTIP_H
#include <QDialog>
#include <QLabel>

class LetterToolTip : public QDialog
{
    Q_OBJECT
public:
    LetterToolTip(QWidget *parent = nullptr);

    ~LetterToolTip();

    void setText(QString str);

protected:

    void paintEvent(QPaintEvent *event);

private:
    QLabel textLabel;
};

#endif // LETTERTOOLTIP_H
