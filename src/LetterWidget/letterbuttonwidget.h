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
    explicit LetterButtonWidget(QWidget *parent = 0);
    ~LetterButtonWidget();

private:
    Ui::LetterButtonWidget *ui;

    QVBoxLayout* mainLayout=NULL;
    QSpacerItem* horizontalSpacer=NULL;
    QSpacerItem* verticalSpacer=NULL;


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
};

#endif // LETTERBUTTONWIDGET_H
