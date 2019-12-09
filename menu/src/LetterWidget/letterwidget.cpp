#include "letterwidget.h"
#include "ui_letterwidget.h"
#include "src/color.h"
#include <QDebug>
#include <QHeaderView>
#include <QScrollBar>
#include <QPushButton>
#include <QTableWidgetItem>
#include <QFrame>
#include <QSlider>
#include "src/UtilityFunction/itemdelegate.h"

LetterWidget::LetterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LetterWidget)
{
    ui->setupUi(this);

    init_widget();
}

LetterWidget::~LetterWidget()
{
    delete ui;
    delete pUkuiMenuInterface;
}

/**
 * 主界面初始化
 */
void LetterWidget::init_widget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    char widgetcolor[100];
    sprintf(widgetcolor, "border:0px;background-color:%s;",MAINVIEWWIDGETCOLOR);
    this->setStyleSheet(QString::fromLocal8Bit(widgetcolor));
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setFixedSize(330,462);

    mainLayout=new  QHBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    applistWid=new QWidget(this);
    applistWid->setStyleSheet("border:0px;background: transparent;");
    applistWid->setFixedSize(this->width(),this->height());
    mainLayout->addWidget(applistWid);
    this->setLayout(mainLayout);

    pUkuiMenuInterface=new UkuiMenuInterface;

    init_applist_widget();

}

/**
 * 初始化应用列表界面
 */
void LetterWidget::init_applist_widget()
{
    QHBoxLayout* layout=new QHBoxLayout();
    layout->setContentsMargins(2,0,2,0);
    applistWid->setLayout(layout);
    applistview=new ListView(this,applistWid->width()-4,applistWid->height(),1);
    layout->addWidget(applistview);
    fill_app_listview();
    connect(applistview,SIGNAL(sendItemClickedSignal(QStringList)),this,SLOT(recvItemClickedSlot(QStringList)));
    connect(applistview,SIGNAL(sendFixedOrUnfixedSignal()),this,SIGNAL(send_update_applist_signal()));

}


/**
 * 填充应用列表
 */
void LetterWidget::fill_app_listview()
{
    int row=0;
    QVector<QStringList> vector=pUkuiMenuInterface->get_alphabetic_classification();
    for(int i=0;i<vector.size();i++)
    {
        QStringList appList=vector.at(i);
        if(!appList.isEmpty())
        {
            QString letterstr;
            if(i<26)
                letterstr=QString(QChar(static_cast<char>(i+65)));
            else if(i==26)
                letterstr="&";
            else
                letterstr="#";
            letterbtnlist.append(letterstr);//存储分类字符
            letterbtnrowlist.append(QString::number(row));//存储分类字符所在行
            data.append(QStringList()<<letterstr<<"0");
            for(int i=0;i<appList.count();i++)
            {
                QString desktopfp=pUkuiMenuInterface->get_desktop_path_by_app_name(appList.at(i));
                data.append(QStringList()<<desktopfp<<"1");
            }
            row+=(appList.count()+1);

        }
    }

//    letterposlist.clear();
//    appsortlist.clear();
//    data.clear();
//    letterposlist=KylinStartMenuInterface::get_app_diff_first_letter_pos();
//    appsortlist=KylinStartMenuInterface::sort_app_name();
//    int row=0;
//    for(int i=0;i<letterposlist.count();i++)
//    {
//        QString startstr=letterposlist.at(i);
//        int start=startstr.toInt();
//        int end;
//        if(i==letterposlist.count()-1)
//            end=appsortlist.count()-1;
//        else{
//            QString endstr=letterposlist.at(i+1);
//            end=endstr.toInt();
//        }

//        QStringList applist=get_alphabetic_classification_applist(start,end);
//        if(!applist.isEmpty())
//        {
//            QString lettersrt=KylinStartMenuInterface::get_app_name_pinyin(applist.at(0));
//            QChar letter=lettersrt.at(0);
//            if(letter<48 || (letter>57 && letter<65) || letter>90)
//                letter='&';
//            else if(letter>=48 && letter<=57)
//                letter='#';
//            else letter=lettersrt.at(0);
//            data.append(QStringList()<<QString(letter)<<"0");
//            letterbtnlist.append(QString(letter));//存储分类字符
//            letterbtnrowlist.append(QString::number(row));//存储分类字符所在行
//            for(int i=0;i<applist.count();i++)
//            {
//                QString desktopfp=KylinStartMenuInterface::get_desktop_path_by_app_name(applist.at(i));
//                data.append(QStringList()<<desktopfp<<"1");
//            }
//            row+=(applist.count()+1);

//        }
//    }
    m_delegate= new ItemDelegate(this,1);
    applistview->setItemDelegate(m_delegate);
    applistview->addData(data);
}

void LetterWidget::recvItemClickedSlot(QStringList arg)
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
void LetterWidget::exec_app_name(QString exec)
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
void LetterWidget::update_app_listview(QString desktopfn, QString appname, int arg)
{
    Q_UNUSED(desktopfn);
    Q_UNUSED(appname);
    Q_UNUSED(arg);
    int row=0;
    data.clear();
    letterbtnlist.clear();
    letterbtnrowlist.clear();
    QVector<QStringList> vector=pUkuiMenuInterface->get_alphabetic_classification();
    for(int i=0;i<vector.size();i++)
    {
        QStringList appList=vector.at(i);
        if(!appList.isEmpty())
        {
            QString letterstr;
            if(i<26)
                letterstr=QString(QChar(static_cast<char>(i+65)));
            else if(i==26)
                letterstr="&";
            else
                letterstr="#";
            letterbtnlist.append(letterstr);//存储分类字符
            letterbtnrowlist.append(QString::number(row));//存储分类字符所在行
            data.append(QStringList()<<letterstr<<"0");
            for(int i=0;i<appList.count();i++)
            {
                QString desktopfp=pUkuiMenuInterface->get_desktop_path_by_app_name(appList.at(i));
                data.append(QStringList()<<desktopfp<<"1");
            }
            row+=(appList.count()+1);

        }
    }
    applistview->updateData(data);
}

/**
 * 应用列表字母分类按钮槽函数
 */
void LetterWidget::app_classificationbtn_clicked_slot()
{
    //加载LetterBUttonWidget界面
    letterbtnwid=new LetterButtonWidget(this);
    connect(this,SIGNAL(send_letterbtn_list(QStringList)),letterbtnwid,SLOT(recv_letterbtn_list(QStringList)));
    emit send_letterbtn_list(letterbtnlist);
    mainLayout->removeWidget(applistWid);
    applistWid->setParent(nullptr);
    mainLayout->addWidget(letterbtnwid);
    emit send_letterwid_state(1,"");

    connect(letterbtnwid, SIGNAL(send_letterbtn_signal(QString)),this,SLOT(recv_letterbtn_signal(QString)));
}

/**
 * 接收LetterButtonWidget界面按钮信号
 */
void LetterWidget::recv_letterbtn_signal(QString btnname)
{
    mainLayout->removeWidget(letterbtnwid);
    letterbtnwid->setParent(nullptr);
    if(letterbtnwid!=nullptr)
    {
        delete letterbtnwid;
        letterbtnwid=nullptr;
    }
    mainLayout->addWidget(applistWid);

    //此处需实现将字母为btnname的应用列表移动到applistWid界面最顶端
    int num=letterbtnlist.indexOf(QString(QChar(btnname.at(0))));
    if(num!=-1)
    {
        int row=letterbtnrowlist.at(num).toInt();
        applistview->verticalScrollBar()->setValue(row);

    }
    emit send_letterwid_state(0,btnname);
}

/**
 * 设置字母分类界面状态
 */
void LetterWidget::set_letterwidge_state(int state, QString btnname)
{
    if(state==1)
    {
        QLayoutItem *child;
        if((child = mainLayout->itemAt(0)) != nullptr) {
            QWidget* childwid=child->widget();
            if(childwid!=nullptr)
            {
                mainLayout->removeWidget(childwid);
                childwid->setParent(nullptr);
            }
        }
        if(letterbtnwid!=nullptr)
        {
            delete letterbtnwid;
            letterbtnwid=nullptr;
        }
        letterbtnwid=new LetterButtonWidget(this);
        connect(letterbtnwid, SIGNAL(send_letterbtn_signal(QString)),this,SLOT(recv_letterbtn_signal(QString)));
        connect(this,SIGNAL(send_letterbtn_list(QStringList)),letterbtnwid,SLOT(recv_letterbtn_list(QStringList)));
        emit send_letterbtn_list(letterbtnlist);
        mainLayout->addWidget(letterbtnwid);
    }
    else{
        QLayoutItem *child;
        if((child = mainLayout->itemAt(0)) != nullptr) {
            QWidget* childwid=child->widget();
            if(childwid!=nullptr)
            {
                mainLayout->removeWidget(childwid);
                childwid->setParent(nullptr);
            }
        }
        if(letterbtnwid!=nullptr)
        {
            delete letterbtnwid;
            letterbtnwid=nullptr;
        }
        mainLayout->addWidget(applistWid);

        //此处需实现将字母为btnname的应用列表移动到applistWid界面最顶端
        int num=letterbtnlist.indexOf(QString(QChar(btnname.at(0))));
        if(num!=-1)
        {
            int row=letterbtnrowlist.at(num).toInt();
            applistview->verticalScrollBar()->setValue(row);
        }
    }

}
