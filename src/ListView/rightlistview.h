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
