#ifndef LETTERWIDGET_H
#define LETTERWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include "letterbuttonwidget.h"

namespace Ui {
class LetterWidget;
}

class LetterWidget : public QWidget
{
    Q_OBJECT

public:
     explicit LetterWidget(QWidget *parent=0);
    ~LetterWidget();

    /**
      * 设置字母排序界面状态
      * @param state为0时字母排序界面加载应用列表，为1时加载纯字母按钮界面LetterButtonWidget
      * @param btnname存放有具体的字母
      */
     void set_letterwidge_state(int state,QString btnname);

private:
    Ui::LetterWidget *ui;

    QHBoxLayout* mainLayout=NULL;

    //应用列表界面
    QWidget* applistWid=NULL;
    LetterButtonWidget* letterbtnwid=NULL;

    QToolButton* btn=NULL;//测试按钮

protected:
    void init_widget();
    void init_applist_widget();//初始化应用列表界面

private slots:
    void app_classificationbtn_clicked_slot();//应用列表字母分类按钮槽函数
    void recv_letterbtn_signal(QString btnname);//接收LetterButtonWidget界面按钮信号

signals:
    /**
     * 向MainViewWidget发送字母排序界面状态信号
     * @param state为0时，btnname为具体的字母，为1时，btnname为空
     */
    void send_letterwid_state(int state,QString btnname);
};

#endif // LETTERWIDGET_H
