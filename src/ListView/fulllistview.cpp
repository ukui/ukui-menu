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

#include "fulllistview.h"
#include "src/UtilityFunction/utility.h"
#include <QDebug>
#include <syslog.h>
#include <QPushButton>
#include <QGSettings>

FullListView::FullListView(QWidget *parent, int module):
    KListView(parent)
{
    this->module = module;
    initWidget();
    m_delegate = new FullItemDelegate(this, module);
    this->setItemDelegate(m_delegate);
    pUkuiMenuInterface = new UkuiMenuInterface;
}

FullListView::~FullListView()
{
    delete menu;
    delete gsetting;
}

void FullListView::initWidget()
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
    this->setGridSize(QSize(Style::AppListGridSizeWidth, Style::AppListGridSizeWidth));
    this->verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
    this->setFrameShape(QFrame::NoFrame);//移除边框
    this->setStyleSheet(QString::fromUtf8("QStandardItemModel#listmodel{border:3px solid #FFFFFF; }QWidget#widget:focus{ border:5px solid #A5A6A1;  } "));
    connect(this, &FullListView::customContextMenuRequested, this, &FullListView::rightClickedSlot);
    connect(this, &FullListView::clicked, this, &FullListView::onClicked);
}

void FullListView::selectFirstItem()
{
    if (this->currentIndex().row() == -1) {
        this->setCurrentIndex(this->model()->index(0, 0));
    }
}

void FullListView::keyPressEvent(QKeyEvent *e)
{
    QRect center = visualRect(currentIndex());

    if (e->type() == QEvent::KeyPress) {
        switch (e->key()) {
            case Qt::Key_Enter:
            case Qt::Key_Return: {
                QModelIndex index = this->currentIndex();
                Q_EMIT clicked(index);
                break;
            }

            case Qt::Key_Left: {
                this->clearFocus();

                if (mapToGlobal(center.topRight()).y() < Style::QueryLineEditHeight + Style::AppListGridSizeWidth) {
                    Q_EMIT sendSetslidebar(-Style::AppListGridSizeWidth);
                }

                this->setFocus();
                return QListView::keyPressEvent(e);
                break;
            }

            case Qt::Key_Right: {
                this->clearFocus();

                if (mapToGlobal(center.bottomRight()).y() > (1080 - Style::AppListGridSizeWidth)) {
                    Q_EMIT sendSetslidebar(Style::AppListGridSizeWidth);
                }

                this->setFocus();
                return QListView::keyPressEvent(e);
                break;
            }

            case Qt::Key_Up: {
                if (module == 0) {
                    if (mapToGlobal(center.topRight()).y() < (Style::QueryLineEditHeight  + Style::AppListGridSizeWidth)) {
                        Q_EMIT sendSetslidebar(-Style::AppListGridSizeWidth);
                    }
                }

                return QListView::keyPressEvent(e);
                break;
            }

            case Qt::Key_Down: {
                if (module == 0) {
                    if (mapToGlobal(center.bottomRight()).y() > (1080 - Style::AppListGridSizeWidth)) {
                        Q_EMIT sendSetslidebar(Style::AppListGridSizeWidth);
                    }
                }

                return QListView::keyPressEvent(e);
                break;
            }

            default:
                return QListView::keyPressEvent(e);
        }
    }
}
