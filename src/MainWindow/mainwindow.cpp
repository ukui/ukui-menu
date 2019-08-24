#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QDebug>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init_mainwindow();
    sidebarwid->set_sidebarbtn_state(1);
    mainviewwid->load_classification_widget(1);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init_mainwindow()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::SplashScreen);
//    this->setStyleSheet("background-color:#0e1316;");
//    this->setWindowOpacity(0.95);//设置总体透明度
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAutoFillBackground(false);
    ui->mainToolBar->hide();
    ui->menuBar->hide();
    ui->statusBar->hide();
    this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    this->setMinimumSize(390,532);
    this->setContentsMargins(0,0,0,0);

    sidebarwid=new SideBarWidget();
    mainviewwid=new MainViewWidget();
    mainwidget=new QWidget();
    mainwidget->setStyleSheet("background:transparent;");

    this->setCentralWidget(mainwidget);
    mainlayout=new QHBoxLayout;
    mainlayout->addWidget(sidebarwid);
    mainlayout->addWidget(mainviewwid);
    mainlayout->setContentsMargins(0,0,0,0);
    mainlayout->setSpacing(0);
    centralWidget()->setLayout(mainlayout);

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
}

/**
 * 显示全屏窗口
 */
void MainWindow::show_fullscreen_widget(int arg)
{
//    this->showMaximized();
    this->setGeometry(QApplication::desktop()->availableGeometry());
    sidebarwid->load_max_sidebar();
    mainviewwid->load_max_mainview();
//    qDebug()<<QApplication::desktop()->availableGeometry().width()<<":"
//           <<QApplication::desktop()->availableGeometry().height();
//    qDebug()<<QApplication::desktop()->width()<<":"<<QApplication::desktop()->height();
    mainviewwid->load_full_classification_widget(arg);
//    sidebarwid->set_sidebarbtn_state(arg);
}

/**
 * 显示默认窗口
 */
void MainWindow::show_default_widget(int arg)
{
//    this->showNormal();
    this->setGeometry(QRect(0,QApplication::desktop()->availableGeometry().height()-532,390,532));
    sidebarwid->load_min_sidebar();
    mainviewwid->load_min_mainview();
    mainviewwid->load_classification_widget(arg);
//    sidebarwid->set_sidebarbtn_state(arg);

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
             qApp->quit();
        }
   }
   return QWidget::event(event);
}
