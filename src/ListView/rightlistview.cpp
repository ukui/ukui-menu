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

#include "rightlistview.h"
#include "src/UtilityFunction/utility.h"
#include <QDebug>
#include <syslog.h>
#include <QPushButton>
#include <QGSettings>

RightListView::RightListView(QWidget *parent):
    QListView(parent)
{
    initWidget();
    pUkuiMenuInterface=new UkuiMenuInterface;
}

RightListView::~RightListView()
{
//    delete menu;
    delete pUkuiMenuInterface;
}

void RightListView::initWidget()
{
    viewport()->setAttribute(Qt::WA_TranslucentBackground);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setViewMode(QListView::IconMode);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setResizeMode(QListView::Adjust);
    this->setTextElideMode(Qt::ElideRight);
    this->setMouseTracking(true);
    this->setMovement(QListView::Static);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setGridSize(QSize(107,107));
    this->verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
    this->setFrameShape(QFrame::NoFrame);//移除边框
    this->setStyleSheet(QString::fromUtf8("QStandardItemModel#listmodel{border:3px solid #FFFFFF; }QWidget#widget:focus{ border:5px solid #A5A6A1;  } "));
    connect(this,&RightListView::customContextMenuRequested,this,&RightListView::rightClickedSlot);
    connect(this,&RightListView::clicked,this,&RightListView::onClicked);
}

void RightListView::addData(QVector<QString> data)
{
    listmodel=new QStandardItemModel(this);
    this->setModel(listmodel);

    Q_FOREACH(QString desktopfp,data)
    {
        QStandardItem* item=new QStandardItem;
        item->setData(QVariant::fromValue<QString>(desktopfp),Qt::DisplayRole);
        listmodel->appendRow(item);
    }
    m_delegate= new RightItemDelegate(this);
    this->setItemDelegate(m_delegate);
}

void RightListView::focusInEvent(QFocusEvent *event)
{
    return QListView::focusInEvent(event);
}

void RightListView::focusOutEvent(QFocusEvent *event)
{
    return QListView::focusOutEvent(event);
}

void RightListView::updateData(QVector<QString> data)
{
    listmodel->clear();
    Q_FOREACH(QString desktopfp,data)
    {
        QStandardItem* item=new QStandardItem;
        item->setData(QVariant::fromValue<QString>(desktopfp),Qt::DisplayRole);
        listmodel->appendRow(item);
    }
}

void RightListView::onClicked(QModelIndex index)
{
     QVariant var = listmodel->data(index, Qt::DisplayRole);
     if(var.isValid())
     {
         QString desktopfp=var.value<QString>();
         execApp(desktopfp);
     }
}

void RightListView::dropEvent(QDropEvent *event)
{
    RightListView *source = qobject_cast<RightListView *>(event->source());
    if (source && source == this)
    {
        dropPos=event->pos();

        if(this->indexAt(dropPos).isValid())
        {
            QVariant var=listmodel->data(indexAt(dropPos), Qt::DisplayRole);
            QString desktopfp=var.value<QString>();
            QFileInfo fileInfo(desktopfp);
            QString desktopfn = fileInfo.fileName();
            QString dragDesktopfp = pressApp.value<QString>();
            QFileInfo dragFileInfo(dragDesktopfp);
            QString dragDesktopfn = dragFileInfo.fileName();
            changeCollectSort(dragDesktopfn, desktopfn);
        }
    }
    Q_EMIT sendCollectViewUpdate();
}

void RightListView::rightClickedSlot(const QPoint &pos)
{
    Q_UNUSED(pos)
    if(!(this->selectionModel()->selectedIndexes().isEmpty()))
    {
        QModelIndex index=this->currentIndex();
        QVariant var=listmodel->data(index, Qt::DisplayRole);
        QString desktopfp=var.value<QString>();
        RightClickMenu menu;
        int ret=menu.showAppBtnMenu(this->mapToGlobal(pos), desktopfp);
        if(module>0)
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
            case 18:
                Q_EMIT sendCollectViewUpdate();
            default:
                break;
            }
        }
        //this->selectionModel()->clear();
    }
}

void RightListView::enterEvent(QEvent *e)
{
    Q_UNUSED(e);
    this->selectionModel()->clear();
    this->verticalScrollBar()->setVisible(true);
}

void RightListView::leaveEvent(QEvent *e)
{
    Q_UNUSED(e);
    this->verticalScrollBar()->setVisible(false);
}

void RightListView::selectFirstItem()
{
    qDebug() << "void FullListView::selectFirstItem()";
    if(this->currentIndex().row() == -1)
    {
        this->setCurrentIndex(this->model()->index(0,0));
    }
}

void RightListView::paintEvent(QPaintEvent *e)
{
    double transparency=getTransparency();
    QPainter painter(this->viewport());

    painter.setBrush(this->palette().base());
    painter.setPen(Qt::transparent);
    painter.setOpacity(transparency);
    painter.drawRect(this->rect());

    QListView::paintEvent(e);
}

void RightListView::keyPressEvent(QKeyEvent* e)
{
     QRect center = visualRect(currentIndex());
    if(e->type() == QEvent::KeyPress)
    {
        switch(e->key())
        {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        {
            QModelIndex index = this->currentIndex();
            Q_EMIT clicked(index);
            break;
        }
        case Qt::Key_Left:
        {
            this->clearFocus();
            if(mapToGlobal(center.topRight()).y() < Style::QueryLineEditHeight + Style::AppListGridSizeWidth)
            {
               Q_EMIT sendSetslidebar(-Style::AppListGridSizeWidth);
            }
            this->setFocus();
            return QListView::keyPressEvent(e);
            break;
        }
        case Qt::Key_Right:
        {

            this->clearFocus();
            if(mapToGlobal(center.bottomRight()).y() > (1080 - Style::AppListGridSizeWidth))
            {
                Q_EMIT sendSetslidebar(Style::AppListGridSizeWidth);
            }
            this->setFocus();
            return QListView::keyPressEvent(e);
            break;
        }
        case Qt::Key_Up:
        {
            if(module == 0)
            {
                if(mapToGlobal(center.topRight()).y() < (Style::QueryLineEditHeight  + Style::AppListGridSizeWidth))
                {
                   Q_EMIT sendSetslidebar(-Style::AppListGridSizeWidth);
                }
            }
            return QListView::keyPressEvent(e);
            break;
        }
        case Qt::Key_Down:
        {
            if(module == 0)
            {
                if(mapToGlobal(center.bottomRight()).y() > (1080 - Style::AppListGridSizeWidth))
                {
                    Q_EMIT sendSetslidebar(Style::AppListGridSizeWidth);
                }
            }
            return QListView::keyPressEvent(e);
            break;
        }

        default:
            return QListView::keyPressEvent(e);
            break;
        }
    }
}

void RightListView::mousePressEvent(QMouseEvent *event)
{
    if(!(this->indexAt(event->pos()).isValid()) && event->button()==Qt::LeftButton)
    {
        Q_EMIT sendHideMainWindowSignal();
    }
    else
    {
        pressApp = listmodel->data(this->indexAt(event->pos()), Qt::DisplayRole);
        return QListView::mousePressEvent(event);
    }
}
