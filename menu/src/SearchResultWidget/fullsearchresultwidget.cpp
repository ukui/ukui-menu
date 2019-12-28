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
    this->setStyleSheet("border:0px;background:transparent;");
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    if(QApplication::desktop()->width()*QApplication::desktop()->height() <= 1600*900)
    {
        this->setFixedSize(QApplication::desktop()->availableGeometry().width()-265,
                           QApplication::desktop()->availableGeometry().height()-87);
    }
    else
    {
        this->setFixedSize(QApplication::desktop()->availableGeometry().width()-286,
                       QApplication::desktop()->availableGeometry().height()-105);
    }

    mainLayout=new QHBoxLayout(this);
    if(QApplication::desktop()->width()*QApplication::desktop()->height() <= 1600*900)
        mainLayout->setContentsMargins(263,0,0,0);
    else
        mainLayout->setContentsMargins(287,0,0,0);
    listview=new FullListView(this,3);
    mainLayout->addWidget(listview);
    this->setLayout(mainLayout);

    data.clear();
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
    listview->updateData(data);
}
