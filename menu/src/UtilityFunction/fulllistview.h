#ifndef FULLLISTVIEW_H
#define FULLLISTVIEW_H
#include <QListView>
#include <QSettings>
#include "src/RightClickMenu/rightclickmenu.h"
#include <ukuimenuinterface.h>
#include "fulllistmodel.h"
#include "src/color.h"

class FullListView : public QListView
{
    Q_OBJECT
public:
    FullListView(QWidget *parent,int width,int height,int module);
    ~FullListView();

    void addData(QStringList data);//字母排序模块添加数据
    void updateData(QStringList data);
    void removeRow(QString desktopfp);
    void insertRow(QString desktopfp);

protected:
    void init_widget();

private:
    FullListModel* listmodel=nullptr;
    int w=0;
    int h=0;
    int module=0;

    RightClickMenu* menu;//右键菜单
    QSettings *setting=nullptr;
    QStringList data;

    UkuiMenuInterface* pUkuiMenuInterface=nullptr;

private slots:
    void onClicked(QModelIndex index);//点击item
    void rightClickedSlot();//右键菜单

signals:
    void sendItemClickedSignal(QString arg);//向字母排序或功能分类模块发送item点击信号
    void sendFixedOrUnfixedSignal();//向字母排序、功能分类发送固定或者取消固定信号
    void send_update_applist_signal();//向FullCommonUseWidget发送更新应用列表信号
};

#endif // FULLLISTVIEW_H
