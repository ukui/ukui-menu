#include "fullletterwidget.h"
#include "ui_fullletterwidget.h"
#include <QDebug>

FullLetterWidget::FullLetterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FullLetterWidget)
{
    ui->setupUi(this);

    init_widget();
}

FullLetterWidget::~FullLetterWidget()
{
    delete ui;
    delete pUkuiMenuInterface;
}

/**
 * 主界面初始化
 */
void FullLetterWidget::init_widget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    char widgetcolor[100];
    sprintf(widgetcolor, "border:0px;background-color:%s;",MAINVIEWWIDGETCOLOR);
    this->setStyleSheet(QString::fromLocal8Bit(widgetcolor));
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setFixedSize(QApplication::desktop()->availableGeometry().width()-160,
                       QApplication::desktop()->availableGeometry().height()-70);

    mainLayout=new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    applistWid=new QWidget(this);
    applistWid->setStyleSheet("border:0px;background: transparent;");
    applistWid->setFixedSize(this->width(),this->height()-70);
    letterlistWid=new QWidget(this);
    letterlistWid->setStyleSheet("border:0px;background: transparent;");
    letterlistWid->setFixedSize(this->width(),70);
    mainLayout->addWidget(applistWid);
    mainLayout->addWidget(letterlistWid);
    this->setLayout(mainLayout);
    pUkuiMenuInterface=new UkuiMenuInterface;

    init_applist_widget();
    init_letterlist_widget();
}

/**
 * 初始化应用列表界面
 */
void FullLetterWidget::init_applist_widget()
{
    QHBoxLayout* layout=new QHBoxLayout(applistWid);
    layout->setContentsMargins(30,0,2,0);
    applistWid->setLayout(layout);

    scrollarea=new ScrollArea;
    scrollareawid=new QWidget;
    scrollarea->setWidget(scrollareawid);
    scrollarea->setFixedSize(applistWid->width()-32-12,applistWid->height());
    scrollarea->setWidgetResizable(true);
    scrollareawidLayout=new QVBoxLayout;
    scrollareawidLayout->setContentsMargins(0,0,0,0);
    scrollareawidLayout->setSpacing(0);
    scrollareawid->setLayout(scrollareawidLayout);
    layout->addWidget(scrollarea);

    fill_app_list();

}

/**
 * 填充应用列表
 */
void FullLetterWidget::fill_app_list()
{
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
            //插入字母分类按钮
            PushButton* letterbtn=new PushButton(letterstr,0,1);
            letterbtn->setFixedSize(scrollarea->width()-32,20);
            scrollareawidLayout->addWidget(letterbtn);
            connect(letterbtn, SIGNAL(clicked()), this,SLOT(app_classificationbtn_clicked_slot()));

            //插入应用列表
            FullListView* listview=new FullListView(this,applistWid->width(),100,1);
            scrollareawidLayout->addWidget(listview);
            data.clear();
            for(int i=0;i<appList.count();i++)
            {
                QString desktopfp=pUkuiMenuInterface->get_desktop_path_by_app_name(appList.at(i));
                data.append(desktopfp);

            }
            FullItemDelegate* itemdelegate=new FullItemDelegate(this,1);
            listview->setItemDelegate(itemdelegate);
            listview->addData(data);

            connect(listview,SIGNAL(sendItemClickedSignal(QString)),this,SLOT(exec_app_name(QString)));
            connect(listview,SIGNAL(sendFixedOrUnfixedSignal()),this,SIGNAL(send_update_applist_signal()));

        }
    }

        resize_scrollarea_controls();
}

/**
 * 执行应用程序
 */
void FullLetterWidget::exec_app_name(QString appname)
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

/**
 * 更新应用列表
 */
void FullLetterWidget::update_app_listview(QString desktopfn, QString appname, int arg)
{
    if(arg==0)//有新软件安装
    {
        qDebug()<<"---有新软件安装---";
//        QString appname=pUkuiMenuInterface->get_app_name(QString("/usr/share/applications/"+desktopfn));
        QString lettersrt=pUkuiMenuInterface->get_app_name_pinyin(appname);
        QChar letter=lettersrt.at(0);
        if(letter<48 || (letter>57 && letter<65) || letter>90)
        {
            letter='&';
        }
        else if(letter>=48 && letter<=57)
        {
            letter='#';
        }
        else letter=lettersrt.at(0);
        if(letterbtnlist.indexOf(QString(letter))==-1)//该软件没有对应分类
        {
            int index=0;
            for(index=0;index<letterbtnlist.count();index++)
            {
                QString indexstr=letterbtnlist.at(index);
                QChar indexch=indexstr.at(0);
                if(letter=='&')
                {
                    if(indexch>=65 && indexch<=90)
                        continue;
                    else
                        break;
                }
                else if(letter=='#')
                    break;
                else{
                    if(indexch=='&' || indexch=='#')
                        break;
                    else{
                        if(indexch>letter)
                            break;
                    }

                }
            }

            int pos=0;
            if(letter!='#')
            {
                if(index==letterbtnlist.count())
                {
                    pos=scrollareawidLayout->count();

                }
                else{
                    pos=index*2;
                }
            }
            else{
                index=letterbtnlist.count();
                pos=scrollareawidLayout->count();
            }
            letterbtnlist.insert(index,QString(letter));
            insert_letter_btn(QString(letter),pos);
            insert_app_listview(desktopfn,pos+1);

        }
        else{//该软件有对应分类
            QStringList desktopfplist;
            QStringList appsortlist=pUkuiMenuInterface->sort_app_name();
            for(int i=0;i<appsortlist.count();i++)
            {
                QString indexstr=pUkuiMenuInterface->get_app_name_pinyin(appsortlist.at(i));
                QChar indexch=indexstr.at(0);
                if(letter=='&')
                {
                    if(indexch<48 || (indexch>57 && indexch<65) || indexch>90)
                    {

                        desktopfplist.append(pUkuiMenuInterface->get_desktop_path_by_app_name(appsortlist.at(i)));
                    }
                    if(indexch>=48 && indexch<=57)
                        break;
                }
                else if(letter=='#')
                {
                    if(indexch>=48 && indexch<=57)
                        desktopfplist.append(pUkuiMenuInterface->get_desktop_path_by_app_name(appsortlist.at(i)));
                }
                else{
                    if(indexch==letter)
                        desktopfplist.append(pUkuiMenuInterface->get_desktop_path_by_app_name(appsortlist.at(i)));
                    if(indexch>letter || indexch<65 || indexch>90)
                        break;
                }
            }
            int pos=letterbtnlist.indexOf(QString(letter));
            QLayoutItem* item=scrollareawidLayout->itemAt(2*pos+1);
            QWidget* wid=item->widget();
            FullListView* listview=qobject_cast<FullListView*>(wid);
            listview->updateData(desktopfplist);
        }
    }
    else{//有软件被卸载
        qDebug()<<"---有软件被卸载---";
        QString letterstr=pUkuiMenuInterface->get_app_name_pinyin(appname);
        QChar letter=letterstr.at(0);
        if(letter<48 || (letter>57 && letter<65) || letter>90)
            letter='&';
        else if(letter>=48 && letter<=57)
            letter='#';
        else letter=letterstr.at(0);
        int pos=letterbtnlist.indexOf(QString(letter));
        QLayoutItem* item=scrollareawidLayout->itemAt(2*pos+1);
        QWidget* wid=item->widget();
        FullListView* listview=qobject_cast<FullListView*>(wid);
        listview->removeRow("/usr/share/applications/"+desktopfn);
        if(listview->model()->rowCount()==0)//该分类只有一个应用，卸载后，listview列表个数为零
        {
            for(int i=1;i>=0;i--)//删除listview和对应的分类按钮
            {
                QLayoutItem* item=scrollareawidLayout->itemAt(2*pos+i);
                QWidget* wid=item->widget();
                scrollareawidLayout->removeWidget(wid);
                wid->setParent(nullptr);
                delete wid;
            }

            letterbtnlist.removeAll(letter);
        }
    }

    resize_scrollarea_controls();
}

void FullLetterWidget::insert_letter_btn(QString letter, int pos)
{

    PushButton* letterbtn=new PushButton(QString(letter),0,1);
    scrollareawidLayout->insertWidget(pos,letterbtn);
    connect(letterbtn, SIGNAL(clicked()), this,SLOT(app_classificationbtn_clicked_slot()));
}

void FullLetterWidget::insert_app_listview(QString desktopfn, int pos)
{
    FullListView* listview=new FullListView(this,applistWid->width(),100,1);
    data.clear();
    data.append("/usr/share/applications/"+desktopfn);
    FullItemDelegate* itemdelegate=new FullItemDelegate(this,1);
    listview->setItemDelegate(itemdelegate);
    listview->addData(data);
    listview->adjustSize();
    scrollareawidLayout->insertWidget(pos,listview);
    connect(listview,SIGNAL(sendItemClickedSignal(QString)),this,SLOT(exec_app_name(QString)));
    connect(listview,SIGNAL(sendFixedOrUnfixedSignal()),this,SIGNAL(send_update_applist_signal()));
}

/**
 * 设置scrollarea所填充控件大小
 */
void FullLetterWidget::resize_scrollarea_controls()
{
    int pos=0;
    letterbtnrowlist.append(QString::number(pos));
    int row=0;
    while(row<scrollareawidLayout->count()/2)
    {
        //分类按钮
        QLayoutItem* letterbtnwidItem=scrollareawidLayout->itemAt(row*2);
        QWidget* letterbtnwid=letterbtnwidItem->widget();
        QPushButton* letterbtn=qobject_cast<QPushButton*>(letterbtnwid);
        letterbtn->setFixedSize(scrollarea->width()-32,20);
        //应用界面
        QLayoutItem* widItem=scrollareawidLayout->itemAt(row*2+1);
        QWidget* wid=widItem->widget();
        FullListView* listview=qobject_cast<FullListView*>(wid);
        listview->adjustSize();
        int dividend=scrollarea->width()/120;
        int rowcount=0;
        if(listview->model()->rowCount()%dividend>0)
        {
            rowcount=listview->model()->rowCount()/dividend+1;
        }
        else
        {
            rowcount=listview->model()->rowCount()/dividend;

        }
        QModelIndex index=listview->model()->index(rowcount-1,0);
        QRect rect=listview->visualRect(index);
        listview->setFixedSize(scrollarea->width()-32,rect.y()+rect.height()+20);
        if(row<scrollareawidLayout->count()/2-1)
        {
            pos+=(20+listview->height());
            letterbtnrowlist.append(QString::number(pos));
        }
        row++;
    }
    scrollarea->widget()->adjustSize();
}

/**
 * 应用列表字母分类按钮槽函数
 */
void FullLetterWidget::app_classificationbtn_clicked_slot()
{
    //加载FullLetterButtonWidget界面
    fullletterbtnwid=new FullLetterButtonWidget(this);
    connect(this,SIGNAL(send_letterbtn_list(QStringList)),fullletterbtnwid,SLOT(recv_letterbtn_list(QStringList)));
    emit send_letterbtn_list(letterbtnlist);
    mainLayout->removeWidget(applistWid);
    applistWid->setParent(nullptr);
    mainLayout->removeWidget(letterlistWid);
    letterlistWid->setParent(nullptr);
    mainLayout->addWidget(fullletterbtnwid);
    emit send_fullletterwid_state(1,"");

    connect(fullletterbtnwid, SIGNAL(send_letterbtn_signal(QString)),this,SLOT(recv_letterbtn_signal(QString)));
}

/**
 * 接收FullLetterButtonWidget界面按钮信号
 */
void FullLetterWidget::recv_letterbtn_signal(QString btnname)
{
    mainLayout->removeWidget(fullletterbtnwid);
    fullletterbtnwid->setParent(nullptr);
    if(fullletterbtnwid!=nullptr)
    {
        delete fullletterbtnwid;
        fullletterbtnwid=nullptr;
    }
    mainLayout->addWidget(applistWid);
    mainLayout->addWidget(letterlistWid);

    int col=0;
    if(QString::compare(btnname,"&&")==0)
        col=26;
    else if(QString::compare(btnname,"#")==0)
        col=27;
    else{
        QByteArray btnnamearr=btnname.toLocal8Bit();
        char* bn=btnnamearr.data();
        col=static_cast<int>(*bn)-65;
    }
    QLayoutItem* item=letterlistscrollarea->widget()->layout()->itemAt(col);
    QWidget* wid=item->widget();
    QToolButton* btn=qobject_cast<QToolButton*>(wid);
    btn->click();

    emit send_fullletterwid_state(0,btnname);
}

/**
 * 设置全屏字母分类界面状态
 */
void FullLetterWidget::set_fullletterwidge_state(int state, QString btnname)
{
    if(state==1)
    {
        QLayoutItem *firstchild;
        if((firstchild = mainLayout->itemAt(1)) != nullptr) {
            QWidget* childwid=firstchild->widget();
            if(childwid!=nullptr)
            {
                mainLayout->removeWidget(childwid);
                childwid->setParent(nullptr);
            }
        }
        QLayoutItem *secondchild;
        if((secondchild = mainLayout->itemAt(0)) != nullptr) {
            QWidget* childwid=secondchild->widget();
            if(childwid!=nullptr)
            {
                mainLayout->removeWidget(childwid);
                childwid->setParent(nullptr);
            }
        }

        if(fullletterbtnwid!=nullptr)
        {
            delete fullletterbtnwid;
            fullletterbtnwid=nullptr;
        }
        fullletterbtnwid=new FullLetterButtonWidget(this);
        connect(this,SIGNAL(send_letterbtn_list(QStringList)),fullletterbtnwid,SLOT(recv_letterbtn_list(QStringList)));
        emit send_letterbtn_list(letterbtnlist);
        connect(fullletterbtnwid, SIGNAL(send_letterbtn_signal(QString)),this,SLOT(recv_letterbtn_signal(QString)));
        mainLayout->addWidget(fullletterbtnwid);

    }
    else{
        QLayoutItem *firstchild;
        if((firstchild = mainLayout->itemAt(1)) != nullptr) {
            QWidget* childwid=firstchild->widget();
            if(childwid!=nullptr)
            {
                mainLayout->removeWidget(childwid);
                childwid->setParent(nullptr);
            }
        }
        QLayoutItem *secondchild;
        if((secondchild = mainLayout->itemAt(0)) != nullptr) {
            QWidget* childwid=secondchild->widget();
            if(childwid!=nullptr)
            {
                mainLayout->removeWidget(childwid);
                childwid->setParent(nullptr);
            }
        }

        if(fullletterbtnwid!=nullptr)
        {
            delete fullletterbtnwid;
            fullletterbtnwid=nullptr;
        }
        mainLayout->addWidget(applistWid);
        mainLayout->addWidget(letterlistWid);

        int col=0;
        if(QString::compare(btnname,"&&")==0)
            col=26;
        else if(QString::compare(btnname,"#")==0)
            col=27;
        else{
            QByteArray btnnamearr=btnname.toLocal8Bit();
            char* bn=btnnamearr.data();
            col=static_cast<int>(*bn)-65;
        }
        QLayoutItem* item=letterlistscrollarea->widget()->layout()->itemAt(col);
        QWidget* wid=item->widget();
        QToolButton* btn=qobject_cast<QToolButton*>(wid);
        btn->click();

    }
}

/**
 * 初始化字母列表界面
 */
void FullLetterWidget::init_letterlist_widget()
{
    letterlistleftSpacer=new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);
    letterlistrightSpacer=new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);

    letterlistLayout=new QHBoxLayout(letterlistWid);
    letterlistLayout->setContentsMargins(0,0,0,0);
    letterlistLayout->setSpacing(10);
    letterlistWid->setLayout(letterlistLayout);

    leftbtn=new ToolButton(40,30,":/data/img/mainviewwidget/leftarrow.svg",":/data/img/mainviewwidget/leftarrow-hover.svg",
                           MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED,1);
    letterlistscrollarea=new ClassifyScrollArea();
    letterlistscrollarea->setFixedSize(28*30+27*10,25);
    letterlistscrollareaWid=new QWidget;
    letterlistscrollareawidLayout=new QHBoxLayout;
    letterlistscrollareawidLayout->setContentsMargins(0,0,0,0);
    letterlistscrollareawidLayout->setSpacing(10);
    letterlistscrollareaWid->setLayout(letterlistscrollareawidLayout);
    letterlistscrollarea->setWidget(letterlistscrollareaWid);
    rightbtn=new ToolButton(40,30,":/data/img/mainviewwidget/rightarrow.svg",":/data/img/mainviewwidget/rightarrow-hover.svg",
                            MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED,1);

    letterlistLayout->addItem(letterlistleftSpacer);
    letterlistLayout->addWidget(leftbtn);
    letterlistLayout->addWidget(letterlistscrollarea);
    letterlistLayout->addWidget(rightbtn);
    letterlistLayout->addItem(letterlistrightSpacer);
    init_letterlist_scrollarea();

    QLayoutItem* item=letterlistscrollarea->widget()->layout()->itemAt(0);
    QWidget* wid=item->widget();
    QToolButton* btn=qobject_cast<QToolButton*>(wid);
    btn->setChecked(true);

    connect(leftbtn, SIGNAL(clicked()), this, SLOT(leftbtn_clicked_slot()));
    connect(rightbtn, SIGNAL(clicked()), this, SLOT(rightbtn_clicked_slot()));

}

/**
 * 初始化字母列表
 */
void FullLetterWidget::init_letterlist_scrollarea()
{
    for(int i=0;i<28;i++)
    {
        QString letterstr;
        if(i==26)
            letterstr="&&";
        else if (i==27) {
            letterstr='#';
        }
        else {
            letterstr=QString(QChar(static_cast<char>(i+65)));
        }
        ClassifyButton* letterbtn=new ClassifyButton(letterstr,0,"");
        letterlistscrollareawidLayout->addWidget(letterbtn);
        connect(letterbtn,SIGNAL(clicked()),this,SLOT(letterbtn_clicked_slot()));

    }
    letterlistscrollarea->widget()->adjustSize();
}

/**
 * 向左按钮槽函数
 */
void FullLetterWidget::leftbtn_clicked_slot()
{
    if(btnPos>0)
    {
        QLayoutItem* afteritem=letterlistscrollarea->widget()->layout()->itemAt(btnPos--);
        QWidget* afterwid=afteritem->widget();
        QToolButton* afterbtn=qobject_cast<QToolButton*>(afterwid);
        afterbtn->setChecked(false);
        QLayoutItem* item=letterlistscrollarea->widget()->layout()->itemAt(btnPos);
        QWidget* wid=item->widget();
        QToolButton* btn=qobject_cast<QToolButton*>(wid);
        btn->click();
//        btn->checkStateSet();
//        int pos=btnPosList.at(btnPos).toInt();
        if((btn->pos().x()+letterlistscrollarea->widget()->pos().x()) <= 0)
        {   if(btnPos>0)
            {
                int val=letterlistscrollarea->horizontalScrollBar()->value();
                letterlistscrollarea->horizontalScrollBar()->setValue(val-40);
            }
            else{
//                qDebug()<<letterlistscrollarea->horizontalScrollBar()->minimum();
                letterlistscrollarea->horizontalScrollBar()->setValue(letterlistscrollarea->horizontalScrollBar()->minimum());
            }

        }

    }
}

/**
 * 向右按钮槽函数
 */
void FullLetterWidget::rightbtn_clicked_slot()
{
    if(btnPos<letterlistscrollarea->widget()->layout()->count()-1)
    {
        QLayoutItem* beforeitem=letterlistscrollarea->widget()->layout()->itemAt(btnPos++);
        QWidget* beforewid=beforeitem->widget();
        QToolButton* beforebtn=qobject_cast<QToolButton*>(beforewid);
        beforebtn->setChecked(false);
        QLayoutItem* item=letterlistscrollarea->widget()->layout()->itemAt(btnPos);
        QWidget* wid=item->widget();
        QToolButton* btn=qobject_cast<QToolButton*>(wid);
        btn->click();
//        qDebug()<<"---"<<btn->pos().x();
//        qDebug()<<"---111---"<<letterlistscrollarea->widget()->pos().x();

        if((btn->pos().x()+letterlistscrollarea->widget()->pos().x()) >= letterlistscrollarea->width())
        {   if(btnPos<letterlistscrollarea->widget()->layout()->count()-1)
            {
                int val=letterlistscrollarea->horizontalScrollBar()->value();
                letterlistscrollarea->horizontalScrollBar()->setValue(val+40);
            }
            else{
//                qDebug()<<scrollarea->horizontalScrollBar()->maximum();
                letterlistscrollarea->horizontalScrollBar()->setValue(letterlistscrollarea->horizontalScrollBar()->maximum());
            }

        }

//        qDebug()<<scrollarea->horizontalScrollBar()->value();

    }
}

/**
 * 字母列表数据项被选定槽函数
 */
void FullLetterWidget::letterbtn_clicked_slot()
{
    QLayoutItem* item=letterlistscrollarea->widget()->layout()->itemAt(btnPos);
    QWidget* wid=item->widget();
    QToolButton* beforebtn=qobject_cast<QToolButton*>(wid);
    beforebtn->setChecked(false);
    QToolButton* btn=dynamic_cast<QToolButton*>(QObject::sender());
    btnPos=letterlistscrollarea->widget()->layout()->indexOf(btn);
    btn->setChecked(true);
    QString letterstr=btn->text().at(0);
    //此处需实现将被选定的字母包含的应用列表移动到applistWid界面最顶端
    int num=letterbtnlist.indexOf(letterstr);
    if(num!=-1)
    {
        int pos=letterbtnrowlist.at(num).toInt();
        scrollarea->verticalScrollBar()->setSliderPosition(pos);
    }
    emit send_fullletterwid_state(0,letterstr);

}
