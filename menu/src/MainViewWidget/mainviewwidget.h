#ifndef MAINVIEWWIDGET_H
#define MAINVIEWWIDGET_H

#include <QWidget>
#include <QSpacerItem>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QScrollBar>
#include <QFileSystemWatcher>
#include <QSettings>
#include "src/CommonUseWidget/commonusewidget.h"
#include "src/LetterWidget/letterwidget.h"
#include "src/FunctionWidget/functionwidget.h"
#include "src/SearchResultWidget/fullsearchresultwidget.h"
#include "src/SearchResultWidget/searchresultwidget.h"
#include "kylin-start-menu-interface.h"

namespace Ui {
class MainViewWidget;
}

class MainViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainViewWidget(QWidget *parent = nullptr);
    ~MainViewWidget();

    /**
     * @设置主界面从全屏回到默认状态时应加载的分类窗口
     */
    void load_classification_widget();

    /**
     * @主界面全屏时应加载的分类窗口
     */
    void load_full_classification_widget();

private:
    Ui::MainViewWidget *ui;

    QSettings* setting;

    QVBoxLayout* mainLayout=nullptr;
    QWidget* topWidget=nullptr;
    QVBoxLayout* topLayout=nullptr;

    QSpacerItem* verticalSpacer=nullptr;
    QWidget* minmaxWidget=nullptr;
    QHBoxLayout* minmaxLayout=nullptr;
    QToolButton* minmaxbtn=nullptr;
    QSpacerItem* horizontalSpacer=nullptr;

    QWidget* queryWidget=nullptr;
    QLineEdit* querylineEdit=nullptr;
    QToolButton* querybtn=nullptr;
    QHBoxLayout* queryLayout=nullptr;

    FullSearchResultWidget* fullsearchresultwid=nullptr;
    SearchResultWidget* searchresultwid=nullptr;
    QHBoxLayout* searchResultWidLayout=nullptr;
    QWidget* searchResultWid=nullptr;
    QTableWidget* searchResultTableWid=nullptr;

    CommonUseWidget* commonusewid=nullptr;
    LetterWidget* letterwid=nullptr;
    FunctionWidget* functionwid=nullptr;

    int widgetState=-1;//分类窗口编号

    bool is_fullWid=false;

    int letterwid_state=0;
    int functionwid_state=0;
    QString letterbtnname;
    QString functionbtnname;

    QFileSystemWatcher* fileWatcher;//监控/usr/share/applications文件夹状态

protected:
    void init_widget();//初始化界面
    void init_top_widget();
    void add_top_control();//添加顶部窗口控件
    void add_query_lineedit();//添加查询框
    void set_min_btn();//设置还原按钮
    void set_max_btn();//设置全屏按钮

    void add_search_result_widget();//添加搜索列表界面
    void fill_search_result_tablewid();//填充默认态程序搜索列表
    void fill_full_search_result_tablewid();//填充全屏态程序搜索列表

    void keyPressEvent(QKeyEvent* e);//查询框响应键盘事件

public:
    void load_min_mainview();//加载默认主视图
    void load_max_mainview();//加载全屏主视图

private slots:
    bool eventFilter(QObject *watched, QEvent *event);
    void fullscreen_btn_slot();//全屏按钮槽函数
    void load_commonuse_widget();//加载常用分类界面
    void load_letter_widget();//加载字母分类界面
    void load_function_widget();//加载功能分类界面
    void default_btn_slot();//默认态按钮槽函数

    void search_app_slot(QString arg);//搜索程序和文件槽函数
    void directoryChangedSlot();//desktop文件目录改变信号槽

signals:
    /**
     * @发送全屏按钮点击信号
     * @param arg当前所加载的分类窗口编号
     */
    void send_fullscreenbtn_signal();

    /**
     * @发送默认(还原)按钮点击信号
     * @param arg当前所加载的分类窗口编号
     */
    void send_defaultbtn_signal();
    void send_search_keyword(QString arg);//发送搜索框的搜索关键字
    void send_hide_mainwindow_signal();//向MainWindow发送隐藏主窗口信号
    void directoryChangedSignal(QString name,int arg);//desktop文件目录改变信号

};

#endif // MAINVIEWWIDGET_H
