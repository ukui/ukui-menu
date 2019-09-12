#ifndef LETTERWIDGET_H
#define LETTERWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QTableWidget>
#include "letterbuttonwidget.h"
#include "src/RightClickMenu/rightclickmenu.h"

namespace Ui {
class LetterWidget;
}

class LetterWidget : public QWidget
{
    Q_OBJECT

public:
     explicit LetterWidget(QWidget *parent=nullptr);
    ~LetterWidget();

    /**
      * 设置字母排序界面状态
      * @param state为0时字母排序界面加载应用列表，为1时加载纯字母按钮界面LetterButtonWidget
      * @param btnname存放有具体的字母
      */
     void set_letterwidge_state(int state,QString btnname);

private:
    Ui::LetterWidget *ui;

    QHBoxLayout* mainLayout=nullptr;

    //应用列表界面
    QWidget* applistWid=nullptr;
    LetterButtonWidget* letterbtnwid=nullptr;

//    QToolButton* btn=nullptr;//测试按钮

    QTableWidget* apptablewid;//应用列表

    QStringList letterbtnlist;//存放字母按钮
    QStringList letterbtnrowlist;//存放字母按钮所在行

    RightClickMenu* menu;//右键菜单

protected:
    void init_widget();
    void init_applist_widget();//初始化应用列表界面

    void init_app_tablewidget();//初始化应用列表
    void fill_app_tablewidget();//填充应用列表
    void insert_letter_btn();//插入字母按钮

private slots:
    void app_classificationbtn_clicked_slot();//应用列表字母分类按钮槽函数
    void recv_letterbtn_signal(QString btnname);//接收LetterButtonWidget界面按钮信号

    void exec_app_name();//执行应用程序

    void right_click_slot();//加载右键菜单

signals:
    /**
     * 向MainViewWidget发送字母排序界面状态信号
     * @param state为0时，btnname为具体的字母，为1时，btnname为空
     */
    void send_letterwid_state(int state,QString btnname);
};

#endif // LETTERWIDGET_H
