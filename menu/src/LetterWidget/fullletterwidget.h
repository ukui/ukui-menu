#ifndef FULLLETTERWIDGET_H
#define FULLLETTERWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QToolButton>
#include <QAbstractItemView>
#include <QSpacerItem>
#include <QLabel>
#include "fullletterbuttonwidget.h"
#include "src/RightClickMenu/rightclickmenu.h"

namespace Ui {
class FullLetterWidget;
}

class FullLetterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FullLetterWidget(QWidget *parent=nullptr);
    ~FullLetterWidget();

    /**
      * 设置全屏字母排序界面状态
      * @param state为0时全屏字母排序界面加载应用列表与字母列表，为1时加载纯字母按钮界面FullLetterButtonWidget
      * @param btnname存放有具体的字母
      */
    void set_fullletterwidge_state(int state,QString btnname);

private:
    Ui::FullLetterWidget *ui;

    //主界面
    QVBoxLayout* mainLayout=nullptr;

    //应用列表界面
    QWidget* applistWid=nullptr;
    FullLetterButtonWidget* fullletterbtnwid=nullptr;

    QToolButton* btn=nullptr;//测试按钮

    QTableWidget* apptablewid;//应用列表

    QStringList letterbtnlist;//存放字母按钮
    QStringList letterbtnrowlist;//存放字母按钮所在行

    //字母列表界面
    QWidget* letterlistWid=nullptr;
    QHBoxLayout* letterlistLayout=nullptr;
    QTableWidget* letterlisttableWid=nullptr;
    QToolButton* leftbtn=nullptr;
    QToolButton* rightbtn=nullptr;
    QSpacerItem* letterlistleftSpacer=nullptr;
    QSpacerItem* letterlistrightSpacer=nullptr;

    RightClickMenu* menu;//右键菜单

protected:
    void init_widget();
    void init_letterlist_widget();//初始化字母列表界面
    void init_letterlist_table();//初始化字母列表界面数据表格letterlisttableWid

    void init_applist_widget();//初始化应用列表界面

    void init_app_tablewidget();//初始化应用列表
    void fill_app_tablewidget();//填充应用列表

private slots:
    void leftbtn_clicked_slot();//向左按钮槽函数
    void rightbtn_clicked_slot();//向右按钮槽函数
    void letterlistitem_selected_slot();//字母列表数据项被选定槽函数

    void app_classificationbtn_clicked_slot();//应用列表分类项槽函数
    void recv_letterbtn_signal(QString btnname);//接收FullLetterButtonWidget界面按钮信号

    void exec_app_name();//执行应用程序

    void right_click_slot();//加载右键菜单

signals:
    /**
     * 向MainViewWidget发送全屏字母排序界面状态信号
     * @param state为0时，btnname为具体的字母，为1时，btnname为空
     */
    void send_fullletterwid_state(int state,QString btnname);
};

#endif // FULLLETTERWIDGET_H
