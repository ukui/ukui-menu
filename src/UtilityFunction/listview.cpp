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
#include "utility.h"
#include <QDebug>

ListView::ListView(QWidget *parent, int width, int height, int module):
    QListView(parent)
{
    this->w=width;
    this->h=height;
    this->module=module;
    initWidget();

    pUkuiMenuInterface=new UkuiMenuInterface;
    menu=new RightClickMenu;

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
    setAttribute(Qt::WA_TranslucentBackground);
    viewport()->setAttribute(Qt::WA_TranslucentBackground);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setResizeMode(QListView::Adjust);
    this->setTextElideMode(Qt::ElideRight);
    this->setViewMode(QListView::ListMode);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setFocusPolicy(Qt::NoFocus);
    this->setMovement(QListView::Static);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setUpdatesEnabled(true);
    this->setSpacing(0);
    this->setContentsMargins(0, 0, 0, 0);
    this->setMouseTracking(true);
    this->verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
    this->setFrameShape(QFrame::NoFrame);
    this->verticalScrollBar()->setProperty("drawScrollBarGroove",false);
    connect(this,&ListView::customContextMenuRequested,this,&ListView::rightClickedSlot);
    connect(this,&ListView::clicked,this,&ListView::onClicked);
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
        if(strlist.at(1).toInt()==1)
        {
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

void ListView::paintEvent(QPaintEvent *e)
{
    double transparency=getTransparency();
    QPainter painter(this->viewport());
    painter.setOpacity(transparency);
    painter.setPen(Qt::NoPen);
    painter.setBrush(this->palette().base());
    painter.fillRect(this->rect(), this->palette().base());

    //滚动条
    QPalette p=this->verticalScrollBar()->palette();
    QColor color(255, 255, 255);
    color.setAlphaF(0.25);
    p.setColor(QPalette::Active,QPalette::Button,color);
    this->verticalScrollBar()->setPalette(p);
    QListView::paintEvent(e);
}
