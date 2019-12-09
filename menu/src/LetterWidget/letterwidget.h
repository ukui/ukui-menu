#ifndef LETTERWIDGET_H
#define LETTERWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QTableWidget>
#include <ukuimenuinterface.h>
#include "letterbuttonwidget.h"
#include "src/UtilityFunction/listview.h"
#include "src/UtilityFunction/itemdelegate.h"

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

    UkuiMenuInterface* pUkuiMenuInterface=nullptr;

    QHBoxLayout* mainLayout=nullptr;

    QWidget* applistWid=nullptr;//应用列表界面
    LetterButtonWidget* letterbtnwid=nullptr;//分类按钮界面

    ListView* applistview=nullptr;
    QVector<QStringList> data;
    ItemDelegate* m_delegate=nullptr;

    QStringList letterbtnlist;//存放字母按钮
    QStringList letterbtnrowlist;//存放字母按钮所在行
    QStringList letterposlist;//存放分类字符位置列表
    QStringList appsortlist;//存放应用排序列表

protected:
    void init_widget();
    void init_applist_widget();//初始化应用列表界面
    void fill_app_listview();//填充应用列表

private slots:
    void app_classificationbtn_clicked_slot();//应用列表字母分类按钮槽函数
    void recv_letterbtn_signal(QString btnname);//接收LetterButtonWidget界面按钮信号
    void exec_app_name(QString exec);//执行应用程序
    void update_app_listview(QString desktopfn,QString appname,int arg);//更新应用列表

    void recvItemClickedSlot(QStringList arg);


signals:
    /**
     * 向MainViewWidget发送字母排序界面状态信号
     * @param state为0时，btnname为具体的字母，为1时，btnname为空
     */
    void send_letterwid_state(int state,QString btnname);
    void send_letterbtn_list(QStringList list);//向LetterButtonWidget发送字母按钮列表
    void send_update_applist_signal();//向常用软件模块发送更新应用列表信号
    void send_hide_mainwindow_signal();//向MainViewWidget发送隐藏主窗口信号


};

#endif // LETTERWIDGET_H
