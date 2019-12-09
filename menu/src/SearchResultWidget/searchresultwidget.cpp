#include "searchresultwidget.h"
#include "ui_searchresultwidget.h"
#include "src/color.h"
#include <QDebug>
#include <QHeaderView>

SearchResultWidget::SearchResultWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchResultWidget)
{
    ui->setupUi(this);
    init_widget();

//    QStringList deskfpList=KylinStartMenuInterface::get_desktop_file_path();
//    QString deskfp;
//    foreach (deskfp, deskfpList) {
//        QString appname=KylinStartMenuInterface::get_app_english_name(deskfp);
//        appnameList.append(appname);
//    }

//    searchappthread=new SearchAppThread;
//    connect(this,SIGNAL(send_search_keyword(QString)),
//            searchappthread,SLOT(recv_search_keyword(QString)));
//    connect(searchappthread,SIGNAL(send_search_result(QStringList)),
//            this,SLOT(recv_search_result(QStringList)));
//    searchappthread->start();
}

SearchResultWidget::~SearchResultWidget()
{
    delete ui;
    delete pUkuiMenuInterface;
}

void SearchResultWidget::init_widget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    char widgetcolor[100];
    sprintf(widgetcolor, "border:0px;background-color:%s;",MAINVIEWWIDGETCOLOR);
    this->setStyleSheet(QString::fromLocal8Bit(widgetcolor));
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setFixedSize(330,462);

    mainLayout=new QHBoxLayout(this);
    mainLayout->setContentsMargins(2,0,2,0);
    listview=new ListView(this,this->width()-4,this->height(),3);
    mainLayout->addWidget(listview);
    this->setLayout(mainLayout);
    data.clear();
    itemdelegate=new ItemDelegate(this,3);
    listview->setItemDelegate(itemdelegate);
    listview->addData(data);
    pUkuiMenuInterface=new UkuiMenuInterface;

    connect(listview,SIGNAL(sendItemClickedSignal(QStringList)),this,SLOT(exec_app_name(QStringList)));
    connect(listview,SIGNAL(sendFixedOrUnfixedSignal()),this,SIGNAL(send_update_applist_signal()));

}

/**
 * 执行应用程序
 */
void SearchResultWidget::exec_app_name(QStringList arg)
{
    emit send_hide_mainwindow_signal();
    QString execpath=pUkuiMenuInterface->get_app_exec(arg.at(0));
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

void SearchResultWidget::update_app_listview(QStringList desktopfplist)
{
    data.clear();
    for(int i=0;i<desktopfplist.count();i++)
    {
        data.append(QStringList()<<desktopfplist.at(i)<<"1");
    }
    listview->updateData(data);
//    itemdelegate=new FullItemDelegate(this,3);
//    listview->setItemDelegate(itemdelegate);
//    listview->addData(data);
}
