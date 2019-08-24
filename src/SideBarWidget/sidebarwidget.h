#ifndef SIDEBARWIDGET_H
#define SIDEBARWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QVBoxLayout>
#include <QSpacerItem>
#include "src/MainViewWidget/mainviewwidget.h"
#include <QLabel>

namespace Ui {
class SideBarWidget;
}

class SideBarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SideBarWidget(QWidget *parent = 0);
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

    QVBoxLayout* mainlayout=NULL;
    QWidget* userwid=NULL;
    QVBoxLayout* userLayout=NULL;

    QWidget* usericonwid=NULL;
    QToolButton* usericonbtn=NULL;
    QHBoxLayout* usericonlayout=NULL;
    QWidget* usernamewid=NULL;
    QHBoxLayout* usernamelayout=NULL;
    QLabel* usernamelabel=NULL;

    QHBoxLayout* btnlayout=NULL;
    QToolButton* commonusebtn=NULL;
    QLabel* commonusebtnname=NULL;
    QToolButton* letterbtn=NULL;
    QLabel* letterbtnname=NULL;
    QToolButton* functionbtn=NULL;
    QLabel* functionbtnname=NULL;
    QToolButton* computerbtn=NULL;
    QLabel* computerbtnname=NULL;
    QToolButton* controlbtn=NULL;
    QLabel* controlbtnname=NULL;
    QToolButton* shutdownbtn=NULL;
    QLabel* shutdownbtnname=NULL;
    QLabel* labelarrow=NULL;
//    QSpacerItem* verticalSpacer=NULL;
    QWidget* spaceWid;

    MainViewWidget* mainviewwid=NULL;

protected:
    void init_widget();//初始化主界面
    void add_sidebar_btn();//添加侧边栏按钮
    void init_usericon_widget();//用户图像界面初始化
    void set_btn_style(QToolButton* btn, QString btnicon);//设置按钮样式

    void set_min_sidebar_btn(QToolButton* btn);//设置默认侧边栏按钮
    void set_max_sidebar_btn(QToolButton* btn);//设置全屏侧边栏按钮

private slots:
    bool eventFilter(QObject *watched, QEvent *event);

signals:
    void send_commonusebtn_signal();//发送常用分类按钮信号
    void send_letterbtn_signal();//发送字母分类按钮信号
    void send_functionbtn_signal();//发送功能分类按钮信号

    void send_fullscreen_commonusebtn_signal();//发送常用分类按钮信号
    void send_fullscreen_letterbtn_signal();//发送字母分类按钮信号
    void send_fullscreen_functionbtn_signal();//发送功能分类按钮信号

};

#endif // SIDEBARWIDGET_H
