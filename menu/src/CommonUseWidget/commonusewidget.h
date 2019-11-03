#ifndef COMMONUSEWIDGET_H
#define COMMONUSEWIDGET_H

#include <QWidget>
#include <QDBusMessage>
#include <QSettings>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QScrollBar>
#include <QLabel>
#include <QPushButton>
#include <QProcess>
#include <QSvgRenderer>
#include <QHeaderView>
#include <QDesktopWidget>
#include "kylin-start-menu-interface.h"
#include "src/RightClickMenu/rightclickmenu.h"
#include "src/UtilityFunction/qflowlayout.h"

namespace Ui {
class CommonUseWidget;
}

class CommonUseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CommonUseWidget(QWidget *parent = nullptr);
    ~CommonUseWidget();

    void load_min_wid();
    void load_max_wid();

private:
    Ui::CommonUseWidget *ui;
    QSettings *setting=nullptr;

    QHBoxLayout* mainLayout=nullptr;
    bool is_fullscreen=false;

    //应用列表界面
    QWidget* applistWid=nullptr;

    QTableWidget* apptablewid=nullptr;//应用列表
    RightClickMenu* menu=nullptr;
    QStringList applist;

    QWidget* wid;
    QFlowLayout* flowlayout;

protected:
    void init_widget();

    void init_applist_widget();//初始化应用列表界面

    void init_app_tablewidget();//初始化应用列表
    void fill_app_tablewidget();//填充应用列表


private slots:
    void ViewOpenedSlot(QDBusMessage msg);
    void exec_app_name();//执行应用程序
    void right_click_slot();//加载右键菜单
    void update_tablewid_slot(QString appname);//更新应用列表槽函数

signals:
    void send_hide_mainwindow_signal();//向MainViewWidget发送隐藏主窗口信号
};

#endif // COMMONUSEWIDGET_H
