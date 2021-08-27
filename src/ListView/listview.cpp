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
#include "src/Widget/functionbuttonwidget.h"
#include "src/UtilityFunction/utility.h"
#include <QDebug>

ListView::ListView(QWidget *parent/*, int width, int height, int module*/):
    QListView(parent)
{
    this->w=300;
    this->h=540;
    this->module=1;
    initWidget();
    listmodel=new QStandardItemModel(this);
    this->setModel(listmodel);
    pUkuiMenuInterface=new UkuiMenuInterface;
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
    this->verticalScrollBar()->setProperty("drawScrollBarGroove",false);
   // this->setFocus();
    connect(this,&ListView::customContextMenuRequested,this,&ListView::rightClickedSlot);
    connect(this,&ListView::clicked,this,&ListView::onClicked);
}

void ListView::addData(QVector<QStringList> data, int module)
{
    this->module = module;
    listmodel->clear();
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
     QString desktopfp = var.value<QStringList>().at(0);
     if(var.isValid())
     {
         if(var.value<QStringList>().at(1).toInt() == 0)
             Q_EMIT sendAppClassificationBtnClicked();
         else
         {
             execApp(desktopfp);
         }
     }
}

void ListView::rightClickedSlot(const QPoint &pos)
{
    if(!this->selectionModel()->selectedIndexes().isEmpty())
    {
        QModelIndex index=this->currentIndex();
        QVariant var=listmodel->data(index, Qt::DisplayRole);
        QStringList strlist=var.value<QStringList>();
        if(strlist.at(1).toInt()==1)
        {
            RightClickMenu menu(this);
            connect(&menu, &RightClickMenu::sendMainWinActiveSignal, this, &ListView::sendMainWinActiveSignal);
            int ret = menu.showAppBtnMenu(this->mapToGlobal(pos), strlist.at(0));
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
                    case 18:
                        Q_EMIT sendUpdateCollectSignal();
                        break;
                    case 1:
                        Q_EMIT sendUpdateAppListSignal();
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
                case 18:
                    Q_EMIT sendUpdateCollectSignal();
                    break;
                default:
                    break;
                }
            }

          //  this->selectionModel()->clear();
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
    QPalette p=this->verticalScrollBar()->palette();
    QColor color(255, 255, 255);
//    QColor color=p.color(QPalette::Active,QPalette::Button);
    color.setAlphaF(0.25);
    p.setColor(QPalette::Active,QPalette::Button,color);
    this->verticalScrollBar()->setPalette(p);
    QListView::paintEvent(e);
}

void ListView::keyPressEvent(QKeyEvent* e)
{
    if(e->type()==QEvent::KeyPress)
    {
        switch(e->key())
        {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        {
            QModelIndex index=this->currentIndex();
            Q_EMIT clicked(index);
        }
            break;
        case Qt::Key_Down:
        {
            if(currentIndex().row() == this->model()->rowCount() - 1)
            {
                setCurrentIndex(this->model()->index(0,0));
                break;
            }
            return QListView::keyPressEvent(e);
            break;
        }
        case Qt::Key_Up:
        {
            if(currentIndex().row() == 0)
            {
                setCurrentIndex(this->model()->index(this->model()->rowCount()-1,0));
                break;
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
