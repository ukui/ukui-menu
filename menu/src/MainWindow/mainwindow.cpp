#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QDebug>
#include <QDesktopWidget>
#include "src/color.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pUkuiMenuInterface=new UkuiMenuInterface;
    UkuiMenuInterface::appInfoVector=pUkuiMenuInterface->create_appinfo_vector();
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
//    this->setStyleSheet("background-color:rgba(14,19,22,92%);");
//    char style[100];
//    sprintf(style, "border:0px;background-color:%s;",MAINVIEWWIDGETCOLOR);
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
    this->setMinimumSize(376+10,590+10);
    this->setContentsMargins(0,10,10,0);

    mainwidget=new QWidget(this);
    sidebarwid=new SideBarWidget(this);
    mainviewwid=new MainViewWidget(this);

    this->setCentralWidget(mainwidget);
    mainlayout=new QHBoxLayout;

    mainlayout->addWidget(mainviewwid);

    line=new QFrame(this);
    line->setFrameShape(QFrame::VLine);
    line->setStyleSheet("background-color:rgba(75,77,79)");
    line->setFixedSize(1,this->height());
    mainlayout->addWidget(line);

    mainlayout->addWidget(sidebarwid);
    mainlayout->setContentsMargins(0,0,0,0);
    mainlayout->setSpacing(0);
    centralWidget()->setLayout(mainlayout);
    mainwidget->setStyleSheet("background:transparent;");

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
}

/**
 * 添加阴影
 */
void MainWindow::paintEvent(QPaintEvent *)
{
    if(!is_fullscreen)
    {
        QPainter painter(this);
        const qreal radius =6;

        QRectF rect1;
        rect1.setX(0);
        rect1.setY(9);
        rect1.setWidth( this->rect().width()-9);
        rect1.setHeight(this->rect().height()-9);

        //加边框
        QPainterPath path1;
        path1.setFillRule(Qt::WindingFill);
        painter.setRenderHint(QPainter::Antialiasing);

        path1.moveTo(rect1.topRight() - QPointF(radius, 0));
        path1.lineTo(rect1.topLeft());
        path1.lineTo(rect1.bottomLeft());
        path1.lineTo(rect1.bottomRight());
        path1.lineTo(rect1.topRight() + QPointF(0, radius));
        path1.quadTo(rect1.topRight(), rect1.topRight() + QPointF(-radius, -0));
        painter.setPen(QPen(QColor("#0F0F0F"),2));
        painter.setOpacity(0.9);
        painter.setBrush(Qt::NoBrush);
        painter.drawPath(path1);

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
    this->setContentsMargins(0,10,10,0);
//    this->setGeometry(QRect(0,QApplication::desktop()->availableGeometry().height()-532,390+20+2,532+20+2));
    this->setGeometry(QRect(0,QApplication::desktop()->availableGeometry().height()-590-10,376+10,590+10));
    sidebarwid->load_min_sidebar();
    mainviewwid->load_min_mainview();
    //插入分割线
    mainlayout->insertWidget(1,line);
    this->repaint();
//    mainviewwid->load_classification_widget(widgetState);
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
    qDebug()<<"---"<<qApp->applicationState();
}

void MainWindow::recv_hover_signal_slot(bool is_hover)
{
    if(is_hover)
        pEnterAnimation->start();
    else
        pLeaveAnimation->start();

}
