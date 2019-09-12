#ifndef RIGHTCLICKMENU_H
#define RIGHTCLICKMENU_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QMenu>
#include <QAction>
#include <QSvgRenderer>
#include <QPainter>
#include <QWidgetAction>
#include <QLabel>

class RightClickMenu: public QObject
{
    Q_OBJECT
public:
    RightClickMenu();
    virtual ~RightClickMenu();
    void show_appbtn_menu();
    void show_shutdown_menu();
    void show_other_menu();

private:
    QMenu* appbtnmenu;
    QWidgetAction* Fix2CommonUseAction;
    QWidget* Fix2CommonUseWid;
    QWidgetAction* Unfixed4CommonUseAction;
    QWidget* Unfixed4CommonUseWid;
    QWidgetAction* Fix2TaskBarAction;
    QWidget* Fix2TaskBarWid;
    QWidgetAction* Unfixed4TaskBarAction;
    QWidget* Unfixed4TaskBarWid;
    QWidgetAction* Add2DesktopAction;
    QWidget* Add2DesktopWid;
    QWidgetAction* UninstallAction;
    QWidget* UninstallWid;
    QWidgetAction* AttributeAction;
    QWidget* AttributeWid;

    QMenu* shutdownmenu;
    QWidgetAction* LockScreenAction;
    QWidget* LockScreenWid;
    QWidgetAction* LogOutAction;
    QWidget* LogOutWid;
    QWidgetAction* RebootAction;
    QWidget* RebootWid;
    QWidgetAction* ShutDownAction;
    QWidget* ShutDownWid;

    QMenu* othermenu;
    QWidgetAction* OtherFix2TaskBarAction;
    QWidget* OtherFix2TaskBarWid;
    QWidgetAction* OtherUnfix2TaskBarAction;
    QWidget* OtherUnfix2TaskBarWid;
    QWidgetAction* OtherListAction;
    QWidget* OtherListWid;

protected:
    void add_appbtn_action();
    void add_shutdown_action();
    void add_other_action();
    void init_widget_action(QWidget* wid, QString iconstr, QString textstr);

private slots:
    void fix2commonuseaction_trigger_slot();
    void unfixed4commonuseaction_trigger_slot();
    void fix2taskbaraction_trigger_slot();
    void unfixed4taskbaraction_trigger_slot();
    void add2desktopaction_trigger_slot();
    void uninstallaction_trigger_slot();
    void attributeaction_trigger_slot();

    void lockscreenaction_trigger_slot();
    void logoutaction_trigger_slot();
    void rebootaction_trigger_slot();
    void shutdownaction_trigger_slot();

    void otherlistaction_trigger_slot();

};

#endif // RIGHTCLICKMENU_H
