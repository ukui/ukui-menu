#include "fullsearchresultwidget.h"
#include "ui_fullsearchresultwidget.h"
#include <QHeaderView>
#include "src/color.h"
#include <QDebug>

FullSearchResultWidget::FullSearchResultWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FullSearchResultWidget)
{
    ui->setupUi(this);
    init_widget();
}

FullSearchResultWidget::~FullSearchResultWidget()
{
    delete ui;
}

void FullSearchResultWidget::init_widget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    char widgetcolor[100];
    sprintf(widgetcolor, "border:0px;background-color:%s;",MAINVIEWWIDGETCOLOR);
    this->setStyleSheet(QString::fromLocal8Bit(widgetcolor));
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setFixedSize(QApplication::desktop()->availableGeometry().width()-160,
                                  QApplication::desktop()->availableGeometry().height()-70);

    mainLayout=new QHBoxLayout(this);
    mainLayout->setContentsMargins(30,0,2,0);
    listview=new FullListView(this,this->width()-32-12,this->height(),3);
    mainLayout->addWidget(listview);
    this->setLayout(mainLayout);

    data.clear();
    itemdelegate=new FullItemDelegate(this,3);
    listview->setItemDelegate(itemdelegate);
    listview->addData(data);

    connect(listview,SIGNAL(sendItemClickedSignal(QString)),this,SLOT(exec_app_name(QString)));
    connect(listview,SIGNAL(sendFixedOrUnfixedSignal()),this,SIGNAL(send_update_applist_signal()));
}

/**
 * 执行应用程序
 */
void FullSearchResultWidget::exec_app_name(QString appname)
{
    emit send_hide_mainwindow_signal();
    QString execpath=pUkuiMenuInterface->get_app_exec(pUkuiMenuInterface->get_desktop_path_by_app_name(appname));
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

void FullSearchResultWidget::update_app_listview(QStringList desktopfplist)
{
    data.clear();
    data=desktopfplist;
//    listview->updateData(data);
    itemdelegate=new FullItemDelegate(this,3);
    listview->setItemDelegate(itemdelegate);
    listview->updateData(data);
}
