#ifndef SIDEBARWIDGET_H
#define SIDEBARWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QMouseEvent>
#include "src/MainViewWidget/mainviewwidget.h"
#include "src/RightClickMenu/rightclickmenu.h"


namespace Ui {
class SideBarWidget;
}

class SideBarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SideBarWidget(QWidget *parent = nullptr);
    ~SideBarWidget();

    /**
     * @设置主界面从全屏回到默认状态时侧边栏按钮焦点状态
     * @param arg按钮编号,1常用分类按钮，2字母分类按钮，3功能分类按钮
     */
    void set_sidebarbtn_state(int arg);

    void load_min_sidebar();//加载默认侧边栏
    void load_max_sidebar();//加载全屏侧边栏


private:
    Ui::SideBarWidget *ui;

    QVBoxLayout* mainlayout=nullptr;
    QWidget* userwid=nullptr;
    QVBoxLayout* userLayout=nullptr;

    QWidget* usericonwid=nullptr;
    QToolButton* usericonbtn=nullptr;
    QHBoxLayout* usericonlayout=nullptr;
    QWidget* usernamewid=nullptr;
    QHBoxLayout* usernamelayout=nullptr;
    QLabel* usernamelabel=nullptr;

    QHBoxLayout* btnlayout=nullptr;
    QLabel* labelicon;
    QPushButton* commonusebtn=nullptr;
    QLabel* commonusebtnname=nullptr;
    QPushButton* letterbtn=nullptr;
    QLabel* letterbtnname=nullptr;
    QPushButton* functionbtn=nullptr;
    QLabel* functionbtnname=nullptr;
    QPushButton* computerbtn=nullptr;
    QLabel* computerbtnname=nullptr;
    QPushButton* controlbtn=nullptr;
    QLabel* controlbtnname=nullptr;
    QPushButton* shutdownbtn=nullptr;
    QLabel* shutdownbtnname=nullptr;
//    QLabel* labelarrow=nullptr;
//    QSpacerItem* verticalSpacer=nullptr;
    QWidget* spaceWid;

    MainViewWidget* mainviewwid=nullptr;

    bool is_fullscreen=false;

    RightClickMenu* shutdownmenu;
    RightClickMenu* othermenu;

protected:
    void init_widget();//初始化主界面
    void add_sidebar_btn();//添加侧边栏按钮
    void init_usericon_widget();//用户图像界面初始化
    void set_btn_style(QPushButton *btn, QString btnicon);//设置按钮样式

    void set_min_sidebar_btn(QPushButton *btn);//设置默认侧边栏按钮
    void set_max_sidebar_btn(QPushButton *btn);//设置全屏侧边栏按钮

    void add_right_click_menu(QPushButton* btn);

//    void mousePressEvent(QMouseEvent* e);

private slots:
//    bool eventFilter(QObject *watched, QEvent *event);
    void commonusebtn_clicked_slot();
    void letterbtn_clicked_slot();
    void functionbtn_clicked_slot();

    void shutdownbtn_right_click_slot();//加载关机按钮右键菜单
    void otherbtn_right_click_slot();//记载右键菜单

signals:
    void send_commonusebtn_signal();//发送常用分类按钮信号
    void send_letterbtn_signal();//发送字母分类按钮信号
    void send_functionbtn_signal();//发送功能分类按钮信号
    void send_fullscreen_commonusebtn_signal();//发送常用分类按钮信号
    void send_fullscreen_letterbtn_signal();//发送字母分类按钮信号
    void send_fullscreen_functionbtn_signal();//发送功能分类按钮信号
    void send_hide_mainwindow_signal();//向MainViewWidget发送隐藏主窗口信号

};

#endif // SIDEBARWIDGET_H
