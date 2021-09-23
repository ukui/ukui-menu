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
//    openDataBase("MainThread");
//    initDatabase();
    QString path=QDir::homePath()+"/.config/ukui/ukui-menu.ini";
    setting=new QSettings(path,QSettings::IniFormat);
    m_pagemanager = new PageManager();
    Style::initWidStyle();
    initUi();
}

TabletWindow::~TabletWindow()
{
    delete m_ukuiMenuInterface;
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

//    GetModelData modeldata;
    m_ukuiMenuInterface=new UkuiMenuInterface;

    leftWidget = new TimeWidget(this);
    leftWidget->setFixedSize(512,m_height);
    firstPageWidget = new QWidget(this);

    setOpacityEffect(0.7);

    initAppListWidget();
    initIconListWidget();

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
//    connect(m_scrollArea->horizontalScrollBar(),&QScrollBar::valueChanged,this,&TabletWindow::on_setScrollBarValue);
    connect(verticalScrollBar, &QScrollBar::valueChanged, this, &TabletWindow::on_setAreaScrollBarValue);
}

/**
 * 初始化应用列表界面
 */
void TabletWindow::initAppListWidget()
{
    layout=new QHBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);

    firstPageLayout =new QHBoxLayout();

    m_scrollArea=new ScrollArea;
    m_scrollAreaWid=new ScrollAreaWid(this);
    m_scrollAreaWid->setStyleSheet("border:0px; background:transparent;");
    m_scrollArea->setFixedSize(m_width,m_height);
    m_scrollArea->setWidget(m_scrollAreaWid);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setStyleSheet("border:0px; background:transparent;");

    m_scrollAreaWidLayout=new QHBoxLayout(m_scrollAreaWid);
    m_scrollAreaWidLayout->setContentsMargins(0,0,0,0);
    m_scrollAreaWidLayout->setSpacing(0);
    layout->addWidget(m_scrollArea);

    connect(m_scrollArea->horizontalScrollBar(),&QScrollBar::valueChanged,
            this,&TabletWindow::valueChangedSlot);
    m_appListBottomSpacer=new QSpacerItem(20,40,QSizePolicy::Fixed,QSizePolicy::Expanding);
    fillAppList();
   // m_scrollAreaWidHeight = m_scrollAreaWid->height();
}

void TabletWindow::modelChanged(bool value)
{
    if (value)
    {
        ways();
        recvHideMainWindowSlot();
    }
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
    m_classificationList.clear();
    QVector<QStringList> vector = UkuiMenuInterface::functionalVector;

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
            QLayoutItem* widItem = m_scrollAreaWidLayout->itemAt(i);
            QWidget* wid = widItem->widget();
            TabletListView* m_listview = qobject_cast<TabletListView*>(wid);
            m_listview->updateData(vector.at(i));
        }
    }
}
/**
 * 填充应用列表
 */
void TabletWindow::fillAppList()
{
    m_classificationList.clear();
    QVector<QStringList> vector/*=UkuiMenuInterface::functionalVector*/;

    m_data.clear();
    keyVector.clear();
    keyValueVector.clear();

//    if(!setting->childGroups().contains("application"))
//    {
//        Q_FOREACH(QString desktopfp,UkuiMenuInterface::allAppVector)
//            m_data.append(desktopfp);

//        setting->beginGroup("application");
//        for (int index = 0; index < m_data.size(); index++) {

//            QFileInfo fileInfo(m_data.at(index));
//            QString desktopfn=fileInfo.fileName();
//            setting->setValue(desktopfn,index);
//            setting->sync();
//        }
//        setting->endGroup();
//    }
//    else
//    {
//        setting->beginGroup("application");
//        QStringList keyList=setting->childKeys();

//        Q_FOREACH(QString desktopfn,keyList)
//        {
//            keyVector.append(desktopfn);
//            keyValueVector.append(setting->value(desktopfn).toInt());
//        }
//        setting->endGroup();
//        qSort(keyList.begin(),keyList.end(),cmpApp);

//        Q_FOREACH(QString desktopfn,keyList)
//            m_data.append("/usr/share/applications/"+desktopfn);
//    }
    vector = m_pagemanager->getAppPageVector();

    for(int i = 0; i < vector.size(); i++)
    {
        QStringList applist = vector.at(i);
        if(!applist.isEmpty())
        {
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

//void TabletWindow::insertClassificationBtn(QString category)
//{
//    SplitBarFrame* classificationbtn=new SplitBarFrame(this,category,m_scrollArea->width()-12,30,2);
//    classificationbtn->setAttribute(Qt::WA_TranslucentBackground);
//    classificationbtn->setAutoFillBackground(false);
//    m_scrollAreaWidLayout->addWidget(classificationbtn);
//    m_classificationList.append(category);
//}

void TabletWindow::on_pageNumberChanged(bool nextPage)
{
//    m_scrollArea->horizontalScrollBar()->setValue(500);
    if(nextPage)
    {
        i++;
        if(i > m_scrollAreaWidLayout->count() - 1)
        {
            i = 0;
        }
        btnGroupClickedSlot(i);
    }
    else
    {
        i--;
        if(i < 0)
        {
            i = m_scrollAreaWidLayout->count() - 1;
        }
        btnGroupClickedSlot(i);
    }
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
        listview->setFixedSize(m_width - 512,m_height);
        firstPageLayout->addWidget(listview);
        m_scrollAreaWidLayout->addWidget(firstPageWidget);
        listview->setGridSize(QSize(216,this->height()/4));
        isFirstPage = false;
    }
    else
    {
        listview = new TabletListView(this,1);
        listview->setFixedSize(m_width,m_height);
        listview->setGridSize(QSize(318,this->height()/4));
        m_scrollAreaWidLayout->addWidget(listview);
    }
    //修复异常黑框问题
    connect(m_scrollArea, &ScrollArea::requestUpdate, listview->viewport(), [=](){
        listview->repaint(listview->rect());
    });
    connect(listview,&TabletListView::pagenumchanged,this,&TabletWindow::on_pageNumberChanged);
    listview->installEventFilter(this);

    m_data.clear();
    for(int i=0;i<desktopfplist.count();i++)
        m_data.append(desktopfplist.at(i));
    listview->addData(m_data);
    connect(listview,&TabletListView::sendItemClickedSignal,this,&TabletWindow::execApplication);
    connect(listview,&TabletListView::sendHideMainWindowSignal,this,&TabletWindow::recvHideMainWindowSlot);
    connect(listview,&TabletListView::sendUpdateAppListSignal,this,&TabletWindow::updateAppListView);
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

void TabletWindow::on_setAreaScrollBarValue(int value)
{
//    m_scrollArea->verticalScrollBar()->setMaximum(maxmumValue);
    m_scrollArea->verticalScrollBar()->setValue(value);
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

void TabletWindow::ways()
{
    //qDebug()<<"7、ways";
//    qDebug()<<"bgPath"<<bgPath<<"pixmap"<<pixmap->isNull();
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
        //qDebug() << "---------" << "scaled" << "----------";
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

//    QDBusReply<bool> res = usrInterface->call("get_current_tabletmode");
//     if (!res || hideBackground)//平板模式 下禁止win隐藏菜单
//    {
        pixmap = blurPixmap(pixmap);
//    }

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

/**
 * 更新应用列表
 */
void TabletWindow::updateAppListView()
{
    reloadAppList();
}

/**
 * 设置scrollarea所填充控件大小
 */
void TabletWindow::resizeScrollAreaControls()
{
//    int row=0;
//    while(row<m_scrollAreaWidLayout->count()/2)
//    {
//        //应用界面
//        QLayoutItem* widItem=m_scrollAreaWidLayout->itemAt(row*2+1);
//        QWidget* wid=widItem->widget();
//        TabletListView* listview=qobject_cast<TabletListView*>(wid);
//        listview->adjustSize();
//        int dividend=(m_scrollArea->width()-Style::SliderSize)/Style::AppListGridSizeWidth;
//        int rowcount=0;
//        if(listview->model()->rowCount()%dividend>0)
//        {
//            rowcount=listview->model()->rowCount()/dividend+1;
//        }
//        else
//        {
//            rowcount=listview->model()->rowCount()/dividend;

//        }

//        listview->setFixedSize(m_scrollArea->width()-Style::SliderSize+1,listview->gridSize().height()*rowcount);
//        row++;
//    }
//    m_scrollArea->widget()->adjustSize();
}

void TabletWindow::recvHideMainWindowSlot()
{
    this->setAttribute(Qt::WA_TranslucentBackground,true);
    this->setAttribute(Qt::WA_X11NetWmWindowTypeDesktop,false);
    this->setWindowFlags(Qt::CustomizeWindowHint |Qt::FramelessWindowHint|Qt::X11BypassWindowManagerHint);
    this->hide();
//     this->hide();
}

/**
 * 初始化图标列表界面
 */
void TabletWindow::initIconListWidget()
{
    m_iconListWidLayout=new QVBoxLayout(m_iconListWid);
    m_iconListWidLayout->setContentsMargins(30,0,0,0);
    m_iconListWidLayout->setSpacing(Style::LeftSpaceBetweenItem);

    m_topSpacerItem=new QSpacerItem(20,40,QSizePolicy::Fixed,QSizePolicy::Expanding);
    m_bottomSpacerItem=new QSpacerItem(20,40,QSizePolicy::Fixed,QSizePolicy::Expanding);

    m_btnGroup=new QButtonGroup(m_iconListWid);
    m_animation = new QPropertyAnimation(m_iconListWid, "geometry");

    m_scrollAnimation = new QPropertyAnimation(m_scrollArea->horizontalScrollBar(), "value");
    m_scrollAnimation->setEasingCurve(QEasingCurve::Linear);
    connect(m_scrollAnimation, &QPropertyAnimation::finished, this, &TabletWindow::animationFinishSlot);
    connect(m_scrollAnimation, &QPropertyAnimation::valueChanged, this, &TabletWindow::animationValueChangedSlot);

    initIconListScrollArea();
}

void TabletWindow::on_setScrollBarValue(int value)
{
//    verticalScrollBar->setMaximum(/*m_scrollArea->verticalScrollBar()->maximum()*//*m_scrollAreaWid->height() - m_scrollArea->height()*/m_scrollAreaWidHeight - 880);
//    verticalScrollBar->setValue(value);
}

/**
 * 初始化图标列表界面数据表格iconlisttableWid
 */
void TabletWindow::initIconListScrollArea()
{
    m_iconListWidLayout->addItem(m_topSpacerItem);
    for(int i=0;i<m_classificationList.size();i++)
    {
        FunctionClassifyButton* iconbtn=new FunctionClassifyButton(
                    Style::LeftBtnWidth,
                    Style::LeftBtnHeight,
                    Style::LeftIconSize,
                    m_classificationList.at(i),
                    true,
                    true);
        iconbtn->setChecked(false);
        m_buttonList.append(iconbtn);
        m_iconListWidLayout->addWidget(iconbtn);
    }
    m_iconListWidLayout->addItem(m_bottomSpacerItem);

    int id=0;
    Q_FOREACH (QAbstractButton* btn, m_buttonList) {
        m_btnGroup->addButton(btn,id++);
    }
    if(m_btnGroup->button(0)!=nullptr)
        m_btnGroup->button(0)->click();
}

void TabletWindow::btnGroupClickedSlot(int pageNum)
{
    disconnect(m_scrollArea->verticalScrollBar(),&QScrollBar::valueChanged,
               this,&TabletWindow::valueChangedSlot);

    m_beginPos=m_scrollArea->horizontalScrollBar()->sliderPosition();

    m_endPos=m_scrollAreaWidLayout->itemAt(pageNum)->widget()->x();
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollAnimation->stop();
    m_scrollAnimation->setStartValue(m_beginPos);
    m_scrollAnimation->setEndValue(m_endPos);
    m_scrollAnimation->start();
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

void TabletWindow::valueChangedSlot(int value)
{
    int index=0;
    while(index<=m_classificationList.count()-1)
    {
        int min=m_scrollAreaWidLayout->itemAt(2*index)->widget()->y();
        int max=0;
        if(index==m_classificationList.count()-1)
            max=m_scrollAreaWid->height();
        else
            max=m_scrollAreaWidLayout->itemAt(2*(index+1))->widget()->y();
        if(value>=min && value<max)
        {
            Q_FOREACH (QAbstractButton* button, m_buttonList) {
                FunctionClassifyButton* fcbutton=qobject_cast<FunctionClassifyButton*>(button);
                if(index==m_buttonList.indexOf(button))
                {
                    fcbutton->setChecked(true);
                }
                else{
                    fcbutton->setChecked(false);
                }
            }
            break;
        }
        else
            index++;
    }
}

QAbstractButton* TabletWindow::getCurLetterButton(int value)
{
    return m_buttonList.at(value);
}


void TabletWindow::enterAnimation()
{
    m_animation->setDuration(500);//动画总时间
    m_animation->setStartValue(QRect(0,0,
                                    0,m_iconListWid->height()));
    m_animation->setEndValue(QRect(Style::LeftMargin,
                                  0,
                                  Style::LeftBtnWidth,
                                  m_iconListWid->height()));
    m_animation->setEasingCurve(QEasingCurve::InQuart);
    m_animation->start();
    m_iconListScrollAreaWid->show();
}

void TabletWindow::setFunctionBtnGeometry()
{
//    int height=m_classificationList.size()*Style::LeftBtnHeight+(m_classificationList.size()-1)*Style::LeftSpaceBetweenItem;
    m_iconListScrollAreaWid->setGeometry(QRect(Style::LeftMargin,
                                            0,
                                            Style::LeftBtnWidth,
                                            m_iconListWid->height()));
    m_iconListScrollAreaWid->show();
}

void TabletWindow::repaintWidget()
{
    updateAppListView();
}

void TabletWindow::widgetMakeZero()
{
    if(m_btnGroup->button(0)!=nullptr)
        m_btnGroup->button(0)->click();
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

void TabletWindow::moveScrollBar(int type)
{
    int height=Style::primaryScreenHeight;
    if(type==0)
        m_scrollArea->verticalScrollBar()->setSliderPosition(m_scrollArea->verticalScrollBar()->sliderPosition()-height*100/1080);
    else
        m_scrollArea->verticalScrollBar()->setSliderPosition(m_scrollArea->verticalScrollBar()->sliderPosition()+height*100/1080);
}

void TabletWindow::onSetSlider(int value)
{
//    if(flag)
//    {
//        flag = false;
//        time->start(100);
        int curvalue = m_scrollArea->verticalScrollBar()->value();
        m_scrollArea->verticalScrollBar()->setValue(curvalue + value);
//        qDebug() << "FullFunctionWidget::onSetSlider" << curvalue;
//    }
}

bool TabletWindow::eventFilter(QObject *watched, QEvent *event)
{
//    if(event->type() == QEvent::MouseButtonPress)
//    {
//        QMouseEvent *event = (QMouseEvent *)event;
//        if(event->button()==Qt::LeftButton)
//        {
//            pressedpos = event->globalPos()-frameGeometry().topLeft();
//        }
//    }

//    if(event->type() == QEvent::MouseButtonRelease)
//    {
//        QMouseEvent *event = (QMouseEvent *)event;

//        if (m_animation->state() == QVariantAnimation::Running)
//            return true;
//        releasepos=event->globalPos()-frameGeometry().topLeft();

//        if(event->button()==Qt::LeftButton)
//        {
//            int dist_y = releasepos.y() - pressedpos.y();
//            int dist_x = releasepos.x() - pressedpos.x();
//            int dis=Style::appLine*(Style::AppListItemSizeHeight);
//            if(((pressedpos-releasepos).manhattanLength()<=QApplication::startDragDistance()||pressedpos==releasepos))
//            {
//                this->hide();
//            }
//            else
//            {
//                if(dist_x<=-50 && Style::ScreenRotation==false /*&&!hideBackground*/) //向左翻
//                {
//                    //collapse();

//                }
//                else if(dist_x>=50 && Style::ScreenRotation==false /*&&!hideBackground*/) //向右翻
//                {
//                   // spread();
//                }
//            }
//            event->accept();
//        }

//    }
//    if(event->type() == QEvent::MouseButtonPress)
//    {
//        QLayoutItem* widItem = m_scrollAreaWidLayout->itemAt(2 * m_buttonList.size() - 1);
//        QWidget* wid = widItem->widget();
//        FullListView* m_listview = qobject_cast<FullListView*>(wid);

//        QLayoutItem* widItemTop = m_scrollAreaWidLayout->itemAt(1);
//        QWidget* widTop = widItemTop->widget();
//        FullListView* m_listviewTop = qobject_cast<FullListView*>(widTop);

//        QKeyEvent *ke = (QKeyEvent *)event;
//        if( ke->key() == Qt::Key_Tab )
//        {
//           Q_EMIT setFocusToSideWin();
//            return true;
//        }

//        if(ke->key() == Qt::Key_Up)
//        {
//            if(!m_listviewTop->hasFocus())
//            {
//                QAbstractButton* buttonTop = getCurLetterButton(( --m_index) % m_buttonList.size());
//                btnGroupClickedSlot(buttonTop);
////                this->m_scrollArea->setFocusToPreChild();
//            }
//            else
//            {
//               m_listview->setFocus();
//               QAbstractButton* button = getCurLetterButton(m_buttonList.size() - 1);
//               btnGroupClickedSlot(button);
//               m_index = m_buttonList.size() - 1;
//            }
//            Q_EMIT selectFirstItem();
//            return true;
//        }
//        if(ke->key() == Qt::Key_Down)
//        {
//            if(!m_listview->hasFocus())
//            {
//                QAbstractButton* button = getCurLetterButton(( ++m_index) % m_buttonList.size());
//                btnGroupClickedSlot(button);
////                this->m_scrollArea->setFocusToNextChild();
//            }
//            else
//            {
//                m_listviewTop->setFocus();
//                QAbstractButton* buttonTop = getCurLetterButton(0);
//                btnGroupClickedSlot(buttonTop);
//                m_listviewTop->setCurrentIndex(m_listviewTop->model()->index(0,0));
//                m_index = 0;
//            }
//            Q_EMIT selectFirstItem();
//            return true;
//        }
//    }
    return QWidget::eventFilter(watched,event);
}

void TabletWindow::functionButtonClick()
{
    if(m_btnGroup->button(0)!=nullptr)
        m_btnGroup->button(0)->click();
    m_index = 0;
}

void TabletWindow::setFocusToThis()
{
    QLayoutItem* widItemTop = m_scrollAreaWidLayout->itemAt(1);
    QWidget* widTop = widItemTop->widget();
    FullListView* m_listviewTop = qobject_cast<FullListView*>(widTop);
    functionButtonClick();
    m_listviewTop->setFocus();
    Q_EMIT selectFirstItem();
}

void TabletWindow::XkbEventsPress(const QString &keycode)
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

