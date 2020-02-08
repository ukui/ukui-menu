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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QDebug>
#include <QDesktopWidget>
#include <QtSingleApplication>
#include "src/Style/style.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pUkuiMenuInterface=new UkuiMenuInterface;
    UkuiMenuInterface::appInfoVector=pUkuiMenuInterface->create_appinfo_vector();
    Style::init_wid_style();
    init_mainwindow();

    pEnterAnimation=new QPropertyAnimation;
    pEnterAnimation->setTargetObject(this);
    pEnterAnimation->setPropertyName("size");
    pEnterAnimation->setDuration(500);
    pEnterAnimation->setStartValue(QSize(390,532));
    pEnterAnimation->setEndValue(QSize(490,532));
    pEnterAnimation->setEasingCurve(QEasingCurve::Linear);

    pLeaveAnimation=new QPropertyAnimation;
    pLeaveAnimation->setTargetObject(this);
    pLeaveAnimation->setPropertyName("size");
    pLeaveAnimation->setDuration(500);
    pLeaveAnimation->setStartValue(QSize(490,532));
    pLeaveAnimation->setEndValue(QSize(390,532));
    pLeaveAnimation->setEasingCurve(QEasingCurve::Linear);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete pUkuiMenuInterface;
}

void MainWindow::init_mainwindow()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::SplashScreen);
//    this->setStyleSheet("background:transparent;");
//    char style[100];
//    sprintf(style, "border:0px;background-color:%s;",DefaultBackground);
//    this->setStyleSheet(QString::fromLocal8Bit(style));
//    this->setWindowOpacity(0.95);//设置总体透明度
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAutoFillBackground(false);
    this->setFocusPolicy(Qt::ClickFocus);

    ui->mainToolBar->hide();
    ui->menuBar->hide();
    ui->statusBar->hide();
    this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
//    this->setMinimumSize(390+20+2,532+20+2);
//    this->setContentsMargins(1,21,21,1);
    this->setMinimumSize(376+2,590+2);
    this->setContentsMargins(0,2,2,0);

    frame=new QFrame(this);
    sidebarwid=new SideBarWidget(this);
    mainviewwid=new MainViewWidget(this);

    this->setCentralWidget(frame);
    mainlayout=new QHBoxLayout;

    mainlayout->addWidget(mainviewwid);

    line=new QFrame(this);
    line->setFrameShape(QFrame::VLine);
    line->setFixedSize(1,this->height());
    mainlayout->addWidget(line);
    char linestyle[100];
    sprintf(linestyle, "background-color:%s;",LineBackground);
    line->setStyleSheet(linestyle);

    mainlayout->addWidget(sidebarwid);
    mainlayout->setContentsMargins(0,0,0,0);
    mainlayout->setSpacing(0);
    centralWidget()->setLayout(mainlayout);
    char style[100];
    sprintf(style, "border:0px;background-color:%s;border-top-right-radius:6px;",DefaultBackground);
    frame->setStyleSheet(style);

//    connect(sidebarwid,SIGNAL(send_hover_signal(bool)),this,SLOT(recv_hover_signal_slot(bool)));
    connect(sidebarwid, SIGNAL(send_commonusebtn_signal()), mainviewwid, SLOT(load_commonuse_widget()));
    connect(sidebarwid,SIGNAL(send_letterbtn_signal()), mainviewwid, SLOT(load_letter_widget()));
    connect(sidebarwid, SIGNAL(send_functionbtn_signal()), mainviewwid, SLOT(load_function_widget()));

    connect(sidebarwid,SIGNAL(send_fullscreen_commonusebtn_signal()),
            mainviewwid,SLOT(load_fullcommonuse_widget()));
    connect(sidebarwid,SIGNAL(send_fullscreen_letterbtn_signal()),
            mainviewwid,SLOT(load_fullletter_widget()));
    connect(sidebarwid, SIGNAL(send_fullscreen_functionbtn_signal()),
            mainviewwid, SLOT(load_fullfunction_widget()));

    connect(mainviewwid,SIGNAL(send_querylineEdit_focusin_signal()),sidebarwid,SLOT(recv_querylineEdit_focusin_slot()));

    connect(sidebarwid,SIGNAL(send_fullscreenbtn_signal()),this,SLOT(show_fullscreen_widget()));
    connect(sidebarwid, SIGNAL(send_defaultbtn_signal()),this,SLOT(show_default_widget()));
    connect(mainviewwid,SIGNAL(send_hide_mainwindow_signal()),this,SLOT(recv_hide_mainwindow_slot()));
    connect(sidebarwid,SIGNAL(send_hide_mainwindow_signal()),this,SLOT(recv_hide_mainwindow_slot()));

    connect(QApplication::desktop(),SIGNAL(resized(int)),this,SLOT(monitor_resolution_change(int)));
}

/**
 * 添加阴影
 */
void MainWindow::paintEvent(QPaintEvent *)
{
    if(!is_fullscreen)
    {
//        QPainter painter(this);
//        const qreal radius =6;

//        QRectF rect1;
//        rect1.setX(0);
//        rect1.setY(2);
//        rect1.setWidth( this->rect().width()-2);
//        rect1.setHeight(this->rect().height()-2);

//        //加边框
//        QPainterPath path1;
//        path1.setFillRule(Qt::WindingFill);
//        painter.setRenderHint(QPainter::Antialiasing);

//        path1.moveTo(rect1.topRight() - QPointF(radius, 0));
//        path1.lineTo(rect1.topLeft());
//        path1.lineTo(rect1.bottomLeft());
//        path1.lineTo(rect1.bottomRight());
//        path1.lineTo(rect1.topRight() + QPointF(0, radius));
//        path1.quadTo(rect1.topRight(), rect1.topRight() + QPointF(-radius, -0));
//        QColor color;
//        color.setNamedColor(QString::fromLocal8Bit(BORDERCOLOR));
//        painter.setPen(QPen(color,2));
//        painter.setOpacity(0.95);
//        painter.setBrush(Qt::NoBrush);
//        painter.drawPath(path1);

//        QRectF rect;
    //    rect.setX(this->rect().x());
    //    rect.setY(this->rect().y());
    //    rect.setWidth( this->rect().width());
    //    rect.setHeight(this->rect().height());

        //加阴影
    //    QPainterPath path;
    //    path.setFillRule(Qt::WindingFill);
    //    char shadowcolor[20];
    //    sprintf(shadowcolor,"%s", SHADOWCOLOR);
    //    QColor color(QString::fromLocal8Bit(shadowcolor));
    //    for(int i=0; i<9; i++)
    //    {
    //        rect.setX(0);
    //        rect.setY((9-i));
    //        rect.setWidth( this->rect().width()-(9-i));
    //        rect.setHeight(this->rect().height()-(9-i));

    //        path.setFillRule(Qt::WindingFill);
    //        path.moveTo(rect.topRight() - QPointF(radius, 0));
    //        path.lineTo(rect.topLeft());
    //        path.lineTo(rect.bottomLeft());
    //        path.lineTo(rect.bottomRight());
    //        path.lineTo(rect.topRight() + QPointF(0, radius));
    //        path.quadTo(rect.topRight(), rect.topRight() + QPointF(-radius, -0));
    //        color.setAlpha(static_cast<int>(150 - qSqrt(i)*50));
    ////        color.setAlphaF(1.0*(20-i)/70);

    //        painter.setPen(QPen(color));
    //        painter.setBrush(Qt::NoBrush);
    //        painter.drawPath(path);
    //    }
    }
}

/**
 * 显示全屏窗口
 */
void MainWindow::show_fullscreen_widget()
{
////    this->showMaximized();
//    is_full=true;
//    classification_widget=arg;
//    sidebarwid->setVisible(false);
//    mainviewwid->setVisible(false);
//    char widgetcolor[100];
//    sprintf(widgetcolor, "border:0px;background-color:%s;",MAINVIEWWIDGETCOLOR);
//    mainwidget->setStyleSheet(QString::fromLocal8Bit(widgetcolor));
//    pAnimation->start();

    is_fullscreen=true;
    this->setContentsMargins(0,0,0,0);
    this->setGeometry(QApplication::desktop()->availableGeometry());
    sidebarwid->load_max_sidebar();
    mainviewwid->load_max_mainview();
    //移除分割线
    mainlayout->removeWidget(line);
    line->setParent(nullptr);
    this->repaint();
//    mainviewwid->load_full_classification_widget(widgetState);

    char style[100];
    sprintf(style, "border:0px;background-color:%s;border-top-right-radius:0px;",DefaultBackground);
//    mainwidget->setStyleSheet(style);
    frame->setStyleSheet(style);
}

/**
 * 显示默认窗口
 */
void MainWindow::show_default_widget()
{
////    this->showNormal();
//    is_full=false;
//    classification_widget=arg;
//    sidebarwid->setVisible(false);
//    mainviewwid->setVisible(false);
//    char widgetcolor[100];
//    sprintf(widgetcolor, "border:0px;background-color:%s;",MAINVIEWWIDGETCOLOR);
//    mainwidget->setStyleSheet(QString::fromLocal8Bit(widgetcolor));
//    pAnimation->start();

    is_fullscreen=false;
    this->setContentsMargins(0,2,2,0);
//    this->setGeometry(QRect(0,QApplication::desktop()->availableGeometry().height()-532,390+20+2,532+20+2));
    this->setGeometry(QRect(0,QApplication::desktop()->availableGeometry().height()-590-2,376+2,590+2));
    sidebarwid->load_min_sidebar();
    mainviewwid->load_min_mainview();
    //插入分割线
    mainlayout->insertWidget(1,line);
    this->repaint();
//    mainviewwid->load_classification_widget(widgetState);

    char style[100];
    sprintf(style, "border:0px;background-color:%s;border-top-right-radius:6px;",DefaultBackground);
//    mainwidget->setStyleSheet(style);
    frame->setStyleSheet(style);
}

/**
 * 鼠标点击窗口外部事件
 */
bool MainWindow::event ( QEvent * event )
{
   if (event->type() == QEvent::ActivationChange)
   {
        if(QApplication::activeWindow() != this)
        {
            this->hide();
//            qDebug()<<qApp->applicationState();
//            this->setWindowState(this->windowState() & Qt::WindowMinimized);
            mainviewwid->widget_make_zero();
            sidebarwid->widget_make_zero();
        }
   }
   return QWidget::event(event);
}

/**
 * 隐藏窗口
 */
void MainWindow::recv_hide_mainwindow_slot()
{
    this->hide();
    mainviewwid->widget_make_zero();
    sidebarwid->widget_make_zero();
}

//void MainWindow::recv_hover_signal_slot(bool is_hover)
//{
//    if(is_hover)
//        pEnterAnimation->start();
//    else
//        pLeaveAnimation->start();
//}

void MainWindow::monitor_resolution_change(int screen)
{
    qApp->quit();
    QProcess::startDetached(QString("/home/kylin/ukui-start-menu/bin/ukui-menu"));
}

void MainWindow::changeEvent(QEvent *e)
{
//    QWindowStateChangeEvent* event=static_cast<QWindowStateChangeEvent*>(e);
//    if(!(event->oldState() & Qt::ApplicationHidden) && (windowState() & Qt::ApplicationActive))
//    if(windowState() & Qt::WindowMinimized)
//    {
//        this->hide();
//    }
//    QMainWindow::changeEvent(e);
}
