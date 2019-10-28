#ifndef LETTERWIDGET_H
#define LETTERWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QTableWidget>
#include "letterbuttonwidget.h"
#include "src/RightClickMenu/rightclickmenu.h"
#include "src/UtilityFunction/qflowlayout.h"

namespace Ui {
class LetterWidget;
}

class LetterWidget : public QWidget
{
    Q_OBJECT

public:
     explicit LetterWidget(QWidget *parent=nullptr);
    ~LetterWidget();

     void load_min_wid();
     void load_max_wid();

private:
    Ui::LetterWidget *ui;

    QHBoxLayout* mainLayout=nullptr;

    //应用列表界面
    QWidget* applistWid=nullptr;
    LetterButtonWidget* letterbtnwid=nullptr;

    QTableWidget* apptablewid;//应用列表

    QStringList letterbtnlist;//存放字母按钮
    QStringList letterbtnrowlist;//存放字母按钮所在行
    QStringList applistnum;//存放每个分类字母的应用个数
    QStringList letterposlist;//存放分类字符位置列表
    QStringList appsortlist;//存放应用排序列表

    RightClickMenu* menu;//右键菜单

protected:
    void init_widget();
    void init_applist_widget();//初始化应用列表界面
    void init_app_tablewidget();//初始化应用列表
    void fill_app_tablewidget();//填充应用列表
    QStringList get_alphabetic_classification_applist(int start,int end);//获取属于同一类分类字符应用列表

private slots:
    void app_classificationbtn_clicked_slot();//应用列表字母分类按钮槽函数
    void recv_letterbtn_signal(QString btnname);//接收LetterButtonWidget界面按钮信号
    void exec_app_name();//执行应用程序
    void right_click_slot();//加载右键菜单
    void update_app_tablewidget();//更新应用列表

signals:
    void send_letterbtn_list(QStringList list);//向LetterButtonWidget发送字母按钮列表
    void send_update_applist_signal();//向CommonUseWidget发送更新应用列表信号
    void send_hide_mainwindow_signal();//向MainViewWidget发送隐藏主窗口信号
};

#endif // LETTERWIDGET_H
