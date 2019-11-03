#ifndef LETTERBUTTONWIDGET_H
#define LETTERBUTTONWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QLabel>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>

namespace Ui {
class LetterButtonWidget;
}

class LetterButtonWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LetterButtonWidget(QWidget *parent = nullptr);
    ~LetterButtonWidget();

    void load_min_wid();
    void load_max_wid();

private:
    Ui::LetterButtonWidget *ui;

    QVBoxLayout* mainLayout=nullptr;
    QSpacerItem* horizontalSpacer=nullptr;
    QSpacerItem* verticalSpacer=nullptr;

protected:
    void init_widget();
    void add_letterbtn_control();//添加字母分类按钮

signals:
    /**
     * 向LetterWidget界面发送字母分类按钮被点击信号
     * @param btnname存放按钮名称
     */
    void send_letterbtn_signal(QString btnname);

private slots:
    void letterbtn_clicked_slot();//字母分类按钮槽函数
    void recv_letterbtn_list(QStringList list);//接收LetterWidget字母按钮列表
};

#endif // LETTERBUTTONWIDGET_H
