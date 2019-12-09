#ifndef FULLSEARCHRESULTWIDGET_H
#define FULLSEARCHRESULTWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QDesktopWidget>
#include <QLabel>
#include <QToolButton>
#include <QScrollBar>
#include <ukuimenuinterface.h>
#include "src/UtilityFunction/fullitemdelegate.h"
#include "src/UtilityFunction/fulllistview.h"

namespace Ui {
class FullSearchResultWidget;
}

class FullSearchResultWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FullSearchResultWidget(QWidget *parent = nullptr);
    ~FullSearchResultWidget();
    void update_app_listview(QStringList desktopfplist);

private:
    Ui::FullSearchResultWidget *ui;

    UkuiMenuInterface* pUkuiMenuInterface=nullptr;

    QHBoxLayout* mainLayout=nullptr;
    FullListView* listview=nullptr;
    FullItemDelegate* itemdelegate=nullptr;
    QStringList data;

protected:
    void init_widget();

private slots:
    void exec_app_name(QString appname);//执行应用程序

signals:
    void send_update_applist_signal();//向CommonUseWidget发送更新应用列表信号
    void send_hide_mainwindow_signal();//向MainViewWidget发送隐藏主窗口信号

};

#endif // FULLSEARCHRESULTWIDGET_H
