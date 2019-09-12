#ifndef FULLLETTERBUTTONWIDGET_H
#define FULLLETTERBUTTONWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QLabel>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>

namespace Ui {
class FullLetterButtonWidget;
}

class FullLetterButtonWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FullLetterButtonWidget(QWidget *parent = nullptr);
    ~FullLetterButtonWidget();

private:
    Ui::FullLetterButtonWidget *ui;

    QVBoxLayout* mainLayout=nullptr;
    QSpacerItem* horizontalSpacer=nullptr;
    QSpacerItem* verticalSpacer=nullptr;


protected:
    void init_widget();
    void add_letterbtn_control();//添加字母分类按钮

signals:
    /**
     * 向FullLetterWidget界面发送字母分类按钮被点击信号
     * @param btnname存放被点击按钮的名称
     */
    void send_letterbtn_signal(QString btnname);

private slots:
    void letterbtn_clicked_slot();//字母分类按钮槽函数
};

#endif // FULLLETTERBUTTONWIDGET_H
