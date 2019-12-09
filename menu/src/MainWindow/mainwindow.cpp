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
    UkuiMenuInterface::pAppInfo=pUkuiMenuInterface->create_appinfo_object();
    init_mainwindow();
    sidebarwid->set_sidebarbtn_state(1);

//    mainviewwid->load_classification_widget(1);

//    pAnimation=new QPropertyAnimation(this,"geometry");
//    pAnimation->setDuration(30);
//    pAnimation->setStartValue(QRect(0,QApplication::desktop()->availableGeometry().height()-532,390,532));
//    pAnimation->setEndValue(QApplication::desktop()->availableGeometry());
//    pAnimation->setEasingCurve(QEasingCurve::Linear);
//    connect(pAnimation,SIGNAL(finished()),this,SLOT(animation_finished_slot()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init_mainwindow()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::SplashScreen);
//    this->setStyleSheet("background-color:rgba(14,19,22,92%);");
//    this->setWindowOpacity(0.95);//设置总体透明度
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAutoFillBackground(false);
    this->setFocusPolicy(Qt::ClickFocus);

    ui->mainToolBar->hide();
    ui->menuBar->hide();
    ui->statusBar->hide();
    this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    this->setMinimumSize(390,532);
    this->setContentsMargins(0,0,0,0);

    mainwidget=new QWidget(this);
    sidebarwid=new SideBarWidget(this);
    mainviewwid=new MainViewWidget(this);

    this->setCentralWidget(mainwidget);
    mainlayout=new QHBoxLayout;
    mainlayout->addWidget(sidebarwid);
    mainlayout->addWidget(mainviewwid);
    mainlayout->setContentsMargins(0,0,0,0);
    mainlayout->setSpacing(0);
    centralWidget()->setLayout(mainlayout);
    mainwidget->setStyleSheet("background:transparent;");

    connect(sidebarwid, SIGNAL(send_commonusebtn_signal()), mainviewwid, SLOT(load_commonuse_widget()));
    connect(sidebarwid,SIGNAL(send_letterbtn_signal()), mainviewwid, SLOT(load_letter_widget()));
    connect(sidebarwid, SIGNAL(send_functionbtn_signal()), mainviewwid, SLOT(load_function_widget()));

    connect(sidebarwid,SIGNAL(send_fullscreen_commonusebtn_signal()),
            mainviewwid,SLOT(load_fullcommonuse_widget()));
    connect(sidebarwid,SIGNAL(send_fullscreen_letterbtn_signal()),
            mainviewwid,SLOT(load_fullletter_widget()));
    connect(sidebarwid, SIGNAL(send_fullscreen_functionbtn_signal()),
            mainviewwid, SLOT(load_fullfunction_widget()));

    connect(mainviewwid,SIGNAL(send_fullscreenbtn_signal(int)),this,SLOT(show_fullscreen_widget(int)));
    connect(mainviewwid, SIGNAL(send_defaultbtn_signal(int)),this,SLOT(show_default_widget(int)));
    connect(mainviewwid,SIGNAL(send_hide_mainwindow_signal()),this,SLOT(recv_hide_mainwindow_slot()));
    connect(sidebarwid,SIGNAL(send_hide_mainwindow_signal()),this,SLOT(recv_hide_mainwindow_slot()));
}

/**
 * 添加阴影
 */
void MainWindow::paintEvent(QPaintEvent *)
{
//    QPainter painter(this);
//    //加边框
//    QPainterPath path;
//    path.setFillRule(Qt::WindingFill);
//    path.addRect(0, 20, this->width()-20, this->height()-20);
//    char bordercolor[20];
//    sprintf(bordercolor,"%s", BORDERCOLOR);
//    painter.setPen(QColor(QString::fromLocal8Bit(bordercolor)));
//    painter.drawPath(path);

//    //加阴影
//    char shadowcolor[20];
//    sprintf(shadowcolor,"%s", SHADOWCOLOR);
//    QColor color(QString::fromLocal8Bit(shadowcolor));
////    for(int i=0; i<10; i++)
//    for(int i=-1; i>=-10; i--)
//    {
//        QPainterPath path;
//        path.setFillRule(Qt::WindingFill);
////        path.addRect(-i, 20-i, this->width()-(10-i)*2, this->height()-(10-i)*2);
//        path.addRect(i, 20+i, this->width()-(10+i)*2, this->height()-(10+i)*2);
//        color.setAlpha(static_cast<int>(150 - qSqrt(-i-1)*50));
//        painter.setPen(color);
//        painter.drawPath(path);
//    }
}

/**
 * 显示全屏窗口
 */
void MainWindow::show_fullscreen_widget(int arg)
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

//    this->setContentsMargins(0,0,0,0);
    this->setGeometry(QApplication::desktop()->availableGeometry());
    sidebarwid->load_max_sidebar();
    mainviewwid->load_max_mainview();
    mainviewwid->load_full_classification_widget(arg);
}

/**
 * 显示默认窗口
 */
void MainWindow::show_default_widget(int arg)
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

//    this->setContentsMargins(1,21,21,1);
    this->setGeometry(QRect(0,QApplication::desktop()->availableGeometry().height()-532,390,532));
    sidebarwid->load_min_sidebar();
    mainviewwid->load_min_mainview();
    mainviewwid->load_classification_widget(arg);
}

//void MainWindow::animation_finished_slot()
//{
//    if(is_full)
//    {
//        this->setContentsMargins(0,0,0,0);
//        sidebarwid->load_max_sidebar();
//        mainviewwid->load_max_mainview();
//        mainviewwid->load_full_classification_widget(classification_widget);
//        pAnimation->setStartValue(QApplication::desktop()->availableGeometry());
//        pAnimation->setEndValue(QRect(0,QApplication::desktop()->availableGeometry().height()-532,390+20+2,532+20+2));
//    }
//    else{
//        this->setContentsMargins(1,21,21,1);
//        sidebarwid->load_min_sidebar();
//        mainviewwid->load_min_mainview();
//        mainviewwid->load_classification_widget(classification_widget);
//        pAnimation->setStartValue(QRect(0,QApplication::desktop()->availableGeometry().height()-532,390+20+2,532+20+2));
//        pAnimation->setEndValue(QApplication::desktop()->availableGeometry());
//    }
//    mainwidget->setStyleSheet("background:transparent;");
//    sidebarwid->setVisible(true);
//    mainviewwid->setVisible(true);
//}

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
}
