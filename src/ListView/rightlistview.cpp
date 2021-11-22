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
    KListView(parent)
{
    initWidget();
    m_delegate = new RightItemDelegate(this);
    this->setItemDelegate(m_delegate);
    pUkuiMenuInterface = new UkuiMenuInterface;
}

RightListView::~RightListView()
{
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
    this->setGridSize(QSize(107, 107));
    this->verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
    this->setFrameShape(QFrame::NoFrame);//移除边框
    this->setStyleSheet(QString::fromUtf8("QStandardItemModel#listmodel{border:3px solid #FFFFFF; }QWidget#widget:focus{ border:5px solid #A5A6A1;  } "));
    connect(this, &RightListView::customContextMenuRequested, this, &RightListView::rightClickedSlot);
    connect(this, &RightListView::clicked, this, &RightListView::onClicked);
}

void RightListView::dropEvent(QDropEvent *event)
{
    RightListView *source = qobject_cast<RightListView *>(event->source());

    if (source && source == this) {
        dropPos = event->pos();

        if (this->indexAt(dropPos).isValid()) {
            QVariant var = listmodel->data(indexAt(dropPos), Qt::DisplayRole);
            QString desktopfp = var.value<QString>();
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

void RightListView::selectFirstItem()
{
    myDebug() << "选择首个元素";

    if (this->currentIndex().row() == -1) {
        this->setCurrentIndex(this->model()->index(0, 0));
    }
}

void RightListView::keyPressEvent(QKeyEvent *e)
{
    if (e->type() == QEvent::KeyPress) {
        switch (e->key()) {
            case Qt::Key_Enter:
            case Qt::Key_Return: {
                QModelIndex index = this->currentIndex();
                Q_EMIT clicked(index);
                break;
            }

            default:
                return QListView::keyPressEvent(e);
                break;
        }
    }
}

void RightListView::enterEvent(QEvent *e)
{
    Q_UNUSED(e);
    this->selectionModel()->clear();
}
