#include "commonusewidget.h"
#include "ui_commonusewidget.h"
#include "src/color.h"
#include <QDBusInterface>
#include <QDBusReply>
#include <QDir>
#include <QDebug>

CommonUseWidget::CommonUseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommonUseWidget)
{
    ui->setupUi(this);
    init_widget();

}

CommonUseWidget::~CommonUseWidget()
{
    delete ui;
}

void CommonUseWidget::init_widget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    char widgetcolor[100];
    sprintf(widgetcolor, "border:0px;background-color:%s;",MAINVIEWWIDGETCOLOR);
    this->setStyleSheet(QString::fromLocal8Bit(widgetcolor));
//    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
//    this->setFixedSize(330,462);

    mainLayout=new  QHBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    applistWid=new QWidget(this);
    applistWid->setStyleSheet("border:0px;background: transparent;");
//    applistWid->setFixedSize(this->width(),this->height());
    mainLayout->addWidget(applistWid);
    this->setLayout(mainLayout);

    QDBusConnection::sessionBus().connect("org.ayatana.bamf","/org/ayatana/bamf/matcher","org.ayatana.bamf.matcher",
                                         QString("ViewOpened"),this,SLOT(ViewOpenedSlot(QDBusMessage)));

    QString path=QDir::homePath()+"/.config/ukui-start-menu/ukui-start-menu.ini";
    setting=new QSettings(path,QSettings::IniFormat);

    init_applist_widget();
}

void CommonUseWidget::ViewOpenedSlot(QDBusMessage msg)
{
    char btncolor[300];
    sprintf(btncolor,"QPushButton{background:transparent;border:0px;color:#ffffff;font-size:14px;padding-left:0px;text-align: left center;}\
            QPushButton:hover{background-color:%s;}\
            QPushButton:pressed{background-color:%s;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);

    QString path=msg.arguments().at(0).value<QString>();
    QString type=msg.arguments().at(1).value<QString>();
    if(QString::compare(type,"application")==0)
    {
//        qDebug()<<path<<type;
        QDBusInterface ifaceapp("org.ayatana.bamf",path,
                                "org.ayatana.bamf.application",QDBusConnection::sessionBus());
        QDBusReply<QString> replyapp =ifaceapp.call("DesktopFile");
        QString deskfp=replyapp.value();
//        qDebug()<<deskfp;

        if(!deskfp.isEmpty())
        {
//            setting->setIniCodec("UTF8");
            setting->beginGroup("activeApps");
            QString appname=KylinStartMenuInterface::get_app_name(deskfp);
            if(!setting->contains(appname))
            {
                setting->setValue(appname,1);
                setting->sync();
            }
            else if(setting->value(appname).toInt()==1)
            {
                setting->setValue(appname,2);
                setting->sync();
                QWidget* wid=apptablewid->cellWidget(0,0);
//                int count=wid->layout()->count();
//                int index=0;
//                for(index=0;index<count;index++)
//                {
//                    QLayoutItem* item=wid->layout()->itemAt(index);
//                    QWidget* wid=item->widget();
//                    QPushButton* btn=qobject_cast<QPushButton*>(wid);
//                    QLayoutItem* btnitem=btn->layout()->itemAt(1);
//                    QWidget* labelwid=btnitem->widget();
//                    QLabel* label=qobject_cast<QLabel*>(labelwid);
//                    QString name=label->text();
//                    if(QString::compare(name,appname)==0)
//                        break;
//                }
//                if(index==count)
//                {
                    QPushButton* btn=new QPushButton;
                    btn->setFixedSize(330-4-14,42);
                    btn->setStyleSheet(QString::fromLocal8Bit(btncolor));
                    QHBoxLayout* layout=new QHBoxLayout(btn);
                    layout->setContentsMargins(15,0,0,0);
                    layout->setSpacing(15);
                    QString iconstr=KylinStartMenuInterface::get_app_icon(deskfp);
                    QIcon icon=QIcon::fromTheme(iconstr);
                    QLabel* labelicon=new QLabel(btn);
                    labelicon->setFixedSize(32,32);
                    QPixmap pixmapicon(icon.pixmap(QSize(32,32)));
                    labelicon->setPixmap(pixmapicon);
                    labelicon->setStyleSheet("QLabel{background:transparent;}");
                    QLabel* labeltext=new QLabel(btn);
                    labeltext->setText(appname);
                    labeltext->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
                    labeltext->adjustSize();
                    layout->addWidget(labelicon);
                    layout->addWidget(labeltext);
                    flowlayout->addWidget(btn);
                    if(!is_fullscreen)
                    {
                        int num=wid->layout()->count();
                        wid->setFixedSize(apptablewid->width()-14,num*42);
                        apptablewid->setRowHeight(0,num*42);
                    }
                    else{
                        int dividend=apptablewid->width()/312;
                        int num=wid->layout()->count();
                        if(num%dividend>0)
                            num=num/dividend+1;
                        else num=num/dividend;
                        QWidget* wid=apptablewid->cellWidget(0,0);
                        wid->setFixedSize(apptablewid->width()-14,num*42);
                        apptablewid->setRowHeight(0,num*42);
                    }
//                }
            }
            setting->endGroup();

        }

    }

//    fill_app_tablewidget();
}

/**
 * 初始化应用列表界面
 */
void CommonUseWidget::init_applist_widget()
{
    QHBoxLayout* layout=new QHBoxLayout(applistWid);
    layout->setContentsMargins(2,0,2,0);
    apptablewid=new QTableWidget(applistWid);
//    apptablewid->setFixedSize(applistWid->width()-4,applistWid->height());
    layout->addWidget(apptablewid);
    applistWid->setLayout(layout);
    init_app_tablewidget();
    fill_app_tablewidget();
    load_min_wid();

}

/**
 * 初始化应用列表
 */
void CommonUseWidget::init_app_tablewidget()
{
    QStringList header;
    header.append("");
    apptablewid->setHorizontalHeaderLabels(header);
    apptablewid->setColumnCount(1);
    apptablewid->verticalHeader()->setDefaultSectionSize(42);
    apptablewid->verticalHeader()->setHidden(true);
    apptablewid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    apptablewid->setFocusPolicy(Qt::NoFocus);
    apptablewid->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    apptablewid->horizontalHeader()->setFixedHeight(0);
    apptablewid->setEditTriggers(QAbstractItemView::NoEditTriggers);
    apptablewid->setSelectionMode(QAbstractItemView::NoSelection);
    apptablewid->setShowGrid(false);
    apptablewid->setStyleSheet("QTableWidget{border:0px;background:transparent;}");
    apptablewid->verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
    apptablewid->verticalScrollBar()->setStyleSheet(
                "QScrollBar{width:12px;padding-top:15px;padding-bottom:15px;background-color:#283138;}"
                "QScrollBar::handle{background-color:#414e59; width:12px;}"
                "QScrollBar::handle:hover{background-color:#697883; }"
                "QScrollBar::handle:pressed{background-color:#8897a3;}"
                "QScrollBar::sub-line{background-color:#283138;height:15px;width:12px;border-image:url(:/data/img/mainviewwidget/uparrow.svg);subcontrol-position:top;}"
                "QScrollBar::sub-line:hover{background-color:#697883;}"
                "QScrollBar::sub-line:pressed{background-color:#8897a3;border-image:url(:/data/img/mainviewwidget/uparrow-pressed.svg);}"
                "QScrollBar::add-line{background-color:#283138;height:15px;width:12px;border-image:url(:/data/img/mainviewwidget/downarrow.svg);subcontrol-position:bottom;}"
                "QScrollBar::add-line:hover{background-color:#697883;}"
                "QScrollBar::add-line:pressed{background-color:#8897a3;border-image:url(:/data/img/mainviewwidget/downarrow-pressed.svg);}"
                );

}

/**
 * 填充应用列表
 */
void CommonUseWidget::fill_app_tablewidget()
{
    char btncolor[300];
    sprintf(btncolor,"QPushButton{background:transparent;border:0px;color:#ffffff;font-size:14px;padding-left:0px;text-align: left center;}\
            QPushButton:hover{background-color:%s;}\
            QPushButton:pressed{background-color:%s;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);

    QStringList keys;
    keys.clear();
    setting->beginGroup("activeApps");
    keys=setting->childKeys();
    applist.clear();
    for(int i=0;i<keys.count();i++)
    {
        int val=setting->value(keys.at(i)).toInt();
        if(val==2 || val==0)
            applist.append(keys.at(i));
    }

    apptablewid->setRowCount(0);
//    apptablewid->setRowCount(applist.count());
    apptablewid->setRowCount(1);
    flowlayout=new QFlowLayout(0,0,0);
    wid=new QWidget;
    wid->setLayout(flowlayout);
    apptablewid->setCellWidget(0,0,wid);
    for(int i=0;i<applist.count();i++)
    {
        QPushButton* btn=new QPushButton;
        btn->setFixedSize(330-4-14,42);
        btn->setStyleSheet(QString::fromLocal8Bit(btncolor));
        QHBoxLayout* layout=new QHBoxLayout(btn);
        layout->setContentsMargins(15,0,0,0);
        layout->setSpacing(15);

        QString iconstr=KylinStartMenuInterface::get_app_icon(KylinStartMenuInterface::get_desktop_path_by_app_name(applist.at(i)));
//        QIcon::setThemeName("ukui-icon-theme");
        QIcon icon=QIcon::fromTheme(iconstr);
        QLabel* labelicon=new QLabel(btn);
        labelicon->setFixedSize(32,32);
        QPixmap pixmapicon(icon.pixmap(QSize(32,32)));
        labelicon->setPixmap(pixmapicon);
        labelicon->setStyleSheet("QLabel{background:transparent;}");
        QLabel* labeltext=new QLabel(btn);
        labeltext->setText(applist.at(i));
        labeltext->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
        labeltext->adjustSize();
        layout->addWidget(labelicon);
        layout->addWidget(labeltext);

        if(setting->value(applist.at(i)).toInt()==0)
        {
            layout->setContentsMargins(15,0,12,0);
            QSvgRenderer* svgRender=new QSvgRenderer(btn);
            svgRender->load(QString(":/data/img/mainviewwidget/lock.svg"));
            QPixmap* pixmap=new QPixmap(14,14);
            pixmap->fill(Qt::transparent);
            QPainter p(pixmap);
            svgRender->render(&p);
            QLabel* lock=new QLabel;
            lock->setPixmap(*pixmap);
            lock->setAlignment(Qt::AlignCenter);
            lock->setStyleSheet("QLabel{background:transparent;}");
            lock->setFixedSize(pixmap->size());
            layout->addWidget(lock);

        }
        btn->setLayout(layout);
        btn->setFocusPolicy(Qt::NoFocus);

//        apptablewid->setCellWidget(i,0,btn);
        flowlayout->addWidget(btn);

        connect(btn,SIGNAL(clicked()),this,SLOT(exec_app_name()));
        btn->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(btn,SIGNAL(customContextMenuRequested(const QPoint&)),this,
                SLOT(right_click_slot()));
    }
    setting->endGroup();
}

void CommonUseWidget::load_min_wid()
{
    is_fullscreen=false;
    this->setGeometry(QRect(60,QApplication::desktop()->availableGeometry().height()-462,
                                 330,462));
    applistWid->setFixedSize(this->width(),this->height());
    apptablewid->setFixedSize(applistWid->width()-4,applistWid->height());
    int num=flowlayout->count();
    QWidget* wid=apptablewid->cellWidget(0,0);
    wid->setFixedSize(apptablewid->width()-14,num*42);
    apptablewid->setRowHeight(0,num*42);
}

void CommonUseWidget::load_max_wid()
{
    is_fullscreen=true;
    this->setGeometry(QRect(160,
                              70,
                              QApplication::desktop()->availableGeometry().width()-160,
                              QApplication::desktop()->availableGeometry().height()-70));

    applistWid->setFixedSize(this->width(),this->height());
    apptablewid->setFixedSize(applistWid->width()-32-12,applistWid->height());
    int num=flowlayout->count();
    int dividend=apptablewid->width()/312;
    if(num%dividend>0)
        num=num/dividend+1;
    else num=num/dividend;
    QWidget* wid=apptablewid->cellWidget(0,0);
    wid->setFixedSize(apptablewid->width()-14,num*42);
    apptablewid->setRowHeight(0,num*42);
}

/**
 * 加载右键菜单
 */
void CommonUseWidget::right_click_slot()
{
    QPushButton* btn=dynamic_cast<QPushButton*>(QObject::sender());
    QLayoutItem* item=btn->layout()->itemAt(1);
    QWidget* wid=item->widget();
    QLabel* label=qobject_cast<QLabel*>(wid);
    QString appname=label->text();

    menu=new RightClickMenu;
    int ret=menu->show_commonuse_appbtn_menu(appname);
    if(ret==1)
    {
        btn->layout()->setContentsMargins(15,0,12,0);
        QSvgRenderer* svgRender=new QSvgRenderer(btn);
        svgRender->load(QString(":/data/img/mainviewwidget/lock.svg"));
        QPixmap* pixmap=new QPixmap(14,14);
        pixmap->fill(Qt::transparent);
        QPainter p(pixmap);
        svgRender->render(&p);
        QLabel* lock=new QLabel;
        lock->setPixmap(*pixmap);
        lock->setAlignment(Qt::AlignCenter);
        lock->setStyleSheet("QLabel{background:transparent;}");
        lock->setFixedSize(pixmap->size());
        btn->layout()->addWidget(lock);
    }
    if(ret==2)
    {
        btn->layout()->setContentsMargins(15,0,0,0);
        QLayoutItem* item=btn->layout()->itemAt(2);
        QWidget* wid=item->widget();
        btn->layout()->removeWidget(wid);
        wid->setParent(nullptr);
        delete wid;
    }
    if(ret==8)
    {
//        QModelIndex index=apptablewid->indexAt(btn->pos());
//        apptablewid->removeCellWidget(index.row(),0);
//        apptablewid->removeRow(index.row());
        flowlayout->removeWidget(btn);
        btn->setParent(nullptr);
        delete btn;

    }
    if(ret==9)
    {
        setting->beginGroup("activeApps");
        int index=0;
        for(index=flowlayout->count()-1;index>=0;index--)
        {
            QLayoutItem* item=flowlayout->itemAt(index);
            QWidget* wid=item->widget();
            QPushButton* btn=qobject_cast<QPushButton*>(wid);
            QLayoutItem* btnitem=btn->layout()->itemAt(1);
            QWidget* labelwid=btnitem->widget();
            QLabel* label=qobject_cast<QLabel*>(labelwid);
            QString name=label->text();
//            QString desktopfp=KylinStartMenuInterface::get_desktop_path_by_app_name(name);
//            QFileInfo fileinfo(desktopfp);
//            QString desktopfn=fileinfo.fileName();
            if(setting->value(name).toInt()>0)
             {
                flowlayout->removeWidget(btn);
                btn->setParent(nullptr);
                delete btn;
                setting->remove(name);
                setting->sync();
//                index--;
            }
        }
        setting->endGroup();
    }
}

/**
 * 执行应用程序
 */
void CommonUseWidget::exec_app_name()
{
    emit send_hide_mainwindow_signal();
    QPushButton* btn=dynamic_cast<QPushButton*>(QObject::sender());
    QLayoutItem* item=btn->layout()->itemAt(1);
    QWidget* wid=item->widget();
    QLabel* label=qobject_cast<QLabel*>(wid);
    QString appname=label->text();
    QString execpath=KylinStartMenuInterface::get_app_exec(KylinStartMenuInterface::get_desktop_path_by_app_name(appname));
    //    qDebug()<<execpath;
    //移除启动参数%u或者%U
    for(int i=0;i<execpath.length();i++)
    {
        if(execpath.at(i)=='%')
        {
            execpath.remove(i,2);
        }
    }
    QProcess *process=new QProcess(this);
    process->startDetached(execpath);
}

/**
 * 更新应用列表槽函数
 */
void CommonUseWidget::update_tablewid_slot(QString appname)
{
    setting->beginGroup("activeApps");
    int count=flowlayout->count();
    int index=0;
    for(index=0;index<count;index++)
    {
        QLayoutItem* item=flowlayout->itemAt(index);
        QWidget* wid=item->widget();
        QPushButton* btn=qobject_cast<QPushButton*>(wid);
        QLayoutItem* btnitem=btn->layout()->itemAt(1);
        QWidget* labelwid=btnitem->widget();
        QLabel* label=qobject_cast<QLabel*>(labelwid);
        QString name=label->text();
        if(QString::compare(appname,name)==0)
        {
            if(setting->value(appname).toInt()==0)
            {
                btn->layout()->setContentsMargins(15,0,12,0);
                QSvgRenderer* svgRender=new QSvgRenderer(btn);
                svgRender->load(QString(":/data/img/mainviewwidget/lock.svg"));
                QPixmap* pixmap=new QPixmap(14,14);
                pixmap->fill(Qt::transparent);
                QPainter p(pixmap);
                svgRender->render(&p);
                QLabel* lock=new QLabel;
                lock->setPixmap(*pixmap);
                lock->setAlignment(Qt::AlignCenter);
                lock->setStyleSheet("QLabel{background:transparent;}");
                lock->setFixedSize(pixmap->size());
                btn->layout()->addWidget(lock);
            }
            else{
                btn->layout()->setContentsMargins(15,0,0,0);
                QLayoutItem* item=btn->layout()->itemAt(2);
                QWidget* wid=item->widget();
                btn->layout()->removeWidget(wid);
                wid->setParent(nullptr);
                delete wid;
            }
            break;
        }

    }
    if(index==count)
    {
        char btncolor[300];
        sprintf(btncolor,"QPushButton{background:transparent;border:0px;color:#ffffff;font-size:14px;padding-left:0px;text-align: left center;}\
                QPushButton:hover{background-color:%s;}\
                QPushButton:pressed{background-color:%s;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);
        QPushButton* btn=new QPushButton;
        btn->setFixedSize(330-4-14,42);
        btn->setStyleSheet(QString::fromLocal8Bit(btncolor));
        QHBoxLayout* layout=new QHBoxLayout(btn);
        layout->setContentsMargins(15,0,12,0);
        layout->setSpacing(15);
        QString iconstr=KylinStartMenuInterface::get_app_icon(KylinStartMenuInterface::get_desktop_path_by_app_name(appname));
        QIcon icon=QIcon::fromTheme(iconstr);
        QLabel* labelicon=new QLabel(btn);
        labelicon->setFixedSize(32,32);
        QPixmap pixmapicon(icon.pixmap(QSize(32,32)));
        labelicon->setPixmap(pixmapicon);
        labelicon->setStyleSheet("QLabel{background:transparent;}");
        QLabel* labeltext=new QLabel(btn);
        labeltext->setText(appname);
        labeltext->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
        labeltext->adjustSize();
        layout->addWidget(labelicon);
        layout->addWidget(labeltext);
        QSvgRenderer* svgRender=new QSvgRenderer(btn);
        svgRender->load(QString(":/data/img/mainviewwidget/lock.svg"));
        QPixmap* pixmap=new QPixmap(14,14);
        pixmap->fill(Qt::transparent);
        QPainter p(pixmap);
        svgRender->render(&p);
        QLabel* lock=new QLabel;
        lock->setPixmap(*pixmap);
        lock->setAlignment(Qt::AlignCenter);
        lock->setStyleSheet("QLabel{background:transparent;}");
        lock->setFixedSize(pixmap->size());
        layout->addWidget(lock);
        btn->setLayout(layout);
        btn->setFocusPolicy(Qt::NoFocus);
        flowlayout->addWidget(btn);

        connect(btn,SIGNAL(clicked()),this,SLOT(exec_app_name()));
        btn->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(btn,SIGNAL(customContextMenuRequested(const QPoint&)),this,
                SLOT(right_click_slot()));
        if(!is_fullscreen)
        {
            int num=wid->layout()->count();
            wid->setFixedSize(apptablewid->width()-14,num*42);
            apptablewid->setRowHeight(0,num*42);
        }
        else{
            int dividend=apptablewid->width()/312;
            int num=wid->layout()->count();
            if(num%dividend>0)
                num=num/dividend+1;
            else num=num/dividend;
            QWidget* wid=apptablewid->cellWidget(0,0);
            wid->setFixedSize(apptablewid->width()-14,num*42);
            apptablewid->setRowHeight(0,num*42);
        }

        setting->setValue(appname,0);
        setting->sync();

    }
    setting->endGroup();
}
