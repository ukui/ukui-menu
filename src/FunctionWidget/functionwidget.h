#ifndef FUNCTIONWIDGET_H
#define FUNCTIONWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include "functionbuttonwidget.h"

namespace Ui {
class FunctionWidget;
}

class FunctionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FunctionWidget(QWidget *parent=0);
    ~FunctionWidget();

    /**
     * 设置功能分类界面状态
     * @param state为0时功能分类界面加载应用列表，为1时加载纯功能分类按钮界面FunctionButtonWidget
     * @param btnname存放有具体的功能分类名称
     */
    void set_functionwidge_state(int state,QString btnname);

private:
    Ui::FunctionWidget *ui;

    //主界面
    QHBoxLayout* mainLayout=NULL;

    //应用列表界面
    QWidget* applistWid=NULL;
    FunctionButtonWidget* functionbtnwid=NULL;

    QToolButton* btn=NULL;//测试按钮

protected:
    void init_widget();
    void init_applist_widget();//初始化应用列表界面

private slots:
    void app_classificationbtn_clicked_slot();//应用列表功能分类按钮槽函数
    void recv_functionbtn_signal(QString btnname);//接收FunctionButtonWidget界面按钮信号

signals:
    /**
     * 向MainViewWidget发送功能分类界面状态
     * 当state为0时，btnname为具体的功能分类名称，当state为1时，btnname为空
     */
    void send_functionwid_state(int state,QString btnname);
};

#endif // FUNCTIONWIDGET_H
