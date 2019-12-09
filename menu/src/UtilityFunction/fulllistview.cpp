#include "fulllistview.h"
#include <QDebug>

FullListView::FullListView(QWidget *parent, int width, int height, int module):
    QListView(parent)
{
    this->w=width;
    this->h=height;
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
//    this->setFixedSize(w,h);
//    char style[400];
//    sprintf(style,"QListView{border:0px;}\
//            QListView:Item{background:transparent;border:0px;color:#ffffff;font-size:14px;padding-left:0px;}\
//            QListView:Item:hover{background-color:%s;}\
//            QListView:Item:pressed{background-color:%s;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);
    this->setStyleSheet("background:transparent;border:0px;");
    this->setSelectionMode(QAbstractItemView::NoSelection);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setIconSize(QSize(50,50));
    this->setViewMode(QListView::IconMode);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setResizeMode(QListView::Adjust);
    this->setSpacing(20);
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(rightClickedSlot()));
    connect(this,SIGNAL(clicked(QModelIndex)),this,SLOT(onClicked(QModelIndex)));

}

void FullListView::addData(QStringList data)
{
    listmodel=new FullListModel(this);
    this->setModel(listmodel);
    listmodel->setData(data);
}

void FullListView::updateData(QStringList data)
{
//    qDebug()<<listmodel->rowCount()<<data.size();
//    listmodel->setData(data);
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

}
