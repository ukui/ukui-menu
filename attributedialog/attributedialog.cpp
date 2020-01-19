/*
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/&gt;.
 *
 */

#include "attributedialog.h"
#include "ui_attributedialog.h"
#include <QDebug>

AttributeDialog::AttributeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AttributeDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint|Qt::Tool);
    this->setFocusPolicy(Qt::StrongFocus);
    init_widget();
}

AttributeDialog::~AttributeDialog()
{
    delete ui;
    delete pUkuiMenuInterface;
}

void AttributeDialog::init_widget()
{
    this->resize(400,335);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setStyleSheet("background:transparent;border:0px;");
    mainLayout=new QHBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    this->setLayout(mainLayout);

    frame=new QFrame;
    frame->setFixedSize(this->width(),this->height());
    frame->setStyleSheet("background-color:rgba(26, 26, 26, 0.95);border:1px solid rgba(255, 255, 255, 0.05);border-radius:6px;");
    frameLayout=new QVBoxLayout;
    frameLayout->setContentsMargins(0,0,0,0);
    frameLayout->setSpacing(0);
    frame->setLayout(frameLayout);
    mainLayout->addWidget(frame);

    //标题栏
    titlebarWid=new QWidget(this);
    titlebarWid->setStyleSheet("border:0px;background:transparent;");
    titlebarWid->setFixedSize(this->width(),45);
    titlebarwidLayout=new QHBoxLayout;
    titlebarwidLayout->setContentsMargins(0,5,4,0);
    titlebarwidLayout->setSpacing(0);
    leftSpacer=new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);
    closebtn=new ToolButton(40,40,QString(":/close.svg"),QString(":/close-white.svg"),
                            AttributeDialogHover,AttributeDialogPressed,3,"");
    connect(closebtn,SIGNAL(clicked()),qApp,SLOT(quit()));

    titlebarwidLayout->addItem(leftSpacer);
    titlebarwidLayout->addWidget(closebtn);
    titlebarWid->setLayout(titlebarwidLayout);

    //上部控件
    upWid=new QWidget(this);
    upWid->setStyleSheet("border:0px;background:transparent;");
    upWid->setFixedSize(this->width(),68);
    upwidLayout=new QHBoxLayout;
    upwidLayout->setContentsMargins(19,10,0,0);
    upwidLayout->setSpacing(22);
    labelIcon=new QLabel(upWid);
    labelappName=new QLabel(upWid);
    labelappName->setStyleSheet("color:#ffffff;font-size:24px;");
    upwidLayout->addWidget(labelIcon);
    upwidLayout->addWidget(labelappName);
    upWid->setLayout(upwidLayout);

    //下部控件
    downWid=new QWidget(this);
    downWid->setStyleSheet("border:0px;background:transparent;");
    downWid->setFixedSize(this->width(),this->height()-titlebarWid->height()-upWid->height()-72);
    downwidLayout=new QVBoxLayout;
    downwidLayout->setContentsMargins(21,20,17,0);
    downWid->setLayout(downwidLayout);

    downmainWid=new QWidget(downWid);
    downmainWid->setFixedSize(downWid->width()-38,downWid->height()-20);
    gridLayout=new QGridLayout;
    gridLayout->setContentsMargins(11,14,11,14);
    gridLayout->setHorizontalSpacing(0);
    gridLayout->setVerticalSpacing(0);
    downmainWid->setLayout(gridLayout);
    downwidLayout->addWidget(downmainWid);
    downmainWid->setStyleSheet("border:0px;background-color:rgba(255, 255, 255, 0.06);");

//    typeEdit=new QTextEdit;
//    execEdit=new QTextEdit;
//    commentEdit=new QTextEdit;
//    typevalueEdit=new QTextEdit;
//    execvalueEdit=new  QTextEdit;
//    commentvalueEdit=new QTextEdit;
//    set_controls_style(typeEdit,typevalueEdit,"类型",32);
//    set_controls_style(execEdit,execvalueEdit,"命令",32);
//    set_controls_style(commentEdit,commentvalueEdit,"备注",downmainWid->height()-64-14);

    typeLabel=new QLabel;
    execLabel=new QLabel;
    commentLabel=new QLabel;
    typevalueEdit=new QLineEdit;
    execvalueEdit=new  QLineEdit;
    commentvalueEdit=new QLineEdit;
//    typeLabel->setText(tr("Type")+":");
//    execLabel->setText(tr("Exec")+":");
//    commentLabel->setText(tr("Comment")+":");
    set_controls_style(typeLabel,typevalueEdit,tr("Type"),38);
    set_controls_style(execLabel,execvalueEdit,tr("Exec"),38);
    set_controls_style(commentLabel,commentvalueEdit,tr("Comment"),38);


    gridLayout->addWidget(typeLabel,0,0);
    gridLayout->addWidget(typevalueEdit,0,1);
    gridLayout->addWidget(execLabel,1,0);
    gridLayout->addWidget(execvalueEdit,1,1);
    gridLayout->addWidget(commentLabel,2,0);
    gridLayout->addWidget(commentvalueEdit,2,1);


    //关闭按钮
    closeWid=new QWidget(this);
    closeWid->setFixedSize(this->width(),72);
    closeWid->setStyleSheet("border:0px;background:transparent;");
    closewidLayout=new QHBoxLayout;
    closewidLayout->setContentsMargins(0,20,20,18);
    btnleftSpacer=new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);
    btn=new QPushButton(closeWid);
    btn->setFixedSize(120,34);
    btn->setText(tr("Close"));
    btn->setFocusPolicy(Qt::NoFocus);
    closewidLayout->addItem(btnleftSpacer);
    closewidLayout->addWidget(btn);
    closeWid->setLayout(closewidLayout);
    btn->setStyleSheet("QPushButton{background-color:rgba(61, 107, 229, 1);border:0px;border-radius:4px;font-size:14px;color:#ffffff;}\
                        QPushButton:hover{background-color:rgba(107, 142, 235, 1);border:0px;border-radius:4px;font-size:14px;color:#ffffff;}\
                        QPushButton:pressed{background-color:rgba(50, 87, 202, 1);border:0px;border-radius:4px;font-size:14px;color:#ffffff;}");
    connect(btn,SIGNAL(clicked(bool)),qApp,SLOT(quit()));

    frameLayout->addWidget(titlebarWid);
    frameLayout->addWidget(upWid);
    frameLayout->addWidget(downWid);
    frameLayout->addWidget(closeWid);

    //右键菜单
    menu=new QMenu;
    menu->setLayoutDirection(Qt::LeftToRight);
    menu->setFixedSize(100+2,36*5+12);
    char style[200];
    sprintf(style, "QMenu{padding-left:2px;padding-top:6px;padding-right:2px;padding-bottom:6px;border:1px solid #626c6e;border-radius:3px;background-color:%s;}",
            RightClickMenuBackground);
    menu->setStyleSheet(style);
    menu->setAttribute(Qt::WA_TranslucentBackground);
    menu->setWindowOpacity(RightClickMenuOpacity);

    cutAction=new QWidgetAction(this);
    cutWid=new QWidget;
    init_widget_action(cutWid,"剪切(T)",false);
    cutAction->setDefaultWidget(cutWid);
    menu->addAction(cutAction);
    connect(cutAction,SIGNAL(triggered()),this,SLOT(cutaction_trigger_slot()));
    cutAction->setEnabled(false);

    copyAction=new QWidgetAction(menu);
    copyWid=new QWidget;
    init_widget_action(copyWid,"复制(C)",true);
    copyAction->setDefaultWidget(copyWid);
    menu->addAction(copyAction);
    connect(copyAction,SIGNAL(triggered()),this,SLOT(copyaction_trigger_slot()));

    pasteAction=new QWidgetAction(menu);
    pasteWid=new QWidget;
    init_widget_action(pasteWid,"粘贴(V)",false);
    pasteAction->setDefaultWidget(pasteWid);
    menu->addAction(pasteAction);
    connect(pasteAction,SIGNAL(triggered()),this,SLOT(pasteaction_trigger_slot()));
    pasteAction->setEnabled(false);

    deleteAction=new QWidgetAction(menu);
    deleteWid=new QWidget;
    init_widget_action(deleteWid,"删除(D)",false);
    deleteAction->setDefaultWidget(deleteWid);
    menu->addAction(deleteAction);
    connect(deleteAction,SIGNAL(triggered()),this,SLOT(deleteaction_trigger_slot()));
    deleteAction->setEnabled(false);

    selectallAction=new QWidgetAction(menu);
    selectallWid=new QWidget;
    init_widget_action(selectallWid,"全选(A)",true);
    selectallAction->setDefaultWidget(selectallWid);
    menu->addAction(selectallAction);
    connect(selectallAction,SIGNAL(triggered()),this,SLOT(selectallaction_trigger_slot()));

    pUkuiMenuInterface=new UkuiMenuInterface;

    wm=new WindowMove;
    wm->move(0,0);
    wm->setParent(this);
    wm->resize(this->width()-45,30);
}

void AttributeDialog::set_controls_style(QLabel *edit, QLineEdit *valueEdit, QString str, int height)
{
//    QByteArray byte=str.toLocal8Bit();
//    char* arg=byte.data();
    edit->setText(str+":");
    edit->setStyleSheet("background:transparent;color:#ffffff;font-size:14px;border:0px;");
    edit->setFixedSize(50,height);
//    edit->setAlignment(Qt::AlignLeft);
    edit->setEnabled(false);
    edit->setContextMenuPolicy(Qt::NoContextMenu);
    edit->setFocusPolicy(Qt::NoFocus);

    valueEdit->setFixedSize(340-50,height);
    valueEdit->setStyleSheet("border:0px;background:transparent;color:#ffffff;font-size:14px;");
//    valueEdit->setReadOnly(false);
//    valueEdit->setEnabled(false);
    valueEdit->setAcceptDrops(false);
//    valueEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    valueEdit->setFocusPolicy(Qt::ClickFocus);
//    valueEdit->setContextMenuPolicy(Qt::CustomContextMenu);
//    connect(valueEdit,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(show_menu()));
}

void AttributeDialog::set_attribute_value(QString desktopfp)
{
    QString appname=pUkuiMenuInterface->get_app_name(desktopfp);
    QString iconstr=pUkuiMenuInterface->get_app_icon(desktopfp);
    QIcon icon=QIcon::fromTheme(iconstr);
    QPixmap pixmapicon(icon.pixmap(QSize(58,58)));
    labelIcon->setPixmap(pixmapicon);
    labelIcon->setFixedSize(58,58);
    this->labelappName->setText(appname);
    QString type=pUkuiMenuInterface->get_app_type(desktopfp);
    QString exec=pUkuiMenuInterface->get_app_exec(desktopfp);
    QString comment=pUkuiMenuInterface->get_app_comment(desktopfp);
    typevalueEdit->setText(type);
    execvalueEdit->setText(exec);
    commentvalueEdit->setText(comment);
    execvalueEdit->setCursorPosition(0);
    commentvalueEdit->setCursorPosition(0);
}

void AttributeDialog::show_menu()
{
    menu->exec(QCursor::pos());
}

void AttributeDialog::init_widget_action(QWidget *wid, QString textstr,bool enabled)
{
    char style[200];
    sprintf(style,"QWidget{background:transparent;border:0px;border-radius:2px;}\
            QWidget:hover{background-color:%s;}",
            RightClickMenuHover);

    char labelstyle[100];
    if(enabled)
        sprintf(labelstyle,"background:transparent;border:0px;color:%s;font-size:14px;",
                RightClickMenuFont);
    else
        sprintf(labelstyle,"background:transparent;border:0px;color:rgba(255, 255, 255, 0.35);font-size:14px;");

    QHBoxLayout* layout=new QHBoxLayout(wid);
    wid->setLayout(layout);
    wid->setFixedSize(96,36);
    wid->setStyleSheet(style);
    wid->setFocusPolicy(Qt::NoFocus);

    QLabel* labeltext=new QLabel(wid);
    labeltext->setStyleSheet(labelstyle);
    QByteArray textbyte=textstr.toLocal8Bit();
    char* text=textbyte.data();
    labeltext->setText(tr(text));
    labeltext->adjustSize();
    layout->addWidget(labeltext);

    layout->setContentsMargins(10,0,0,0);
}

void AttributeDialog::cutaction_trigger_slot()
{
    qDebug()<<"---1---";
}

void AttributeDialog::copyaction_trigger_slot()
{
    qDebug()<<"---2---";
    QTextEdit* textEdit=dynamic_cast<QTextEdit*>(QObject::sender());
    textEdit->copy();
}

void AttributeDialog::pasteaction_trigger_slot()
{
    qDebug()<<"---3---";
}

void AttributeDialog::deleteaction_trigger_slot()
{
    qDebug()<<"---4---";
}

void AttributeDialog::selectallaction_trigger_slot()
{
    qDebug()<<"---5---";
//    QLineEdit* textEdit=dynamic_cast<QLineEdit*>(QObject::sender());
//    textEdit->selectAll();
}
