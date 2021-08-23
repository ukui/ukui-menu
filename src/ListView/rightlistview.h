#ifndef RIGHTLISTVIEW_H
#define RIGHTLISTVIEW_H
#include <QListView>
#include <QSettings>
#include "src/RightClickMenu/rightclickmenu.h"
#include "src/Interface/ukuimenuinterface.h"
#include <QEvent>
#include <QScrollBar>
#include <QToolTip>
#include <QStandardItemModel>
#include "src/ViewItem/rightitemdelegate.h"
#include "src/Style/style.h"
#include "src/UtilityFunction/utility.h"

class RightListView : public QListView
{
    Q_OBJECT
public:
    RightListView(QWidget *parent);
    ~RightListView();

    void addData(QVector<QString> data);//字母排序模块添加数据
    void updateData(QVector<QString> data);

protected:
    void initWidget();
    void keyPressEvent(QKeyEvent* e);
    void enterEvent(QEvent* e) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent* e) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *e) override;
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void dropEvent(QDropEvent *event);

private:
    RightItemDelegate* m_delegate=nullptr;
    QStandardItemModel* listmodel=nullptr;
    int module=0;
    double transparency;
    RightClickMenu* menu=nullptr;//右键菜单
    QStringList data;
    QGSettings* gsetting;
    UkuiMenuInterface* pUkuiMenuInterface=nullptr;
    QPoint dropPos;
    QVariant pressApp;

//    bool inCurView;

public Q_SLOTS:
    void onClicked(QModelIndex index);//点击item
    void rightClickedSlot(const QPoint &pos);//右键菜单
    void selectFirstItem();

Q_SIGNALS:
    void sendItemClickedSignal(QString arg);//发送item点击信号
    void sendHideMainWindowSignal();
    void sendUpdateAppListSignal();
    void sendSetslidebar(int value);
    void sendCollectViewUpdate();
};

#endif // FULLLISTVIEW_H
