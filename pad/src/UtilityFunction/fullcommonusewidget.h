#ifndef FullCommonUseWidget_H
#define FullCommonUseWidget_H

#include <QWidget>
#include <QSettings>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QDir>
#include <QProcess>
#include "src/Interface/ukuimenuinterface.h"
#include "src/Style/style.h"
#include "src/UtilityFunction/fulllistview.h"
#include <gio/gdesktopappinfo.h>
#include <QSettings>
#include <QVector>


class FullCommonUseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FullCommonUseWidget(QWidget *parent = nullptr);
    ~FullCommonUseWidget();

    /**
     * @brief Update application list
     */
    void updateListView(QString desktopfp);

    void deleteAppListView();

    /**
     * @brief fill application list
     */
    void fillAppList();


private:
    UkuiMenuInterface* m_ukuiMenuInterface=nullptr;
    FullListView* m_listView=nullptr;
    QStringList m_data;
    QSpacerItem *m_spaceItem=nullptr;
    QSettings* setting;
    static QVector<QString> keyVector;
    static QVector<int> keyValueVector;

protected:
    /**
     * @brief Initializes UI
     */
    void initUi();
    /**
     * @brief Initialize the application list interface
     */
    void initAppListWidget();

    static bool cmpApp(QString &arg_1,QString &arg_2);


public Q_SLOTS:
    /**
     * @brief Open the application
     * @param arg: Desktop file path
     */
    void execApplication(QString desktopfp);
    /**
     * @brief Update the application list slot function
     */
    void updateListViewSlot();

Q_SIGNALS:
    /**
     * @brief Send a hidden main window signal to the MainViewWidget
     */
    void sendHideMainWindowSignal();
    void sendSortApplistSignal();
};

#endif // FullCommonUseWidget_H
