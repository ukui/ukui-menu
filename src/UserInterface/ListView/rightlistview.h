#ifndef RIGHTLISTVIEW_H
#define RIGHTLISTVIEW_H
#include <QListView>
#include <QSettings>
#include "rightclickmenu.h"
#include "ukuimenuinterface.h"
#include <QEvent>
#include <QScrollBar>
#include <QToolTip>
#include <QStandardItemModel>
#include "right_item_delegate.h"
#include "style.h"
#include "utility.h"
#include "klistview.h"

class RightListView : public KListView
{
    Q_OBJECT
public:
    RightListView(QWidget *parent);
    ~RightListView();

    void initWidget();
protected:

    void keyPressEvent(QKeyEvent *e);
    void dropEvent(QDropEvent *event);
    void enterEvent(QEvent *e);

private:
    RightClickMenu *menu = nullptr; //右键菜单
    QStringList data;
    QGSettings *gsetting;
    QPoint dropPos;

//    bool inCurView;

public Q_SLOTS:

//    void rightClickedSlot(const QPoint &pos);//右键菜单
    void selectFirstItem();

Q_SIGNALS:

    void sendSetslidebar(int value);
};

#endif // FULLLISTVIEW_H
