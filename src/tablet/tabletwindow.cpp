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

#include "tabletwindow.h"
#include <QDesktopWidget>
#include <QHeaderView>
#include "src/Style/style.h"
#include <QDebug>
#include <QSvgRenderer>
#include <QPainter>
#include "src/DataProcess/getmodeldata.h"
#include "src/ListView/tabletlistview.h"
#include "src/tablet/XEventMonitor/xeventmonitor.h"
#include <QGraphicsEffect>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QEvent>

QT_BEGIN_NAMESPACE
extern void qt_blurImage(QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0);
QT_END_NAMESPACE

TabletWindow::TabletWindow(QWidget *parent) :
    QWidget(parent)
{
    QString path=QDir::homePath()+"/.config/ukui/ukui-menu.ini";
    setting=new QSettings(path,QSettings::IniFormat);
    m_pagemanager = new PageManager();
    Style::initWidStyle();
    initUi();
}

TabletWindow::~TabletWindow()
{

}

QVector<QString> TabletWindow::keyVector=QVector<QString>();
QVector<int> TabletWindow::keyValueVector=QVector<int>();

void TabletWindow::initUi()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setAttribute(Qt::WA_TranslucentBackground,true);
    this->setAutoFillBackground(false);
    this->setFocusPolicy(Qt::NoFocus);
    m_width=QApplication::primaryScreen()->geometry().width();
    m_height=QApplication::primaryScreen()->geometry().height();
    this->setFixedSize(/*Style::MainViewWidWidth*/m_width,
                       m_height);
    pixmap=new QPixmap;

    leftWidget = new TimeWidget(this);
    leftWidget->setFixedSize(512,m_height);
    firstPageWidget = new QWidget(this);

    buttonGroup = new QButtonGroup;

    buttonBoxLayout = new QHBoxLayout;
    buttonBoxLayout->setAlignment(Qt::AlignHCenter);
    buttonBoxLayout->setSpacing(0);
    buttonWidget = new QWidget(this);
    buttonWidget->setLayout(buttonBoxLayout);
    buttonBoxLayout->setContentsMargins(0,0,0,0);

    setOpacityEffect(0.7);
    bool isfile=appListFile();//判断监控看、路径是否存在
    m_fileWatcher=new QFileSystemWatcher;
 //   m_fileWatcher->addPath(QDir::homePath()+"/.cache/uksc/applist");
 //   connect(m_fileWatcher,&QFileSystemWatcher::fileChanged,this,&MainWindow::directoryChangedSlot);

    m_fileWatcher->addPaths(QStringList()<<QString("/usr/share/applications")
                                      <<QString(QDir::homePath()+"/.local/share/applications/"));
    connect(m_fileWatcher,&QFileSystemWatcher::directoryChanged,this,&TabletWindow::directoryChangedSlot);


    m_fileWatcher1=new QFileSystemWatcher;
    bool ismonitor=m_fileWatcher1->addPath(QDir::homePath()+"/.config/ukui/desktop_applist");
    //m_fileWatcher1->addPaths(QStringList()<<QDir::homePath()+"/.config/ukui/desktop_applist");
    connect(m_fileWatcher1,&QFileSystemWatcher::fileChanged,this,&TabletWindow::directoryChangedSlot);
//    if(isfile)
//    {
//        QString filepath=QDir::homePath()+"/.config/ukui/ukui-menu.ini";
//        QSettings *filetsetting=new QSettings(filepath,QSettings::IniFormat);
//        filetsetting->beginGroup("ukui-menu-sysapplist");
//        filetsetting->setValue("desktop_applist",1);
//        filetsetting->sync();
//        filetsetting->endGroup();
//        if(ismonitor)
//        {
//            filetsetting->beginGroup("ukui-menu-sysapplist");
//            filetsetting->setValue("desktop_applist",0);
//            filetsetting->sync();
//            filetsetting->endGroup();
//        }
//        filetsetting->beginGroup("application");
//        filetsetting->setValue("kylin-user-guide.desktop",1000);
//        filetsetting->sync();
//        filetsetting->endGroup();
//    }

    m_directoryChangedThread=new TabletDirectoryChangedThread;
    connect(this,&TabletWindow::sendDirectoryPath,m_directoryChangedThread,&TabletDirectoryChangedThread::recvDirectoryPath);
    connect(m_directoryChangedThread,&TabletDirectoryChangedThread::requestUpdateSignal,this,&TabletWindow::requestUpdateSlot);
    connect(m_directoryChangedThread,&TabletDirectoryChangedThread::deleteAppSignal,this,&TabletWindow::requestDeleteAppSlot);

    initAppListWidget();

    m_scrollAnimation = new QPropertyAnimation(m_scrollArea->horizontalScrollBar(), "value");
    m_scrollAnimation->setEasingCurve(QEasingCurve::Linear);
    connect(m_scrollAnimation, &QPropertyAnimation::finished, this, &TabletWindow::animationFinishSlot);
    connect(m_scrollAnimation, &QPropertyAnimation::valueChanged, this, &TabletWindow::animationValueChangedSlot);

    flag = true;
    //翻页灵敏度时间调节
    time = new QTimer(this);
    connect(time,&QTimer::timeout,[=](){
        if(flag == false)
        {
            flag = true;
            time->stop();
        }
    });

    if(QGSettings::isSchemaInstalled(QString("org.mate.background").toLocal8Bit()))
    {
        bg_setting=new QGSettings(QString("org.mate.background").toLocal8Bit());
        bgPath=bg_setting->get("picture-filename").toString();
        bgOption = bg_setting->get("pictureOptions").toString();

        connect(bg_setting,&QGSettings::changed,this, [=](const QString &key)
        {
            if (key == "pictureFilename")
            {
                //在每个屏幕上绘制背景
                bgPath = bg_setting->get("picture-filename").toString();
                bgOption = bg_setting->get("pictureOptions").toString();
                ways();//壁纸显示方式
            }
            if (key == "pictureOptions")
            {
                //在每个屏幕上绘制背景
                bgOption = bg_setting->get("pictureOptions").toString();
                ways();
            }
        });
    }

    usrInterface = new QDBusInterface("com.kylin.statusmanager.interface",
                              "/",
                              "com.kylin.statusmanager.interface",
                              QDBusConnection::sessionBus());

     QDBusConnection::sessionBus().connect("com.kylin.statusmanager.interface",
                                         "/",
                                         "com.kylin.statusmanager.interface",
                                         "mode_change_signal",
                                         this,
                                         SLOT(modelChanged(bool))
                                         );

     //特效模式,此处Gsetting不明确，需进一步确认
     if(QGSettings::isSchemaInstalled(QString("org.ukui.control-center.personalise").toLocal8Bit()))
     {
         bg_effect = new QGSettings(QString("org.ukui.control-center.personalise").toLocal8Bit());
         setOpacityEffect(bg_effect->get("transparency").toReal());

         connect(bg_effect,&QGSettings::changed,[this] (const QString &key)
         {
             if (key == "effect")
             {
                 if(bg_effect->get("effect").toBool())
                 {
                     setOpacityEffect(bg_effect->get("transparency").toReal());
                 }
                 else
                 {
                     setOpacityEffect(bg_effect->get("transparency").toReal());
                 }
             }
         });
     }

     //pc下鼠标功能
    XEventMonitor::instance()->start();
    connect(XEventMonitor::instance(), SIGNAL(keyRelease(QString)),
         this,SLOT(XkbEventsRelease(QString)));
    connect(XEventMonitor::instance(), SIGNAL(keyPress(QString)),
         this,SLOT(XkbEventsPress(QString)));

    ways();
    buttonWidgetShow();
    connect(this,&TabletWindow::pagenumchanged,this,&TabletWindow::pageNumberChanged);
}

/**
 * 初始化应用列表界面
 */
void TabletWindow::initAppListWidget()
{
    layout=new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);

    firstPageLayout =new QHBoxLayout();

    m_scrollArea=new ScrollArea;
    m_scrollAreaWid=new ScrollAreaWid(this);
    m_scrollAreaWid->setStyleSheet("border:0px; background:transparent;");
    m_scrollArea->setFixedSize(m_width,m_height - 20);
    m_scrollArea->setWidget(m_scrollAreaWid);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setStyleSheet("border:0px; background:transparent;");

    m_scrollAreaWidLayout=new QHBoxLayout(m_scrollAreaWid);
    m_scrollAreaWidLayout->setContentsMargins(0,0,0,0);
    m_scrollAreaWidLayout->setSpacing(0);
    layout->addWidget(m_scrollArea);
    layout->addWidget(buttonWidget);
    buttonWidget->setFixedSize(1920,40);
    m_appListBottomSpacer=new QSpacerItem(20,40,QSizePolicy::Fixed,QSizePolicy::Expanding);
    fillAppList();
}

void TabletWindow::modelChanged(bool value)
{
    if (value)
    {
        ways();
        recvHideMainWindowSlot();
    }
}

bool TabletWindow::appListFile()
{
//    qDebug()<<"应用列表文件==============================是否文件存在";
    QFile fp;//要包含必要的头文件，这里省略
    QString path=QDir::homePath()+"/.config/ukui/desktop_applist";
    fp.setFileName(path);                      //为fp指定包含路径的文件名
    if(fp.exists())                                     //若存在，读取
    {
        //QString(text);
        qDebug()<<"文件存在";
/*        fp.open(QIODevice::ReadOnly); */                  //打开 和 关闭 要紧密相关
     return 1;
    }
    else                                                //若不存在，则通过open操作新建文件
    {
//        qDebug()<<"文件不存在";
        fp.open(QIODevice::ReadWrite|QIODevice::Text);  //不存在的情况下，打开包含了新建文件的操作
        //fp.write("I am writing file");
//        qDebug()<<"正在写文件";
        fp.close();
        return 1;
    }
    return 0;

}

//打开PC模式下的开始菜单
void TabletWindow::showPCMenu()
{
    this->show();
    this->raise();
    this->activateWindow();
}

//改变搜索框及工具栏透明度
void TabletWindow::setOpacityEffect(const qreal& num)
{
    leftWidget->setDownOpacityEffect(num); //全局搜索框透明度
}

void TabletWindow::reloadAppList()
{
    QVector<QStringList> vector;

    m_data.clear();
    keyVector.clear();
    keyValueVector.clear();

    vector = m_pagemanager->getAppPageVector();
    if(!vector.at(0).isEmpty())
    {
        QLayoutItem* widItem = firstPageLayout->itemAt(1);
        QWidget* wid = widItem->widget();
        TabletListView* m_listview = qobject_cast<TabletListView*>(wid);
        m_listview->updateData(vector.at(0));
    }
    for(int i = 1; i < vector.size(); i++)
    {
        if(!vector.at(i).isEmpty())
        {
            QLayoutItem* widItem = m_scrollAreaWidLayout->itemAt(i * 2);
            QWidget* wid = widItem->widget();
            TabletListView* m_listview = qobject_cast<TabletListView*>(wid);
            m_listview->updateData(vector.at(i));
        }
    }
}

void TabletWindow::reloadWidget()
{
    QLayoutItem* child;
    if(firstPageLayout->count() == 2)
    {
        QLayoutItem* widItem = firstPageLayout->itemAt(1);
        QWidget* wid = widItem->widget();
        TabletListView* m_listview = qobject_cast<TabletListView*>(wid);
        delete m_listview;
        firstPageLayout->removeWidget(leftWidget);
    }

    while((child = m_scrollAreaWidLayout->takeAt(1)) != 0)
    {
        if(child->widget() != 0)
        {
            delete child->widget();
        }
        delete child;
    }
    isFirstPage = true;
    fillAppList();
    buttonWidgetShow();
}
/**
 * 填充应用列表
 */
void TabletWindow::fillAppList()
{
    QVector<QStringList> vector;

    m_data.clear();
    keyVector.clear();
    keyValueVector.clear();

    vector = m_pagemanager->getAppPageVector();

    for(int i = 0; i < vector.size(); i++)
    {
        QStringList applist = vector.at(i);
        if(!applist.isEmpty())
        {
            if(!isFirstPage)
            {
                 insertAppList(QStringList());
            }
            insertAppList(applist);
        }
    }
}
bool TabletWindow::cmpApp(QString &arg_1, QString &arg_2)
{
    if(keyValueVector.at(keyVector.indexOf(arg_1)) < keyValueVector.at(keyVector.indexOf(arg_2)))
        return true;
    else
        return false;
}

void TabletWindow::directoryChangedSlot()
{
    Q_EMIT sendDirectoryPath(QString("/usr/share/applications"));
    m_directoryChangedThread->start();
}
void TabletWindow::requestUpdateSlot(QString desktopfp)
{
    m_directoryChangedThread->quit();
    reloadWidget();
    connect(m_fileWatcher1,&QFileSystemWatcher::fileChanged,this,&TabletWindow::directoryChangedSlot);
}

void TabletWindow::requestDeleteAppSlot()
{
    m_directoryChangedThread->quit();
    reloadWidget();
    connect(m_fileWatcher1,&QFileSystemWatcher::fileChanged,this,&TabletWindow::directoryChangedSlot);
}

void TabletWindow::on_pageNumberChanged(bool nextPage)
{
    if(nextPage)
    {
        curPageNum++;
        if(curPageNum > (m_scrollAreaWidLayout->count() -1) / 2)
        {
            curPageNum = (m_scrollAreaWidLayout->count() -1) / 2;
        }
    }
    else
    {
        curPageNum--;
        if(curPageNum < 0)
        {
            curPageNum = 0;
        }
    }
    btnGroupClickedSlot(curPageNum * 2);
    pageNumberChanged(curPageNum + 1);
}

void TabletWindow::insertAppList(QStringList desktopfplist)
{
    TabletListView* listview = nullptr;
    if(isFirstPage)
    {
        listview = new TabletListView(this,0);
        firstPageLayout->setSpacing(60);
        firstPageWidget->setLayout(firstPageLayout);
        firstPageLayout->addWidget(leftWidget);
        listview->setFixedSize(m_width - 512,m_height - 20);
        firstPageLayout->addWidget(listview);
        m_scrollAreaWidLayout->addWidget(firstPageWidget);
        listview->setGridSize(QSize(216,(this->height() - 20)/4));
        isFirstPage = false;
    }
    else
    {
        listview = new TabletListView(this,1);
        listview->setFixedSize(m_width,m_height - 20);
        listview->setGridSize(QSize(318,(this->height() - 20)/4));
        m_scrollAreaWidLayout->addWidget(listview);
    }
    //修复异常黑框问题
    connect(m_scrollArea, &ScrollArea::requestUpdate, listview->viewport(), [=](){
        listview->repaint(listview->rect());
    });
    connect(listview,&TabletListView::pagenumchanged,this,&TabletWindow::on_pageNumberChanged);

    m_data.clear();
    for(int i=0;i<desktopfplist.count();i++)
        m_data.append(desktopfplist.at(i));
    listview->addData(m_data);
    connect(listview,&TabletListView::sendItemClickedSignal,this,&TabletWindow::execApplication);
    connect(listview,&TabletListView::sendHideMainWindowSignal,this,&TabletWindow::recvHideMainWindowSlot);
    connect(listview,&TabletListView::sendUpdateAppListSignal,this,&TabletWindow::reloadAppList);
}

void TabletWindow::recvStartMenuSlot()
{
    QDBusReply<bool> res = usrInterface->call("get_current_tabletmode");
    if(this->isVisible())//wgx
    {
        if (!res)//平板模式 下禁止win隐藏菜单
        {
            this->hide();
        }
    }
    else{
        if (!res)//平板模式 下禁止win隐藏菜单
        {
            this->showPCMenu();
        }
    }
}

/**
 * 执行应用程序
 */
void TabletWindow::execApplication(QString desktopfp)
{
    Q_EMIT sendHideMainWindowSignal();
    execApp(desktopfp);
}

void TabletWindow::paintEvent(QPaintEvent *event)
{
    backgroundPic();//bgPath,picrect
    //在每个屏幕上绘制背景
//    return QWidget::paintEvent(event);
}

QImage TabletWindow::applyEffectToImage(QImage src, QGraphicsEffect *effect, int extent)
{
    //qDebug()<<"6、绘制特效";
    if(src.isNull()) return QImage();   //No need to do anything else!
    if(!effect) return src;             //No need to do anything else!
    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(QPixmap::fromImage(src));
    item.setGraphicsEffect(effect);
    scene.addItem(&item);
    QImage res(src.size()+QSize(extent*2, extent*2), QImage::Format_ARGB32);
    res.fill(Qt::transparent);//transparent
    QPainter ptr(&res);
    scene.render(&ptr, QRectF( -extent, -extent, src.width()+extent*2, src.height()+extent*2 ) ,QRectF());
    return res;
}

QPixmap * TabletWindow::blurPixmap(QPixmap *pixmap)
{
    //qDebug()<<"blurPixmap";
    QPainter painter(pixmap);
    QImage srcImg = pixmap->toImage();
    //qt_blurImage(&painter,srcImg,2,true,false);//top 27000//150

    QGraphicsBlurEffect *blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(70);
    QImage result = applyEffectToImage(srcImg, blur,70);
    qt_blurImage(&painter,result,1,false,false);//top 27000//150
    painter.end();
    return pixmap;
}

void TabletWindow::pageNumberChanged(int pageNum)
{
    if(m_pagemanager->getAppPageVector().size() != 1)
    {
        for(int page=1; page <= m_pagemanager->getAppPageVector().size(); page++)
        {
             if(pageNum == page)
             {
                 buttonGroup->button(page)->setStyleSheet("QPushButton{border-image:url(:/data/img/mainviewwidget/selected.svg);}"
                                                          "QPushButton:hover{border-image: url(:/data/img/mainviewwidget/selected.svg);}"
                                                          "QPushButton:pressed{border-image: url(:/data/img/mainviewwidget/selected.svg);}");
             }
             else
             {
                 buttonGroup->button(page)->setStyleSheet("QPushButton{border-image:url(:/data/img/mainviewwidget/select.svg);}"
                                                              "QPushButton:hover{border-image: url(:/data/img/mainviewwidget/select.svg);}"
                                                              "QPushButton:pressed{border-image: url(:/data/img/mainviewwidget/select.svg);}");
            }
        }
    }
}

void TabletWindow::ways()
{
    pixmap1 = QPixmap(bgPath);
    if (bgOption == "zoom" || bgOption == "" || bgOption == NULL)
        bgOption = "scaled";
    if (bgOption == "centered") //居中
    {
        qDebug() << "---------" << "centered" << "----------";
        pixmap->load(bgPath);

    }
    else if (bgOption=="stretched") //拉伸
    {
        pixmap1 = pixmap1.scaled(this->size());
        pixmap = &pixmap1;
    }
    else if (bgOption == "scaled") //填充
    {
        pixmap->load(bgPath);
    }
    else if (bgOption == "wallpaper") //平铺
    {
        pixmap->load(bgPath);

    }
    else
    {
        pixmap1 = pixmap1.scaled(this->size());
        pixmap = &pixmap1;
    }

    pixmap = blurPixmap(pixmap);
}

/**
 * @brief FullBackgroundWidget::getPaddingPixmap
 * @param pixmap 需要填充的图像
 * @param width  容器宽度
 * @param height 容器高度
 * @return
 */
QPixmap TabletWindow::getPaddingPixmap(QPixmap pixmap, int width, int height)
{
    if (pixmap.isNull() || pixmap.width() == 0 || pixmap.height() == 0)
    {
        return QPixmap();
    }

    bool useHeight;
    float scaled = 0.0;
    QPixmap scaledPixmap;
    QPixmap paddingPixmap;
    qint64 rw = qint64(height) * qint64(pixmap.width()) / qint64(pixmap.height());

    useHeight = (rw >= width);
    if (useHeight) {
        scaled = float(height) / float(pixmap.height());
        scaledPixmap = pixmap.scaled(pixmap.width() * scaled, height);
        paddingPixmap = scaledPixmap.copy((pixmap.width() * scaled - width) / 2 , 0, width, height);
    } else {
        scaled = float(width) / float(pixmap.width());
        scaledPixmap = pixmap.scaled(width, pixmap.height() * scaled);
        paddingPixmap = scaledPixmap.copy(0 , (pixmap.height() * scaled - height) / 2,width, height);
    }

    return paddingPixmap;
}

void TabletWindow::backgroundPic()//const QString &bgPath,QRect rect
{
//    qDebug()<<"5、绘制背景";
    QPainter painter(this);
    if(/*hideBackground*/false)
    {
        QColor cor;
        cor="#252729";
        painter.setBrush(cor);
        painter.drawRect(this->rect());
    }
    else if (bgOption == "zoom" || bgOption == "" || bgOption == NULL)
        bgOption = "scaled";
    else if (bgOption== "centered") //居中
    {
        QColor cor;
        cor="#000000";
        painter.setBrush(cor);
        painter.drawRect(this->rect());
        painter.drawPixmap(this->width()/2-pixmap->width()/2,
                           this->height()/2-pixmap->height()/2,
                           *pixmap);
    }
    else if (bgOption == "stretched") //拉伸
    {
        //qDebug() << "---------" << "stretched" << "----------";
        painter.drawPixmap(this->rect(),*pixmap);
    }
    else if (bgOption == "scaled") //填充
    {
        qDebug() << "---------" << "scaled" << "---------";
        painter.drawPixmap(this->geometry(), getPaddingPixmap(*pixmap, this->size().width(),this->size().height()));
    }
    else if (bgOption == "wallpaper") //平铺
    {
        //qDebug() << "---------" << "wallpaper" << "----------";
        int drawedWidth = 0;
        int drawedHeight = 0;
        while(1)
        {
            drawedWidth= 0;
            while(1)
            {
                painter.drawPixmap(drawedWidth,drawedHeight,*pixmap);
                drawedWidth += pixmap->width();
                if(drawedWidth >= this->width())
                    break;
            }
            drawedHeight += pixmap->height();
            if(drawedHeight >= this->height())
                break;
        }
    }
    else
    {
        painter.drawPixmap(this->rect(),*pixmap);
    }
}

void TabletWindow::recvHideMainWindowSlot()
{
    this->setAttribute(Qt::WA_TranslucentBackground,true);
    this->setAttribute(Qt::WA_X11NetWmWindowTypeDesktop,false);
    this->setWindowFlags(Qt::CustomizeWindowHint |Qt::FramelessWindowHint|Qt::X11BypassWindowManagerHint);
    this->hide();
}

void TabletWindow::btnGroupClickedSlot(int pageNum)
{
    m_beginPos=m_scrollArea->horizontalScrollBar()->sliderPosition();

    m_endPos=m_scrollAreaWidLayout->itemAt(pageNum)->widget()->x();
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollAnimation->stop();
    m_scrollAnimation->setDuration(500);
    m_scrollAnimation->setStartValue(m_beginPos);
    m_scrollAnimation->setEndValue(m_endPos);
    m_scrollAnimation->start();
}

void TabletWindow::buttonWidgetShow()
{
    //qDebug()<<"4、buttonWidgetShow";
    delete buttonBoxLayout;
    buttonBoxLayout = new QHBoxLayout;
    buttonWidget->setLayout(buttonBoxLayout);
    buttonBoxLayout->setAlignment(Qt::AlignHCenter);
    buttonBoxLayout->setSpacing(16);
    buttonBoxLayout->setContentsMargins(0,0,0,0);

    for (auto button : buttonGroup->buttons()) {
        buttonGroup->removeButton(button);
        button->deleteLater();
    }
    QDBusReply<bool> var = usrInterface->call("get_current_tabletmode");
//    res = var;
    for(int page=1;page<= m_pagemanager->getAppPageVector().size();page++)
    {
         button=new QPushButton;
         button->setFocusPolicy(Qt::NoFocus);
         button->setFixedSize(24,24);

         button->setStyleSheet("QPushButton{border-image: url(:/data/img/mainviewwidget/select.svg);}"
                                 "QPushButton:hover{border-image: url(:/img/mainviewwidget/select.svg);}"
                                 "QPushButton:pressed{border-image:url(:/img/mainviewwidget/select.svg);}");

         if(page == 1)
         {
             button->setStyleSheet("QPushButton{border-image:url(:/data/img/mainviewwidget/selected.svg);}"
                                     "QPushButton:hover{border-image: url(:/data/img/mainviewwidget/selected.svg);}"
                                     "QPushButton:pressed{border-image: url(:/data/img/mainviewwidget/selected.svg);}");
         }
         buttonBoxLayout->addWidget(button);
         buttonGroup->addButton(button,page);
    }
    btnGroupClickedSlot(0);
    curPageNum = 0;
    connect(buttonGroup,QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),this,&TabletWindow::buttonClicked);
}

void TabletWindow::buttonClicked(QAbstractButton *button)
{ 
    int idd = buttonGroup->id(button);
    Style::nowpagenum = idd;
//    QDBusReply<bool> res = usrInterface->call("get_current_tabletmode");
    for(int page = 1; page <= m_pagemanager->getAppPageVector().size(); page++)
    {
         if(idd == page)
         {
             buttonGroup->button(page)->setStyleSheet("QPushButton{border-image:url(:/data/img/mainviewwidget/selected.svg);}"
                                                      "QPushButton:hover{border-image: url(:/data/img/mainviewwidget/selected.svg);}"
                                                      "QPushButton:pressed{border-image: url(:/data/img/mainviewwidget/selected.svg);}");
         }
         else
         {

             buttonGroup->button(page)->setStyleSheet("QPushButton{border-image:url(:/data/img/mainviewwidget/select.svg);}"
                                                      "QPushButton:hover{border-image: url(:/data/img/mainviewwidget/select.svg);}"
                                                      "QPushButton:pressed{border-image: url(:/data/img/mainviewwidget/select.svg);}");
         }
    }
    curPageNum = idd - 1;
    btnGroupClickedSlot(curPageNum * 2);
}

void TabletWindow::animationFinishSlot()
{
    if(m_scrollArea->horizontalScrollBar()->value()==m_endPos ||
            m_scrollArea->horizontalScrollBar()->value()==m_scrollArea->horizontalScrollBar()->maximum())
    {
        m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    }
}

void TabletWindow::animationValueChangedSlot(const QVariant &value)
{
    Q_UNUSED(value);
    if (sender() != m_scrollAnimation)
        return;

    QPropertyAnimation *ani = qobject_cast<QPropertyAnimation *>(sender());

    if (m_endPos != ani->endValue())
        ani->setEndValue(m_endPos);
}

void TabletWindow::XkbEventsPress(const QString &keycode)
{
    QString KeyName;
    if(keycode.length() >= 8){
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

void TabletWindow::XkbEventsRelease(const QString &keycode)
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

    QDBusReply<bool> res = usrInterface->call("get_current_tabletmode");
    if (usrInterface && res) {
        qWarning() << QTime::currentTime()
                   << " Now is tablet mode, and it is forbidden to hide or show the menu after 'win'.'Esc'";
        return;
    }

    /**以下代码是非平板模式需要处理的键盘按键**/
    if ((keycode == "Super_L") || (keycode == "Super_R")) {
        qDebug() << "(ActiveWindow, SelfWindow):(" << QApplication::activeWindow() << ", " << this <<")";
        if (QApplication::activeWindow() == this) {
//            if (m_CommonUseWidget->m_listView->isDraging()) {
//                qWarning() << "Icon is been draging";
//            }
            this->hide();
        } else {
            this->showPCMenu();
        }
    }

    if (keycode == "Escape") {
        this->hide();
    }
}

void TabletWindow::winKeyReleaseSlot(const QString &key)
{
    if(key=="winKeyRelease" || key=="win-key-release")
    {
        if(QGSettings::isSchemaInstalled(QString("org.ukui.session").toLocal8Bit()))
        {
            QGSettings gsetting(QString("org.ukui.session").toLocal8Bit());
            if(gsetting.get(QString("win-key-release")).toBool())
            {
                disconnect(XEventMonitor::instance(), SIGNAL(keyRelease(QString)),
                        this,SLOT(XkbEventsRelease(QString)));
                disconnect(XEventMonitor::instance(), SIGNAL(keyPress(QString)),
                        this,SLOT(XkbEventsPress(QString)));
            }
            else
            {
                connect(XEventMonitor::instance(), SIGNAL(keyRelease(QString)),
                        this,SLOT(XkbEventsRelease(QString)));
                connect(XEventMonitor::instance(), SIGNAL(keyPress(QString)),
                        this,SLOT(XkbEventsPress(QString)));
            }
        }
    }
}

