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
#include <QDebug>

FullListView::FullListView(QWidget *parent, int module):
    QListView(parent)
{
    this->module=module;
    initWidget();

    pUkuiMenuInterface=new UkuiMenuInterface;

    QString path=QDir::homePath()+"/.config/ukui/ukui-menu.ini";
    setting=new QSettings(path,QSettings::IniFormat);

}

FullListView::~FullListView()
{
    delete pUkuiMenuInterface;
}

void FullListView::initWidget()
{
    char style[500];
    sprintf(style,"QListView{border:0px;}\
            QListView:Item{background:transparent;border:0px;color:#ffffff;font-size:14px;padding-left:0px;}\
            QListView:Item:hover{background:transparent;}\
            QListView:Item:pressed{background:transparent;}");

    if(module!=1 && module!=2)
        this->verticalScrollBar()->setStyleSheet("QScrollBar{width:3px;padding-top:0px;padding-bottom:0px;background:transparent;border-radius:6px;}"
                                             "QScrollBar::handle{background-color:rgba(255,255,255,0.25); width:3px;border-radius:1.5px;}"
                                             "QScrollBar::handle:hover{background-color:#697883;border-radius:1.5px;}"
                                             "QScrollBar::handle:pressed{background-color:#8897a3;border-radius:1.5px;}"
                                             "QScrollBar::sub-line{background-color:transparent;height:0px;width:0px;}"
                                             "QScrollBar::add-line{background-color:transparent;height:0px;width:0px;}"
                                             );


    this->setStyleSheet(style);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    if(module==1 || module==2)
        this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    this->setIconSize(QSize(80,80));
    this->setViewMode(QListView::IconMode);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setResizeMode(QListView::Adjust);
    this->setTextElideMode(Qt::ElideRight);
    this->setMouseTracking(true);
    this->setFocusPolicy(Qt::NoFocus);
//    this->setUpdatesEnabled(true);

//    this->setSpacing(32);
    this->setGridSize(QSize(Style::AppListGridSizeWidth,Style::AppListGridSizeWidth));
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(rightClickedSlot()));
    connect(this,SIGNAL(clicked(QModelIndex)),this,SLOT(onClicked(QModelIndex)));
    connect(this,SIGNAL(entered(QModelIndex)),this,SLOT(showToolTip(QModelIndex)));
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
         QString appname=pUkuiMenuInterface->getAppName(desktopfp);
         Q_EMIT sendItemClickedSignal(appname);

     }
}

void FullListView::rightClickedSlot()
{
    if(!(this->selectionModel()->selectedIndexes().isEmpty()))
    {
        QModelIndex index=this->currentIndex();
        QVariant var=listmodel->data(index, Qt::DisplayRole);
        QString desktopfp=var.value<QString>();
        QString appname=pUkuiMenuInterface->getAppName(desktopfp);
        menu=new RightClickMenu(this);
        if(module>0)
        {
            int ret=menu->showAppBtnMenu(appname);
            if(ret==1 || ret==2)
            {
                Q_EMIT sendFixedOrUnfixedSignal();
            }
            if(ret==7)
                Q_EMIT sendHideMainWindowSignal();
        }
        else{
            int ret=menu->showCommonUseAppBtnMenu(appname);
            if(ret==1 || ret==2)
            {
                this->setCurrentIndex(index);
            }

            if(ret==7)
                Q_EMIT sendHideMainWindowSignal();

            if(ret==8 || ret==9)
            {
                QStringList keys;
                keys.clear();
                setting->beginGroup("application");
                keys=setting->childKeys();
                QStringList applist;
                applist.clear();
                for(int i=0;i<keys.count();i++)
                {
                    int val=setting->value(keys.at(i)).toInt();
                    if(val==2 || val==0)
                        applist.append(keys.at(i));
                }

                data.clear();
                for(int i=0;i<applist.count();i++)
                {
//                    QString desktopfp=pUkuiMenuInterface->getDesktopPathByAppName(applist.at(i));
                    QString desktopfp=QString("/usr/share/applications/"+applist.at(i));
                    data.append(desktopfp);
                }
                this->updateData(data);
                setting->endGroup();
                Q_EMIT sendUpdateAppListSignal();
            }
        }

        this->selectionModel()->clear();
    }
}

void FullListView::enterEvent(QEvent *e)
{
    Q_UNUSED(e);
    this->verticalScrollBar()->setVisible(true);
}

void FullListView::leaveEvent(QEvent *e)
{
    Q_UNUSED(e);
    this->verticalScrollBar()->setVisible(false);
}

void FullListView::showToolTip(const QModelIndex &index)
{
//    qDebug()<<this->y()<<this->visualRect(index).y();
//    if(!index.isValid())
//        return;

//    QVariant var=listmodel->data(index, Qt::DisplayRole);
//    QString desktopfp=var.value<QString>();
//    QString appname=pUkuiMenuInterface->getAppName(desktopfp);

////    if(file_name.isEmpty())
////        return;

//    QToolTip::showText(QCursor::pos(), appname,this);
//    QToolTip::hideText();
}

//bool FullListView::event(QEvent* e)
//{
//    if (e->type() == QEvent::ToolTip) {
//        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(e);
//        QModelIndex index=this->currentIndex();
//        QVariant var=listmodel->data(index, Qt::DisplayRole);
//        QString desktopfp=var.value<QString>();
//        QString appname=pUkuiMenuInterface->getAppName(desktopfp);
//        if (!appname.isEmpty()) {
//            QToolTip::showText(helpEvent->globalPos(), appname);
//        } else {
//            QToolTip::hideText();
//            e->ignore();
//        }
//        return true;
//    }
//    return QWidget::event(e);
//}
