#ifndef KLISTVIEW_H
#define KLISTVIEW_H
#include <QListView>
#include "src/ViewItem/kitemdelegate.h"
#include <QStandardItemModel>
#include <QEvent>
#include <QMouseEvent>

class KListView : public QListView
{
    Q_OBJECT
public:
    KListView(QWidget *parent);
    ~KListView();
    void initWidget();

    KItemDelegate *m_delegate = nullptr;
    QStandardItemModel *listmodel = nullptr;
    int module = 0;
    double transparency;
    QVariant pressApp;
    UkuiMenuInterface *pUkuiMenuInterface = nullptr;
protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *event);
public:
    void addData(QStringList data);
    void updateData(QStringList data);

Q_SIGNALS:
    void sendHideMainWindowSignal();
    void sendItemClickedSignal(QString arg);//发送item点击信号
    void sendUpdateAppListSignal();
    void sendCollectViewUpdate();

public Q_SLOTS:
    void rightClickedSlot(const QPoint &pos);
    void onClicked(QModelIndex index);
};

#endif // KLISTVIEW_H
