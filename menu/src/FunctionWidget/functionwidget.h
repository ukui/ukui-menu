#ifndef FUNCTIONWIDGET_H
#define FUNCTIONWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include "src/UtilityFunction/scrollarea.h"
#include "functionbuttonwidget.h"
#include "src/RightClickMenu/rightclickmenu.h"
#include "src/UtilityFunction/qflowlayout.h"
#include "src/UtilityFunction/pushbutton.h"
#include "src/UtilityFunction/classifyscrollarea.h"
#include "src/UtilityFunction/classifybutton.h"
#include "src/UtilityFunction/toolbutton.h"

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

    bool is_fullscreen=false;
    bool is_functionbtnwid=false;
    FunctionButtonWidget* functionbtnwid=nullptr;
    //主界面
    QVBoxLayout* mainLayout=nullptr;

    //应用列表界面
    QWidget* applistWid=nullptr;

    ScrollArea* scrollarea=nullptr;
    QWidget* scrollareawid=nullptr;
    QVBoxLayout* scrollareawidLayout=nullptr;

    QStringList classificationbtnlist;//存放分类按钮
    QStringList classificationbtnrowlist;//存放分类按钮所在行

    RightClickMenu* menu=nullptr;//右键菜单

    //图标列表界面
    QStringList iconlist;
    QStringList iconlightlist;
    QStringList functionnamelist;
    QWidget* iconlistWid=nullptr;
    QHBoxLayout* iconlistLayout=nullptr;
    QTableWidget* iconlisttableWid=nullptr;
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
    void init_applist_widget();//初始化应用列表界面
    void fill_app_list();//填充应用列表
    void insert_classification_btn(QString btnname);//插入分类按钮
    void insert_app_list(QStringList appnamelist);//插入应用列表

    void init_iconlist_widget();//初始化图标列表界面
    void init_iconlist_scrollarea();//初始化图标列表

    void add_app(QString classify,QString desktopfn,int num);//添加应用
    void insert_classification_btn(QString classify,int pos);//插入分类按钮
    void insert_appbtn_wid(QString desktopfn,int row);//插入应用按钮界面

private slots:
    void app_classificationbtn_clicked_slot();//应用列表功能分类按钮槽函数
    void recv_functionbtn_signal(QString btnname);//接收FunctionButtonWidget界面按钮信号
    void update_app_list(QString name,int arg);//更新应用列表

    void leftbtn_clicked_slot();//向左按钮槽函数
    void rightbtn_clicked_slot();//向右按钮槽函数
    void iconbtn_clicked_slot();//图标列表按钮被点击槽函数
    void iconbtn_checked_slot(bool check);//图标列表按钮check状态改变

signals:
    void send_classificationbtn_list(QStringList list);//向FunctionButtonWidget界面发送分类按钮列表
    void send_update_applist_signal(QString appname);//向CommonUseWidget发送更新应用列表信号
    void send_hide_mainwindow_signal();//向MainViewWidget发送隐藏主窗口信号


};

#endif // FUNCTIONWIDGET_H
