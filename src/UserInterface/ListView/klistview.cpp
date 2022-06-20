#include <QPainter>
#include "klistview.h"
#include "utility.h"
#include <QScrollBar>
#include "rightclickmenu.h"

KListView::KListView(QWidget *parent):
    QListView(parent)
{
}

KListView::~KListView()
{
    delete m_delegate;
    delete listmodel;
    delete pUkuiMenuInterface;
}

void KListView::addData(QStringList data)
{
    listmodel = new QStandardItemModel(this);
    this->setModel(listmodel);

    Q_FOREACH (QString desktopfp, data) {
        QStandardItem *item = new QStandardItem;
        item->setData(QVariant::fromValue<QString>(desktopfp), Qt::DisplayRole);
        listmodel->appendRow(item);
    }
}

void KListView::updateData(QStringList data)
{
    listmodel->clear();

    Q_FOREACH (QString desktopfp, data) {
        QStandardItem *item = new QStandardItem;
        item->setData(QVariant::fromValue<QString>(desktopfp), Qt::DisplayRole);
        listmodel->appendRow(item);
    }
}

void KListView::onClicked(QModelIndex index)
{
    QVariant var = listmodel->data(index, Qt::DisplayRole);

    if (var.isValid()) {
        Q_EMIT sendHideMainWindowSignal();
        QString desktopfp = var.value<QString>();
        execApp(desktopfp);
    }
}

void KListView::paintEvent(QPaintEvent *e)
{
    double transparency = getTransparency();
    QPainter painter(this->viewport());
    painter.setBrush(this->palette().base());
    painter.setPen(Qt::transparent);
    painter.setOpacity(transparency);
    painter.drawRect(this->rect());
    QListView::paintEvent(e);
}

void KListView::mousePressEvent(QMouseEvent *event)
{
    if (!(this->indexAt(event->pos()).isValid()) && event->button() == Qt::LeftButton) {
        Q_EMIT sendHideMainWindowSignal();
    } else {
        pressApp = listmodel->data(this->indexAt(event->pos()), Qt::DisplayRole);
        return QListView::mousePressEvent(event);
    }
}

void KListView::rightClickedSlot(const QPoint &pos)
{
    Q_UNUSED(pos)

    if (!(this->selectionModel()->selectedIndexes().isEmpty())) {
        QModelIndex index = this->currentIndex();
        QVariant var = listmodel->data(index, Qt::DisplayRole);
        QStringList strlist = var.value<QStringList>();

        if (strlist.count() == 1 || ((strlist.count() == 2) && (strlist.at(1).toInt() == 1))) {
            RightClickMenu menu;
            int ret = menu.showAppBtnMenu(this->mapToGlobal(pos), strlist.at(0));

            switch (ret) {
                case 1:
                case 2:
                    Q_EMIT sendUpdateAppListSignal();
                    break;

                case 6:
                case 7:
                    Q_EMIT sendHideMainWindowSignal();
                    break;

                case 18:
                    Q_EMIT sendCollectViewUpdate();
                    break;

                default:
                    break;
            }
        }
    }
}
