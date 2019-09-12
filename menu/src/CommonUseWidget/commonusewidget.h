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
    explicit CommonUseWidget(QWidget *parent = nullptr);
    ~CommonUseWidget();

private:
    Ui::CommonUseWidget *ui;

protected:
    void init_widget();
};

#endif // COMMONUSEWIDGET_H
