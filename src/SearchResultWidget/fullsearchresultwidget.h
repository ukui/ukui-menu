#ifndef FULLSEARCHRESULTWIDGET_H
#define FULLSEARCHRESULTWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QDesktopWidget>
#include <QLabel>
#include <QToolButton>
#include <QScrollBar>

namespace Ui {
class FullSearchResultWidget;
}

class FullSearchResultWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FullSearchResultWidget(QWidget *parent = 0);
    ~FullSearchResultWidget();

private:
    Ui::FullSearchResultWidget *ui;

    QHBoxLayout* mainLayout=NULL;
    QTableWidget* searchResultTableWid=NULL;
    QStringList searchResultList;

protected:
    void init_widget();
    void init_search_result_table();//初始化搜索结果列表
    void fill_search_result_table();//填充程序搜索结果列表

private slots:
    void recv_search_keyword(QString arg);//接收MainViewWidget界面搜索框的搜索关键字


};

#endif // FULLSEARCHRESULTWIDGET_H
