#ifndef FULLCOMMONUSEWIDGET_H
#define FULLCOMMONUSEWIDGET_H

#include <QWidget>

namespace Ui {
class FullCommonUseWidget;
}

class FullCommonUseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FullCommonUseWidget(QWidget *parent = nullptr);
    ~FullCommonUseWidget();

private:
    Ui::FullCommonUseWidget *ui;

protected:
    void init_widget();
};

#endif // FULLCOMMONUSEWIDGET_H
