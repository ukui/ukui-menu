#ifndef FUNCTIONWIDGET_H
#define FUNCTIONWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QScrollBar>
#include <QPushButton>
#include <ukuimenuinterface.h>
#include "functionbuttonwidget.h"
#include "src/RightClickMenu/rightclickmenu.h"
#include "src/UtilityFunction/listview.h"
#include "src/UtilityFunction/itemdelegate.h"

namespace Ui {
class FunctionWidget;
}

class FunctionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FunctionWidget(QWidget *parent=nullptr);
    ~FunctionWidget();

    /**
     * 设置功能分类界面状态
     * @param state为0时功能分类界面加载应用列表，为1时加载纯功能分类按钮界面FunctionButtonWidget
     * @param btnname存放有具体的功能分类名称
     */
    void set_functionwidge_state(int state,QString btnname);

private:
    Ui::FunctionWidget *ui;

    UkuiMenuInterface* pUkuiMenuInterface=nullptr;

    //主界面
    QHBoxLayout* mainLayout=nullptr;


    QWidget* applistWid=nullptr;//应用列表界面
    FunctionButtonWidget* functionbtnwid=nullptr;//分类列表界面

    ListView* applistview=nullptr;
    QVector<QStringList> data;
    ItemDelegate* m_delegate=nullptr;

    QStringList classificationbtnlist;//存放分类按钮
    QStringList classificationbtnrowlist;//存放分类按钮所在行
    int row=0;

    RightClickMenu* menu;//右键菜单

protected:
    void init_widget();
    void init_applist_widget();//初始化应用列表界面
    void fill_app_listview();//填充应用列表
    void insert_classification_btn(QString btnname);//插入分类按钮
    void insert_app_list(QStringList appnamelist);//插入应用列表

private slots:
    void app_classificationbtn_clicked_slot();//应用列表功能分类按钮槽函数
    void recv_functionbtn_signal(QString btnname);//接收FunctionButtonWidget界面按钮信号
    void exec_app_name(QString exec);//执行应用程序
    void update_app_listview(QString desktopfn,QString appname,int arg);//更新应用列表

    void recvItemClickedSlot(QStringList arg);

signals:
    /**
     * 向MainViewWidget发送功能分类界面状态
     * 当state为0时，btnname为具体的功能分类名称，当state为1时，btnname为空
     */
    void send_functionwid_state(int state,QString btnname);
    void send_classificationbtn_list(QStringList list);//向FunctionButtonWidget界面发送分类按钮列表
    void send_update_applist_signal();//向常用软件模块发送更新应用列表信号
    void send_hide_mainwindow_signal();//向MainViewWidget发送隐藏主窗口信号


};

#endif // FUNCTIONWIDGET_H
