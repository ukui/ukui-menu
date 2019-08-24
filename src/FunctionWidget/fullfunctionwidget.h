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
#include "fullfunctionbuttonwidget.h"

namespace Ui {
class FullFunctionWidget;
}

class FullFunctionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FullFunctionWidget(QWidget *parent=0);
    ~FullFunctionWidget();

    /**
     * 设置全屏功能分类界面状态
     * @param state为0时全屏功能分类界面加载应用列表与功能图标列表，为1时加载纯功能分类按钮界面FullFunctionButtonWidget
     * @param btnname存放有具体的功能分类名称
     */
    void set_fullfunctionwidge_state(int state,QString btnname);

private:
    Ui::FullFunctionWidget *ui;

    //主界面
    QVBoxLayout* mainLayout=NULL;

    //应用列表界面
    QWidget* applistWid=NULL;
    FullFunctionButtonWidget* fullfunbtnwid=NULL;

    QToolButton* btn;//测试按钮

    //图标列表界面
    QStringList iconlist;
    QStringList iconlightlist;
    QStringList functionnamelist;
    QWidget* iconlistWid=NULL;
    QHBoxLayout* iconlistLayout=NULL;
    QTableWidget* iconlisttableWid=NULL;
    QToolButton* leftbtn=NULL;
    QToolButton* rightbtn=NULL;
    QSpacerItem* iconlistleftSpacer=NULL;
    QSpacerItem* iconlistrightSpacer=NULL;

protected:
    void init_widget();
    void init_iconlist_widget();//初始化图标列表界面
    void init_iconlist_table();//初始化图标列表界面数据表格iconlisttableWid

    void init_applist_widget();//初始化应用列表界面

private slots:
    void leftbtn_clicked_slot();//向左按钮槽函数
    void rightbtn_clicked_slot();//向右按钮槽函数
    void iconlistitem_selected_slot();//图标列表数据项被选定槽函数

    void app_classificationbtn_clicked_slot();//应用列表功能分类按钮槽函数
    void recv_functionbtn_signal(QString btnname);//接收FullFunctionButtonWidget界面按钮信号

signals:
    /**
     * 向MainViewWidget发送全屏功能分类界面状态
     * 当state为0时，btnname为具体的功能分类名称，当state为1时，btnname为空
     */
    void send_fullfunctionwid_state(int state,QString btnname);
};

#endif // FULLFUNCTIONWIDGET_H
