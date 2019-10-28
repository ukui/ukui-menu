#ifndef FUNCTIONWIDGET_H
#define FUNCTIONWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QScrollBar>
#include <QPushButton>
#include "functionbuttonwidget.h"
#include "src/RightClickMenu/rightclickmenu.h"
#include "src/UtilityFunction/qflowlayout.h"

namespace Ui {
class FunctionWidget;
}

class FunctionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FunctionWidget(QWidget *parent=nullptr);
    ~FunctionWidget();

    void load_min_wid();
    void load_max_wid();

private:
    Ui::FunctionWidget *ui;

    //主界面
    QHBoxLayout* mainLayout=nullptr;

    //应用列表界面
    QWidget* applistWid=nullptr;
    FunctionButtonWidget* functionbtnwid=nullptr;

    QTableWidget* apptablewid;//应用列表

    QStringList classificationbtnlist;//存放分类按钮
    QStringList classificationbtnrowlist;//存放分类按钮所在行
    QStringList applistnum;//存放每个分类字母的应用个数

    RightClickMenu* menu;//右键菜单

protected:
    void init_widget();
    void init_applist_widget();//初始化应用列表界面
    void init_app_tablewidget();//初始化应用列表
    void fill_app_tablewidget();//填充应用列表
    void insert_classification_btn(QString btnname);//插入分类按钮
    void insert_app_list(QStringList appnamelist);//插入应用列表

private slots:
    void app_classificationbtn_clicked_slot();//应用列表功能分类按钮槽函数
    void recv_functionbtn_signal(QString btnname);//接收FunctionButtonWidget界面按钮信号
    void exec_app_name();//执行应用程序
    void right_click_slot();//加载右键菜单
    void update_app_tablewidget();//更新应用列表

signals:
    void send_classificationbtn_list(QStringList list);//向FunctionButtonWidget界面发送分类按钮列表
    void send_update_applist_signal();//向CommonUseWidget发送更新应用列表信号
    void send_hide_mainwindow_signal();//向MainViewWidget发送隐藏主窗口信号


};

#endif // FUNCTIONWIDGET_H
