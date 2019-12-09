#ifndef FULLCOMMONUSEWIDGET_H
#define FULLCOMMONUSEWIDGET_H

#include <QWidget>
#include <QSettings>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QScrollBar>
#include <QLabel>
#include <QPushButton>
#include <QDir>
#include <QProcess>
#include <QSvgRenderer>
#include <QHeaderView>
#include <ukuimenuinterface.h>
#include "src/RightClickMenu/rightclickmenu.h"
#include "src/UtilityFunction/fullitemdelegate.h"
#include "src/UtilityFunction/fulllistview.h"

namespace Ui {
class FullCommonUseWidget;
}

class FullCommonUseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FullCommonUseWidget(QWidget *parent = nullptr);
    ~FullCommonUseWidget();

private:
    Ui::FullCommonUseWidget *ui;

    UkuiMenuInterface* pUkuiMenuInterface=nullptr;

    QSettings *setting=nullptr;
    //主界面
    QVBoxLayout* mainLayout=nullptr;

    //应用列表界面
    QWidget* applistWid=nullptr;
    QTableWidget* apptablewid=nullptr;//应用列表

    FullListView* listview=nullptr;
    FullItemDelegate* m_delegate=nullptr;
    QStringList data;

    QFileSystemWatcher* fileWatcher=nullptr;//监控文件夹状态
    RightClickMenu* menu=nullptr;

protected:
    void init_widget();
    void init_applist_widget();//初始化应用列表界面
    void fill_app_tablewidget();//填充应用列表

private slots:
    void update_listview_slot();//更新应用列表槽函数
    void exec_app_name(QString appname);//执行应用程序

signals:
    void send_update_applist_signal();//向CommonUseWidget发送更新应用列表信号
    void send_hide_mainwindow_signal();//向MainViewWidget发送隐藏主窗口信号
};

#endif // FULLCOMMONUSEWIDGET_H
