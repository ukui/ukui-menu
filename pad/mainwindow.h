#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>
#include <QVBoxLayout>
#include <QListView>
#include <QDateTime>
#include <QTimer>
#include <QStandardItemModel>
#include <QFrame>
#include "src/Interface/ukuimenuinterface.h"
#include "src/UtilityFunction/fulllistview.h"
#include "src/UtilityFunction/fullcommonusewidget.h"
#include "src/MainViewWidget/directorychangedthread.h"
#include <QSettings>
#include <QAction>
#include <qgsettings.h>
#include <QLocale>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void widgetMakeZero();
    void leftWidgetDisplay();
//    void searchAppSlot(QString arg);


private:
    QDateTime* dateTime;
    QTimer* timer;
    QLocale* locale;


    QFrame *m_frame=nullptr;

    QWidget * rightWidget;
    QHBoxLayout* mainLayout;

    //左侧
    QWidget* leftWidget=nullptr;
    QWidget* leftUpWidget=nullptr;
    QWidget* upLeftWidget=nullptr;
    QWidget* downWidget=nullptr;


    QVBoxLayout* leftLayout;
    QHBoxLayout* leftUpLayout;
    QVBoxLayout* downLayout;

    QHBoxLayout* upLeftLayout;
    QVBoxLayout* upRightLayout;


    QLabel* timeLabel;
    QLabel* weekLabel;
    QLabel* dateLabel;
    QLineEdit* searchEdit;
    QAction* searchAction;

    //右侧
    QWidget* upRightWidget=nullptr;
    QHBoxLayout* rightLayout;
    FullCommonUseWidget* m_CommonUseWidget=nullptr;
    FullListView* m_listView=nullptr;
    QStandardItemModel* listmodel=nullptr;

    //监控
    QFileSystemWatcher* m_fileWatcher=nullptr;
    DirectoryChangedThread* m_directoryChangedThread=nullptr;
    UkuiMenuInterface* m_ukuiMenuInterface=nullptr;

    //记录应用显示位置
    QSettings* m_setting=nullptr;
    QGSettings* m_gsetting=nullptr;

    //
    QGSettings* gsetting;
    QPropertyAnimation *animation;

    QGSettings* tabletMode=nullptr;
    QString searchContent;

public Q_SLOTS:

    void directoryChangedSlot();
    //void iconThemeChangeSlot(QString key);
    void requestUpdateSlot(QString desktopfp);
    void requestDeleteAppSlot();
    void displayMainViewSlot();
    void obtainSearchResult();

Q_SIGNALS:

//    void directoryChangedSignal();
    void sendDirectoryPath(QString arg);

};


#endif // MAINWINDOW_H
