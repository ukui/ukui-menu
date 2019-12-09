#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QSettings>
#include <ukuimenuinterface.h>
#include "src/color.h"
#include "src/RightClickMenu/rightclickmenu.h"

class PushButton : public QPushButton
{
    Q_OBJECT
public:
    /**
     * @param classify为0时为分类按钮、1为应用按钮
     * @param module为0时为常用模块，1为字母模块、2为功能模块
     */
    PushButton(QString name,int classify, int module);
    ~PushButton();

private:
    QString name;
    int classify;
    int module;
    RightClickMenu* menu=nullptr;//右键菜单
    QSettings *setting=nullptr;
    UkuiMenuInterface* pUkuiMenuInterface=nullptr;

protected:
    void init_app_btn();

private slots:
    void exec_app_name();//执行应用程序
    void right_click_slot();//加载右键菜单

signals:
    void send_hide_mainwindow_signal();//向MainViewWidget发送隐藏主窗口信号
    void send_right_click_signal(int ret);//向CommonUseWidget发送邮件菜单返回值信号
    void send_update_applist_signal(QString appname);//向LetterWidget或者FunctionWidget发送更新应用列表信号

};

#endif // PUSHBUTTON_H
