#include "functionwidget.h"
#include "ui_functionwidget.h"
#include "src/color.h"
#include <QDebug>

FunctionWidget::FunctionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FunctionWidget)
{
    ui->setupUi(this);

    init_widget();
}

FunctionWidget::~FunctionWidget()
{
    delete ui;
    delete menu;
    delete pUkuiMenuInterface;
}

/**
 * 主界面初始化
 */
void FunctionWidget::init_widget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setStyleSheet("border:0px;background:transparent;");
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setFixedSize(320,500);

    mainLayout=new QHBoxLayout(this);
    mainLayout->setContentsMargins(2,0,2,0);
    mainLayout->setSpacing(0);
    this->setLayout(mainLayout);
    pUkuiMenuInterface=new UkuiMenuInterface;

    init_applist_widget();

}

/**
 * 初始化应用列表界面
 */
void FunctionWidget::init_applist_widget()
{
    applistview=new ListView(this,this->width()-4,this->height(),2);
    mainLayout->addWidget(applistview);
    fill_app_listview();
    connect(applistview,SIGNAL(sendItemClickedSignal(QStringList)),this,SLOT(recvItemClickedSlot(QStringList)));
    connect(applistview,SIGNAL(sendFixedOrUnfixedSignal()),this,SIGNAL(send_update_applist_signal()));

}


/**
 * 填充应用列表
 */
void FunctionWidget::fill_app_listview()
{
    classificationbtnlist.clear();
    classificationbtnrowlist.clear();

    QVector<QStringList> vector=pUkuiMenuInterface->get_functional_classification();

    QStringList recentlist=vector.at(0);
    if(!recentlist.isEmpty())
    {
        insert_classification_btn("最近");
        insert_app_list(recentlist);
    }

    QStringList netlist=vector.at(1);
    if(!netlist.isEmpty())
    {
        insert_classification_btn("网络");
        insert_app_list(netlist);
    }
    QStringList sociallist=vector.at(2);
    if(!sociallist.isEmpty())
    {
        insert_classification_btn("社交");
        insert_app_list(sociallist);
    }
    QStringList avlist=vector.at(3);
    if(!avlist.isEmpty())
    {
        insert_classification_btn("影音");
        insert_app_list(avlist);
    }
    QStringList developlist=vector.at(4);
    if(!developlist.isEmpty())
    {
        insert_classification_btn("开发");
        insert_app_list(developlist);
    }
    QStringList graphicslist=vector.at(5);
    if(!graphicslist.isEmpty())
    {
        insert_classification_btn("图像");
        insert_app_list(graphicslist);
    }
    QStringList gamelist=vector.at(6);
    if(!gamelist.isEmpty())
    {
        insert_classification_btn("游戏");
        insert_app_list(gamelist);
    }
    QStringList officelist=vector.at(7);
    if(!officelist.isEmpty())
    {
        insert_classification_btn("办公");
        insert_app_list(officelist);
    }
    QStringList educationlist=vector.at(8);
    if(!educationlist.isEmpty())
    {
        insert_classification_btn("教育");

        insert_app_list(educationlist);
    }

    QStringList systemadminlist=vector.at(9);
    if(!systemadminlist.isEmpty())
    {
        insert_classification_btn("系统");
        insert_app_list(systemadminlist);
    }
    QStringList otherlist=vector.at(10);
    if(!otherlist.isEmpty())
    {
        insert_classification_btn("其它");
        insert_app_list(otherlist);
    }

    m_delegate= new ItemDelegate(this,2);
    applistview->setItemDelegate(m_delegate);
    applistview->addData(data);
}

void FunctionWidget::insert_classification_btn(QString btnname)
{
    classificationbtnlist.append(btnname);
    data.append(QStringList()<<btnname<<"0");
    classificationbtnrowlist.append(QString::number(row));
}

void FunctionWidget::insert_app_list(QStringList appnamelist)
{
    row+=(appnamelist.count()+1);
    for(int i=0;i<appnamelist.count();i++)
    {

        QString desktopfp=pUkuiMenuInterface->get_desktop_path_by_app_name(appnamelist.at(i));
        data.append(QStringList()<<desktopfp<<"1");
    }
}

void FunctionWidget::recvItemClickedSlot(QStringList arg)
{
    if(arg.at(1).toInt()==0)
    {
        app_classificationbtn_clicked_slot();
    }
    else{
        QString exec=pUkuiMenuInterface->get_app_exec(arg.at(0));
        exec_app_name(exec);
    }

}

/**
 * 执行应用程序
 */
void FunctionWidget::exec_app_name(QString exec)
{
    emit send_hide_mainwindow_signal();
    //移除启动参数%u或者%U
    for(int i=0;i<exec.length();i++)
    {
        if(exec.at(i)=='%')
        {
            exec.remove(i,2);
        }
    }
    QProcess *process=new QProcess(this);
    process->startDetached(exec);
}

/**
 * 更新应用列表
 */
void FunctionWidget::update_app_listview()
{
    classificationbtnlist.clear();
    classificationbtnrowlist.clear();
    row=0;
    data.clear();

    QVector<QStringList> vector=pUkuiMenuInterface->get_functional_classification();

    QStringList recentlist=vector.at(0);
    if(!recentlist.isEmpty())
    {
        insert_classification_btn("最近");
        insert_app_list(recentlist);
    }

    QStringList netlist=vector.at(1);
    if(!netlist.isEmpty())
    {
        insert_classification_btn("网络");
        insert_app_list(netlist);
    }
    QStringList sociallist=vector.at(2);
    if(!sociallist.isEmpty())
    {
        insert_classification_btn("社交");
        insert_app_list(sociallist);
    }
    QStringList avlist=vector.at(3);
    if(!avlist.isEmpty())
    {
        insert_classification_btn("影音");
        insert_app_list(avlist);
    }
    QStringList developlist=vector.at(4);
    if(!developlist.isEmpty())
    {
        insert_classification_btn("开发");
        insert_app_list(developlist);
    }
    QStringList graphicslist=vector.at(5);
    if(!graphicslist.isEmpty())
    {
        insert_classification_btn("图像");
        insert_app_list(graphicslist);
    }
    QStringList gamelist=vector.at(6);
    if(!gamelist.isEmpty())
    {
        insert_classification_btn("游戏");
        insert_app_list(gamelist);
    }
    QStringList officelist=vector.at(7);
    if(!officelist.isEmpty())
    {
        insert_classification_btn("办公");
        insert_app_list(officelist);
    }
    QStringList educationlist=vector.at(8);
    if(!educationlist.isEmpty())
    {
        insert_classification_btn("教育");

        insert_app_list(educationlist);
    }

    QStringList systemadminlist=vector.at(9);
    if(!systemadminlist.isEmpty())
    {
        insert_classification_btn("系统");
        insert_app_list(systemadminlist);
    }
    QStringList otherlist=vector.at(10);
    if(!otherlist.isEmpty())
    {
        insert_classification_btn("其它");
        insert_app_list(otherlist);
    }
    applistview->updateData(data);
}

/**
 * 应用列表功能分类按钮槽函数
 */
void FunctionWidget::app_classificationbtn_clicked_slot()
{
    //加载FunctionButtonWidget界面
    functionbtnwid=new FunctionButtonWidget(this);
    connect(this,SIGNAL(send_classificationbtn_list(QStringList)),functionbtnwid,SLOT(recv_classificationbtn_list(QStringList)));
    emit send_classificationbtn_list(classificationbtnlist);
    mainLayout->removeWidget(applistview);
    applistview->setParent(nullptr);
    mainLayout->addWidget(functionbtnwid);

    connect(functionbtnwid, SIGNAL(send_functionbtn_signal(QString)),this,SLOT(recv_functionbtn_signal(QString)));
}

/**
 * 接收FunctionButtonWidget界面按钮信号
 */
void FunctionWidget::recv_functionbtn_signal(QString btnname)
{
    mainLayout->removeWidget(functionbtnwid);
    functionbtnwid->setParent(nullptr);
    if(functionbtnwid!=nullptr)
    {

        delete functionbtnwid;
        functionbtnwid=nullptr;
    }
    mainLayout->addWidget(applistview);

    //此处需实现将功能为btnname的应用列表移动到applistWid界面最顶端
    int index=classificationbtnlist.indexOf(btnname);
    if(index!=-1)
    {
        int row=classificationbtnrowlist.at(index).toInt();
        applistview->verticalScrollBar()->setValue(row);
    }
}

void FunctionWidget::widget_make_zero()
{
    if(functionbtnwid!=nullptr)
    {
        mainLayout->removeWidget(functionbtnwid);
        functionbtnwid->setParent(nullptr);
        delete functionbtnwid;
        functionbtnwid=nullptr;
        mainLayout->addWidget(applistview);
    }
    applistview->verticalScrollBar()->setValue(0);
}
