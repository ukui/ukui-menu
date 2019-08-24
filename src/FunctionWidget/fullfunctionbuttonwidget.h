#ifndef FULLFUNCTIONBUTTONWIDGET_H
#define FULLFUNCTIONBUTTONWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QLabel>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>

namespace Ui {
class FullFunctionButtonWidget;
}

class FullFunctionButtonWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FullFunctionButtonWidget(QWidget *parent = 0);
    ~FullFunctionButtonWidget();

private:
    Ui::FullFunctionButtonWidget *ui;

    QVBoxLayout* mainLayout=NULL;
    QSpacerItem* horizontalSpacer=NULL;
    QSpacerItem* verticalSpacer=NULL;


protected:
    void init_widget();
    void add_functionbtn_control(QToolButton *firstbtn, QToolButton *secondbtn);//添加功能分类按钮
    void set_functionbtn_style(QToolButton* btn, QString btnicon, QString btnname);//设置按钮样式

signals:
    /**
     * 向FullFunctionWidget界面发送功能分类按钮被点击信号
     * @param btnname存放被点击按钮的名称
     */
    void send_functionbtn_signal(QString btnname);

private slots:
    void functionbtn_clicked_slot();//功能分类按钮槽函数

};

#endif // FULLFUNCTIONBUTTONWIDGET_H
