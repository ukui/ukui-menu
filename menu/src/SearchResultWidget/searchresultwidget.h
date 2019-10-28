#ifndef SEARCHRESULTWIDGET_H
#define SEARCHRESULTWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QDesktopWidget>
#include <QLabel>
#include <QToolButton>
#include <QScrollBar>
#include <QPushButton>
#include "kylin-start-menu-interface.h"
#include "searchappthread.h"

namespace Ui {
class SearchResultWidget;
}

class SearchResultWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SearchResultWidget(QWidget *parent = nullptr);
    ~SearchResultWidget();

private:
    Ui::SearchResultWidget *ui;

    QHBoxLayout* mainLayout=nullptr;
    QTableWidget* searchResultTableWid=nullptr;
    SearchAppThread* searchappthread=nullptr;
    QStringList searchResultList;
//    QStringList appnameList;

protected:
    void init_widget();
    void init_search_result_table();//初始化搜索结果列表
    void fill_search_result_table();//填充程序搜索结果列表

private slots:
    void recv_search_keyword(QString arg);//接收MainViewWidget界面搜索框的搜索关键字
    void recv_search_result(QStringList list);

signals:
    void send_search_keyword(QString arg);//向SearchAppThread发送搜索关键字
};

#endif // SEARCHRESULTWIDGET_H
