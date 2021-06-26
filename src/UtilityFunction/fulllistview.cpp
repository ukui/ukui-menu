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
#include "utility.h"
#include <QDebug>
#include <syslog.h>
#include <QPushButton>
#include <QGSettings>

FullListView::FullListView(QWidget *parent, int module):
    QListView(parent)
{
    this->module=module;
    initWidget();
//    inCurView = false;
    pUkuiMenuInterface=new UkuiMenuInterface;
//    menu=new RightClickMenu;
}

FullListView::~FullListView()
{
//    delete menu;
    delete pUkuiMenuInterface;
}

void FullListView::initWidget()
{
    viewport()->setAttribute(Qt::WA_TranslucentBackground);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    if(module==1 || module==2)s
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setViewMode(QListView::IconMode);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setResizeMode(QListView::Adjust);
    this->setTextElideMode(Qt::ElideRight);
    this->setMouseTracking(true);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setMovement(QListView::Static);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setGridSize(QSize(Style::AppListGridSizeWidth,Style::AppListGridSizeWidth));
    this->verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
    this->setFrameShape(QFrame::NoFrame);//移除边框
    this->setStyleSheet(QString::fromUtf8("QStandardItemModel#listmodel{border:3px solid #FFFFFF; }QWidget#widget:focus{ border:5px solid #A5A6A1;  } "));
    connect(this,&FullListView::customContextMenuRequested,this,&FullListView::rightClickedSlot);

    connect(this,&FullListView::clicked,this,&FullListView::onClicked);

//    if(QGSettings::isSchemaInstalled(QString("org.ukui.control-center.personalise").toLocal8Bit()))
//    {
//        gsetting = new QGSettings(QString("org.ukui.control-center.personalise").toLocal8Bit());
//        if(gsetting->keys().contains(QString("transparency")))
//            transparency = gsetting->get("transparency").toDouble();
//        connect(gsetting,&QGSettings::changed,[this] (const QString &key)
//        {
//            if (key == "transparency")
//            {
//                transparency = gsetting->get("transparency").toDouble();
//            }
//        });
//    }
}

void FullListView::addData(QStringList data)
{
    listmodel=new QStandardItemModel(this);
    this->setModel(listmodel);

    Q_FOREACH(QString desktopfp,data)
    {
        QStandardItem* item=new QStandardItem;
        item->setData(QVariant::fromValue<QString>(desktopfp),Qt::DisplayRole);
        listmodel->appendRow(item);
    }
    m_delegate= new FullItemDelegate(this,module);
    this->setItemDelegate(m_delegate);
}

void FullListView::focusInEvent(QFocusEvent *event)
{
    //inCurView = true;
    return QListView::focusInEvent(event);
}

void FullListView::focusOutEvent(QFocusEvent *event)
{
    //inCurView = false;
    return QListView::focusOutEvent(event);
}

void FullListView::updateData(QStringList data)
{
    listmodel->clear();
    Q_FOREACH(QString desktopfp,data)
    {
        QStandardItem* item=new QStandardItem;
        item->setData(QVariant::fromValue<QString>(desktopfp),Qt::DisplayRole);
        listmodel->appendRow(item);
    }
}

void FullListView::onClicked(QModelIndex index)
{
     QVariant var = listmodel->data(index, Qt::DisplayRole);
     if(var.isValid())
     {
         QString desktopfp=var.value<QString>();
         Q_EMIT sendItemClickedSignal(desktopfp);

     }
}

void FullListView::rightClickedSlot(const QPoint &pos)
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
            default:
                break;
            }
        }
        //this->selectionModel()->clear();
    }
}

void FullListView::enterEvent(QEvent *e)
{
    Q_UNUSED(e);
    this->selectionModel()->clear();
    this->verticalScrollBar()->setVisible(true);
}

void FullListView::leaveEvent(QEvent *e)
{
    Q_UNUSED(e);
    this->verticalScrollBar()->setVisible(false);
//    QWidget *current_focus_widget;
   // current_focus_widget = QApplication::focusWidget();
//    current_focus_widget = QWidget::focusWidget();
//    QPushButton *le= qobject_cast<QPushButton*>(current_focus_widget);

}

void FullListView::selectFirstItem()
{
    qDebug() << "void FullListView::selectFirstItem()";
    if(this->currentIndex().row() == -1)
    {
        this->setCurrentIndex(this->model()->index(0,0));
    }
}

void FullListView::paintEvent(QPaintEvent *e)
{
    double transparency=getTransparency();
    QPainter painter(this->viewport());

//    if(QGSettings::isSchemaInstalled(QString("org.ukui.control-center.personalise").toLocal8Bit()))
//    {
//        QGSettings gsetting(QString("org.ukui.control-center.personalise").toLocal8Bit());
//        if(gsetting.keys().contains(QString("effect")))
//        {
//            if(gsetting.get("effect").toBool())
//            {
//                painter.setBrush(Qt::black);
//                painter.setPen(Qt::transparent);
//                painter.setOpacity(0.25);
//                painter.drawRect(this->rect());
//            }
//            else
//            {
//                painter.setBrush(this->palette().base());
//                painter.setPen(Qt::transparent);
//                painter.setOpacity(transparency);
//                painter.drawRect(this->rect());
//            }
//        }
//        else
//        {
//            painter.setBrush(this->palette().base());
//            painter.setPen(Qt::transparent);
//            painter.setOpacity(transparency);
//            painter.drawRect(this->rect());
//        }
//    }
//    else
//    {
        painter.setBrush(this->palette().base());
        painter.setPen(Qt::transparent);
        painter.setOpacity(transparency);
        painter.drawRect(this->rect());
//    }
    QListView::paintEvent(e);
}

void FullListView::keyPressEvent(QKeyEvent* e)
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
//void FullListView::mousePressEvent(QMouseEvent *event)
//{
//    if(!(this->indexAt(event->pos()).isValid()) && event->button()==Qt::LeftButton)
//                                                 Q_EMIT sendHideMainWindowSignal();
//    else{
//        if(event->button()==Qt::LeftButton)
//            Q_EMIT clicked(this->indexAt(event->pos()));
//        if(event->button()==Qt::RightButton)
//        {
//             this->selectionModel()->setCurrentIndex(this->indexAt(event->pos()),QItemSelectionModel::SelectCurrent);
//            Q_EMIT customCont     extMenuRequested(event->pos());
//        }
//    }
//}
