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

    QString path=QDir::homePath()+"/.config/ukui/ukui-menu.ini";
    setting=new QSettings(path,QSettings::IniFormat);

}
ListView::~ListView()
{
    delete pUkuiMenuInterface;
}

void ListView::initWidget()
{
    this->setFixedSize(w,h);
    char style[400];
    sprintf(style,"QListView{border:0px;}\
            QListView:Item{background:transparent;border:0px;color:#ffffff;font-size:14px;padding-left:0px;}\
            QListView:Item:hover{background:transparent;}\
            QListView:Item:pressed{background:transparent;}");

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
//    this->setIconSize(QSize(28,28));
//    this->setSpacing(10);
    this->setGridSize(QSize(300,48));
    this->setViewMode(QListView::ListMode);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setFocusPolicy(Qt::NoFocus);
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(rightClickedSlot()));
    connect(this,SIGNAL(clicked(QModelIndex)),this,SLOT(onClicked(QModelIndex)));

}

//void ListView::addData(QVector<QStringList> data, QStringList rowlist)
//{
//    this->rowlist.clear();
//    model=new ListModel(this);
//    this->setModel(model);
//    model->setData(data);
//    this->rowlist=rowlist;
//}

void ListView::addData(QVector<QStringList> data)
{
    model=new ListModel(this);
    this->setModel(model);
    model->setData(data);
}

void ListView::updateData(QVector<QStringList> data)
{
//    qDebug()<<model->rowCount();
//    model->removeRows(0,model->rowCount());
//    qDebug()<<model->rowCount()<<data.size();
//    model->setData(data);
    model->updateData(data);
}

void ListView::onClicked(QModelIndex index)
{
     QVariant var = model->data(index, Qt::DisplayRole);
     if(var.isValid())
     {
//         qDebug()<<var.value<QStringList>()<<index.row();
         Q_EMIT sendItemClickedSignal(var.value<QStringList>());

     }
}

void ListView::rightClickedSlot()
{
    if(!this->selectionModel()->selectedIndexes().isEmpty())
    {
        menu=new RightClickMenu(this);
        QModelIndex index=this->currentIndex();
        QVariant var=model->data(index, Qt::DisplayRole);
        QStringList strlist=var.value<QStringList>();
        if(module>0)
        {
            if(strlist.at(1).toInt()==1)
            {
                QString appname=pUkuiMenuInterface->getAppName(strlist.at(0));
                int ret=menu->showAppBtnMenu(appname);
                if(ret==1 || ret==2)
                    Q_EMIT sendFixedOrUnfixedSignal();
                if(ret==7)
                    Q_EMIT sendHideMainWindowSignal();
            }
        }
        else{
            QString appname=pUkuiMenuInterface->getAppName(strlist.at(0));
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
                    data.append(QStringList()<<desktopfp<<"1");
                }

                this->updateData(data);
                setting->endGroup();

                Q_EMIT sendUpdateAppListSignal();
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
