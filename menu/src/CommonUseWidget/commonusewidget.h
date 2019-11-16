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
#include "src/UtilityFunction/scrollarea.h"
#include "kylin-start-menu-interface.h"
#include "src/RightClickMenu/rightclickmenu.h"
#include "src/UtilityFunction/qflowlayout.h"
#include "src/UtilityFunction/pushbutton.h"

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

    ScrollArea* scrollarea;
    QWidget* scrollareawid;
    QFlowLayout* flowlayout;

    RightClickMenu* menu=nullptr;
    QStringList applist;

    QWidget* wid;

protected:
    void init_widget();

    void init_applist_widget();//初始化应用列表界面

    void fill_app_list();//填充应用列表


private slots:
    void ViewOpenedSlot(QDBusMessage msg);
    void update_app_list(QString appname);//字母排序与功能分类模块固定或取消固定到常用软件时更新应用列表
    void recv_right_click_slot(int ret);//接收右键菜单返回值槽函数
    void update_app_list(QString name,int arg);//有软件卸载时更新应用列表

signals:
    void send_hide_mainwindow_signal();//向MainViewWidget发送隐藏主窗口信号
};

#endif // COMMONUSEWIDGET_H
