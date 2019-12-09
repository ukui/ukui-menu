#ifndef FULLFUNCTIONWIDGET_H
#define FULLFUNCTIONWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QToolButton>
#include <QAbstractItemView>
#include <QSpacerItem>
#include <QLabel>
#include <QScrollBar>
#include <QPushButton>
#include <ukuimenuinterface.h>
#include "fullfunctionbuttonwidget.h"
//#include "src/RightClickMenu/rightclickmenu.h"
#include "src/UtilityFunction/scrollarea.h"
#include "src/UtilityFunction/pushbutton.h"
#include "src/UtilityFunction/fulllistview.h"
#include "src/UtilityFunction/classifyscrollarea.h"
#include "src/UtilityFunction/classifybutton.h"
#include "src/UtilityFunction/toolbutton.h"
#include "src/UtilityFunction/fullitemdelegate.h"

namespace Ui {
class FullFunctionWidget;
}

class FullFunctionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FullFunctionWidget(QWidget *parent=nullptr);
    ~FullFunctionWidget();

    /**
     * 设置全屏功能分类界面状态
     * @param state为0时全屏功能分类界面加载应用列表与功能图标列表，为1时加载纯功能分类按钮界面FullFunctionButtonWidget
     * @param btnname存放有具体的功能分类名称
     */
    void set_fullfunctionwidge_state(int state,QString btnname);

private:
    Ui::FullFunctionWidget *ui;

    UkuiMenuInterface* pUkuiMenuInterface=nullptr;

    //主界面
    QVBoxLayout* mainLayout=nullptr;

    //应用列表界面
    QWidget* applistWid=nullptr;
    FullFunctionButtonWidget* fullfunbtnwid=nullptr;

    ScrollArea* scrollarea=nullptr;
    QWidget* scrollareawid=nullptr;
    QVBoxLayout* scrollareawidLayout=nullptr;
    QStringList classificationbtnlist;//存放分类按钮
    QStringList classificationbtnrowlist;//存放分类按钮所在行
    QStringList data;
//    int pos=0;

    //图标列表界面
    QStringList iconlist;
    QStringList iconlightlist;
    QStringList functionnamelist;
    QWidget* iconlistWid=nullptr;
    QHBoxLayout* iconlistLayout=nullptr;
    ToolButton* leftbtn=nullptr;
    ToolButton* rightbtn=nullptr;
    QSpacerItem* iconlistleftSpacer=nullptr;
    QSpacerItem* iconlistrightSpacer=nullptr;

    ClassifyScrollArea* iconlistscrollarea=nullptr;
    QWidget* iconlistscrollareaWid=nullptr;
    QHBoxLayout* iconlistscrollareawidLayout=nullptr;
    int btnPos=0;
    int beforebtnPos=0;

protected:
    void init_widget();
    void init_iconlist_widget();//初始化图标列表界面
    void init_iconlist_scrollarea();//初始化图标列表界面数据表格iconlisttableWid

    void init_applist_widget();//初始化应用列表界面
    void fill_app_tablewidget();//填充应用列表
    void insert_classification_btn(QString btnname);//插入分类按钮
    void insert_app_list(QStringList appnamelist);//插入应用列表
    void resize_scrollarea_controls();//设置scrollarea填充控件大小
    void add_app(QString classify,QString desktopfn,int num);//添加应用
    void insert_classification_btn(QString classify,int pos);//插入分类按钮
    void insert_app_listview(QString desktopfn,int pos);//插入应用按钮界面

private slots:
    void exec_app_name(QString appname);//执行应用程序
    void leftbtn_clicked_slot();//向左按钮槽函数
    void rightbtn_clicked_slot();//向右按钮槽函数
    void iconbtn_clicked_slot();//图标列表数据项被点击槽函数
    void iconbtn_checked_slot(bool check);//图标列表按钮check状态改变

    void app_classificationbtn_clicked_slot();//应用列表功能分类按钮槽函数
    void recv_functionbtn_signal(QString btnname);//接收FullFunctionButtonWidget界面按钮信号
    void update_app_listview(QString desktopfn,QString appname,int arg);//更新应用列表

signals:
    /**
     * 向MainViewWidget发送全屏功能分类界面状态
     * 当state为0时，btnname为具体的功能分类名称，当state为1时，btnname为空
     */
    void send_fullfunctionwid_state(int state,QString btnname);
    void send_classificationbtn_list(QStringList list);//向FullFunctionButtonWidget界面发送分类按钮列表
    void send_update_applist_signal();//向常用软件模块发送更新应用列表信号
    void send_hide_mainwindow_signal();//向MainViewWidget发送隐藏主窗口信号
};

#endif // FULLFUNCTIONWIDGET_H
