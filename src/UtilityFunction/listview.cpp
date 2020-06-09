/*
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/&gt;.
 *
 */

#include "listview.h"
#include <QDebug>

ListView::ListView(QWidget *parent, int width, int height, int module):
    QListView(parent)
{
    this->w=width;
    this->h=height;
    this->module=module;
    initWidget();

    pUkuiMenuInterface=new UkuiMenuInterface;
    menu=new RightClickMenu(nullptr,0);

//    m_scrollAnimation=new QPropertyAnimation(this->verticalScrollBar(), "value");
//    m_scrollAnimation->setEasingCurve(QEasingCurve::OutQuint);
//    m_scrollAnimation->setDuration(800);
//    connect(m_scrollAnimation, &QPropertyAnimation::valueChanged, this, &ListView::animationValueChangedSlot);
//    connect(m_scrollAnimation, &QPropertyAnimation::finished, this, &ListView::animationFinishSlot);

    QString path=QDir::homePath()+"/.config/ukui/ukui-menu.ini";
    setting=new QSettings(path,QSettings::IniFormat);

}
ListView::~ListView()
{
    delete menu;
    delete pUkuiMenuInterface;
}

void ListView::initWidget()
{
//    this->setFixedSize(w,h);

    this->verticalScrollBar()->setStyleSheet("QScrollBar{padding-top:0px;padding-bottom:0px;background:transparent;width:3px;border-radius:1.5px;}"
                                             "QScrollBar::handle{background-color:rgba(255,255,255,0.25); width:3px;border-radius:1.5px;}"
                                             "QScrollBar::handle:hover{background-color:#697883;width:3px;border-radius:1.5px;}"
                                             "QScrollBar::handle:pressed{background-color:#8897a3;width:3px;border-radius:1.5px;}"
                                             "QScrollBar::sub-line{background-color:transparent;height:0px;width:0px;}"
                                             "QScrollBar::add-line{background-color:transparent;height:0px;width:0px;}"
                                             );
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    this->setGridSize(QSize(310,48));
    this->setResizeMode(QListView::Adjust);
    this->setTextElideMode(Qt::ElideRight);
    this->setViewMode(QListView::ListMode);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setFocusPolicy(Qt::NoFocus);
    this->setMovement(QListView::Static);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setUpdatesEnabled(true);
//    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setSpacing(0);
    this->setContentsMargins(0, 0, 0, 0);
    this->setMouseTracking(true);
    connect(this,&ListView::customContextMenuRequested,this,&ListView::rightClickedSlot);
    connect(this,&ListView::clicked,this,&ListView::onClicked);
//    connect(this,&ListView::entered, this, &ListView::setCurrentIndex, Qt::QueuedConnection);


}

void ListView::addData(QVector<QStringList> data)
{
    listmodel=new QStandardItemModel(this);
    this->setModel(listmodel);
    Q_FOREACH(QStringList desktopfp,data)
    {
        QStandardItem* item=new QStandardItem;
        item->setData(QVariant::fromValue<QStringList>(desktopfp),Qt::DisplayRole);
        listmodel->appendRow(item);
    }
    m_delegate= new ItemDelegate(this,module);
    this->setItemDelegate(m_delegate);
}

void ListView::updateData(QVector<QStringList> data)
{
    listmodel->clear();
    Q_FOREACH(QStringList desktopfp,data)
    {
        QStandardItem* item=new QStandardItem;
        item->setData(QVariant::fromValue<QStringList>(desktopfp),Qt::DisplayRole);
        listmodel->appendRow(item);
    }
//    Q_EMIT dataChanged(createIndex(0,0), createIndex(listmodel->rowCount()-1,0));
}

void ListView::onClicked(QModelIndex index)
{
     QVariant var = listmodel->data(index, Qt::DisplayRole);
     if(var.isValid())
     {
         QString valstr=var.value<QStringList>().at(1);
         if(valstr.toInt()==1)
         {
             QString desktopfp=var.value<QStringList>().at(0);
             QFileInfo fileInfo(desktopfp);
             QString desktopfn=fileInfo.fileName();
             setting->beginGroup("recentapp");
             setting->remove(desktopfn);
             setting->sync();
             setting->endGroup();
         }
         Q_EMIT sendItemClickedSignal(var.value<QStringList>());
     }
}

void ListView::rightClickedSlot()
{
    if(!this->selectionModel()->selectedIndexes().isEmpty())
    {
        QModelIndex index=this->currentIndex();
        QVariant var=listmodel->data(index, Qt::DisplayRole);
        QStringList strlist=var.value<QStringList>();
        int ret=menu->showAppBtnMenu(strlist.at(0));
        if(module>0)
        {
            if(strlist.at(1).toInt()==1)
            {
                switch (ret) {
                case 6:
                    Q_EMIT sendHideMainWindowSignal();
                    break;
                case 7:
                    Q_EMIT sendHideMainWindowSignal();
                    break;
                default:
                    break;
                }
            }
        }
        else{
            switch (ret) {
            case 1:
                Q_EMIT sendUpdateAppListSignal();
                break;
            case 2:
                Q_EMIT sendUpdateAppListSignal();
                break;
            case 6:
                Q_EMIT sendHideMainWindowSignal();
                break;
            case 7:
                Q_EMIT sendHideMainWindowSignal();
                break;
            default:
                break;
            }
        }

        this->selectionModel()->clear();
    }
}

void ListView::enterEvent(QEvent *e)
{
    Q_UNUSED(e);
    this->verticalScrollBar()->setVisible(true);
}

void ListView::leaveEvent(QEvent *e)
{
    Q_UNUSED(e);
    this->verticalScrollBar()->setVisible(false);
}

//void ListView::wheelEvent(QWheelEvent *e)
//{
//    if (this->verticalScrollBar()->value() < this->verticalScrollBar()->maximum() &&
//        this->verticalScrollBar()->value() > this->verticalScrollBar()->minimum())
//        this->selectionModel()->clear();
//    int offset = -e->angleDelta().y();
//    m_scrollAnimation->stop();
//    m_scrollAnimation->setStartValue(verticalScrollBar()->value());
//    m_scrollAnimation->setEndValue(verticalScrollBar()->value() + offset * m_speedTime);
//    m_scrollAnimation->start();
//}

//void ListView::animationValueChangedSlot(const QVariant &value)
//{
//    Q_UNUSED(value);
//    if (this->verticalScrollBar()->value() == this->verticalScrollBar()->maximum() ||
//        this->verticalScrollBar()->value() == this->verticalScrollBar()->minimum()) {
//        blockSignals(false);
//    } else {
//        blockSignals(true);
//    }
//}

//void ListView::animationFinishSlot()
//{
//    blockSignals(false);
////    QPoint pos = mapFromGlobal(QCursor::pos());
////    Q_EMIT entered(indexAt(pos));
//}
