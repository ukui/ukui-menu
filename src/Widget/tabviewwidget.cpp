#include "tabviewwidget.h"
#include <QTabBar>

TabViewWidget::TabViewWidget()
{
    setStyleSheet("background:transparent");
    collectionTab = new QWidget();
    recentTab = new QWidget();
    collectionTab->setObjectName(QString::fromUtf8("collectionTab"));
    recentTab->setObjectName(QString::fromUtf8("recentTab"));
    this->addTab(collectionTab, QString());
    this->addTab(recentTab, QString());
    this->tabBar()->hide();
    this->setCurrentIndex(0);
    initAppListWidget();
    updateListView();
}

void TabViewWidget::initAppListWidget()
{
    m_collectListView = new RightListView(collectionTab);
    m_collectListView->setFixedSize(352, 428);
    m_collectListView->show();
}

void TabViewWidget::updateListView()
{
    m_data.clear();

    Q_FOREACH (QString desktopfp, m_ukuiMenuInterface->getAllClassification()) {
        m_data.append(desktopfp);
    }

    m_collectListView->addData(m_data);
}

