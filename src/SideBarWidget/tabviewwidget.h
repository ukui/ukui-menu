#ifndef TABVIEWWIDGET_H
#define TABVIEWWIDGET_H
#include <QTabWidget>
#include "src/SideBarWidget/rightlistview.h"

class TabViewWidget: public QTabWidget
{
public:
    TabViewWidget();

protected:

    void initAppListWidget();
    void updateListView();

private:

    UkuiMenuInterface* m_ukuiMenuInterface=nullptr;
    QWidget *collectionTab = nullptr;
    QWidget *recentTab = nullptr;
    RightListView * m_collectListView = nullptr;
    QVector<QString> m_data;
};

#endif // TABVIEWWIDGET_H