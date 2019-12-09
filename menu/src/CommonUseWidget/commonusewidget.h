#ifndef COMMONUSEWIDGET_H
#define COMMONUSEWIDGET_H

#include <QWidget>
#include <QDBusMessage>
#include <QSettings>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QScrollBar>
#include <QLabel>
#include <QPushButton>
#include <QProcess>
#include <QSvgRenderer>
#include <QHeaderView>
#include <ukuimenuinterface.h>
#include "src/RightClickMenu/rightclickmenu.h"
#include "src/UtilityFunction/listview.h"
#include "src/UtilityFunction/itemdelegate.h"

namespace Ui {
class CommonUseWidget;
}

class CommonUseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CommonUseWidget(QWidget *parent = nullptr);
    ~CommonUseWidget();

private:
    Ui::CommonUseWidget *ui;
    QSettings *setting=nullptr;

    QHBoxLayout* mainLayout=nullptr;

    UkuiMenuInterface* pUkuiMenuInterface=nullptr;
    //应用列表界面
    QWidget* applistWid=nullptr;

    ListView* listview=nullptr;
    ItemDelegate* m_delegate=nullptr;
    QVector<QStringList> data;

    RightClickMenu* menu=nullptr;

protected:
    void init_widget();
    void init_applist_widget();//初始化应用列表界面
    void fill_app_tablewidget();//填充应用列表


private slots:
    void update_listview_slot();//更新应用列表槽函数
    void exec_app_name(QStringList arg);//执行应用程序

signals:
    void send_hide_mainwindow_signal();//向MainViewWidget发送隐藏主窗口信号
    void send_update_applist_signal();//向FullCommonUseWidget发送更新应用列表信号
};

#endif // COMMONUSEWIDGET_H
