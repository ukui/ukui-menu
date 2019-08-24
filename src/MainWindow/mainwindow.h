#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include "src/SideBarWidget/sidebarwidget.h"
#include "src/MainViewWidget/mainviewwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    //主窗口样式
    QWidget* mainwidget=NULL;
    QHBoxLayout* mainlayout=NULL;

    //侧边栏
    SideBarWidget* sidebarwid=NULL;

    //主视图
    MainViewWidget* mainviewwid=NULL;

protected:
    void init_mainwindow();

    bool event(QEvent *event);//鼠标点击窗口外部事件

private slots:
    /**
     * @显示全屏窗口
     * @param arg分类窗口编号
     */
    void show_fullscreen_widget(int arg);
    /**
     * @显示默认(还原)窗口
     * @param arg分类窗口编号
     */
    void show_default_widget(int arg);
};

#endif // MAINWINDOW_H
