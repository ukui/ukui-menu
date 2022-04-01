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
#include "function_button_widget.h"
#include "utility.h"
#include <QDebug>
#include <QDesktopServices>

ListView::ListView(QWidget *parent/*, int width, int height, int module*/):
    KListView(parent)
{
    this->w = 300;
    this->h = 540;
    this->module = 1;
    initWidget();
    listmodel = new QStandardItemModel(this);
    this->setModel(listmodel);
    pUkuiMenuInterface = new UkuiMenuInterface;
    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
}

ListView::~ListView()
{
    delete pUkuiMenuInterface;
}

void ListView::initWidget()
{
    setAttribute(Qt::WA_TranslucentBackground);
    viewport()->setAttribute(Qt::WA_TranslucentBackground);
    viewport()->setAutoFillBackground(false);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setResizeMode(QListView::Adjust);
    this->setTextElideMode(Qt::ElideRight);
    this->setViewMode(QListView::ListMode);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setMovement(QListView::Static);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setUpdatesEnabled(true);
    this->setSpacing(0);
    this->setContentsMargins(0, 0, 0, 0);
    this->setMouseTracking(true);
    this->verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
    this->setFrameShape(QFrame::NoFrame);
    this->verticalScrollBar()->setProperty("drawScrollBarGroove", false);
    connect(this, &ListView::customContextMenuRequested, this, &ListView::rightClickedSlot);
    connect(this, &ListView::clicked, this, &ListView::onClicked);
}

void ListView::addData(QVector<QStringList> data, int module)
{
    this->module = module;
    listmodel->clear();

    Q_FOREACH (QStringList desktopfp, data) {
        QStandardItem *item = new QStandardItem;
        item->setData(QVariant::fromValue<QStringList>(desktopfp), Qt::DisplayRole);
        listmodel->appendRow(item);
    }

    m_delegate = new ItemDelegate(this, module);
    this->setItemDelegate(m_delegate);
}

void ListView::updateData(QVector<QStringList> data)
{
    listmodel->clear();

    Q_FOREACH (QStringList desktopfp, data) {
        QStandardItem *item = new QStandardItem;
        item->setData(QVariant::fromValue<QStringList>(desktopfp), Qt::DisplayRole);
        listmodel->appendRow(item);
    }
}

void ListView::onClicked(QModelIndex index)
{
    QVariant var = listmodel->data(index, Qt::DisplayRole);
    QString desktopfp = var.value<QStringList>().at(0);

    if (var.isValid()) {
        if ((var.value<QStringList>().size() == 5) && (!desktopfp.endsWith(".desktop"))) { //专用于处理最近页的uri
            QUrl url(desktopfp);
            QDesktopServices::openUrl(url);
            return;
        }

        if (var.value<QStringList>().at(1).toInt() == 0) {
            Q_EMIT sendAppClassificationBtnClicked();
        } else {
            execApp(desktopfp);
        }
    }
}

void ListView::enterEvent(QEvent *e)
{
    Q_UNUSED(e);
    this->selectionModel()->clear();
    this->verticalScrollBar()->setVisible(true);
}

void ListView::leaveEvent(QEvent *e)
{
    Q_UNUSED(e);
    this->verticalScrollBar()->setVisible(false);
}

void ListView::paintEvent(QPaintEvent *e)
{
    //滚动条
    QPalette p = this->verticalScrollBar()->palette();
    QColor color(255, 255, 255);
    color.setAlphaF(0.25);
    p.setColor(QPalette::Active, QPalette::Button, color);
    this->verticalScrollBar()->setPalette(p);
    QListView::paintEvent(e);
}

void ListView::keyPressEvent(QKeyEvent *e)
{
    if (e->type() == QEvent::KeyPress) {
        switch (e->key()) {
            case Qt::Key_Enter:
            case Qt::Key_Return: {
                QModelIndex index = this->currentIndex();
                Q_EMIT clicked(index);
            }
            break;

            case Qt::Key_Down: {
                if (currentIndex().row() == this->model()->rowCount() - 1) {
                    setCurrentIndex(this->model()->index(0, 0));
                    break;
                }

                return QListView::keyPressEvent(e);
                break;
            }

            case Qt::Key_Up: {
                if (currentIndex().row() == 0) {
                    setCurrentIndex(this->model()->index(this->model()->rowCount() - 1, 0));
                    break;
                }

                return QListView::keyPressEvent(e);
                break;
            }

           case Qt::Key_Right: {
           break;
            }

           case Qt::Key_Left: {
           break;
            }

           default:
                return QListView::keyPressEvent(e);
                break;
        }
    }
}
