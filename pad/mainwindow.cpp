#include "mainwindow.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QRect>
#include <QDebug>
#include <QDateTime>
#include <QSettings>
//#include <QSvgRenderer>
#include <QIcon>
#include <qgsettings.h>
#include <QProcess>
#define TABLED_SCHEMA "org.ukui.SettingsDaemon.plugins.tablet-mode"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
   // setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);
    this->resize(QApplication::primaryScreen()->geometry().width(),QApplication::primaryScreen()->geometry().height()-46);
    this->setAttribute(Qt::WA_TranslucentBackground,true);
    this->setAutoFillBackground(false);
    this->setFocusPolicy(Qt::ClickFocus);
    this->setProperty("useStyleWindowManager",false);
    m_frame=new QFrame;
    this->setCentralWidget(m_frame);


    //获取数据
    UkuiMenuInterface m_ukuiMenuInterface;
    UkuiMenuInterface::appInfoVector=m_ukuiMenuInterface.createAppInfoVector();
    UkuiMenuInterface::allAppVector=m_ukuiMenuInterface.getAllApp();

    //左侧
    leftWidget=new QWidget;
    leftWidget->setFixedSize(511,QApplication::primaryScreen()->geometry().height()-46);
    leftLayout=new QVBoxLayout();
    //左主
    leftWidget->setLayout(leftLayout);
    leftLayout->setContentsMargins(111,96,0,0);

    //右侧界面
    m_CommonUseWidget=new FullCommonUseWidget();

    QHBoxLayout  *mainLayout =new QHBoxLayout();
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);
    m_frame->setLayout(mainLayout);
    mainLayout->addWidget(leftWidget);
    mainLayout->addWidget(m_CommonUseWidget);


    //获取系统背景图片 gsettings get org.mate.background picture-filename
    QGSettings* gsetting=new QGSettings(QString("org.mate.background").toLocal8Bit());
    QString iconPath =gsetting->get("picture-filename").toString();

    m_frame->setAutoFillBackground(true);
    QPixmap pixmap = QPixmap(iconPath).scaled(this->size());
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(pixmap));
    m_frame->setPalette(palette);

    QPropertyAnimation *animation = new QPropertyAnimation(m_frame->window(), "windowOpacity");
     animation->setDuration(500);
     animation->setStartValue(0);
     animation->setEndValue(1);
     animation->start();
    connect(animation,&QPropertyAnimation::finished,this,&MainWindow::displayMainViewSlot);

    //平板
    tabletMode=new QGSettings(TABLED_SCHEMA);
    connect(tabletMode,&QGSettings::changed,[this] (const QString &key) {
        bool res = tabletMode->get("tablet-mode").toBool();
        qDebug()<<res;
        if(!res)
            this->hide();
        else
             this->show();

    });


    //监控.desktop文件目录
    m_fileWatcher=new QFileSystemWatcher;
    m_fileWatcher->addPaths(QStringList()<<"/usr/share/applications");
    connect(m_fileWatcher,&QFileSystemWatcher::directoryChanged,this,&MainWindow::directoryChangedSlot);
    m_directoryChangedThread=new DirectoryChangedThread;
    connect(this,&MainWindow::sendDirectoryPath,m_directoryChangedThread,&DirectoryChangedThread::recvDirectoryPath);
    connect(m_directoryChangedThread,&DirectoryChangedThread::requestUpdateSignal,this,&MainWindow::requestUpdateSlot);
    connect(m_directoryChangedThread,&DirectoryChangedThread::deleteAppSignal,this,&MainWindow::requestDeleteAppSlot);


}

MainWindow::~MainWindow()
{
    delete m_ukuiMenuInterface;
    delete m_fileWatcher;
    delete m_directoryChangedThread;
}

void MainWindow::directoryChangedSlot()
{
    Q_EMIT sendDirectoryPath(QString("/usr/share/applications"));
    m_directoryChangedThread->start();

}
void MainWindow::requestUpdateSlot(QString desktopfp)
{
    m_directoryChangedThread->quit();
    m_CommonUseWidget->updateListView(desktopfp);
}

void MainWindow::requestDeleteAppSlot()
{
    m_directoryChangedThread->quit();
    m_CommonUseWidget->deleteAppListView();
}

void MainWindow::displayMainViewSlot()
{
    //加载左侧界面
    leftWidgetDisplay();
    //右侧界面填充数据
    m_CommonUseWidget->fillAppList();
}

void MainWindow::leftWidgetDisplay()
{
    //左上
    leftUpWidget=new QWidget;
    leftUpWidget->setFixedSize(400,96);
    leftUpLayout=new QHBoxLayout();
    leftUpWidget->setLayout(leftUpLayout);
    leftUpLayout->setContentsMargins(0,0,0,0);

    //左下
    downWidget=new QWidget;
    downWidget->setFixedSize(400,56);


     //左上左
    upLeftWidget=new QWidget;
    upLeftWidget->setFixedSize(263,96);
    upLeftLayout=new QHBoxLayout();
    upLeftWidget->setLayout(upLeftLayout);
    upLeftLayout->setContentsMargins(0,0,0,0);
   // upLeftWidget->setStyleSheet("border-width:1px;border-style:solid;border-color:red");

    //左上右
    upRightWidget=new QWidget;
    upRightWidget->setFixedSize(170,100);
    upRightLayout=new QVBoxLayout();
    upRightWidget->setLayout(upRightLayout);
    upRightLayout->setContentsMargins(8,8,0,0);
   // upRightWidget->setStyleSheet("border-width:1px;border-style:solid;border-color:red");

     //左下
    downLayout=new QVBoxLayout();
    downWidget->setLayout(downLayout);

    //左侧控件
    timeLabel=new QLabel();
    weekLabel=new QLabel();
    dateLabel=new QLabel();

    //搜索
    searchEdit=new QLineEdit();
    searchEdit->setPlaceholderText(tr("Search your application、file"));
    QAction* searchAction =new QAction();
    searchAction->setIcon(QIcon(":/img/search.png"));
    searchEdit->addAction(searchAction,QLineEdit::LeadingPosition);
    searchEdit->setStyleSheet("border-radius:10px;");
    searchEdit->setFocusPolicy(Qt::ClickFocus);
    connect(searchEdit, &QLineEdit::returnPressed, this, &MainWindow::obtainSearchResult);


    dateTime = new QDateTime;
    timer = new QTimer(this);

    //获得语言环境
    QLocale currentlocale = QLocale::system();
    QLocale::setDefault(QLocale(currentlocale.language(),currentlocale.country()));
    locale = new QLocale;


    downLayout->addWidget(searchEdit);

    leftLayout->addWidget(leftUpWidget);
    leftLayout->addWidget((downWidget));
    leftLayout->addStretch();

   leftUpLayout->addWidget(upLeftWidget);
   leftUpLayout->addWidget(upRightWidget);

   upLeftLayout->addWidget(timeLabel);
   upRightLayout->addWidget(weekLabel);
   upRightLayout->addWidget(dateLabel);
   weekLabel->setAlignment(Qt::AlignCenter|Qt::AlignHCenter|Qt::AlignBottom);
   dateLabel->setAlignment(Qt::AlignCenter);

   timeLabel->setStyleSheet("border:0px;background:transparent;font-size:96px;color:white;");
   dateLabel->setStyleSheet("border:0px;background:transparent;font-size:38px;color:white;");
   weekLabel->setStyleSheet("border:0px;background:transparent;font-size:38px;color:white;");


   timeLabel->setText(locale->toString(dateTime->currentDateTime().time(),"hh:mm"));
   dateLabel->setText(locale->toString(dateTime->currentDateTime().date(),"MM-dd"));
   weekLabel->setText(locale->toString(dateTime->currentDateTime().date(),"dddd"));
//   timeLabel->setText(dateTime->currentDateTime().toString("hh:mm"));
//   dateLabel->setText(dateTime->currentDateTime().toString("MM-dd"));
//   weekLabel->setText(dateTime->currentDateTime().toString("dddd"));



   connect(timer,&QTimer::timeout,[this] () {
       timeLabel->setText(locale->toString(dateTime->currentDateTime().time(),"hh:mm"));
       dateLabel->setText(locale->toString(dateTime->currentDateTime().date(),"MM-dd"));
       weekLabel->setText(locale->toString(dateTime->currentDateTime().date(),"dddd"));

   });

   timer->start(30000);
}

void MainWindow::obtainSearchResult()
{
    //获取搜索框内容
    QString searchContent= searchEdit->text();
    QProcess p;
    p.setProgram(QString("peony"));
    p.setArguments(QStringList()<<QString("search:///search_uris=file:///&name_regexp=%1&recursive=1").arg(searchContent));
    p.startDetached(p.program(), p.arguments());
    p.waitForFinished(-1);

}
