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
#include <QDesktopWidget>
#include <QHeaderView>
#include "src/color.h"
#include <QScrollBar>
#include "ukuimenuinterface.h"
#include <QPushButton>
#include <QTableWidgetItem>
#include <QFrame>
#include <QSvgRenderer>
#include <QPainter>
#include <ukuimenuinterface.h>
#include "fullletterbuttonwidget.h"
#include "src/UtilityFunction/scrollarea.h"
#include "src/UtilityFunction/pushbutton.h"
#include "src/UtilityFunction/fulllistview.h"
#include "src/UtilityFunction/classifybutton.h"
#include "src/UtilityFunction/classifyscrollarea.h"
#include "src/UtilityFunction/toolbutton.h"
#include "src/UtilityFunction/fullitemdelegate.h"

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

    UkuiMenuInterface* pUkuiMenuInterface=nullptr;

    //主界面
    QVBoxLayout* mainLayout=nullptr;

    //应用列表界面
    QWidget* applistWid=nullptr;
    FullLetterButtonWidget* fullletterbtnwid=nullptr;

    ScrollArea* scrollarea=nullptr;
    QWidget* scrollareawid=nullptr;
    QVBoxLayout* scrollareawidLayout=nullptr;
    QStringList letterbtnlist;//存放字母按钮
    QStringList letterbtnrowlist;//存放字母按钮所在行
    QStringList letterposlist;//存放分类字符位置列表
    QStringList appsortlist;//存放应用排序列表
    QStringList data;

    //字母列表界面
    QWidget* letterlistWid=nullptr;
    QHBoxLayout* letterlistLayout=nullptr;
    ToolButton* leftbtn=nullptr;
    ToolButton* rightbtn=nullptr;
    QSpacerItem* letterlistleftSpacer=nullptr;
    QSpacerItem* letterlistrightSpacer=nullptr;

    ClassifyScrollArea* letterlistscrollarea=nullptr;
    QWidget* letterlistscrollareaWid=nullptr;
    QHBoxLayout* letterlistscrollareawidLayout=nullptr;
    int btnPos=0;

protected:
    void init_widget();
    void fill_app_list();//填充应用列表
    void init_letterlist_widget();//初始化字母列表界面
    void init_letterlist_scrollarea();//初始化字母列表
    void init_applist_widget();//初始化应用列表界面
    void insert_letter_btn(QString letter,int pos);//插入字母按钮
    void insert_app_listview(QString desktopfn,int pos);//插入应用按钮界面
    void resize_scrollarea_controls();//设置scrollarea填充控件大小

private slots:
    void leftbtn_clicked_slot();//向左按钮槽函数
    void rightbtn_clicked_slot();//向右按钮槽函数
    void letterbtn_clicked_slot();//字母列表数据项被选定槽函数
    void app_classificationbtn_clicked_slot();//应用列表分类项槽函数
    void recv_letterbtn_signal(QString btnname);//接收FullLetterButtonWidget界面按钮信号
    void exec_app_name(QString appname);//执行应用程序
    void update_app_listview(QString desktopfn,QString appname,int arg);//更新应用列表

signals:
    /**
     * 向MainViewWidget发送全屏字母排序界面状态信号
     * @param state为0时，btnname为具体的字母，为1时，btnname为空
     */
    void send_fullletterwid_state(int state,QString btnname);
    void send_letterbtn_list(QStringList list);//向FullLetterButtonWidget发送字母按钮列表
    void send_update_applist_signal();//向常用软件模块发送更新应用列表信号
    void send_hide_mainwindow_signal();//向MainViewWidget发送隐藏主窗口信号
};

#endif // FULLLETTERWIDGET_H
