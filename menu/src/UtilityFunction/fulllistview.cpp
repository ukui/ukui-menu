#include "fulllistview.h"
#include <QDebug>

FullListView::FullListView(QWidget *parent, int module):
    QListView(parent)
{
    this->module=module;
    init_widget();

    pUkuiMenuInterface=new UkuiMenuInterface;

    QString path=QDir::homePath()+"/.config/ukui-menu/ukui-menu.ini";
    setting=new QSettings(path,QSettings::IniFormat);

}

FullListView::~FullListView()
{
    delete pUkuiMenuInterface;
}

void FullListView::init_widget()
{
    char style[400];
    sprintf(style,"QListView{border:0px;}\
            QListView:Item{background:transparent;border:0px;color:#ffffff;font-size:14px;padding-left:0px;}\
            QListView:Item:hover{background:transparent;}\
            QListView:Item:pressed{background:transparent;}");

    this->verticalScrollBar()->setStyleSheet("QScrollBar{width:3px;padding-top:0px;padding-bottom:0px;background-color:#283138;border-radius:6px;}"
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
    this->setIconSize(QSize(80,80));
    this->setViewMode(QListView::IconMode);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setResizeMode(QListView::Adjust);
//    this->setUpdatesEnabled(true);
    if(QApplication::desktop()->width()*QApplication::desktop()->height() <= 1600*900)
    {
//        this->setSpacing(32);
        this->setGridSize(QSize(159,159));
    }
    else
    {
//        this->setSpacing(40);
        this->setGridSize(QSize(200,200));
    }

    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(rightClickedSlot()));
    connect(this,SIGNAL(clicked(QModelIndex)),this,SLOT(onClicked(QModelIndex)));

}

void FullListView::addData(QStringList data)
{
    m_delegate= new FullItemDelegate(this,module);
    this->setItemDelegate(m_delegate);
    listmodel=new FullListModel(this);
    this->setModel(listmodel);
    listmodel->setData(data);
}

void FullListView::updateData(QStringList data)
{
    m_delegate->setParent(nullptr);
    delete m_delegate;
    m_delegate=new FullItemDelegate(this,module);
    this->setItemDelegate(m_delegate);
    listmodel->updateData(data);
}

void FullListView::removeRow(QString desktopfp)
{
    listmodel->removeRow(desktopfp);
}

void FullListView::insertRow(QString desktopfp)
{
    listmodel->insertRow(desktopfp);
}

void FullListView::onClicked(QModelIndex index)
{
     QVariant var = listmodel->data(index, Qt::DisplayRole);
     if(var.isValid())
     {

         emit sendItemClickedSignal(var.value<QString>());

     }
}

void FullListView::rightClickedSlot()
{
    if(!(this->selectionModel()->selectedIndexes().isEmpty()))
    {
        QModelIndex index=this->currentIndex();
        QVariant var=listmodel->data(index, Qt::DisplayRole);
        QString appname=var.value<QString>();
        menu=new RightClickMenu;
        if(module>0)
        {
            int ret=menu->show_appbtn_menu(appname);
            if(ret==1 || ret==2)
            {
                emit sendFixedOrUnfixedSignal();
            }
        }
        else{
            int ret=menu->show_commonuse_appbtn_menu(appname);
            if(ret==1 || ret==2)
            {
                this->setCurrentIndex(index);
            }

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
                    QString desktopfp=pUkuiMenuInterface->get_desktop_path_by_app_name(applist.at(i));
                    data.append(desktopfp);
                }
                this->updateData(data);
                setting->endGroup();
                emit send_update_applist_signal();
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
