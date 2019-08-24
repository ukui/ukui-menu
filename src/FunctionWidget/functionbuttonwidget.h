#ifndef FUNCTIONBUTTONWIDGET_H
#define FUNCTIONBUTTONWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QLabel>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>

namespace Ui {
class FunctionButtonWidget;
}

class FunctionButtonWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FunctionButtonWidget(QWidget *parent = 0);
    ~FunctionButtonWidget();

private:
    Ui::FunctionButtonWidget *ui;

    QVBoxLayout* mainLayout=NULL;
    QSpacerItem* horizontalSpacer=NULL;
    QSpacerItem* verticalSpacer=NULL;


protected:
    void init_widget();
    void add_functionbtn_control(QToolButton *firstbtn, QToolButton *secondbtn);//添加功能分类按钮
    void set_functionbtn_style(QToolButton* btn, QString btnicon, QString btnname);//设置功能分类按钮样式

signals:
    /**
     * 向FunctionWidget界面发送功能分类按钮被点击信号
     * @param btnname存放被点击按钮名称
     */
    void send_functionbtn_signal(QString btnname);

private slots:
    void functionbtn_clicked_slot();//功能分类按钮槽函数
};

#endif // FUNCTIONBUTTONWIDGET_H
