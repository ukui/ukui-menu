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
#include <QtX11Extras/QX11Info>
#include <syslog.h>
#include "src/XEventMonitor/xeventmonitor.h"
#include "src/Style/style.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pUkuiMenuInterface=new UkuiMenuInterface;
    UkuiMenuInterface::appInfoVector=pUkuiMenuInterface->createAppInfoVector();
    UkuiMenuInterface::alphabeticVector=pUkuiMenuInterface->getAlphabeticClassification();
    UkuiMenuInterface::functionalVector=pUkuiMenuInterface->getFunctionalClassification();
    UkuiMenuInterface::allAppVector=pUkuiMenuInterface->getAllApp();
    Style::initWidStyle();
    QString path=QDir::homePath()+"/.config/ukui/ukui-menu.ini";
    setting=new QSettings(path,QSettings::IniFormat);
    initMainWindow();
}

MainWindow::~MainWindow()
{
    XEventMonitor::instance()->quit();
    delete ui;
    delete pUkuiMenuInterface;
}

void MainWindow::initMainWindow()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAutoFillBackground(false);
    this->setFocusPolicy(Qt::StrongFocus);

//    QPainterPath path;
//    auto rect = this->rect();
//    rect.adjust(1, 1, -1, -1);
//    path.addRoundedRect(rect, 6, 6);
//    setProperty("blurRegion", QRegion(path.toFillPolygon().toPolygon()));

    ui->menubar->hide();
    ui->statusbar->hide();
    this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    this->setMinimumSize(376,590);
    this->setContentsMargins(0,0,0,0);

    frame=new QFrame(this);
    mainviewwid=new MainViewWidget(this);
    sidebarwid=new SideBarWidget(this);

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
    int position=0;
    int panelSize=0;
    if(QGSettings::isSchemaInstalled(QString("org.ukui.panel.settings").toLocal8Bit()))
    {
        QGSettings* gsetting=new QGSettings(QString("org.ukui.panel.settings").toLocal8Bit());
        if(gsetting->keys().contains(QString("panelposition")))
            position=gsetting->get("panelposition").toInt();
        else
            position=0;
        if(gsetting->keys().contains(QString("panelsize")))
            panelSize=gsetting->get("panelsize").toInt();
        else
            panelSize=46;

        connect(gsetting,SIGNAL(changed(QString)),
                this,SLOT(panelChangedSlot(QString)));
    }
    else
    {
        position=0;
        panelSize=46;
    }
    char style[100];
    if(position==0)
        sprintf(style, "border:0px;background-color:%s;border-top-right-radius:6px;",DefaultBackground);
    else if(position==1 || position==2)
        sprintf(style, "border:0px;background-color:%s;border-bottom-right-radius:6px;",DefaultBackground);
    else
        sprintf(style, "border:0px;background-color:%s;border-bottom-left-radius:6px;",DefaultBackground);
    frame->setStyleSheet(style);

    pAnimation = new QPropertyAnimation(this, "geometry");
    connect(pAnimation,SIGNAL(stateChanged(QAbstractAnimation::State, QAbstractAnimation::State)),
            this,SLOT(stateChangedSlot(QAbstractAnimation::State,QAbstractAnimation::State)));

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

    connect(QApplication::primaryScreen(),SIGNAL(geometryChanged(QRect)),
            this,SLOT(monitorResolutionChange(QRect)));
    connect(qApp,SIGNAL(primaryScreenChanged(QScreen*)),this,
            SLOT(primaryScreenChangedSlot(QScreen*)));

    XEventMonitor::instance()->start();
    connect(XEventMonitor::instance(), SIGNAL(keyRelease(const QString &)),
            this, SLOT(XkbEventsRelease(const QString &)));
    connect(XEventMonitor::instance(), SIGNAL(keyPress(const QString &)),
            this, SLOT(XkbEventsPress(const QString &)));

}

/**
 * 添加阴影
 */
//void MainWindow::paintEvent(QPaintEvent *)
//{
//    if(!is_fullscreen)
//    {
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
//    }
//}

/**
 * 显示全屏窗口
 */
void MainWindow::showFullScreenWidget()
{
    is_fullscreen=true;
    this->setContentsMargins(0,0,0,0);
    int position=0;
    int panelSize=0;
    if(QGSettings::isSchemaInstalled(QString("org.ukui.panel.settings").toLocal8Bit()))
    {
        QGSettings* gsetting=new QGSettings(QString("org.ukui.panel.settings").toLocal8Bit());
        if(gsetting->keys().contains(QString("panelposition")))
            position=gsetting->get("panelposition").toInt();
        else
            position=0;
        if(gsetting->keys().contains(QString("panelsize")))
            panelSize=gsetting->get("panelsize").toInt();
        else
            panelSize=46;
    }
    else
    {
        position=0;
        panelSize=46;
    }
    int x=QApplication::primaryScreen()->geometry().x();
    int y=QApplication::primaryScreen()->geometry().y();
    QRect startRect;
    QRect endRect;
    if(position==0)
    {
        startRect.setRect(x,Style::heightavailable-590,376,590);
        endRect.setRect(x,y,Style::widthavailable,Style::heightavailable);
    }
    else if(position==1)
    {
        startRect.setRect(x,panelSize,376,590);
        endRect.setRect(x,panelSize,Style::widthavailable,Style::heightavailable);
    }
    else if(position==2)
    {
        startRect.setRect(panelSize,y,376,590);
        endRect.setRect(panelSize,y,Style::widthavailable,Style::heightavailable);
    }
    else
    {
        startRect.setRect(Style::widthavailable-376,y,376,590);
        endRect.setRect(x,y,Style::widthavailable,Style::heightavailable);
    }

    mainlayout->removeWidget(mainviewwid);
    mainviewwid->setParent(nullptr);
    mainlayout->removeWidget(line);
    line->setParent(nullptr);
    mainlayout->removeWidget(sidebarwid);
    sidebarwid->setParent(nullptr);

    pAnimation->setDuration(100);//动画总时间
    pAnimation->setStartValue(startRect);
    pAnimation->setEndValue(endRect);
    pAnimation->setEasingCurve(QEasingCurve::Linear);
    pAnimation->start();
}

/**
 * 显示默认窗口
 */
void MainWindow::showDefaultWidget()
{
    is_fullscreen=false;
    this->setContentsMargins(0,0,0,0);
    int position=0;
    int panelSize=0;
    if(QGSettings::isSchemaInstalled(QString("org.ukui.panel.settings").toLocal8Bit()))
    {
        QGSettings* gsetting=new QGSettings(QString("org.ukui.panel.settings").toLocal8Bit());
        if(gsetting->keys().contains(QString("panelposition")))
            position=gsetting->get("panelposition").toInt();
        else
            position=0;
        if(gsetting->keys().contains(QString("panelsize")))
            panelSize=gsetting->get("panelsize").toInt();
        else
            panelSize=46;
    }
    else
    {
        position=0;
        panelSize=46;
    }
    char style[100];
    int x=QApplication::primaryScreen()->geometry().x();
    int y=QApplication::primaryScreen()->geometry().y();
    QRect startRect;
    QRect endRect;
    if(position==0)
    {
        endRect.setRect(x,Style::heightavailable-590,376,590);
        startRect.setRect(x,y,Style::widthavailable,Style::heightavailable);
        sprintf(style, "border:0px;background-color:%s;border-top-right-radius:6px;",DefaultBackground);
    }
    else if(position==1)
    {
        endRect.setRect(x,panelSize,376,590);
        startRect.setRect(x,panelSize,Style::widthavailable,Style::heightavailable);
        sprintf(style, "border:0px;background-color:%s;border-bottom-right-radius:6px;",DefaultBackground);
    }
    else if(position==2)
    {
        endRect.setRect(panelSize,y,376,590);
        startRect.setRect(panelSize,y,Style::widthavailable,Style::heightavailable);
        sprintf(style, "border:0px;background-color:%s;border-bottom-right-radius:6px;",DefaultBackground);
    }
    else
    {
        endRect.setRect(Style::widthavailable-376,y,376,590);
        startRect.setRect(x,y,Style::widthavailable,Style::heightavailable);
        sprintf(style, "border:0px;background-color:%s;border-bottom-left-radius:6px;",DefaultBackground);
    }

    mainlayout->removeWidget(mainviewwid);
    mainviewwid->setParent(nullptr);
    mainlayout->removeWidget(sidebarwid);
    sidebarwid->setParent(nullptr);

    pAnimation->setDuration(100);//动画总时间
    pAnimation->setStartValue(startRect);
    pAnimation->setEndValue(endRect);
    pAnimation->setEasingCurve(QEasingCurve::Linear);
    pAnimation->start();
}

void MainWindow::stateChangedSlot(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    Q_UNUSED(oldState);
    if(is_fullscreen && newState==QAbstractAnimation::Stopped)
    {
        mainlayout->addWidget(mainviewwid);
        mainlayout->addWidget(sidebarwid);
        sidebarwid->loadMaxSidebar();
        mainviewwid->loadMaxMainView();
        sidebarwid->enterAnimation();
        setFrameStyle();
    }
    if(!is_fullscreen && newState==QAbstractAnimation::Stopped)
    {
        mainlayout->addWidget(mainviewwid);
        mainlayout->addWidget(line);
        mainlayout->addWidget(sidebarwid);
        sidebarwid->loadMinSidebar();
        mainviewwid->loadMinMainView();
        setFrameStyle();
    }
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
            mainviewwid->widgetMakeZero();
            sidebarwid->widgetMakeZero();
        }
   }
   return QWidget::event(event);
}

void MainWindow::XkbEventsPress(const QString &keycode)
{
    QString KeyName;
    if (keycode.length() >= 8){
        KeyName = keycode.left(8);
    }
    if(KeyName.compare("Super_L+")==0){
        WinFlag = true;
    }
    if(WinFlag && keycode == "Super_L"){
        WinFlag = false;
        return;
    }

}

void MainWindow::XkbEventsRelease(const QString &keycode)
{
    QString KeyName;
    static bool winFlag=false;
    if (keycode.length() >= 8){
        KeyName = keycode.left(8);
    }
    if(KeyName.compare("Super_L+")==0){
        winFlag = true;
    }
    if(winFlag && keycode == "Super_L"){
        winFlag = false;
        return;
    }else if(WinFlag && keycode == "Super_L")
        return;

    if((keycode == "Super_L") || (keycode == "Super_R"))
    {
        if(QApplication::activeWindow() == this)
        {
            this->hide();
            mainviewwid->widgetMakeZero();
            sidebarwid->widgetMakeZero();
        }
        else{
            this->loadMainWindow();
            this->show();
            this->raise();
            this->activateWindow();
        }
    }

    if(keycode == "Escape")
    {
        this->hide();
        mainviewwid->widgetMakeZero();
        sidebarwid->widgetMakeZero();
    }
}

/**
 * 隐藏窗口
 */
void MainWindow::recvHideMainWindowSlot()
{
    this->hide();
    mainviewwid->widgetMakeZero();
    sidebarwid->widgetMakeZero();
}

void MainWindow::loadMainWindow()
{
    QDateTime dt=QDateTime::currentDateTime();
    int currentDateTime=dt.toTime_t();
    int nDaySec=24*60*60;
    setting->beginGroup("recentapp");
    QStringList recentAppKeys=setting->allKeys();
    for(int i=0;i<recentAppKeys.count();i++)
    {
        if((currentDateTime-setting->value(recentAppKeys.at(i)).toInt())/nDaySec >= 3)
            setting->remove(recentAppKeys.at(i));
    }
    setting->sync();
    setting->endGroup();

    int position=0;
    int panelSize=0;
    if(QGSettings::isSchemaInstalled(QString("org.ukui.panel.settings").toLocal8Bit()))
    {
        QGSettings* gsetting=new QGSettings(QString("org.ukui.panel.settings").toLocal8Bit());
        if(gsetting->keys().contains(QString("panelposition")))
            position=gsetting->get("panelposition").toInt();
        else
            position=0;
        if(gsetting->keys().contains(QString("panelsize")))
            panelSize=gsetting->get("panelsize").toInt();
        else
            panelSize=46;
    }
    else
    {
        position=0;
        panelSize=46;
    }
    int x=QApplication::primaryScreen()->geometry().x();
    int y=QApplication::primaryScreen()->geometry().y();
    //默认开启默认态
    if(QGSettings::isSchemaInstalled(QString("org.ukui.control-center.desktop").toLocal8Bit()))
    {
        QGSettings* gsetting=new QGSettings(QString("org.ukui.control-center.desktop").toLocal8Bit());
        bool ret=false;
        if(gsetting->keys().contains(QString("menufullScreen")))
            ret=gsetting->get("menufull-screen").toBool();
        if(ret)
        {
            if(position==0)
                this->setGeometry(QRect(x,y,QApplication::primaryScreen()->geometry().width(),QApplication::primaryScreen()->geometry().height()-panelSize));
            else if(position==1)
                this->setGeometry(QRect(x,panelSize,QApplication::primaryScreen()->geometry().width(),QApplication::primaryScreen()->geometry().height()-panelSize));
            else if(position==2)
                this->setGeometry(QRect(panelSize,y,QApplication::primaryScreen()->geometry().width()-panelSize,QApplication::primaryScreen()->geometry().height()));
            else
                this->setGeometry(QRect(x,y,QApplication::primaryScreen()->geometry().width()-panelSize,QApplication::primaryScreen()->geometry().height()));
            mainlayout->removeWidget(line);
            line->setParent(nullptr);
            sidebarwid->loadMaxSidebar();
            mainviewwid->loadMaxMainView();
            sidebarwid->enterAnimation();
            is_fullscreen=true;
            setFrameStyle();
        }
        else
        {
            if(position==0)
                this->setGeometry(QRect(x,QApplication::primaryScreen()->geometry().height()-panelSize-590,
                                          376,590));
            else if(position==1)
                this->setGeometry(QRect(x,panelSize,376,590));
            else if(position==2)
                this->setGeometry(QRect(panelSize,y,376,590));
            else
                this->setGeometry(QRect(QApplication::primaryScreen()->geometry().width()-panelSize-376,y,
                                          376,590));
            mainlayout->insertWidget(1,line);
            sidebarwid->loadMinSidebar();
            mainviewwid->loadMinMainView();
            is_fullscreen=false;
            setFrameStyle();
        }
    }
    else
    {
        if(position==0)
            this->setGeometry(QRect(x,QApplication::primaryScreen()->geometry().height()-panelSize-590,
                                      376,590));
        else if(position==1)
            this->setGeometry(QRect(x,panelSize,376,590));
        else if(position==2)
            this->setGeometry(QRect(panelSize,y,376,590));
        else
            this->setGeometry(QRect(QApplication::primaryScreen()->geometry().width()-panelSize-376,y,
                                      376,590));
        mainlayout->insertWidget(1,line);
        sidebarwid->loadMinSidebar();
        mainviewwid->loadMinMainView();
        is_fullscreen=false;
        setFrameStyle();
    }
}

void MainWindow::monitorResolutionChange(QRect rect)
{
    Q_UNUSED(rect);
    Style::initWidStyle();
    mainviewwid->repaintWidget();
}

void MainWindow::primaryScreenChangedSlot(QScreen *screen)
{
    Q_UNUSED(screen);
    Style::initWidStyle();
    mainviewwid->repaintWidget();
}

void MainWindow::panelChangedSlot(QString key)
{
    Q_UNUSED(key);
    Style::initWidStyle();
    mainviewwid->repaintWidget();
}

void MainWindow::setFrameStyle()
{
    int position=0;
    int panelSize=0;
    if(QGSettings::isSchemaInstalled(QString("org.ukui.panel.settings").toLocal8Bit()))
    {
        QGSettings* gsetting=new QGSettings(QString("org.ukui.panel.settings").toLocal8Bit());
        if(gsetting->keys().contains(QString("panelposition")))
            position=gsetting->get("panelposition").toInt();
        else
            position=0;
        if(gsetting->keys().contains(QString("panelsize")))
            panelSize=gsetting->get("panelsize").toInt();
        else
            panelSize=46;
    }
    else
    {
        position=0;
        panelSize=46;
    }
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

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if(e->type()==KeyPress+4)
    {
        QKeyEvent* ke=static_cast<QKeyEvent*>(e);
        if((ke->key()>=0x30 && ke->key()<=0x39) || (ke->key()>=0x41 && ke->key()<=0x5a))
        {
            mainviewwid->setLineEditFocus(e->text());
        }

        switch(e->key()){
        case Qt::Key_Up:
            mainviewwid->moveScrollBar(0);
            break;
        case Qt::Key_Down:
            mainviewwid->moveScrollBar(1);
            break;
        default:
            break;
        }
    }
}
