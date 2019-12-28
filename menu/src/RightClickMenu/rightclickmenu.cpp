#include "rightclickmenu.h"
#include <QDebug>

RightClickMenu::RightClickMenu()
{
    QString path=QDir::homePath()+"/.config/ukui-menu/ukui-menu.ini";
    setting=new QSettings(path,QSettings::IniFormat);

    //其它按钮右键菜单项
    othermenu=new QMenu();
    othermenu->setLayoutDirection(Qt::LeftToRight);
    othermenu->setFixedSize(250,34*2);
    OtherFix2TaskBarAction=new QWidgetAction(othermenu);
    OtherFix2TaskBarWid=new QWidget();
    OtherUnfix2TaskBarAction=new QWidgetAction(othermenu);
    OtherUnfix2TaskBarWid=new QWidget();
    OtherListAction=new QWidgetAction(othermenu);
    OtherListWid=new QWidget();

    //常用应用按钮右键菜单
    cuappbtnmenu=new QMenu();
    cuappbtnmenu->setLayoutDirection(Qt::LeftToRight);
    cuappbtnmenu->setFixedSize(250,34*7+15);
    CuFix2CommonUseAction=new QWidgetAction(cuappbtnmenu);
    CuFix2CommonUseWid=new QWidget();
    CuUnfixed4CommonUseAction=new QWidgetAction(cuappbtnmenu);
    CuUnfixed4CommonUseWid=new QWidget();
    CuFix2TaskBarAction=new QWidgetAction(cuappbtnmenu);
    CuFix2TaskBarWid=new QWidget();
    CuUnfixed4TaskBarAction=new QWidgetAction(cuappbtnmenu);
    CuUnfixed4TaskBarWid=new QWidget();
    CuAdd2DesktopAction=new QWidgetAction(cuappbtnmenu);
    CuAdd2DesktopWid=new QWidget();
    CuDeleteAction=new QWidgetAction(cuappbtnmenu);
    CuDeleteWid=new QWidget();
    CuDeleteAllAction=new QWidgetAction(cuappbtnmenu);
    CuDeleteAllWid=new QWidget();
    CuUninstallAction=new QWidgetAction(cuappbtnmenu);
    CuUninstallWid=new QWidget();
    CuAttributeAction=new QWidgetAction(cuappbtnmenu);
    CuAttributeWid=new QWidget();

    //普通应用按钮右键菜单
    appbtnmenu=new QMenu();
    appbtnmenu->setLayoutDirection(Qt::LeftToRight);
    appbtnmenu->setFixedSize(250,34*5+10);
    Fix2CommonUseAction=new QWidgetAction(appbtnmenu);
    Fix2CommonUseWid=new QWidget();
    Unfixed4CommonUseAction=new QWidgetAction(appbtnmenu);
    Unfixed4CommonUseWid=new QWidget();
    Fix2TaskBarAction=new QWidgetAction(appbtnmenu);
    Fix2TaskBarWid=new QWidget();
    Unfixed4TaskBarAction=new QWidgetAction(appbtnmenu);
    Unfixed4TaskBarWid=new QWidget();
    Add2DesktopAction=new QWidgetAction(appbtnmenu);
    Add2DesktopWid=new QWidget();
    UninstallAction=new QWidgetAction(appbtnmenu);
    UninstallWid=new QWidget();
    AttributeAction=new QWidgetAction(appbtnmenu);
    AttributeWid=new QWidget();

    pUkuiMenuInterface=new UkuiMenuInterface;

    add_shutdown_action();
    add_other_action();
}

RightClickMenu::~RightClickMenu()
{
    delete cuappbtnmenu;
    delete CuFix2CommonUseWid;
    delete CuUnfixed4CommonUseWid;
    delete CuFix2TaskBarWid;
    delete CuUnfixed4TaskBarWid;
    delete CuAdd2DesktopWid;
    delete CuDeleteAllWid;
    delete CuDeleteAllWid;
    delete CuUninstallWid;
    delete CuAttributeWid;

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

    delete pUkuiMenuInterface;

}

//常用应用按钮右键菜单
void RightClickMenu::add_commonuse_appbtn_action()
{
    setting->beginGroup("application");
    if(!setting->contains(appname) || setting->value(appname).toInt()>0)
    {
        init_widget_action(CuFix2CommonUseWid,":/data/img/mainviewwidget/fixed.svg","固定到“常用软件”");
        CuFix2CommonUseAction->setDefaultWidget(CuFix2CommonUseWid);
        cuappbtnmenu->addAction(CuFix2CommonUseAction);
        connect(CuFix2CommonUseAction, SIGNAL(triggered()),this,SLOT(fix2commonuseaction_trigger_slot()));
    }

    if(setting->contains(appname) && setting->value(appname).toInt()==0)
    {
        init_widget_action(CuUnfixed4CommonUseWid,":/data/img/mainviewwidget/unfixed.svg","从“常用软件”取消固定");
        CuUnfixed4CommonUseAction->setDefaultWidget(CuUnfixed4CommonUseWid);
        cuappbtnmenu->addAction(CuUnfixed4CommonUseAction);
        connect(CuUnfixed4CommonUseAction, SIGNAL(triggered()),this,SLOT(unfixed4commonuseaction_trigger_slot()));
    }

    QString desktopfp=pUkuiMenuInterface->get_desktop_path_by_app_name(appname);
    QDBusInterface iface("com.ukui.panel.desktop",
                         "/",
                         "com.ukui.panel.desktop",
                         QDBusConnection::sessionBus());

    QDBusReply<bool> ret=iface.call("CheckIfExist",desktopfp);
    if(!ret)
    {
        init_widget_action(CuFix2TaskBarWid,":/data/img/mainviewwidget/fixed.svg","固定到任务栏");
        CuFix2TaskBarAction->setDefaultWidget(CuFix2TaskBarWid);
        cuappbtnmenu->addAction(CuFix2TaskBarAction);
        connect(CuFix2TaskBarAction, SIGNAL(triggered()),this,SLOT(fix2taskbaraction_trigger_slot()));
    }
    else {
        init_widget_action(CuUnfixed4TaskBarWid,":/data/img/mainviewwidget/unfixed.svg","从任务栏取消固定");
        CuUnfixed4TaskBarAction->setDefaultWidget(CuUnfixed4TaskBarWid);
        cuappbtnmenu->addAction(CuUnfixed4TaskBarAction);
        connect(CuUnfixed4TaskBarAction, SIGNAL(triggered()),this,SLOT(unfixed4taskbaraction_trigger_slot()));
    }

    init_widget_action(CuAdd2DesktopWid,"","添加到桌面快捷方式");
    CuAdd2DesktopAction->setDefaultWidget(CuAdd2DesktopWid);
    cuappbtnmenu->addAction(CuAdd2DesktopAction);
    connect(CuAdd2DesktopAction, SIGNAL(triggered()),this,SLOT(add2desktopaction_trigger_slot()));


    cuappbtnmenu->addSeparator();

    init_widget_action(CuDeleteWid,"","从列表中删除");
    CuDeleteAction->setDefaultWidget(CuDeleteWid);
    cuappbtnmenu->addAction(CuDeleteAction);
    connect(CuDeleteAction,SIGNAL(triggered()),this,SLOT(cudeleteaction_trigger_slot()));
    if(setting->contains(appname) && setting->value(appname).toInt()==0)
    {
        QLayoutItem* item=CuDeleteWid->layout()->itemAt(0);
        QWidget* wid=item->widget();
        QLabel* label=qobject_cast<QLabel*>(wid);
        label->setStyleSheet("QLabel{background:transparent;border:0px;color:#4Dffffff;font-size:14px;}");
        CuDeleteAction->setDisabled(true);
    }

    init_widget_action(CuDeleteAllWid,"","删除所有");
    CuDeleteAllAction->setDefaultWidget(CuDeleteAllWid);
    cuappbtnmenu->addAction(CuDeleteAllAction);
    connect(CuDeleteAllAction,SIGNAL(triggered()),this,SLOT(cudeleteallaction_trigger_slot()));
    QStringList keys=setting->childKeys();
    int count;
    for(count=0;count<keys.count();count++)
    {
        if(setting->value(keys.at(count)).toInt()==2)
            break;
    }
    if(count==keys.count())
    {
        QLayoutItem* item=CuDeleteAllWid->layout()->itemAt(0);
        QWidget* wid=item->widget();
        QLabel* label=qobject_cast<QLabel*>(wid);
        label->setStyleSheet("QLabel{background:transparent;border:0px;color:#4Dffffff;font-size:14px;}");
        CuDeleteAllAction->setDisabled(true);
    }

    cuappbtnmenu->addSeparator();

    init_widget_action(CuUninstallWid,":/data/img/mainviewwidget/uninstall.svg","卸载");
    CuUninstallAction->setDefaultWidget(CuUninstallWid);
    cuappbtnmenu->addAction(CuUninstallAction);
    connect(CuUninstallAction, SIGNAL(triggered()),this,SLOT(uninstallaction_trigger_slot()));

    cuappbtnmenu->addSeparator();

    init_widget_action(CuAttributeWid,":/data/img/mainviewwidget/attributeaction.svg","属性");
    CuAttributeAction->setDefaultWidget(CuAttributeWid);
    cuappbtnmenu->addAction(CuAttributeAction);
    connect(CuAttributeAction, SIGNAL(triggered()),this,SLOT(attributeaction_trigger_slot()));

    cuappbtnmenu->setStyleSheet("QMenu{border:1px solid #626c6e;background-color:#151a1e;}\
                         QMenu::separator{height:1px;background:#475760;margin-left:2px;margin-right:2px;margin-top:2px;margin-bottom:2px;}");

    setting->endGroup();
}

//普通应用按钮右键菜单
void RightClickMenu::add_appbtn_action()
{

    setting->beginGroup("application");
    if(!setting->contains(appname) || setting->value(appname).toInt()>0)
    {
        init_widget_action(Fix2CommonUseWid,":/data/img/mainviewwidget/fixed.svg","固定到“常用软件”");
        Fix2CommonUseAction->setDefaultWidget(Fix2CommonUseWid);
        appbtnmenu->addAction(Fix2CommonUseAction);
        connect(Fix2CommonUseAction, SIGNAL(triggered()),this,SLOT(fix2commonuseaction_trigger_slot()));
    }

    if(setting->contains(appname) && setting->value(appname).toInt()==0)
    {
        init_widget_action(Unfixed4CommonUseWid,":/data/img/mainviewwidget/unfixed.svg","从“常用软件”取消固定");
        Unfixed4CommonUseAction->setDefaultWidget(Unfixed4CommonUseWid);
        appbtnmenu->addAction(Unfixed4CommonUseAction);
        connect(Unfixed4CommonUseAction, SIGNAL(triggered()),this,SLOT(unfixed4commonuseaction_trigger_slot()));
    }
    setting->endGroup();

    QString desktopfp=pUkuiMenuInterface->get_desktop_path_by_app_name(appname);
    QDBusInterface iface("com.ukui.panel.desktop",
                         "/",
                         "com.ukui.panel.desktop",
                         QDBusConnection::sessionBus());

    QDBusReply<bool> ret=iface.call("CheckIfExist",desktopfp);
    if(!ret)
    {
        init_widget_action(Fix2TaskBarWid,":/data/img/mainviewwidget/fixed.svg","固定到任务栏");
        Fix2TaskBarAction->setDefaultWidget(Fix2TaskBarWid);
        appbtnmenu->addAction(Fix2TaskBarAction);
        connect(Fix2TaskBarAction, SIGNAL(triggered()),this,SLOT(fix2taskbaraction_trigger_slot()));

    }
    else{
        init_widget_action(Unfixed4TaskBarWid,":/data/img/mainviewwidget/unfixed.svg","从任务栏取消固定");
        Unfixed4TaskBarAction->setDefaultWidget(Unfixed4TaskBarWid);
        appbtnmenu->addAction(Unfixed4TaskBarAction);
        connect(Unfixed4TaskBarAction, SIGNAL(triggered()),this,SLOT(unfixed4taskbaraction_trigger_slot()));
    }

    init_widget_action(Add2DesktopWid,"","添加到桌面快捷方式");
    Add2DesktopAction->setDefaultWidget(Add2DesktopWid);
    appbtnmenu->addAction(Add2DesktopAction);
    connect(Add2DesktopAction, SIGNAL(triggered()),this,SLOT(add2desktopaction_trigger_slot()));


    appbtnmenu->addSeparator();

    init_widget_action(UninstallWid,":/data/img/mainviewwidget/uninstall.svg","卸载");
    UninstallAction->setDefaultWidget(UninstallWid);
    appbtnmenu->addAction(UninstallAction);
    connect(UninstallAction, SIGNAL(triggered()),this,SLOT(uninstallaction_trigger_slot()));

    appbtnmenu->addSeparator();

    init_widget_action(AttributeWid,":/data/img/mainviewwidget/attributeaction.svg","属性");
    AttributeAction->setDefaultWidget(AttributeWid);
    appbtnmenu->addAction(AttributeAction);
    connect(AttributeAction, SIGNAL(triggered()),this,SLOT(attributeaction_trigger_slot()));

    appbtnmenu->setStyleSheet("QMenu{border:1px solid #626c6e;background-color:#151a1e;}\
                         QMenu::separator{height:1px;background:#475760;margin-left:2px;margin-right:2px;margin-top:2px;margin-bottom:2px;}");
}

//关机按钮右键菜单
void RightClickMenu::add_shutdown_action()
{
    shutdownmenu=new QMenu();
    shutdownmenu->setLayoutDirection(Qt::LeftToRight);
    shutdownmenu->setFixedSize(250,34*5);

    LockScreenAction=new QWidgetAction(shutdownmenu);
    LockScreenWid=new QWidget();
    init_widget_action(LockScreenWid,":/data/img/sidebarwidget/lock.svg"," 锁定屏幕");
    LockScreenAction->setDefaultWidget(LockScreenWid);
    shutdownmenu->addAction(LockScreenAction);
    connect(LockScreenAction,SIGNAL(triggered()),this,SLOT(lockscreenaction_trigger_slot()));

    SwitchUserAction=new QWidgetAction(shutdownmenu);
    SwitchUserWid=new QWidget();
    init_widget_action(SwitchUserWid,"","切换用户");
    SwitchUserAction->setDefaultWidget(SwitchUserWid);
    shutdownmenu->addAction(SwitchUserAction);
    connect(SwitchUserAction,SIGNAL(triggered()),this,SLOT(switchuseraction_trigger_slot()));

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

//其它按钮右键菜单
void RightClickMenu::add_other_action()
{
    init_widget_action(OtherFix2TaskBarWid,":/data/img/sidebarwidget/fixed.svg","固定到任务栏");
    OtherFix2TaskBarAction->setDefaultWidget(OtherFix2TaskBarWid);
    othermenu->addAction(OtherFix2TaskBarAction);
    connect(OtherFix2TaskBarAction,SIGNAL(triggered()),this,SLOT(fix2taskbaraction_trigger_slot()));

    init_widget_action(OtherUnfix2TaskBarWid,":/data/img/sidebarwidget/unfixed.svg","从任务栏取消固定");
    OtherUnfix2TaskBarAction->setDefaultWidget(OtherUnfix2TaskBarWid);
//    othermenu->addAction(OtherUnfix2TaskBarAction);
    connect(OtherUnfix2TaskBarAction,SIGNAL(triggered()),this,SLOT(unfixed4taskbaraction_trigger_slot()));

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
    action_number=1;
    setting->beginGroup("application");
    setting->setValue(appname,0);
    setting->sync();
    setting->endGroup();

}

void RightClickMenu::unfixed4commonuseaction_trigger_slot()
{
    action_number=2;
    setting->beginGroup("application");
    setting->setValue(appname,2);
    setting->sync();
    setting->endGroup();

}

void RightClickMenu::fix2taskbaraction_trigger_slot()
{
    QString desktopfp=pUkuiMenuInterface->get_desktop_path_by_app_name(appname);
    QDBusInterface iface("com.ukui.panel.desktop",
                         "/",
                         "com.ukui.panel.desktop",
                         QDBusConnection::sessionBus());
    QDBusReply<QVariant> ret=iface.call("AddToTaskbar",desktopfp);
    //    qDebug()<<desktopfp<<ret.value().toBool();
    action_number=3;
}

void RightClickMenu::unfixed4taskbaraction_trigger_slot()
{
    QString desktopfp=pUkuiMenuInterface->get_desktop_path_by_app_name(appname);
    QDBusInterface iface("com.ukui.panel.desktop",
                         "/",
                         "com.ukui.panel.desktop",
                         QDBusConnection::sessionBus());
    QDBusReply<QVariant> ret=iface.call("RemoveFromTaskbar",desktopfp);
    action_number=4;
}

void RightClickMenu::add2desktopaction_trigger_slot()
{
    action_number=5;
}

void RightClickMenu::uninstallaction_trigger_slot()
{
    action_number=6;
}

void RightClickMenu::attributeaction_trigger_slot()
{
    action_number=7;
    QString desktopfp=pUkuiMenuInterface->get_desktop_path_by_app_name(appname);
    char command[100];
    sprintf(command,"ukui-menu-attr %s",desktopfp.toLocal8Bit().data());
    QProcess* proc=new QProcess(this);
    proc->startDetached(command);
}

void RightClickMenu::cudeleteaction_trigger_slot()
{
    action_number=8;
    setting->beginGroup("application");
    setting->remove(appname);
    setting->sync();
    setting->endGroup();
}

void RightClickMenu::cudeleteallaction_trigger_slot()
{
    action_number=9;
    setting->beginGroup("application");
    QStringList keys=setting->childKeys();
    for(int i=0;i<keys.count();i++)
    {
        if(setting->value(keys.at(i)).toInt()==2)
        {
            setting->remove(keys.at(i));
        }
    }
    setting->sync();
    setting->endGroup();

}

void RightClickMenu::lockscreenaction_trigger_slot()
{
    action_number=10;
}

void RightClickMenu::switchuseraction_trigger_slot()
{
    action_number=11;
}

void RightClickMenu::logoutaction_trigger_slot()
{
    action_number=12;
}

void RightClickMenu::rebootaction_trigger_slot()
{
    action_number=13;
}

void RightClickMenu::shutdownaction_trigger_slot()
{
    action_number=14;
}

void RightClickMenu::otherlistaction_trigger_slot()
{
    action_number=15;
    qDebug()<<"---2---";
}

int RightClickMenu::show_commonuse_appbtn_menu(QString appname)
{
    this->appname=appname;
    add_commonuse_appbtn_action();
    cuappbtnmenu->exec(QCursor::pos());
    return action_number;
}

int RightClickMenu::show_appbtn_menu(QString appname)
{
    this->appname=appname;
    add_appbtn_action();
    appbtnmenu->exec(QCursor::pos());
    return action_number;
}

int RightClickMenu::show_shutdown_menu()
{
    shutdownmenu->exec(QCursor::pos());
    return action_number;

}

void RightClickMenu::show_other_menu()
{
    othermenu->exec(QCursor::pos());
}


