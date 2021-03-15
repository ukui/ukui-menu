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
    m_ukuiMenuInterface=new UkuiMenuInterface;
    UkuiMenuInterface::appInfoVector=m_ukuiMenuInterface->createAppInfoVector();
    UkuiMenuInterface::alphabeticVector=m_ukuiMenuInterface->getAlphabeticClassification();
    UkuiMenuInterface::functionalVector=m_ukuiMenuInterface->getFunctionalClassification();
    UkuiMenuInterface::allAppVector=m_ukuiMenuInterface->getAllClassification();

    Style::initWidStyle();
    QString path=QDir::homePath()+"/.config/ukui/ukui-menu.ini";
    m_setting=new QSettings(path,QSettings::IniFormat);
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
}

MainWindow::~MainWindow()
{
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
    m_line=new QFrame;
    m_line->setFrameShape(QFrame::VLine);
    m_line->setFixedSize(1,this->height()-1);
    m_line->setEnabled(false);
    mainlayout->addWidget(m_line);
    mainlayout->addWidget(m_sideBarWid);

    m_animation = new QPropertyAnimation(this, "geometry");
//    connect(m_animation, &QPropertyAnimation::valueChanged, this, &MainWindow::animationValueChangedSlot);
    connect(m_animation,&QPropertyAnimation::finished,this,&MainWindow::animationValueFinishedSlot);

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

//    connect(QApplication::primaryScreen(),&QScreen::geometryChanged,
//            this,&MainWindow::monitorResolutionChange);
//    connect(qApp,&QApplication::primaryScreenChanged,this,
//            &MainWindow::primaryScreenChangedSlot);

    connect(QApplication::desktop(),&QDesktopWidget::resized,this, [=]{
        qDebug()<<"---分辨率变化---";
        repaintWidget();
    });
    connect(QApplication::desktop(),&QDesktopWidget::primaryScreenChanged,this,[=]{
        qDebug()<<"---主屏幕变化---";
        repaintWidget();
    });

    connect(QApplication::desktop(),&QDesktopWidget::screenCountChanged,this,[=]{
        qDebug()<<"---屏幕数量变化---";
        repaintWidget();
    });


    if(QGSettings::isSchemaInstalled(QString("org.ukui.panel.settings").toLocal8Bit()))
    {
        QGSettings* gsetting=new QGSettings(QString("org.ukui.panel.settings").toLocal8Bit());
        connect(gsetting,&QGSettings::changed,
                this,&MainWindow::panelChangedSlot);
    }

//    QDBusConnection::sessionBus().connect("com.ukui.menu","/com/ukui/menu","local.test.MainWindow",
//                                         QString("sendStartMenuSignal"),this,SLOT(recvStartMenuSlot()));
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    double transparency=getTransparency();

    QRect rect = this->rect();
//    rect.setWidth(this->rect().width());
//    rect.setHeight(this->rect().height());
//    rect.setX(this->rect().x());
//    rect.setY(this->rect().y());
//    rect.setWidth(this->rect().width());
//    rect.setHeight(this->rect().height());
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
//        QGSettings gsetting(QString("org.mate.background").toLocal8Bit());
//        QString iconPath =gsetting.get("picture-filename").toString();
//        painter.drawPixmap(0,0,this->width(),this->height(),QPixmap(iconPath));

        if(QGSettings::isSchemaInstalled(QString("org.ukui.control-center.personalise").toLocal8Bit()))
        {
            QGSettings gsetting(QString("org.ukui.control-center.personalise").toLocal8Bit());
            if(gsetting.keys().contains(QString("effect")))
            {
                if(gsetting.get("effect").toBool())
                {
                    painter.setBrush(Qt::black);
                    painter.setPen(Qt::transparent);
                    painter.setOpacity(0.25);
                    painter.drawRect(this->rect());
                }
                else
                {
                    painter.setBrush(this->palette().base());
                    painter.setPen(Qt::transparent);
                    painter.setOpacity(transparency);
                    painter.drawRect(this->rect());
                }
            }
            else
            {
                painter.setBrush(this->palette().base());
                painter.setPen(Qt::transparent);
                painter.setOpacity(transparency);
                painter.drawRect(this->rect());
            }
        }
        else
        {
            painter.setBrush(this->palette().base());
            painter.setPen(Qt::transparent);
            painter.setOpacity(transparency);
            painter.drawRect(this->rect());
        }

        QPainterPath path;
        path.addRect(this->rect());
//        setProperty("blurRegion", QRegion(path.toFillPolygon().toPolygon()));
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

    int x = getScreenGeometry("x");
    int y = getScreenGeometry("y");
    int width = getScreenGeometry("width");
    int height = getScreenGeometry("height");
    if(width==0 || height==0)
    {
        QRect rect=QApplication::desktop()->screenGeometry(0);
        x=rect.x();
        y=rect.y();
        width=rect.width();
        height=rect.height();
    }
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
#define MARGIN 4
void MainWindow::showDefaultWidget()
{
    m_isFullScreen=false;
    this->setContentsMargins(0,0,0,0);

    QDBusInterface iface("org.ukui.panel",
                         "/panel/position",
                         "org.ukui.panel",
                         QDBusConnection::sessionBus());
    QDBusReply<QVariantList> reply=iface.call("GetPrimaryScreenGeometry");
    QVariantList position_list=reply.value();
    /*
     * 通过这个dbus接口获取到的5个参数分别为 ：可用屏幕大小的x坐标、y坐标、宽度、高度，任务栏位置
    */
    QRect startRect;
    QRect endRect;
    switch(reply.value().at(4).toInt()){
    case 1:
        endRect.setRect(position_list.at(0).toInt()+MARGIN,position_list.at(1).toInt()+MARGIN,Style::minw,Style::minh);
        startRect.setRect(position_list.at(0).toInt(),position_list.at(1).toInt(),position_list.at(2).toInt(),position_list.at(3).toInt());
        break;
    case 2:
        endRect.setRect(position_list.at(0).toInt()+MARGIN,position_list.at(1).toInt()+MARGIN,Style::minw,Style::minh);
        startRect.setRect(position_list.at(0).toInt(),position_list.at(1).toInt(),position_list.at(2).toInt(),position_list.at(3).toInt());
        break;
    case 3:
        endRect.setRect(position_list.at(0).toInt()+position_list.at(2).toInt()-Style::minw-MARGIN,position_list.at(1).toInt()+MARGIN,Style::minw,Style::minh);
        startRect.setRect(position_list.at(0).toInt(),position_list.at(1).toInt(),position_list.at(2).toInt(),position_list.at(3).toInt());
        break;
    default:
        endRect.setRect(position_list.at(0).toInt()+MARGIN,position_list.at(1).toInt()+position_list.at(3).toInt()-Style::minh-3,Style::minw,Style::minh);
        startRect.setRect(position_list.at(0).toInt(),position_list.at(1).toInt(),position_list.at(2).toInt(),position_list.at(3).toInt());
        break;
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
    QString arg;
    if(m_isFullScreen)
    {
        this->centralWidget()->layout()->addWidget(m_mainViewWid);
        this->centralWidget()->layout()->addWidget(m_sideBarWid);
        m_sideBarWid->loadMaxSidebar();
        m_mainViewWid->loadMaxMainView();
        m_sideBarWid->enterAnimation();
        arg=QString("Full screen!");
    }
    else
    {
        this->centralWidget()->layout()->addWidget(m_mainViewWid);
        this->centralWidget()->layout()->addWidget(m_line);
        this->centralWidget()->layout()->addWidget(m_sideBarWid);
        m_sideBarWid->loadMinSidebar();
        m_mainViewWid->loadMinMainView();
        arg=QString("Default screen!");
    }
    int x = getScreenGeometry("x");
    int y = getScreenGeometry("y");
    int width = getScreenGeometry("width");
    int height = getScreenGeometry("height");
    if(width==0 || height==0)
    {
        QRect rect=QApplication::desktop()->screenGeometry(0);
        x=rect.x();
        y=rect.y();
        width=rect.width();
        height=rect.height();
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
        }
   }
   return QWidget::event(event);
}

void MainWindow::recvStartMenuSlot()
{
    if(this->isVisible())
    {
        this->hide();
        m_mainViewWid->widgetMakeZero();
    }
    else{
        m_mainViewWid->widgetMakeZero();
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
    int x = getScreenGeometry("x");
    int y = getScreenGeometry("y");
    int width = getScreenGeometry("width");
    int height = getScreenGeometry("height");
    if(width==0 || height==0)
    {
        QRect rect=QApplication::desktop()->screenGeometry(0);
        x=rect.x();
        y=rect.y();
        width=rect.width();
        height=rect.height();
    }
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

void MainWindow::monitorResolutionChange(QRect rect)
{
    Q_UNUSED(rect);
    qDebug()<<"---分辨率变化---";
    repaintWidget();
}

void MainWindow::primaryScreenChangedSlot(QScreen *screen)
{
    Q_UNUSED(screen);
    qDebug()<<"---主屏幕变化---";
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
    m_line->setFixedSize(1,this->height()-1);
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
        int x = getScreenGeometry("x");
        int y = getScreenGeometry("y");
        int width = getScreenGeometry("width");
        int height = getScreenGeometry("height");
        if(width==0 || height==0)
        {
            QRect rect=QApplication::desktop()->screenGeometry(0);
            x=rect.x();
            y=rect.y();
            width=rect.width();
            height=rect.height();
        }
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
    if(e->type()==QEvent::KeyPress)
    {
        if((e->key()>=0x30 && e->key()<=0x39) || (e->key()>=0x41 && e->key()<=0x5a))
        {
            m_mainViewWid->setLineEditFocus(e->text());
        }
        if(e->key()==Qt::Key_Escape)
        {
            this->hide();
            m_mainViewWid->widgetMakeZero();
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
