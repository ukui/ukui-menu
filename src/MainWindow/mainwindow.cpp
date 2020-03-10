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
    Style::initWidStyle();
    QString path=QDir::homePath()+"/.config/ukui/ukui-menu.ini";
    setting=new QSettings(path,QSettings::IniFormat);
    //获取当前时间戳
    QDateTime dt=QDateTime::currentDateTime();
    int currentDateTime=dt.toTime_t();
    int nDaySec=24*60*60;
    setting->beginGroup("datetime");
    QStringList dateTimeKeys=setting->allKeys();
    QStringList timeOutKeys;
    timeOutKeys.clear();
    for(int i=0;i<dateTimeKeys.count();i++)
    {
        if((currentDateTime-setting->value(dateTimeKeys.at(i)).toInt())/nDaySec >= 7)
        {
            timeOutKeys.append(dateTimeKeys.at(i));
        }

    }
    setting->endGroup();
    for(int i=0;i<timeOutKeys.count();i++)
    {
        setting->beginGroup("application");
        setting->remove(timeOutKeys.at(i));
        setting->sync();
        setting->endGroup();
        setting->beginGroup("datetime");
        setting->remove(timeOutKeys.at(i));
        setting->sync();
        setting->endGroup();
    }

    setting->beginGroup("recentapp");
    QStringList recentAppKeys=setting->allKeys();
    for(int i=0;i<recentAppKeys.count();i++)
    {
        if((currentDateTime-setting->value(recentAppKeys.at(i)).toInt())/nDaySec >= 7)
            setting->remove(recentAppKeys.at(i));
    }
    setting->sync();
    setting->endGroup();

    initMainWindow();

//    pEnterAnimation=new QPropertyAnimation;
//    pEnterAnimation->setTargetObject(this);
//    pEnterAnimation->setPropertyName("size");
//    pEnterAnimation->setDuration(500);
//    pEnterAnimation->setStartValue(QSize(390,532));
//    pEnterAnimation->setEndValue(QSize(490,532));
//    pEnterAnimation->setEasingCurve(QEasingCurve::Linear);

//    pLeaveAnimation=new QPropertyAnimation;
//    pLeaveAnimation->setTargetObject(this);
//    pLeaveAnimation->setPropertyName("size");
//    pLeaveAnimation->setDuration(500);
//    pLeaveAnimation->setStartValue(QSize(490,532));
//    pLeaveAnimation->setEndValue(QSize(390,532));
//    pLeaveAnimation->setEasingCurve(QEasingCurve::Linear);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete pUkuiMenuInterface;
}

void MainWindow::initMainWindow()
{
//    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::SplashScreen);
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::Popup);
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
    this->setMinimumSize(376,590);
    this->setContentsMargins(0,0,0,0);

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
    QDBusInterface iface("com.ukui.panel.desktop",
                         "/",
                         "com.ukui.panel.desktop",
                         QDBusConnection::sessionBus());
    QDBusReply<int> position=iface.call("GetPanelPosition","");
    char style[100];
    if(position==0)
        sprintf(style, "border:0px;background-color:%s;border-top-right-radius:6px;",DefaultBackground);
    else if(position==1 || position==2)
        sprintf(style, "border:0px;background-color:%s;border-bottom-right-radius:6px;",DefaultBackground);
    else
        sprintf(style, "border:0px;background-color:%s;border-bottom-left-radius:6px;",DefaultBackground);
    frame->setStyleSheet(style);

    connect(sidebarwid, SIGNAL(sendCommonUseBtnSignal()), mainviewwid, SLOT(loadCommonUseWidget()));
    connect(sidebarwid,SIGNAL(sendLetterBtnSignal()), mainviewwid, SLOT(loadLetterWidget()));
    connect(sidebarwid, SIGNAL(sendFunctionBtnSignal()), mainviewwid, SLOT(loadFunctionWidget()));

    connect(sidebarwid,SIGNAL(sendFullScreenCommonUseBtnSignal()),
            mainviewwid,SLOT(loadFullCommonUseWidget()));
    connect(sidebarwid,SIGNAL(sendFullScreenLetterBtnSignal()),
            mainviewwid,SLOT(loadFullLetterWidget()));
    connect(sidebarwid, SIGNAL(sendFullScreenFunctionBtnSignal()),
            mainviewwid, SLOT(loadFullFunctionWidget()));

    connect(sidebarwid,SIGNAL(sendFullScreenBtnSignal()),this,SLOT(showFullScreenWidget()));
    connect(sidebarwid, SIGNAL(sendDefaultBtnSignal()),this,SLOT(showDefaultWidget()));
    connect(mainviewwid,SIGNAL(sendHideMainWindowSignal()),this,SLOT(recvHideMainWindowSlot()));
    connect(sidebarwid,SIGNAL(sendHideMainWindowSignal()),this,SLOT(recvHideMainWindowSlot()));

    connect(QApplication::desktop(),SIGNAL(resized(int)),this,SLOT(monitorResolutionChange(int)));
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
void MainWindow::showFullScreenWidget()
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
    QDBusInterface iface("com.ukui.panel.desktop",
                         "/",
                         "com.ukui.panel.desktop",
                         QDBusConnection::sessionBus());

    QDBusReply<int> position=iface.call("GetPanelPosition","");
    QDBusReply<int> panelSize=iface.call("GetPanelSize","");
    if(position==0)
        this->setGeometry(QRect(0,0,Style::widthavailable,Style::heightavailable));
    else if(position==1)
        this->setGeometry(QRect(0,panelSize,Style::widthavailable,Style::heightavailable));
    else if(position==2)
        this->setGeometry(QRect(panelSize,0,Style::widthavailable,Style::heightavailable));
    else
        this->setGeometry(QRect(0,0,Style::widthavailable,Style::heightavailable));
//    this->setGeometry(QApplication::desktop()->availableGeometry());
    sidebarwid->loadMaxSidebar();
    mainviewwid->loadMaxMainView();
    //移除分割线
    mainlayout->removeWidget(line);
    line->setParent(nullptr);
    this->repaint();

    char style[100];
    sprintf(style, "border:0px;background-color:%s;border-top-right-radius:0px;",DefaultBackground);
    frame->setStyleSheet(style);
}

/**
 * 显示默认窗口
 */
void MainWindow::showDefaultWidget()
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
    this->setContentsMargins(0,0,0,0);
    QDBusInterface iface("com.ukui.panel.desktop",
                         "/",
                         "com.ukui.panel.desktop",
                         QDBusConnection::sessionBus());

    QDBusReply<int> position=iface.call("GetPanelPosition","");
    QDBusReply<int> panelSize=iface.call("GetPanelSize","");
    char style[100];
    if(position==0)
    {
        sprintf(style, "border:0px;background-color:%s;border-top-right-radius:6px;",DefaultBackground);
        this->setGeometry(QRect(0,Style::heightavailable-590,376,590));
    }
    else if(position==1)
    {
        sprintf(style, "border:0px;background-color:%s;border-bottom-right-radius:6px;",DefaultBackground);
        this->setGeometry(QRect(0,panelSize,376,590));
    }
    else if(position==2)
    {
        sprintf(style, "border:0px;background-color:%s;border-bottom-right-radius:6px;",DefaultBackground);
        this->setGeometry(QRect(panelSize,0,376,590));
    }
    else
    {
        sprintf(style, "border:0px;background-color:%s;border-bottom-left-radius:6px;",DefaultBackground);
        this->setGeometry(QRect(Style::widthavailable-376,0,376,590));
    }
    sidebarwid->loadMinSidebar();
    mainviewwid->loadMinMainView();
    //插入分割线
    mainlayout->insertWidget(1,line);
    this->repaint();
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
//            mainviewwid->widgetMakeZero();
//            sidebarwid->widgetMakeZero();
        }
   }
   return QWidget::event(event);
}

/**
 * 隐藏窗口
 */
void MainWindow::recvHideMainWindowSlot()
{
    this->hide();
//    mainviewwid->widgetMakeZero();
//    sidebarwid->widgetMakeZero();
}

bool MainWindow::checkIfFullScreen()
{
    return is_fullscreen;
}

void MainWindow::mainWindowMakeZero()
{
    mainviewwid->widgetMakeZero();
    sidebarwid->widgetMakeZero();
}

//void MainWindow::recv_hover_signal_slot(bool is_hover)
//{
//    if(is_hover)
//        pEnterAnimation->start();
//    else
//        pLeaveAnimation->start();
//}

void MainWindow::monitorResolutionChange(int screen)
{
    Q_UNUSED(screen);
    qApp->quit();
    QProcess::startDetached(QString("/usr/bin/ukui-menu"));
}

void MainWindow::setFrameStyle()
{
    QDBusInterface iface("com.ukui.panel.desktop",
                         "/",
                         "com.ukui.panel.desktop",
                         QDBusConnection::sessionBus());

    QDBusReply<int> position=iface.call("GetPanelPosition","");
    char style[100];
    if(!is_fullscreen)
    {
        if(position==0)
        {
            sprintf(style, "border:0px;background-color:%s;border-top-right-radius:6px;",DefaultBackground);
        }
        else if(position==1)
        {
            sprintf(style, "border:0px;background-color:%s;border-bottom-right-radius:6px;",DefaultBackground);
        }
        else if(position==2)
        {
            sprintf(style, "border:0px;background-color:%s;border-bottom-right-radius:6px;",DefaultBackground);
        }
        else
        {
            sprintf(style, "border:0px;background-color:%s;border-bottom-left-radius:6px;",DefaultBackground);
        }
    }
    else {
        sprintf(style, "border:0px;background-color:%s;border-top-right-radius:0px;",DefaultBackground);
    }
    frame->setStyleSheet(style);
}
