#include "rightclickmenu.h"
#include <QDebug>

RightClickMenu::RightClickMenu()
{
    add_appbtn_action();
    add_shutdown_action();
    add_other_action();

}

RightClickMenu::~RightClickMenu()
{
    delete appbtnmenu;
    delete Fix2CommonUseWid;
    delete Unfixed4CommonUseWid;
    delete Fix2TaskBarWid;
    delete Unfixed4TaskBarWid;
    delete Add2DesktopWid;
    delete UninstallWid;
    delete AttributeWid;

    delete shutdownmenu;
    delete LockScreenWid;
    delete LogOutWid;
    delete RebootWid;
    delete ShutDownWid;

    delete othermenu;
    delete OtherFix2TaskBarWid;
    delete OtherUnfix2TaskBarWid;
    delete OtherListWid;

}

void RightClickMenu::add_appbtn_action()
{
    appbtnmenu=new QMenu();
    appbtnmenu->setLayoutDirection(Qt::LeftToRight);
    appbtnmenu->setFixedSize(250,34*5+10);

    Fix2CommonUseAction=new QWidgetAction(appbtnmenu);
    Fix2CommonUseWid=new QWidget();
    init_widget_action(Fix2CommonUseWid,":/data/img/mainviewwidget/fixed.svg","固定到“常用软件”");
    Fix2CommonUseAction->setDefaultWidget(Fix2CommonUseWid);
    appbtnmenu->addAction(Fix2CommonUseAction);
    connect(Fix2CommonUseAction, SIGNAL(triggered()),this,SLOT(fix2commonuseaction_trigger_slot()));

    Unfixed4CommonUseAction=new QWidgetAction(appbtnmenu);
    Unfixed4CommonUseWid=new QWidget();
    init_widget_action(Unfixed4CommonUseWid,":/data/img/mainviewwidget/unfixed.svg","从“常用软件”取消固定");
    Unfixed4CommonUseAction->setDefaultWidget(Unfixed4CommonUseWid);
//    appbtnmenu->addAction(Unfixed4CommonUseAction);
    connect(Unfixed4CommonUseAction, SIGNAL(triggered()),this,SLOT(unfixed4commonuseaction_trigger_slot()));

    Fix2TaskBarAction=new QWidgetAction(appbtnmenu);
    Fix2TaskBarWid=new QWidget();
    init_widget_action(Fix2TaskBarWid,":/data/img/mainviewwidget/fixed.svg","固定到任务栏");
    Fix2TaskBarAction->setDefaultWidget(Fix2TaskBarWid);
    appbtnmenu->addAction(Fix2TaskBarAction);
    connect(Fix2TaskBarAction, SIGNAL(triggered()),this,SLOT(fix2taskbaraction_trigger_slot()));

    Unfixed4TaskBarAction=new QWidgetAction(appbtnmenu);
    Unfixed4TaskBarWid=new QWidget();
    init_widget_action(Unfixed4TaskBarWid,":/data/img/mainviewwidget/unfixed.svg","从任务栏取消固定");
    Unfixed4TaskBarAction->setDefaultWidget(Unfixed4TaskBarWid);
//    appbtnmenu->addAction(Unfixed4TaskBarAction);
    connect(Unfixed4TaskBarAction, SIGNAL(triggered()),this,SLOT(unfixed4taskbaraction_trigger_slot()));

    Add2DesktopAction=new QWidgetAction(appbtnmenu);
    Add2DesktopWid=new QWidget();
    init_widget_action(Add2DesktopWid,"","添加到桌面快捷方式");
    Add2DesktopAction->setDefaultWidget(Add2DesktopWid);
    appbtnmenu->addAction(Add2DesktopAction);
    connect(Add2DesktopAction, SIGNAL(triggered()),this,SLOT(add2desktopaction_trigger_slot()));


    appbtnmenu->addSeparator();

    UninstallAction=new QWidgetAction(appbtnmenu);
    UninstallWid=new QWidget();
    init_widget_action(UninstallWid,":/data/img/mainviewwidget/uninstall.svg","卸载");
    UninstallAction->setDefaultWidget(UninstallWid);
    appbtnmenu->addAction(UninstallAction);
    connect(UninstallAction, SIGNAL(triggered()),this,SLOT(uninstallaction_trigger_slot()));

    appbtnmenu->addSeparator();

    AttributeAction=new QWidgetAction(appbtnmenu);
    AttributeWid=new QWidget();
    init_widget_action(AttributeWid,":/data/img/mainviewwidget/attributeaction.svg","属性");
    AttributeAction->setDefaultWidget(AttributeWid);
    appbtnmenu->addAction(AttributeAction);
    connect(AttributeAction, SIGNAL(triggered()),this,SLOT(attributeaction_trigger_slot()));

    appbtnmenu->setStyleSheet("QMenu{border:1px solid #626c6e;background-color:#151a1e;}\
                         QMenu::separator{height:1px;background:#475760;margin-left:2px;margin-right:2px;margin-top:2px;margin-bottom:2px;}");
}

void RightClickMenu::add_shutdown_action()
{
    shutdownmenu=new QMenu();
    shutdownmenu->setLayoutDirection(Qt::LeftToRight);
    shutdownmenu->setFixedSize(250,34*4);

    LockScreenAction=new QWidgetAction(shutdownmenu);
    LockScreenWid=new QWidget();
    init_widget_action(LockScreenWid,":/data/img/sidebarwidget/lock.svg"," 锁定屏幕");
    LockScreenAction->setDefaultWidget(LockScreenWid);
    shutdownmenu->addAction(LockScreenAction);
    connect(LockScreenAction,SIGNAL(triggered()),this,SLOT(lockscreenaction_trigger_slot()));

    LogOutAction=new QWidgetAction(shutdownmenu);
    LogOutWid=new QWidget();
    init_widget_action(LogOutWid,"","注销");
    LogOutAction->setDefaultWidget(LogOutWid);
    shutdownmenu->addAction(LogOutAction);
    connect(LogOutAction,SIGNAL(triggered()),this,SLOT(logoutaction_trigger_slot()));

    RebootAction=new QWidgetAction(shutdownmenu);
    RebootWid=new QWidget();
    init_widget_action(RebootWid,"","重启计算机");
    RebootAction->setDefaultWidget(RebootWid);
    shutdownmenu->addAction(RebootAction);
    connect(RebootAction,SIGNAL(triggered()),this,SLOT(rebootaction_trigger_slot()));

    ShutDownAction=new QWidgetAction(shutdownmenu);
    ShutDownWid=new QWidget();
    init_widget_action(ShutDownWid,":/data/img/sidebarwidget/rcshutdown.svg","关闭计算机");
    ShutDownAction->setDefaultWidget(ShutDownWid);
    shutdownmenu->addAction(ShutDownAction);
    connect(ShutDownAction,SIGNAL(triggered()),this,SLOT(shutdownaction_trigger_slot()));

    shutdownmenu->setStyleSheet("QMenu{border:1px solid #626c6e;background-color:#151a1e;}");
}

void RightClickMenu::add_other_action()
{
    othermenu=new QMenu();
    othermenu->setLayoutDirection(Qt::LeftToRight);
    othermenu->setFixedSize(250,34*2);

    OtherFix2TaskBarAction=new QWidgetAction(othermenu);
    OtherFix2TaskBarWid=new QWidget();
    init_widget_action(OtherFix2TaskBarWid,":/data/img/sidebarwidget/fixed.svg","固定到任务栏");
    OtherFix2TaskBarAction->setDefaultWidget(OtherFix2TaskBarWid);
    othermenu->addAction(OtherFix2TaskBarAction);
    connect(OtherFix2TaskBarAction,SIGNAL(triggered()),this,SLOT(fix2taskbaraction_trigger_slot()));

    OtherUnfix2TaskBarAction=new QWidgetAction(othermenu);
    OtherUnfix2TaskBarWid=new QWidget();
    init_widget_action(OtherUnfix2TaskBarWid,":/data/img/sidebarwidget/unfixed.svg","从任务栏取消固定");
    OtherUnfix2TaskBarAction->setDefaultWidget(OtherUnfix2TaskBarWid);
//    othermenu->addAction(OtherUnfix2TaskBarAction);
    connect(OtherUnfix2TaskBarAction,SIGNAL(triggered()),this,SLOT(unfixed4taskbaraction_trigger_slot()));

    OtherListAction=new QWidgetAction(othermenu);
    OtherListWid=new QWidget();
    init_widget_action(OtherListWid,":/data/img/sidebarwidget/setting.svg","设置开始菜单显示列表");
    OtherListAction->setDefaultWidget(OtherListWid);
    othermenu->addAction(OtherListAction);
    connect(OtherListAction,SIGNAL(triggered()),this,SLOT(otherlistaction_trigger_slot()));

    othermenu->setStyleSheet("QMenu{border:1px solid #626c6e;background-color:#151a1e;}");

}

void RightClickMenu::init_widget_action(QWidget *wid, QString iconstr, QString textstr)
{
    QString style="QWidget{background:transparent;border:0px;}\
            QWidget:hover{background-color:#33bed8ef;}\
            QWidget:pressed{background-color:#1abed8ef;}";

    QHBoxLayout* layout=new QHBoxLayout(wid);
    wid->setLayout(layout);
    wid->setFixedSize(250,34);
    wid->setStyleSheet(style);
    wid->setFocusPolicy(Qt::NoFocus);

    if(!iconstr.isEmpty())
    {
        QLabel* labelicon=new QLabel(wid);
        QSvgRenderer* svg=new QSvgRenderer(wid);
        svg->load(iconstr);
        QPixmap* pixmap=new QPixmap(16,16);
        pixmap->fill(Qt::transparent);
        QPainter p(pixmap);
        svg->render(&p);
        labelicon->setPixmap(*pixmap);
        labelicon->setFixedSize(pixmap->size());
        labelicon->setAlignment(Qt::AlignCenter);
        labelicon->setStyleSheet("QLabel{background:transparent;border:0px;}");
        layout->addWidget(labelicon);

    }

    QLabel* labeltext=new QLabel(wid);
    labeltext->setStyleSheet("background:transparent;border:0px;color:#ffffff;font-size:14px;");
    QByteArray textbyte=textstr.toLocal8Bit();
    char* text=textbyte.data();
    labeltext->setText(tr(text));
    labeltext->adjustSize();
    layout->addWidget(labeltext);

    if(!iconstr.isEmpty())
    {
        layout->setContentsMargins(10,0,wid->width()-16-labeltext->width()-20,0);
        layout->setSpacing(10);
    }
    else {
        layout->setContentsMargins(36,0,0,0);
    }
}

void RightClickMenu::fix2commonuseaction_trigger_slot()
{
    qDebug()<<"---1--";
}

void RightClickMenu::unfixed4commonuseaction_trigger_slot()
{
    qDebug()<<"---11--";
}

void RightClickMenu::fix2taskbaraction_trigger_slot()
{
    qDebug()<<"---2--";
}

void RightClickMenu::unfixed4taskbaraction_trigger_slot()
{
    qDebug()<<"---22--";
}

void RightClickMenu::add2desktopaction_trigger_slot()
{
    qDebug()<<"---3--";
}

void RightClickMenu::uninstallaction_trigger_slot()
{
    qDebug()<<"---4--";
}

void RightClickMenu::attributeaction_trigger_slot()
{
    qDebug()<<"---5--";
}

void RightClickMenu::show_appbtn_menu()
{
    appbtnmenu->exec(QCursor::pos());
}

void RightClickMenu::lockscreenaction_trigger_slot()
{
    qDebug()<<"---1---";
}

void RightClickMenu::logoutaction_trigger_slot()
{
    qDebug()<<"---2---";
}

void RightClickMenu::rebootaction_trigger_slot()
{
    qDebug()<<"---3---";
}

void RightClickMenu::shutdownaction_trigger_slot()
{
    qDebug()<<"---4---";
}

void RightClickMenu::show_shutdown_menu()
{
    shutdownmenu->exec(QCursor::pos());

}

void RightClickMenu::otherlistaction_trigger_slot()
{
    qDebug()<<"---2---";
}

void RightClickMenu::show_other_menu()
{
    othermenu->exec(QCursor::pos());
}


