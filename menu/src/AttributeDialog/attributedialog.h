#ifndef ATTRIBUTEDIALOG_H
#define ATTRIBUTEDIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QSpacerItem>
#include <QToolButton>
#include <QPushButton>
#include <QTableWidget>
#include <QScrollBar>
#include <QPixmap>
#include <QTextEdit>
#include <ukuimenuinterface.h>
#include "src/UtilityFunction/toolbutton.h"
#include "src/color.h"

namespace Ui {
class AttributeDialog;
}

class AttributeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AttributeDialog(QWidget *parent = 0);
    ~AttributeDialog();

private:
    Ui::AttributeDialog *ui;

    UkuiMenuInterface* pUkuiMenuInterface=nullptr;

    QVBoxLayout* mainLayout=nullptr;
    QWidget* titlebarWid=nullptr;
    QWidget* upWid=nullptr;
    QFrame* line=nullptr;
    QWidget* downWid=nullptr;
    //标题栏
    QHBoxLayout* titlebarwidLayout=nullptr;
    QLabel* appnameLabel=nullptr;
    QLabel* titlebarLabel=nullptr;
//    ToolButton* minbtn=nullptr;
//    QHBoxLayout* minbtnLayout;
//    QLabel* minbtnLabel;
    ToolButton* closebtn=nullptr;
//    QHBoxLayout* closebtnLayout;
//    QLabel* closebtnLabel;
    QSpacerItem* leftSpacer=nullptr;
    QSpacerItem* rightSpacer=nullptr;
    //分割线上部控件
    QHBoxLayout* upwidLayout=nullptr;
    QLabel* labelIcon=nullptr;
    QLabel* labelappName=nullptr;
    //分割线下部控件
    QVBoxLayout* downwidLayout;
    QTableWidget* tableWid;
//    QLabel* typeLabel=nullptr;
//    QLabel* execLabel=nullptr;
//    QLabel* commentLabel=nullptr;
//    QLabel* typevalueLabel=nullptr;
//    QLineEdit* execvalueEdit=nullptr;
//    QLabel* commentvalueLabel=nullptr;

    QTextEdit* typeEdit=nullptr;
    QTextEdit* execEdit=nullptr;
    QTextEdit* commentEdit=nullptr;
    QTextEdit* typevalueEdit=nullptr;
    QTextEdit* execvalueEdit=nullptr;
    QTextEdit* commentvalueEdit=nullptr;

//    QWidget* leftWid;
//    QVBoxLayout* leftwidLayout;
//    QLabel* typeLabel=nullptr;
//    QLabel* execLabel=nullptr;
//    QLabel* commentLabel=nullptr;

//    QWidget* rightWid;
//    QVBoxLayout* rightwidLayout;
//    QLabel* typevalueLabel=nullptr;
//    QLabel* execvalueLabel=nullptr;
//    QLabel* commentvalueLabel=nullptr;

//    QWidget* typeWid=nullptr;
//    QHBoxLayout* typewidLayout=nullptr;
//    QLabel* typeLabel=nullptr;
//    QLabel* typevalueLabel=nullptr;
//    QWidget* execWid=nullptr;
//    QHBoxLayout* execwidLayout=nullptr;
//    QLabel* execLabel=nullptr;
//    QLabel* execvalueLabel=nullptr;
//    QWidget* commentWid=nullptr;
//    QHBoxLayout* commentwidLayout=nullptr;
//    QWidget* commentlabelWid;
//    QVBoxLayout* commentlabelwidLayout;
//    QLabel* commentLabel=nullptr;
//    QWidget* commentvaluelabelWid;
//    QVBoxLayout* commentvaluelabelwidLayout;
//    QTextEdit* commentvalueLabel=nullptr;

//    QSpacerItem* verticalSpacer;

    //关闭按钮
    QWidget* closeWid;
    QHBoxLayout* closewidLayout;
    QSpacerItem* btnleftSpacer;
    QPushButton* btn;

protected:
    void init_widget();
    void set_controls_style(QTextEdit* edit,QTextEdit* valueEdit,QString str,int height);

private slots:
    void recv_attribute_value(QString appname);
};

#endif // ATTRIBUTEDIALOG_H
