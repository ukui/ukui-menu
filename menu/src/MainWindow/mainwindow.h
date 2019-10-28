#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QPaintEvent>
#include <QPainter>
#include <QtMath>
#include "src/SideBarWidget/sidebarwidget.h"
#include "src/MainViewWidget/mainviewwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    //主窗口样式
    QWidget* mainwidget=nullptr;
    QHBoxLayout* mainlayout=nullptr;

    //侧边栏
    SideBarWidget* sidebarwid=nullptr;

    //主视图
    MainViewWidget* mainviewwid=nullptr;

    //动画
//    QPropertyAnimation* pAnimation=nullptr;
//    bool is_full=false;//是否全屏
//    int classification_widget;

protected:
    void init_mainwindow();

    bool event(QEvent *event);//鼠标点击窗口外部事件

    void paintEvent(QPaintEvent*);//添加阴影

private slots:
    /**
     * @显示全屏窗口
     * @param arg分类窗口编号
     */
    void show_fullscreen_widget();
    /**
     * @显示默认(还原)窗口
     * @param arg分类窗口编号
     */
    void show_default_widget();
    void recv_hide_mainwindow_slot();//接收隐藏主窗口信号


//    void animation_finished_slot();


};

#endif // MAINWINDOW_H
