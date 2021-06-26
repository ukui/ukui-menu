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
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonValue>
#include "src/Style/style.h"
#include <QPalette>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    openDataBase("MainThread");
    m_ukuiMenuInterface=new UkuiMenuInterface;
    UkuiMenuInterface::appInfoVector=m_ukuiMenuInterface->createAppInfoVector();
    initDatabase();
    UkuiMenuInterface::alphabeticVector=m_ukuiMenuInterface->getAlphabeticClassification();
    UkuiMenuInterface::functionalVector=m_ukuiMenuInterface->getFunctionalClassification();
    UkuiMenuInterface::allAppVector=m_ukuiMenuInterface->getAllClassification();

    Style::initWidStyle();
    initUi();


    m_dbus=new DBus;
    new MenuAdaptor(m_dbus);
    QDBusConnection con=QDBusConnection::sessionBus();
    if(!con.registerService("org.ukui.menu") ||
            !con.registerObject("/org/ukui/menu",m_dbus))
    {
        qDebug()<<"error:"<<con.lastError().message();
    }
    connect(m_dbus,&DBus::sendReloadSignal,this,[=]
    {
        UkuiMenuInterface::appInfoVector.clear();
        UkuiMenuInterface::alphabeticVector.clear();
        UkuiMenuInterface::functionalVector.clear();
        UkuiMenuInterface::appInfoVector=m_ukuiMenuInterface->createAppInfoVector();
        UkuiMenuInterface::alphabeticVector=m_ukuiMenuInterface->getAlphabeticClassification();
        UkuiMenuInterface::functionalVector=m_ukuiMenuInterface->getFunctionalClassification();
        Q_EMIT m_mainViewWid->reloadUkuiMenu();
    });

    connect(m_dbus,&DBus::winKeyResponseSignal,this,[=]{
        if(QGSettings::isSchemaInstalled(QString("org.ukui.session").toLocal8Bit()))
        {
            QGSettings gsetting(QString("org.ukui.session").toLocal8Bit());
            if(gsetting.keys().contains("win-key-release"))
                if(gsetting.get("win-key-release").toBool())
                    return;
        }
        if(QGSettings::isSchemaInstalled(QString("org.ukui.screenshot").toLocal8Bit()))
        {
            QGSettings gsetting(QString("org.ukui.screenshot").toLocal8Bit());
            if(gsetting.keys().contains("isrunning"))
                if(gsetting.get("isrunning").toBool())
                    return;
        }

        if(this->isVisible())
        {
            this->hide();
        }
        else{
            this->loadMainWindow();
            this->show();
            this->raise();
            this->activateWindow();
        }
        m_mainViewWid->widgetMakeZero();
    });
}

MainWindow::~MainWindow()
{
    closeDataBase("MainThread");
    delete m_ukuiMenuInterface;
}

void MainWindow::initUi()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAutoFillBackground(false);
    this->setFocusPolicy(Qt::NoFocus);

    this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    this->setMinimumSize(Style::minw,Style::minh);
    this->setContentsMargins(0,0,0,0);
    this->setFocus();
    m_frame=new QFrame;
    m_mainViewWid=new MainViewWidget;
    m_sideBarWid=new SideBarWidget;
//    m_frame->installEventFilter(this);
//    m_mainViewWid->installEventFilter(this);
//    m_sideBarWid->installEventFilter(this);

//    setTabOrder(m_mainViewWid , m_sideBarWid);
//    setTabOrder(m_sideBarWid , m_mainViewWid);

    this->setCentralWidget(m_frame);
    QHBoxLayout *mainlayout=new QHBoxLayout;
    mainlayout->setContentsMargins(0,0,0,0);
    mainlayout->setSpacing(0);
    m_frame->setLayout(mainlayout);

    mainlayout->addWidget(m_mainViewWid);
    m_line=new QFrame;
    m_line->installEventFilter(this);
    m_line->setFrameShape(QFrame::VLine);
    m_line->setFixedSize(1,this->height()-1);
    m_line->setEnabled(false);
    mainlayout->addWidget(m_line);
    mainlayout->addWidget(m_sideBarWid);
    m_sideBarWid->loadMinSidebar();
    m_mainViewWid->loadMinMainView();


    m_animation = new QPropertyAnimation(this, "geometry");
    connect(m_animation,&QPropertyAnimation::finished,this,&MainWindow::animationValueFinishedSlot);

    connect(m_sideBarWid, &SideBarWidget::setFocusToMainWin, m_mainViewWid, &MainViewWidget::setFocusToThis);
    connect(m_sideBarWid, &SideBarWidget::sendCommonUseBtnSignal, m_mainViewWid, &MainViewWidget::loadCommonUseWidget);
    connect(m_sideBarWid,&SideBarWidget::sendLetterBtnSignal, m_mainViewWid, &MainViewWidget::loadLetterWidget);
    connect(m_sideBarWid, &SideBarWidget::sendFunctionBtnSignal, m_mainViewWid, &MainViewWidget::loadFunctionWidget);

    connect(m_sideBarWid,&SideBarWidget::sendFullScreenCommonUseBtnSignal,
            m_mainViewWid,&MainViewWidget::loadFullCommonUseWidget);
    connect(m_sideBarWid,&SideBarWidget::sendFullScreenLetterBtnSignal,
            m_mainViewWid,&MainViewWidget::loadFullLetterWidget);
    connect(m_sideBarWid,&SideBarWidget::sendFullScreenFunctionBtnSignal,
            m_mainViewWid,&MainViewWidget::loadFullFunctionWidget);

    connect(m_sideBarWid, &SideBarWidget::setSlideBar, m_mainViewWid, &MainViewWidget::setSlideBar);

    connect(m_sideBarWid,&SideBarWidget::sendFullScreenBtnSignal,this,&MainWindow::showFullScreenWidget);
    connect(m_sideBarWid,&SideBarWidget::sendDefaultBtnSignal,this,&MainWindow::showDefaultWidget);
    connect(m_sideBarWid, &SideBarWidget::sendShowMainWindowSignal, this, &MainWindow::activeWindowSolt);
    connect(m_mainViewWid,&MainViewWidget::sendHideMainWindowSignal,this,&MainWindow::recvHideMainWindowSlot);
    connect(m_sideBarWid,&SideBarWidget::sendHideMainWindowSignal,this,&MainWindow::recvHideMainWindowSlot);

    connect(m_mainViewWid,&MainViewWidget::setFocusToSideWin,m_sideBarWid,&SideBarWidget::setFocusToThis);
    connect(this, &MainWindow::setFocusSignal, m_mainViewWid, &MainViewWidget::changeFocuDown);
//    connect(QApplication::desktop(),&QDesktopWidget::resized,this, [=]{
//        repaintWidget();
//    });
//    connect(QApplication::desktop(),&QDesktopWidget::primaryScreenChanged,this,[=]{
//        repaintWidget();
//    });

//    connect(QApplication::desktop(),&QDesktopWidget::screenCountChanged,this,[=]{
//        repaintWidget();
//    });

    QDBusConnection::sessionBus().connect(DBUS_NAME,DBUS_PATH,DBUS_INTERFACE,QString("PanelGeometryRefresh"),this,SLOT(primaryScreenChangeSlot()));

    //监听屏幕缩放
    if(QGSettings::isSchemaInstalled(QString("org.ukui.SettingsDaemon.plugins.xsettings").toLocal8Bit()))
    {
        QGSettings* m_gsetting=new QGSettings(QString("org.ukui.SettingsDaemon.plugins.xsettings").toLocal8Bit());
        connect(m_gsetting,&QGSettings::changed,this,[=](const QString &key)
        {
            if(key=="scalingFactor")
                repaintWidget();
        });
    }

    if(QGSettings::isSchemaInstalled(QString("org.ukui.panel.settings").toLocal8Bit()))
    {
        QGSettings* gsetting=new QGSettings(QString("org.ukui.panel.settings").toLocal8Bit());
        connect(gsetting,&QGSettings::changed,
                this,&MainWindow::panelChangedSlot);
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    double transparency=getTransparency();

    QRect rect = this->rect();
    QPainterPath path;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    painter.setPen(Qt::transparent);
    if(!m_isFullScreen)
    {
        qreal radius=6;
        path.moveTo(rect.topRight() - QPointF(radius, 0));
        path.lineTo(rect.topLeft() + QPointF(radius, 0));
        path.quadTo(rect.topLeft(), rect.topLeft() + QPointF(0, radius));
        path.lineTo(rect.bottomLeft() + QPointF(0, -radius));
        path.quadTo(rect.bottomLeft(), rect.bottomLeft() + QPointF(radius, 0));
        path.lineTo(rect.bottomRight() - QPointF(radius, 0));
        path.quadTo(rect.bottomRight(), rect.bottomRight() + QPointF(0, -radius));
        path.lineTo(rect.topRight() + QPointF(0, radius));
        path.quadTo(rect.topRight(), rect.topRight() + QPointF(-radius, -0));

        painter.setBrush(this->palette().base());
        painter.setPen(Qt::transparent);
        painter.setOpacity(transparency);
        painter.drawPath(path);

//        setProperty("blurRegion", QRegion(path.toFillPolygon().toPolygon()));
        KWindowEffects::enableBlurBehind(this->winId(), true, QRegion(path.toFillPolygon().toPolygon()));
    }
    else//全屏固定背景色(黑底白字)
    {
//        if(QGSettings::isSchemaInstalled(QString("org.ukui.control-center.personalise").toLocal8Bit()))
//        {
//            QGSettings gsetting(QString("org.ukui.control-center.personalise").toLocal8Bit());
//            if(gsetting.keys().contains(QString("effect")))
//            {
//                if(gsetting.get("effect").toBool())
//                {
//                    painter.setBrush(Qt::black);
//                    painter.setPen(Qt::transparent);
//                    painter.setOpacity(0.25);
//                    painter.drawRect(this->rect());
//                }
//                else
//                {
//                    painter.setBrush(this->palette().base());
//                    painter.setPen(Qt::transparent);
//                    painter.setOpacity(transparency);
//                    painter.drawRect(this->rect());
//                }
//            }
//            else
//            {
//                painter.setBrush(this->palette().base());
//                painter.setPen(Qt::transparent);
//                painter.setOpacity(transparency);
//                painter.drawRect(this->rect());
//            }
//        }
//        else
//        {
            painter.setBrush(this->palette().base());
            painter.setPen(Qt::transparent);
            painter.setOpacity(transparency);
            painter.drawRect(this->rect());
//        }

        QPainterPath path;
        path.addRect(this->rect());
        KWindowEffects::enableBlurBehind(this->winId(), true, QRegion(path.toFillPolygon().toPolygon()));
    }
    QMainWindow::paintEvent(event);
}

/**
 * 显示全屏窗口
 */
void MainWindow::showFullScreenWidget()
{
    m_isFullScreen=true;
    this->setContentsMargins(0,0,0,0);
    int position=Style::panelPosition;
    int panelSize=Style::panelSize;
    int x = Style::primaryScreenX;
    int y = Style::primaryScreenY;
    int width = Style::primaryScreenWidth;
    int height = Style::primaryScreenHeight;
    QRect startRect;
    QRect endRect;
    if(position==0)
    {
        startRect.setRect(x+4,y+height-panelSize-Style::minh-3,Style::minw,Style::minh);
        endRect.setRect(x,y,width,height-panelSize);
    }
    else if(position==1)
    {
        startRect.setRect(x+4,y+panelSize+4,Style::minw,Style::minh);
        endRect.setRect(x,y+panelSize,width,height-panelSize);
    }
    else if(position==2)
    {
        startRect.setRect(x+panelSize+4,y+4,Style::minw,Style::minh);
        endRect.setRect(x+panelSize,y,width-panelSize,height);
    }
    else
    {
        startRect.setRect(x+width-panelSize-Style::minw-4,y+4,Style::minw,Style::minh);
        endRect.setRect(x,y,width-panelSize,height);
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
    int position=Style::panelPosition;
    int panelSize=Style::panelSize;
    int x = Style::primaryScreenX;
    int y = Style::primaryScreenY;
    int width = Style::primaryScreenWidth;
    int height = Style::primaryScreenHeight;
    QRect startRect;
    QRect endRect;
    if(position==0)
    {
        endRect.setRect(x+4,y+height-panelSize-Style::minh-3,Style::minw,Style::minh);
        startRect.setRect(x,y,width,height-panelSize);
    }
    else if(position==1)
    {
        endRect.setRect(x+4,y+panelSize+4,Style::minw,Style::minh);
        startRect.setRect(x,y+panelSize,width,height-panelSize);
    }
    else if(position==2)
    {
        endRect.setRect(x+panelSize+4,y+4,Style::minw,Style::minh);
        startRect.setRect(x+panelSize,y,width-panelSize,height);
    }
    else
    {
        endRect.setRect(x+width-panelSize-Style::minw-4,y+4,Style::minw,Style::minh);
        startRect.setRect(x,y,width-panelSize,height);
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

void MainWindow::animationValueChangedSlot(const QVariant &value)
{
    if(m_isFullScreen)
    {
        QPainterPath path;
        path.addRect(this->rect());
//        setProperty("blurRegion", QRegion(path.toFillPolygon().toPolygon()));
        KWindowEffects::enableBlurBehind(this->winId(), true, QRegion(path.toFillPolygon().toPolygon()));
    }
}

void MainWindow::animationValueFinishedSlot()
{
    if(m_isFullScreen)
    {
        this->centralWidget()->layout()->addWidget(m_mainViewWid);
        this->centralWidget()->layout()->addWidget(m_sideBarWid);
        m_sideBarWid->loadMaxSidebar();
        m_mainViewWid->loadMaxMainView();
        m_sideBarWid->enterAnimation();
    }
    else
    {
        this->centralWidget()->layout()->addWidget(m_mainViewWid);
        this->centralWidget()->layout()->addWidget(m_line);
        this->centralWidget()->layout()->addWidget(m_sideBarWid);
        m_sideBarWid->loadMinSidebar();
        m_mainViewWid->loadMinMainView();
    }
}

void MainWindow::activeWindowSolt(bool flag)
{
    QTimer::singleShot(30,this, SLOT(mainWinShowSlot()));
}

void MainWindow::mainWinShowSlot()
{
     this->activateWindow();
     qDebug() << "void MainWindow::activeWindowSolt()";
}

/**
 * 鼠标点击窗口外部事件
 */
bool MainWindow::event ( QEvent * event )
{
   if (event->type() == QEvent::ActivationChange)
  // if(QEvent::WindowDeactivate == event->type() && m_canHide)//窗口停用
   {
       qDebug() << " * 鼠标点击窗口外部事件";
        if(QApplication::activeWindow() != this)
        {
            this->hide();
            m_mainViewWid->widgetMakeZero();
        }
   }

   if (event->type() == QEvent::KeyPress)
   {
       QKeyEvent *keyEvent = (QKeyEvent *) event;
       if (keyEvent->key() == Qt::Key_Tab)
       {
           m_mainViewWid->setFocus();
           Q_EMIT setFocusSignal();
           return true;
       }
       if(keyEvent->key() == Qt::Key_Up || keyEvent->key() == Qt::Key_Down ||
               keyEvent->key() == Qt::Key_Left || keyEvent->key() == Qt::Key_Right)
       {
         m_mainViewWid->setFocus();
         Q_EMIT setFocusSignal();
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
//    m_mainViewWid->widgetMakeZero();
//    m_sideBarWid->widgetMakeZero();
}

void MainWindow::loadMainWindow()
{
    cleanTimeoutApp();

    int position=Style::panelPosition;
    int panelSize=Style::panelSize;
    int x = Style::primaryScreenX;
    int y = Style::primaryScreenY;
    int width = Style::primaryScreenWidth;
    int height = Style::primaryScreenHeight;
    this->setFocus();
    if(m_isFullScreen)
    {
        //修复界面黑框问题
        QRect startRect;
        QRect endRect;
        if(position==0)
        {
            startRect.setRect(x+4,y+height-panelSize-Style::minh-3,Style::minw,Style::minh);
            endRect.setRect(x,y,width,height-panelSize);
        }
        else if(position==1)
        {
            startRect.setRect(x+4,y+panelSize+4,Style::minw,Style::minh);
            endRect.setRect(x,y+panelSize,width,height-panelSize);
        }
        else if(position==2)
        {
            startRect.setRect(x+panelSize+4,y+4,Style::minw,Style::minh);
            endRect.setRect(x+panelSize,y,width-panelSize,height);
        }
        else
        {
            startRect.setRect(x+width-panelSize-Style::minw-4,y+4,Style::minw,Style::minh);
            endRect.setRect(x,y,width-panelSize,height);
        }

        this->centralWidget()->layout()->removeWidget(m_mainViewWid);
        m_mainViewWid->setParent(nullptr);
        this->centralWidget()->layout()->removeWidget(m_line);
        m_line->setParent(nullptr);
        this->centralWidget()->layout()->removeWidget(m_sideBarWid);
        m_sideBarWid->setParent(nullptr);
//        m_sideBarWid->clearFocus();
//        m_mainViewWid->clearFocus();
        m_animation->setDuration(1);//动画总时间
        m_animation->setStartValue(startRect);
        m_animation->setEndValue(endRect);
        m_animation->setEasingCurve(QEasingCurve::Linear);
        m_animation->start();
    }
    else
    {
        if(position==0)
            this->setGeometry(QRect(x+4,y+height-panelSize-Style::minh-3,
                                      Style::minw,Style::minh));
        else if(position==1)
            this->setGeometry(QRect(x+4,y+panelSize+4,Style::minw,Style::minh));
        else if(position==2)
            this->setGeometry(QRect(x+panelSize+4,y+4,Style::minw,Style::minh));
        else
            this->setGeometry(QRect(x+width-panelSize-Style::minw-4,y+4,
                                      Style::minw,Style::minh));

        m_sideBarWid->loadMinSidebar();
        m_mainViewWid->loadMinMainView();
    }
}

void MainWindow::panelChangedSlot(QString key)
{
    Q_UNUSED(key);
    repaintWidget();
}

void MainWindow::primaryScreenChangeSlot()
{
    repaintWidget();
}

void MainWindow::repaintWidget()
{
    Style::initWidStyle();
    this->setMinimumSize(Style::minw,Style::minh);
    m_line->setFixedSize(1,this->height()-1);
    m_mainViewWid->repaintWidget();

    if(QApplication::activeWindow() == this)
    {
        int position=Style::panelPosition;
        int panelSize=Style::panelSize;
        int x = Style::primaryScreenX;
        int y = Style::primaryScreenY;
        int width = Style::primaryScreenWidth;
        int height = Style::primaryScreenHeight;
        if(m_isFullScreen)
        {
            if(position==0)
                this->setGeometry(QRect(x,y,width,height-panelSize));
            else if(position==1)
                this->setGeometry(QRect(x,y+panelSize,width,height-panelSize));
            else if(position==2)
                this->setGeometry(QRect(x+panelSize,y,width-panelSize,height));
            else
                this->setGeometry(QRect(x,y,width-panelSize,height));
            m_sideBarWid->loadMaxSidebar();
            m_sideBarWid->setSideBarBtnGeometry();
            m_mainViewWid->resizeControl();
        }
        else
        {
            if(position==0)
                this->setGeometry(QRect(x+4,y+height-panelSize-Style::minh-3,
                                          Style::minw,Style::minh));
            else if(position==1)
                this->setGeometry(QRect(x+4,y+panelSize+4,Style::minw,Style::minh));
            else if(position==2)
                this->setGeometry(QRect(x+panelSize+4,y+4,Style::minw,Style::minh));
            else
                this->setGeometry(QRect(x+width-panelSize-Style::minw-4,y+4,
                                          Style::minw,Style::minh));

            m_sideBarWid->loadMinSidebar();
            m_mainViewWid->resizeControl();
        }
    }
}
//void MainWindow::setFrameStyle()
//{
//    int position=0;
//    int panelSize=0;
//    if(QGSettings::isSchemaInstalled(QString("org.ukui.panel.settings").toLocal8Bit()))
//    {
//        QGSettings* gsetting=new QGSettings(QString("org.ukui.panel.settings").toLocal8Bit());
//        if(gsetting->keys().contains(QString("panelposition")))
//            position=gsetting->get("panelposition").toInt();
//        else
//            position=0;
//        if(gsetting->keys().contains(QString("panelsize")))
//            panelSize=gsetting->get("panelsize").toInt();
//        else
//            panelSize=46;
//    }
//    else
//    {
//        position=0;
//        panelSize=46;
//    }

//    QRectF rect;
//    rect.setX(this->rect().x()+1);
//    rect.setY(this->rect().y()+1);
//    rect.setWidth(this->rect().width()-2);
//    rect.setHeight(this->rect().height()-2);
//    qreal radius = 0;
//    QPainterPath path;
//    if(!m_isFullScreen)
//    {
//        radius=12;
//    }
//    else
//    {
//        radius=0;
//    }

//    path.moveTo(rect.topRight() - QPointF(radius, 0));
//    path.lineTo(rect.topLeft() + QPointF(radius, 0));
//    path.quadTo(rect.topLeft(), rect.topLeft() + QPointF(0, radius));
//    path.lineTo(rect.bottomLeft() + QPointF(0, -radius));
//    path.quadTo(rect.bottomLeft(), rect.bottomLeft() + QPointF(radius, 0));
//    path.lineTo(rect.bottomRight() - QPointF(radius, 0));
//    path.quadTo(rect.bottomRight(), rect.bottomRight() + QPointF(0, -radius));
//    path.lineTo(rect.topRight() + QPointF(0, radius));
//    path.quadTo(rect.topRight(), rect.topRight() + QPointF(-radius, -0));

//    setProperty("blurRegion", QRegion(path.toFillPolygon().toPolygon()));
//}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if(e->type() == QEvent::KeyPress)
    {
        if((e->key() >= 0x30 && e->key() <= 0x39) || (e->key() >= 0x41 && e->key() <= 0x5a))
        {
            m_mainViewWid->setLineEditFocus(e->text());
        }
        if(e->key() == Qt::Key_Backspace)
        {
            m_mainViewWid->setLineEditFocus("");
        }
        if(e->key() == Qt::Key_Escape)
        {
            this->hide();
            m_mainViewWid->widgetMakeZero();
        }
//        if(e->key() == Qt::Key_Up || e->key() == Qt::Key_Down ||
//                e->key() == Qt::Key_Left || e->key() == Qt::Key_Right)
//        {
//          qDebug() << "11111111111111111";
//          m_sideBarWid->setFocus();
//          Q_EMIT setFocusSignal();
//        }
    }
}
