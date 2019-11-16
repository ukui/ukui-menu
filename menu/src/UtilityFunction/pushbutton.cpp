#include "pushbutton.h"

PushButton::PushButton(QString name, int classify, int module)
{
    this->name=name;
    this->classify=classify;
    this->module=module;
    QString path=QDir::homePath()+"/.config/ukui-start-menu/ukui-start-menu.ini";
    setting=new QSettings(path,QSettings::IniFormat);
    init_app_btn();
}

void PushButton::init_app_btn()
{
    char btnstyle[300];
    sprintf(btnstyle,"QPushButton{background:transparent;border:0px;color:#ffffff;font-size:14px;padding-left:0px;text-align: left center;}\
            QPushButton:hover{background-color:%s;}\
            QPushButton:pressed{background-color:%s;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);

    if(classify==0)
    {
        if(module==1)
        {
            this->setStyleSheet(btnstyle);
            this->setFocusPolicy(Qt::NoFocus);
            QHBoxLayout* letterlayout=new QHBoxLayout(this);
            letterlayout->setContentsMargins(15,0,0,0);
            letterlayout->setSpacing(6);
            QFrame* line=new QFrame(this);
            line->setFrameShape(QFrame::HLine);
            line->setStyleSheet("background-color:#626c6e");
            QLabel* letterlabel=new QLabel(this);
            letterlabel->setFixedSize(20,20);
            letterlabel->setAlignment(Qt::AlignCenter);
            letterlabel->setStyleSheet("color:#ffffff;font-size:14px;");
            letterlabel->setText(name);
            letterlayout->addWidget(line);
            letterlayout->addWidget(letterlabel);
            this->setLayout(letterlayout);
        }
        else {
            this->setStyleSheet(btnstyle);
            this->setFocusPolicy(Qt::NoFocus);
            QHBoxLayout* classificationlayout=new QHBoxLayout(this);
            classificationlayout->setContentsMargins(15,0,0,0);
            classificationlayout->setSpacing(6);
            QFrame* line=new QFrame(this);
            line->setFrameShape(QFrame::HLine);
            line->setStyleSheet("background-color:#626c6e");
            QLabel* classificationlabel=new QLabel(this);
            classificationlabel->setFixedSize(56,20);
            classificationlabel->setAlignment(Qt::AlignCenter);
            classificationlabel->setStyleSheet("color:#ffffff;font-size:14px;");
            classificationlabel->setText(name);
            classificationlayout->addWidget(line);
            classificationlayout->addWidget(classificationlabel);
            this->setLayout(classificationlayout);
        }
    }
    else
    {
        this->setFixedSize(330-4-14,42);
        this->setStyleSheet(QString::fromLocal8Bit(btnstyle));
        QHBoxLayout* layout=new QHBoxLayout(this);
        layout->setContentsMargins(15,0,0,0);
        layout->setSpacing(15);
        this->setLayout(layout);
        this->setFocusPolicy(Qt::NoFocus);

        QString iconstr=KylinStartMenuInterface::get_app_icon(KylinStartMenuInterface::get_desktop_path_by_app_name(name));
        QIcon icon=QIcon::fromTheme(iconstr);
        QLabel* labelicon=new QLabel(this);
        labelicon->setFixedSize(32,32);
        QPixmap pixmapicon(icon.pixmap(QSize(32,32)));
        labelicon->setPixmap(pixmapicon);
        labelicon->setStyleSheet("QLabel{background:transparent;}");
        QLabel* labeltext=new QLabel(this);
        labeltext->setText(name);
        labeltext->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:14px;}");
        layout->addWidget(labelicon);
        layout->addWidget(labeltext);

        if(module==0)
        {
            setting->beginGroup("activeApps");
            if(setting->value(name).toInt()==0)
            {
                layout->setContentsMargins(15,0,12,0);
                QSvgRenderer* svgRender=new QSvgRenderer(this);
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
            setting->endGroup();
        }

        connect(this,SIGNAL(clicked()),this,SLOT(exec_app_name()));
        this->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(this,SIGNAL(customContextMenuRequested(const QPoint&)),this,
                SLOT(right_click_slot()));
    }
}

void PushButton::exec_app_name()
{
//    emit send_hide_mainwindow_signal();
    QPushButton* btn=dynamic_cast<QPushButton*>(QObject::sender());
    QLayoutItem* child=btn->layout()->itemAt(1);
    QWidget* wid=child->widget();
    QLabel* appnamelabel=qobject_cast<QLabel*>(wid);
    QString appname=appnamelabel->text();
    QString execpath=KylinStartMenuInterface::get_app_exec(KylinStartMenuInterface::get_desktop_path_by_app_name(appname));
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

void PushButton::right_click_slot()
{
//    char style[200];
//    sprintf(style,"QPushButton{background-color:%s;border:0px;color:#ffffff;font-size:14px;padding-left:15px;text-align: left center;}", MAINVIEWBTNHOVER);

//    char btnstyle[300];
//    sprintf(btnstyle,"QPushButton{background:transparent;border:0px;color:#ffffff;font-size:14px;padding-left:15px;text-align: left center;}\
//            QPushButton:hover{background-color:%s;}\
//            QPushButton:pressed{background-color:%s;}", MAINVIEWBTNHOVER,MAINVIEWBTNPRESSED);

    QPushButton* btn=dynamic_cast<QPushButton*>(QObject::sender());
    QLayoutItem* item=btn->layout()->itemAt(1);
    QWidget* wid=item->widget();
    QLabel* label=qobject_cast<QLabel*>(wid);
    QString appname=label->text();
//    btn->setStyleSheet(style);
    menu=new RightClickMenu;
    if(module==0)
    {
        int ret=menu->show_commonuse_appbtn_menu(appname);
        emit send_right_click_signal(ret);
    }
    else {
        int ret=menu->show_appbtn_menu(appname);
        if(ret==1 || ret==2)
            emit send_update_applist_signal(appname);
    }
}
