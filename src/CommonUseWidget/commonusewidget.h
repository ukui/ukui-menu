#ifndef COMMONUSEWIDGET_H
#define COMMONUSEWIDGET_H

#include <QWidget>

namespace Ui {
class CommonUseWidget;
}

class CommonUseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CommonUseWidget(QWidget *parent = 0);
    ~CommonUseWidget();

private:
    Ui::CommonUseWidget *ui;

protected:
    void init_widget();
};

#endif // COMMONUSEWIDGET_H
