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
    mainLayout->addWidget(applistWid);
    this->setLayout(mainLayout);

    QDBusConnection::sessionBus().connect("org.ayatana.bamf","/org/ayatana/bamf/matcher","org.ayatana.bamf.matcher",
                                         QString("ViewOpened"),this,SLOT(ViewOpenedSlot(QDBusMessage)));

    QString path=QDir::homePath()+"/.config/ukui-start-menu/ukui-start-menu.ini";
    setting=new QSettings(path,QSettings::IniFormat);

    init_applist_widget();
}

/**
 * 初始化应用列表界面
 */
void CommonUseWidget::init_applist_widget()
{
    QHBoxLayout* layout=new QHBoxLayout(applistWid);
    layout->setContentsMargins(2,0,2,0);
    applistWid->setLayout(layout);
    scrollarea=new ScrollArea;
    scrollareawid=new QWidget;
    scrollarea->setWidget(scrollareawid);
    scrollarea->setWidgetResizable(true);
    flowlayout=new QFlowLayout(0,0,0);
    scrollareawid->setLayout(flowlayout);
    layout->addWidget(scrollarea);
    fill_app_list();
    load_min_wid();

}

void CommonUseWidget::ViewOpenedSlot(QDBusMessage msg)
{
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
                PushButton* btn=new PushButton(appname,1,0);
                flowlayout->addWidget(btn);
                flowlayout->update();
                connect(btn,SIGNAL(clicked()),
                        this,SIGNAL(send_hide_mainwindow_signal()));
                connect(btn,SIGNAL(send_right_click_signal(int)),
                        this,SLOT(recv_right_click_slot(int)));

                if(!is_fullscreen)
                {
                    int num=flowlayout->count();
                    scrollareawid->setFixedSize(scrollarea->width()-14,num*42);
                }
                else{
                    int dividend=scrollarea->width()/312;
                    int num=flowlayout->count();
                    if(num%dividend>0)
                        num=num/dividend+1;
                    else num=num/dividend;
                    scrollareawid->setFixedSize(scrollarea->width()-14,num*42);
                }
            }
            setting->endGroup();

        }

    }
}

/**
 * 填充应用列表
 */
void CommonUseWidget::fill_app_list()
{
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

    for(int i=0;i<applist.count();i++)
    {
        PushButton* btn=new PushButton(applist.at(i),1,0);
        flowlayout->addWidget(btn);
        connect(btn,SIGNAL(clicked()),
                this,SIGNAL(send_hide_mainwindow_signal()));
        connect(btn,SIGNAL(send_right_click_signal(int)),
                this,SLOT(recv_right_click_slot(int)));
    }
    flowlayout->update();
    setting->endGroup();
}

void CommonUseWidget::load_min_wid()
{
    is_fullscreen=false;
    this->setGeometry(QRect(60,QApplication::desktop()->availableGeometry().height()-462,
                                 330,462));
    applistWid->setFixedSize(this->width(),this->height());
    scrollarea->setFixedSize(applistWid->width()-4,applistWid->height());
    int num=flowlayout->count();
    scrollareawid->setFixedSize(scrollarea->width()-14,num*42);
}

void CommonUseWidget::load_max_wid()
{
    is_fullscreen=true;
    this->setGeometry(QRect(160,
                              70,
                              QApplication::desktop()->availableGeometry().width()-160,
                              QApplication::desktop()->availableGeometry().height()-70));

    applistWid->setFixedSize(this->width(),this->height());
    scrollarea->setFixedSize(applistWid->width()-32-12,applistWid->height());
    int num=flowlayout->count();
    int dividend=scrollarea->width()/312;
    if(num%dividend>0)
        num=num/dividend+1;
    else num=num/dividend;
    scrollareawid->setFixedSize(scrollarea->width()-14,num*42);
}

void CommonUseWidget::recv_right_click_slot(int ret)
{
    QPushButton* btn=dynamic_cast<QPushButton*>(QObject::sender());
    QLayoutItem* btnitem=btn->layout()->itemAt(1);
    QWidget* labelwid=btnitem->widget();
    QLabel* label=qobject_cast<QLabel*>(labelwid);
    QString name=label->text();
    if(ret==1)
    {
        btn->layout()->setContentsMargins(15,0,12,0);
        QSvgRenderer* svgRender=new QSvgRenderer(btn);
        svgRender->load(QString(":/data/img/mainviewwidget/lock.svg"));
        QPixmap* pixmap=new QPixmap(16,16);
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
        setting->beginGroup("activeApps");
        setting->remove(name);
        setting->sync();
        setting->endGroup();
        flowlayout->removeWidget(btn);
        btn->setParent(nullptr);
        delete btn;
        flowlayout->update();

    }
    if(ret==9)
    {
        setting->beginGroup("activeApps");
        int index=0;
        while(index<flowlayout->count())
        {
//            qDebug()<<index;
            QLayoutItem* item=flowlayout->itemAt(index);
            QWidget* wid=item->widget();
            QPushButton* btn=qobject_cast<QPushButton*>(wid);
            QLayoutItem* btnitem=btn->layout()->itemAt(1);
            QWidget* labelwid=btnitem->widget();
            QLabel* label=qobject_cast<QLabel*>(labelwid);
            QString name=label->text();
            if(setting->value(name).toInt()>0)
             {
                flowlayout->removeWidget(btn);
                btn->setParent(nullptr);
                delete btn;
                setting->remove(name);
                setting->sync();
                index--;
            }
            index++;
        }
        setting->endGroup();
        flowlayout->update();
    }

    if(ret==8 || ret==9)
    {
        if(is_fullscreen)
        {
            int num=flowlayout->count();
            int dividend=scrollarea->width()/312;
            if(num%dividend>0)
                num=num/dividend+1;
            else num=num/dividend;
            scrollareawid->setFixedSize(scrollarea->width()-14,num*42);
        }
        else
        {
            int num=flowlayout->count();
            scrollareawid->setFixedSize(scrollarea->width()-14,num*42);
        }
    }
}

/**
 * 字母排序与功能分类模块固定或取消固定到常用软件时更新应用列表
 */
void CommonUseWidget::update_app_list(QString appname)
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
        if(QString::compare(appname,name)==0)//应用在常用列表中
        {
            if(setting->value(appname).toInt()==0)//固定
            {
                btn->layout()->setContentsMargins(15,0,12,0);
                QSvgRenderer* svgRender=new QSvgRenderer(btn);
                svgRender->load(QString(":/data/img/mainviewwidget/lock.svg"));
                QPixmap* pixmap=new QPixmap(16,16);
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
            else{//取消固定
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
    if(index==count)//应用不在常用列表里面
    {
        PushButton* btn=new PushButton(appname,1,0);
        flowlayout->addWidget(btn);
        connect(btn,SIGNAL(send_right_click_signal(int)),
                this,SLOT(recv_right_click_slot(int)));
        if(is_fullscreen)
        {
            int num=flowlayout->count();
            int dividend=scrollarea->width()/312;
            if(num%dividend>0)
                num=num/dividend+1;
            else num=num/dividend;
            scrollareawid->setFixedSize(scrollarea->width()-14,num*42);
        }
        else
        {
            int num=flowlayout->count();
            scrollareawid->setFixedSize(scrollarea->width()-14,num*42);
        }

        setting->setValue(appname,0);
        setting->sync();
    }
    flowlayout->update();
    setting->endGroup();
}

/**
 * 有软件卸载时更新应用列表
 */
void CommonUseWidget::update_app_list(QString name,int arg)
{
    setting->beginGroup("activeApps");
    if(arg==-1)
    {
        if(setting->value(name)==0 || setting->value(name)==2)
        {
            for(int index=0;index<flowlayout->count();index++)
            {
                QLayoutItem* item=flowlayout->itemAt(index);
                QWidget* wid=item->widget();
                QPushButton* btn=qobject_cast<QPushButton*>(wid);
                QLayoutItem* btnitem=btn->layout()->itemAt(1);
                QWidget* labelwid=btnitem->widget();
                QLabel* label=qobject_cast<QLabel*>(labelwid);
                QString appname=label->text();
                if(QString::compare(appname,name)==0)//应用在常用列表中
                {
                    flowlayout->removeWidget(btn);
                    btn->setParent(nullptr);
                    delete btn;
                    flowlayout->update();
                    break;
                }
            }
            setting->remove(name);
            setting->sync();
        }
        if(setting->value(name)==1){
            setting->remove(name);
            setting->sync();
        }
    }
    setting->endGroup();
}
