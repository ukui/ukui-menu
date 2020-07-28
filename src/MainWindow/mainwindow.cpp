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
#include <QHBoxLayout>
#include <QDebug>
#include <QDesktopWidget>
#include <QtSingleApplication>
#include <QtX11Extras/QX11Info>
#include <syslog.h>
#include "src/XEventMonitor/xeventmonitor.h"
#include "src/Style/style.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    m_ukuiMenuInterface=new UkuiMenuInterface;
    UkuiMenuInterface::appInfoVector=m_ukuiMenuInterface->createAppInfoVector();
    UkuiMenuInterface::alphabeticVector=m_ukuiMenuInterface->getAlphabeticClassification();
    UkuiMenuInterface::functionalVector=m_ukuiMenuInterface->getFunctionalClassification();
    UkuiMenuInterface::allAppVector=m_ukuiMenuInterface->getAllApp();
    Style::initWidStyle();
    QString path=QDir::homePath()+"/.config/ukui/ukui-menu.ini";
    m_setting=new QSettings(path,QSettings::IniFormat);
    initUi();
}

MainWindow::~MainWindow()
{
    XEventMonitor::instance()->quit();
    delete m_ukuiMenuInterface;
}

void MainWindow::initUi()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAutoFillBackground(false);
    this->setFocusPolicy(Qt::StrongFocus);

    this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    this->setMinimumSize(Style::minw,Style::minh);
    this->setContentsMargins(0,0,0,0);

    m_frame=new QFrame;
    m_mainViewWid=new MainViewWidget;
    m_sideBarWid=new SideBarWidget;

    this->setCentralWidget(m_frame);
    QHBoxLayout *mainlayout=new QHBoxLayout;
    mainlayout->setContentsMargins(0,0,0,0);
    mainlayout->setSpacing(0);
    m_frame->setLayout(mainlayout);

    mainlayout->addWidget(m_mainViewWid);
    m_line=new QFrame(this);
    m_line->setFrameShape(QFrame::VLine);
    m_line->setFixedSize(1,this->height());
    mainlayout->addWidget(m_line);
    char linestyle[100];
    sprintf(linestyle, "background-color:%s;",LineBackground);
    m_line->setStyleSheet(linestyle);
    mainlayout->addWidget(m_sideBarWid);

    m_animation = new QPropertyAnimation(this, "geometry");
    connect(m_animation,&QPropertyAnimation::stateChanged,
            this,&MainWindow::stateChangedSlot);
    connect(m_animation, &QPropertyAnimation::valueChanged, this, &MainWindow::animationValueChangedSlot);

    connect(m_sideBarWid, &SideBarWidget::sendCommonUseBtnSignal, m_mainViewWid, &MainViewWidget::loadCommonUseWidget);
    connect(m_sideBarWid,&SideBarWidget::sendLetterBtnSignal, m_mainViewWid, &MainViewWidget::loadLetterWidget);
    connect(m_sideBarWid, &SideBarWidget::sendFunctionBtnSignal, m_mainViewWid, &MainViewWidget::loadFunctionWidget);

    connect(m_sideBarWid,&SideBarWidget::sendFullScreenCommonUseBtnSignal,
            m_mainViewWid,&MainViewWidget::loadFullCommonUseWidget);
    connect(m_sideBarWid,&SideBarWidget::sendFullScreenLetterBtnSignal,
            m_mainViewWid,&MainViewWidget::loadFullLetterWidget);
    connect(m_sideBarWid,&SideBarWidget::sendFullScreenFunctionBtnSignal,
            m_mainViewWid,&MainViewWidget::loadFullFunctionWidget);

    connect(m_sideBarWid,&SideBarWidget::sendFullScreenBtnSignal,this,&MainWindow::showFullScreenWidget);
    connect(m_sideBarWid,&SideBarWidget::sendDefaultBtnSignal,this,&MainWindow::showDefaultWidget);
    connect(m_mainViewWid,&MainViewWidget::sendHideMainWindowSignal,this,&MainWindow::recvHideMainWindowSlot);
    connect(m_sideBarWid,&SideBarWidget::sendHideMainWindowSignal,this,&MainWindow::recvHideMainWindowSlot);

    connect(QApplication::primaryScreen(),&QScreen::geometryChanged,
            this,&MainWindow::monitorResolutionChange);
    connect(qApp,&QApplication::primaryScreenChanged,this,
            &MainWindow::primaryScreenChangedSlot);

    XEventMonitor::instance()->start();
    connect(XEventMonitor::instance(), SIGNAL(keyRelease(QString)),
            this,SLOT(XkbEventsRelease(QString)));
    connect(XEventMonitor::instance(), SIGNAL(keyPress(QString)),
            this,SLOT(XkbEventsPress(QString)));

    if(QGSettings::isSchemaInstalled(QString("org.ukui.panel.settings").toLocal8Bit()))
    {
        QGSettings* gsetting=new QGSettings(QString("org.ukui.panel.settings").toLocal8Bit());
        connect(gsetting,&QGSettings::changed,
                this,&MainWindow::panelChangedSlot);
    }

//    QDBusConnection::sessionBus().connect("com.ukui.menu","/com/ukui/menu","local.test.MainWindow",
//                                         QString("sendStartMenuSignal"),this,SLOT(recvStartMenuSlot()));
}

/**
 * 添加阴影
 */
//void MainWindow::paintEvent(QPaintEvent *)
//{
//    if(!m_isFullScreen)
//    {

////        Q_UNUSED(event)
//        QPainter p(this);
//        p.setPen(Qt::NoPen);
//        p.setBrush(QBrush(QColor(0, 0, 0, 180)));
//        p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
//        p.drawRoundedRect(0, 0, 376,590, 6, 6);

////        QPainter painter(this);
////        const qreal radius =6;

////        QRectF rect1;
////        rect1.setX(0);
////        rect1.setY(2);
////        rect1.setWidth( this->rect().width()-2);
////        rect1.setHeight(this->rect().height()-2);

//        //加边框
////        QPainterPath path1;
////        path1.setFillRule(Qt::WindingFill);
////        painter.setRenderHint(QPainter::Antialiasing);

////        path1.moveTo(rect1.topRight() - QPointF(radius, 0));
////        path1.lineTo(rect1.topLeft());
////        path1.lineTo(rect1.bottomLeft());
////        path1.lineTo(rect1.bottomRight());
////        path1.lineTo(rect1.topRight() + QPointF(0, radius));
////        path1.quadTo(rect1.topRight(), rect1.topRight() + QPointF(-radius, -0));
////        QColor color;
////        color.setNamedColor(QString::fromLocal8Bit(BORDERCOLOR));
////        painter.setPen(QPen(color,2));
////        painter.setOpacity(0.95);
////        painter.setBrush(Qt::NoBrush);
////        painter.drawPath(path1);

////        QRectF rect;
////        rect.setX(this->rect().x());
////        rect.setY(this->rect().y());
////        rect.setWidth( this->rect().width());
////        rect.setHeight(this->rect().height());

////        加阴影
////        QPainterPath path;
////        path.setFillRule(Qt::WindingFill);
////        char shadowcolor[20];
////        sprintf(shadowcolor,"%s", SHADOWCOLOR);
////        QColor color(QString::fromLocal8Bit(shadowcolor));
////        for(int i=0; i<9; i++)
////        {
////            rect.setX(0);
////            rect.setY((9-i));
////            rect.setWidth( this->rect().width()-(9-i));
////            rect.setHeight(this->rect().height()-(9-i));

////            path.setFillRule(Qt::WindingFill);
////            path.moveTo(rect.topRight() - QPointF(radius, 0));
////            path.lineTo(rect.topLeft());
////            path.lineTo(rect.bottomLeft());
////            path.lineTo(rect.bottomRight());
////            path.lineTo(rect.topRight() + QPointF(0, radius));
////            path.quadTo(rect.topRight(), rect.topRight() + QPointF(-radius, -0));
////            color.setAlpha(static_cast<int>(150 - qSqrt(i)*50));
////    //        color.setAlphaF(1.0*(20-i)/70);

////            painter.setPen(QPen(color));
////            painter.setBrush(Qt::NoBrush);
////            painter.drawPath(path);
////        }
//    }
//}

/**
 * 显示全屏窗口
 */
void MainWindow::showFullScreenWidget()
{
    m_isFullScreen=true;
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
        startRect.setRect(x,y+QApplication::primaryScreen()->geometry().height()-panelSize-Style::minh,Style::minw,Style::minh);
        endRect.setRect(x,y,QApplication::primaryScreen()->geometry().width(),QApplication::primaryScreen()->geometry().height()-panelSize);
    }
    else if(position==1)
    {
        startRect.setRect(x,y+panelSize,Style::minw,Style::minh);
        endRect.setRect(x,y+panelSize,QApplication::primaryScreen()->geometry().width(),QApplication::primaryScreen()->geometry().height()-panelSize);
    }
    else if(position==2)
    {
        startRect.setRect(x+panelSize,y,Style::minw,Style::minh);
        endRect.setRect(x+panelSize,y,QApplication::primaryScreen()->geometry().width()-panelSize,QApplication::primaryScreen()->geometry().height());
    }
    else
    {
        startRect.setRect(x+QApplication::primaryScreen()->geometry().width()-panelSize-Style::minw,y,Style::minw,Style::minh);
        endRect.setRect(x,y,QApplication::primaryScreen()->geometry().width()-panelSize,QApplication::primaryScreen()->geometry().height());
    }

    this->centralWidget()->layout()->removeWidget(m_mainViewWid);
    m_mainViewWid->setParent(nullptr);
    this->centralWidget()->layout()->removeWidget(m_line);
    m_line->setParent(nullptr);
    this->centralWidget()->layout()->removeWidget(m_sideBarWid);
    m_sideBarWid->setParent(nullptr);

    m_animation->setDuration(100);//动画总时间
    m_animation->setStartValue(startRect);
    m_animation->setEndValue(endRect);
    m_animation->setEasingCurve(QEasingCurve::Linear);
    m_animation->start();
}

/**
 * 显示默认窗口
 */
void MainWindow::showDefaultWidget()
{
    m_isFullScreen=false;
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
        endRect.setRect(x,y+QApplication::primaryScreen()->geometry().height()-panelSize-Style::minh,Style::minw,Style::minh);
        startRect.setRect(x,y,QApplication::primaryScreen()->geometry().width(),QApplication::primaryScreen()->geometry().height()-panelSize);
    }
    else if(position==1)
    {
        endRect.setRect(x,y+panelSize,Style::minw,Style::minh);
        startRect.setRect(x,y+panelSize,QApplication::primaryScreen()->geometry().width(),QApplication::primaryScreen()->geometry().height()-panelSize);
    }
    else if(position==2)
    {
        endRect.setRect(x+panelSize,y,Style::minw,Style::minh);
        startRect.setRect(x+panelSize,y,QApplication::primaryScreen()->geometry().width()-panelSize,QApplication::primaryScreen()->geometry().height());
    }
    else
    {
        endRect.setRect(x+QApplication::primaryScreen()->geometry().width()-panelSize-Style::minw,y,Style::minw,Style::minh);
        startRect.setRect(x,y,QApplication::primaryScreen()->geometry().width()-panelSize,QApplication::primaryScreen()->geometry().height());
    }

    this->centralWidget()->layout()->removeWidget(m_mainViewWid);
    m_mainViewWid->setParent(nullptr);
    this->centralWidget()->layout()->removeWidget(m_sideBarWid);
    m_sideBarWid->setParent(nullptr);

    m_animation->setDuration(100);//动画总时间
    m_animation->setStartValue(startRect);
    m_animation->setEndValue(endRect);
    m_animation->setEasingCurve(QEasingCurve::Linear);
    m_animation->start();
}

void MainWindow::stateChangedSlot(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    Q_UNUSED(oldState);
    if(m_isFullScreen && newState==QAbstractAnimation::Stopped)
    {
        this->centralWidget()->layout()->addWidget(m_mainViewWid);
        this->centralWidget()->layout()->addWidget(m_sideBarWid);
        m_sideBarWid->loadMaxSidebar();
        m_mainViewWid->loadMaxMainView();
        m_sideBarWid->enterAnimation();
        setFrameStyle();
    }
    if(!m_isFullScreen && newState==QAbstractAnimation::Stopped)
    {
        this->centralWidget()->layout()->addWidget(m_mainViewWid);
        this->centralWidget()->layout()->addWidget(m_line);
        this->centralWidget()->layout()->addWidget(m_sideBarWid);
        m_sideBarWid->loadMinSidebar();
        m_mainViewWid->loadMinMainView();
        setFrameStyle();
    }
}

void MainWindow::animationValueChangedSlot(const QVariant &value)
{
    if(m_isFullScreen)
    {
        QPainterPath path;
        path.addRect(this->rect());
        setProperty("blurRegion", QRegion(path.toFillPolygon().toPolygon()));
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
            m_mainViewWid->widgetMakeZero();
//            m_sideBarWid->widgetMakeZero();
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
        m_winFlag = true;
    }
    if(m_winFlag && keycode == "Super_L"){
        m_winFlag = false;
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
    }else if(m_winFlag && keycode == "Super_L")
        return;

    if((keycode == "Super_L") || (keycode == "Super_R"))
    {
//        if(this->isVisible())
        if(QApplication::activeWindow() == this)
        {
            this->hide();
            m_mainViewWid->widgetMakeZero();
//            m_sideBarWid->widgetMakeZero();
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
        m_mainViewWid->widgetMakeZero();
//        m_sideBarWid->widgetMakeZero();
    }
}

void MainWindow::recvStartMenuSlot()
{
    if(this->isVisible())
    {
        this->hide();
        m_mainViewWid->widgetMakeZero();
//        m_sideBarWid->widgetMakeZero();
    }
    else{
        m_mainViewWid->widgetMakeZero();
//        m_sideBarWid->widgetMakeZero();
        this->loadMainWindow();
        this->show();
        this->raise();
        this->activateWindow();
    }
}

/**
 * 隐藏窗口
 */
void MainWindow::recvHideMainWindowSlot()
{
    this->hide();
//    m_mainViewWid->widgetMakeZero();
//    m_sideBarWid->widgetMakeZero();
}

void MainWindow::loadMainWindow()
{
    QDateTime dt=QDateTime::currentDateTime();
    int currentDateTime=dt.toTime_t();
    int nDaySec=24*60*60;
    m_setting->beginGroup("recentapp");
    QStringList recentAppKeys=m_setting->allKeys();
    for(int i=0;i<recentAppKeys.count();i++)
    {
        if((currentDateTime-m_setting->value(recentAppKeys.at(i)).toInt())/nDaySec >= 3)
            m_setting->remove(recentAppKeys.at(i));
    }
    m_setting->sync();
    m_setting->endGroup();

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
    if(m_isFullScreen)
    {
        if(position==0)
            this->setGeometry(QRect(x,y,QApplication::primaryScreen()->geometry().width(),QApplication::primaryScreen()->geometry().height()-panelSize));
        else if(position==1)
            this->setGeometry(QRect(x,y+panelSize,QApplication::primaryScreen()->geometry().width(),QApplication::primaryScreen()->geometry().height()-panelSize));
        else if(position==2)
            this->setGeometry(QRect(x+panelSize,y,QApplication::primaryScreen()->geometry().width()-panelSize,QApplication::primaryScreen()->geometry().height()));
        else
            this->setGeometry(QRect(x,y,QApplication::primaryScreen()->geometry().width()-panelSize,QApplication::primaryScreen()->geometry().height()));
        m_sideBarWid->loadMaxSidebar();
        m_sideBarWid->setSideBarBtnGeometry();
        m_mainViewWid->loadMaxMainView();
        QPainterPath path;
        path.addRect(this->rect());
        setProperty("blurRegion", QRegion(path.toFillPolygon().toPolygon()));
    }
    else
    {
        if(position==0)
            this->setGeometry(QRect(x,y+QApplication::primaryScreen()->geometry().height()-panelSize-Style::minh,
                                      Style::minw,Style::minh));
        else if(position==1)
            this->setGeometry(QRect(x,y+panelSize,Style::minw,Style::minh));
        else if(position==2)
            this->setGeometry(QRect(x+panelSize,y,Style::minw,Style::minh));
        else
            this->setGeometry(QRect(x+QApplication::primaryScreen()->geometry().width()-panelSize-Style::minw,y,
                                      Style::minw,Style::minh));

        m_sideBarWid->loadMinSidebar();
        m_mainViewWid->loadMinMainView();
    }
    setFrameStyle();

    //默认开启默认态
//    if(QGSettings::isSchemaInstalled(QString("org.ukui.control-center.desktop").toLocal8Bit()))
//    {
//        QGSettings* gsetting=new QGSettings(QString("org.ukui.control-center.desktop").toLocal8Bit());
//        bool ret=false;
//        if(gsetting->keys().contains(QString("menufullScreen")))
//            ret=gsetting->get("menufull-screen").toBool();
//        if(ret)
//        {
//            if(position==0)
//                this->setGeometry(QRect(x,y,QApplication::primaryScreen()->geometry().width(),QApplication::primaryScreen()->geometry().height()-panelSize));
//            else if(position==1)
//                this->setGeometry(QRect(x,y+panelSize,QApplication::primaryScreen()->geometry().width(),QApplication::primaryScreen()->geometry().height()-panelSize));
//            else if(position==2)
//                this->setGeometry(QRect(x+panelSize,y,QApplication::primaryScreen()->geometry().width()-panelSize,QApplication::primaryScreen()->geometry().height()));
//            else
//                this->setGeometry(QRect(x,y,QApplication::primaryScreen()->geometry().width()-panelSize,QApplication::primaryScreen()->geometry().height()));
//            this->centralWidget()->layout()->removeWidget(m_line);
//            m_line->setParent(nullptr);
//            m_sideBarWid->loadMaxSidebar();
//            m_mainViewWid->loadMaxMainView();
//            m_sideBarWid->enterAnimation();
//            QPainterPath path;
//            path.addRect(this->rect());
//            setProperty("blurRegion", QRegion(path.toFillPolygon().toPolygon()));
//            m_isFullScreen=true;
//        }
//        else
//        {
//            if(position==0)
//                this->setGeometry(QRect(x,y+QApplication::primaryScreen()->geometry().height()-panelSize-Style::minh,
//                                          Style::minw,Style::minh));
//            else if(position==1)
//                this->setGeometry(QRect(x,y+panelSize,Style::minw,Style::minh));
//            else if(position==2)
//                this->setGeometry(QRect(x+panelSize,y,Style::minw,Style::minh));
//            else
//                this->setGeometry(QRect(x+QApplication::primaryScreen()->geometry().width()-panelSize-Style::minw,y,
//                                          Style::minw,Style::minh));
//            QHBoxLayout* mainLayout=qobject_cast<QHBoxLayout*>(this->centralWidget()->layout());
//            mainLayout->insertWidget(1,m_line);
//            m_sideBarWid->loadMinSidebar();
//            m_mainViewWid->loadMinMainView();
//            m_isFullScreen=false;
//        }
//    }
//    else
//    {
//        if(position==0)
//            this->setGeometry(QRect(x,y+QApplication::primaryScreen()->geometry().height()-panelSize-Style::minh,
//                                      Style::minw,Style::minh));
//        else if(position==1)
//            this->setGeometry(QRect(x,y+panelSize,Style::minw,Style::minh));
//        else if(position==2)
//            this->setGeometry(QRect(x+panelSize,y,Style::minw,Style::minh));
//        else
//            this->setGeometry(QRect(x+QApplication::primaryScreen()->geometry().width()-panelSize-Style::minw,y,
//                                      Style::minw,Style::minh));

//        QHBoxLayout *mainLayout=qobject_cast<QHBoxLayout*>(this->centralWidget()->layout());
//        mainLayout->insertWidget(1,m_line);
//        m_sideBarWid->loadMinSidebar();
//        m_mainViewWid->loadMinMainView();
//        m_isFullScreen=false;
//    }

//    setFrameStyle();
}

void MainWindow::monitorResolutionChange(QRect rect)
{
    Q_UNUSED(rect);
    repaintWidget();
}

void MainWindow::primaryScreenChangedSlot(QScreen *screen)
{
    Q_UNUSED(screen);
    repaintWidget();

}

void MainWindow::panelChangedSlot(QString key)
{
    Q_UNUSED(key);
    repaintWidget();
}

void MainWindow::repaintWidget()
{
    Style::initWidStyle();
    this->setMinimumSize(Style::minw,Style::minh);
    m_line->setFixedSize(1,this->height());
    m_mainViewWid->repaintWidget();

    if(QApplication::activeWindow() == this)
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
        int x=QApplication::primaryScreen()->geometry().x();
        int y=QApplication::primaryScreen()->geometry().y();
        if(m_isFullScreen)
        {
            if(position==0)
                this->setGeometry(QRect(x,y,QApplication::primaryScreen()->geometry().width(),QApplication::primaryScreen()->geometry().height()-panelSize));
            else if(position==1)
                this->setGeometry(QRect(x,y+panelSize,QApplication::primaryScreen()->geometry().width(),QApplication::primaryScreen()->geometry().height()-panelSize));
            else if(position==2)
                this->setGeometry(QRect(x+panelSize,y,QApplication::primaryScreen()->geometry().width()-panelSize,QApplication::primaryScreen()->geometry().height()));
            else
                this->setGeometry(QRect(x,y,QApplication::primaryScreen()->geometry().width()-panelSize,QApplication::primaryScreen()->geometry().height()));
            m_sideBarWid->loadMaxSidebar();
            m_sideBarWid->setSideBarBtnGeometry();
            m_mainViewWid->resizeControl();
            QPainterPath path;
            path.addRect(this->rect());
            setProperty("blurRegion", QRegion(path.toFillPolygon().toPolygon()));
        }
        else
        {
            if(position==0)
                this->setGeometry(QRect(x,y+QApplication::primaryScreen()->geometry().height()-panelSize-Style::minh,
                                          Style::minw,Style::minh));
            else if(position==1)
                this->setGeometry(QRect(x,y+panelSize,Style::minw,Style::minh));
            else if(position==2)
                this->setGeometry(QRect(x+panelSize,y,Style::minw,Style::minh));
            else
                this->setGeometry(QRect(x+QApplication::primaryScreen()->geometry().width()-panelSize-Style::minw,y,
                                          Style::minw,Style::minh));

//            QHBoxLayout *mainLayout=qobject_cast<QHBoxLayout*>(this->centralWidget()->layout());
//            mainLayout->insertWidget(1,m_line);
            m_sideBarWid->loadMinSidebar();
            m_mainViewWid->resizeControl();
        }
        setFrameStyle();
    }
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

    QString m_defaultBackground;
    if(QGSettings::isSchemaInstalled(QString("org.ukui.control-center.personalise").toLocal8Bit()))
    {
        QGSettings* gsetting=new QGSettings(QString("org.ukui.control-center.personalise").toLocal8Bit());
        if(gsetting->keys().contains(QString("transparency")))
        {
            double transparency=gsetting->get("transparency").toDouble();
            m_defaultBackground=QString("rgba(19, 19, 20,"+QString::number(transparency)+")");
        }
        else
            m_defaultBackground=QString("rgba(19, 19, 20, 0.7)");
    }
    else
        m_defaultBackground=QString("rgba(19, 19, 20, 0.7)");

    if(!m_isFullScreen)
    {
        QRectF rect;
        rect.setX(this->rect().x()+1);
        rect.setY(this->rect().y()+1);
        rect.setWidth(this->rect().width()-2);
        rect.setHeight(this->rect().height()-2);
        const qreal radius = 6;
        QPainterPath path;

        if(position==0)
        {
            //右上角
            sprintf(style, "border:0px;background-color:%s;border-top-right-radius:6px;",m_defaultBackground.toLocal8Bit().data());
            path.moveTo(rect.topRight() - QPointF(radius, 0));
            path.lineTo(rect.topLeft());
            path.lineTo(rect.bottomLeft());
            path.lineTo(rect.bottomRight());
            path.lineTo(rect.topRight() + QPointF(0, radius));
            path.quadTo(rect.topRight(), rect.topRight() + QPointF(-radius, -0));
        }
        else if(position==1)
        {
            //右下角
            sprintf(style, "border:0px;background-color:%s;border-bottom-right-radius:6px;",m_defaultBackground.toLocal8Bit().data());
            path.moveTo(rect.topRight());
            path.lineTo(rect.topLeft());
            path.lineTo(rect.bottomLeft());
            path.lineTo(rect.bottomRight() - QPointF(radius, 0));
            path.quadTo(rect.bottomRight(), rect.bottomRight() + QPointF(0, -radius));
            path.lineTo(rect.topRight());
        }
        else if(position==2)
        {
            //右下角
            sprintf(style, "border:0px;background-color:%s;border-bottom-right-radius:6px;",m_defaultBackground.toLocal8Bit().data());
            path.moveTo(rect.topRight());
            path.lineTo(rect.topLeft());
            path.lineTo(rect.bottomLeft());
            path.lineTo(rect.bottomRight() - QPointF(radius, 0));
            path.quadTo(rect.bottomRight(), rect.bottomRight() + QPointF(0, -radius));
            path.lineTo(rect.topRight());
        }
        else
        {
            //左下角
            sprintf(style, "border:0px;background-color:%s;border-bottom-left-radius:6px;",m_defaultBackground.toLocal8Bit().data());
            path.moveTo(rect.topRight());
            path.lineTo(rect.topLeft());
            path.lineTo(rect.bottomLeft() + QPointF(0, -radius));
            path.quadTo(rect.bottomLeft(), rect.bottomLeft() + QPointF(radius, 0));
            path.lineTo(rect.bottomRight());
            path.lineTo(rect.topRight());
        }
        setProperty("blurRegion", QRegion(path.toFillPolygon().toPolygon()));
    }
    else {
        sprintf(style, "border:0px;background-color:%s;border-radius:0px;",m_defaultBackground.toLocal8Bit().data());
    }
    m_frame->setStyleSheet(style);
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if(e->type()==KeyPress+4)
    {
        QKeyEvent* ke=static_cast<QKeyEvent*>(e);
        if((ke->key()>=0x30 && ke->key()<=0x39) || (ke->key()>=0x41 && ke->key()<=0x5a))
        {
            m_mainViewWid->setLineEditFocus(e->text());
        }

//        switch(e->key()){
//        case Qt::Key_Up:
//            m_mainViewWid->moveScrollBar(0);
//            break;
//        case Qt::Key_Down:
//            m_mainViewWid->moveScrollBar(1);
//            break;
//        default:
//            break;
//        }
    }
}
