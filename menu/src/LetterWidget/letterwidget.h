#ifndef LETTERWIDGET_H
#define LETTERWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QScrollBar>
#include "src/UtilityFunction/scrollarea.h"
#include "letterbuttonwidget.h"
#include "src/RightClickMenu/rightclickmenu.h"
#include "src/UtilityFunction/qflowlayout.h"
#include "src/UtilityFunction/pushbutton.h"

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

    bool is_fullscreen=false;
    bool is_letterbtnwid=false;
    LetterButtonWidget* letterbtnwid=nullptr;
    QVBoxLayout* mainLayout=nullptr;

    //应用列表界面
    QWidget* applistWid=nullptr;

    ScrollArea* scrollarea=nullptr;
    QWidget* scrollareawid=nullptr;
    QVBoxLayout* scrollareawidLayout=nullptr;
    QStringList letterbtnlist;//存放字母按钮
    QStringList letterbtnrowlist;//存放字母按钮所在行
    QStringList letterposlist;//存放分类字符位置列表
    QStringList appsortlist;//存放应用排序列表

    RightClickMenu* menu=nullptr;//右键菜单

    //字母列表界面
    QWidget* letterlistWid=nullptr;
    QHBoxLayout* letterlistLayout=nullptr;
    QTableWidget* letterlisttableWid=nullptr;
    QToolButton* leftbtn=nullptr;
    QToolButton* rightbtn=nullptr;
    QSpacerItem* letterlistleftSpacer=nullptr;
    QSpacerItem* letterlistrightSpacer=nullptr;

protected:
    void init_widget();
    void init_applist_widget();//初始化应用列表界面
    void fill_app_list();//填充应用列表
    QStringList get_alphabetic_classification_applist(int start,int end);//获取属于同一类分类字符应用列表

    void init_letterlist_widget();//初始化字母列表界面
    void init_letterlist_table();//初始化字母列表界面数据表格letterlisttableWid

    void insert_letter_btn(QString letter,int pos);//插入字母按钮
    void insert_appbtn_wid(QString desktopfn,int pos);//插入应用按钮界面

private slots:
    void app_classificationbtn_clicked_slot();//应用列表字母分类按钮槽函数
    void recv_letterbtn_signal(QString btnname);//接收LetterButtonWidget界面按钮信号
    void update_app_list(QString name,int arg);//更新应用列表

    void leftbtn_clicked_slot();//向左按钮槽函数
    void rightbtn_clicked_slot();//向右按钮槽函数
    void letterlistitem_selected_slot();//字母列表数据项被选定槽函数

signals:
    void send_letterbtn_list(QStringList list);//向LetterButtonWidget发送字母按钮列表
    void send_update_applist_signal(QString appname);//向CommonUseWidget发送更新应用列表信号
    void send_hide_mainwindow_signal();//向MainViewWidget发送隐藏主窗口信号
};

#endif // LETTERWIDGET_H
