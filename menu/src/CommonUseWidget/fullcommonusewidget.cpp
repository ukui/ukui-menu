#include "fullcommonusewidget.h"
#include "ui_fullcommonusewidget.h"
#include <QDesktopWidget>
#include "src/color.h"
#include <QDebug>

FullCommonUseWidget::FullCommonUseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FullCommonUseWidget)
{
    ui->setupUi(this);
    init_widget();
}

FullCommonUseWidget::~FullCommonUseWidget()
{
    delete ui;
    delete pUkuiMenuInterface;
}

void FullCommonUseWidget::init_widget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setStyleSheet("border:0px;background:transparent;");
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    if(QApplication::desktop()->width()*QApplication::desktop()->height() <= 1600*900)
        this->setFixedSize(QApplication::desktop()->availableGeometry().width()-265,
                           QApplication::desktop()->availableGeometry().height()-87);
    else
        this->setFixedSize(QApplication::desktop()->availableGeometry().width()-286,
                       QApplication::desktop()->availableGeometry().height()-105);

    mainLayout=new QVBoxLayout(this);
    this->setLayout(mainLayout);

    pUkuiMenuInterface=new UkuiMenuInterface;

    QString path=QDir::homePath()+"/.config/ukui-menu/ukui-menu.ini";
    setting=new QSettings(path,QSettings::IniFormat);

    init_applist_widget();
    fill_app_tablewidget();
}

void FullCommonUseWidget::init_applist_widget()
{
    if(QApplication::desktop()->width()*QApplication::desktop()->height() <= 1600*900)
        mainLayout->setContentsMargins(263,0,0,0);
    else
        mainLayout->setContentsMargins(287,0,0,0);
    listview=new FullListView(this,0);
    mainLayout->addWidget(listview);
    connect(listview,SIGNAL(sendItemClickedSignal(QString)),this,SLOT(exec_app_name(QString)));
    connect(listview,SIGNAL(send_update_applist_signal()),this,SIGNAL(send_update_applist_signal()));
}

void FullCommonUseWidget::fill_app_tablewidget()
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

    listview->addData(data);
    setting->endGroup();
}

/**
 * 执行应用程序
 */
void FullCommonUseWidget::exec_app_name(QString appname)
{
    emit send_hide_mainwindow_signal();
    QString execpath=pUkuiMenuInterface->get_app_exec(pUkuiMenuInterface->get_desktop_path_by_app_name(appname));
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
 * 更新应用列表
 */
void FullCommonUseWidget::update_listview_slot()
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

    listview->updateData(data);
    setting->endGroup();
}

void FullCommonUseWidget::widget_make_zero()
{
    listview->verticalScrollBar()->setSliderPosition(0);
}
