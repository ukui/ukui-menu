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
#include "style.h"
#include <QDebug>
#include <QSvgRenderer>
#include <QPainter>
#include "tabletlistview.h"
#include "xeventmonitor.h"
#include <QGraphicsEffect>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QEvent>

QT_BEGIN_NAMESPACE
extern void qt_blurImage(QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0);
QT_END_NAMESPACE

TabletWindow::TabletWindow(QWidget *parent) :
    QWidget(parent),
    m_scrollArea(new QScrollArea(this)),
    m_scrollAreaWid(new ScrollAreaWid(this)),
    m_backPixmap(new QPixmap),
    m_leftWidget(new FunctionWidget(this)),
    m_firstPageWidget(new QWidget(this)),
    m_pagemanager(new PageManager()),
    m_buttonBoxLayout(new QHBoxLayout),
    m_buttonGroup(new QButtonGroup),
    m_buttonWidget(new QWidget(this)),
    m_appFileWatcher(new QFileSystemWatcher),
    m_appListFileWatcher(new QFileSystemWatcher),
    m_configFileWatcher(new QFileSystemWatcher),
    m_directoryChangedThread(new TabletDirectoryChangedThread)

{
    QString path = QDir::homePath() + "/.config/ukui/ukui-menu.ini";
    m_setting = new QSettings(path, QSettings::IniFormat);
    setProperty("useStyleWindowManager", false);
    initSize();
    initUi();
}

TabletWindow::~TabletWindow()
{
}

QVector<QString> TabletWindow::m_keyVector = QVector<QString>();
QVector<int> TabletWindow::m_keyValueVector = QVector<int>();

void TabletWindow::initSize()
{
    Style::initWidStyle();
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->setFixedSize(Style::ScreenWidth, Style::ScreenHeight);
    m_leftWidget->setFixedSize(Style::m_leftWidWidth, Style::CenterWindHeight);
    m_leftWidget->layout()->setContentsMargins(Style::TimeWidgetLeft, Style::TimeWidgetTop, 0, 0);
    m_scrollAreaWid->setFixedHeight(Style::CenterWindHeight);
    m_scrollArea->setFixedSize(Style::ScreenWidth, Style::CenterWindHeight);
    m_buttonWidget->setFixedSize(Style::OtherPageViewWidth, 30);
}

void TabletWindow::initUi()
{
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAutoFillBackground(false);
    this->setFocusPolicy(Qt::NoFocus);
    m_animationPage.setParent(this);
    m_firstPageWidget->installEventFilter(this);
    m_buttonBoxLayout->setAlignment(Qt::AlignHCenter);
    m_buttonBoxLayout->setSpacing(0);
    m_buttonWidget->setLayout(m_buttonBoxLayout);
    m_buttonBoxLayout->setContentsMargins(0, 0, 0, 0);
    m_scrollAnimation = new QPropertyAnimation(&m_animationPage, "pos");
    connect(m_scrollAnimation, &QPropertyAnimation::finished, this, &TabletWindow::animationFinishSlot);
    m_scrollAnimation->setEasingCurve(QEasingCurve::OutExpo);
    setOpacityEffect(0.7);
    fileWatcher();
    initAppListWidget();
    setBackground();
    initStatusManager();
    initTransparency();
    registDbusService();
    ways();
    buttonWidgetShow();
    connect(m_leftWidget, &FunctionWidget::hideTabletWindow, this, &TabletWindow::recvHideMainWindowSlot);

    if (checkapplist()) {
        directoryChangedSlot();//更新应用列表
    }

    initXEventMonitor();
}

void TabletWindow::initXEventMonitor()
{
    //pc下鼠标功能
    XEventMonitor::instance()->start();
    connect(XEventMonitor::instance(), SIGNAL(keyRelease(QString)),
            this, SLOT(xkbEventsRelease(QString)));
    connect(XEventMonitor::instance(), SIGNAL(keyPress(QString)),
            this, SLOT(xkbEventsPress(QString)));
}

void TabletWindow::fileWatcher()
{
    m_configFileWatcher->addPath(QDir::homePath() + "/.cache/ukui-menu/ukui-menu.ini");
    connect(m_configFileWatcher, &QFileSystemWatcher::fileChanged, this, [ = ]() {
        m_configFileWatcher->addPath(QDir::homePath() + "/.cache/ukui-menu/ukui-menu.ini");
        this->repaint();
    });
    m_appFileWatcher->addPaths(QStringList() << QString("/usr/share/applications")
                               << QString(QDir::homePath() + "/.local/share/applications/"));
    connect(m_appFileWatcher, &QFileSystemWatcher::directoryChanged, this, &TabletWindow::directoryChangedSlot);
    m_appListFileWatcher->addPath(QDir::homePath() + "/.config/ukui/desktop_applist");
    connect(m_appListFileWatcher, &QFileSystemWatcher::fileChanged, this, &TabletWindow::directoryChangedSlot);
    connect(m_directoryChangedThread, &TabletDirectoryChangedThread::requestUpdateSignal, this, &TabletWindow::requestUpdateSlot);
    connect(m_directoryChangedThread, &TabletDirectoryChangedThread::deleteAppSignal, this, &TabletWindow::requestDeleteAppSlot);
}

void TabletWindow::initTransparency()
{
    //特效模式,此处Gsetting不明确，需进一步确认
    if (QGSettings::isSchemaInstalled(QString("org.ukui.control-center.personalise").toLocal8Bit())) {
        m_bgEffect = new QGSettings(QString("org.ukui.control-center.personalise").toLocal8Bit());

        if (m_bgEffect->keys().contains("transparency")) {
            setOpacityEffect(m_bgEffect->get("transparency").toReal());
            connect(m_bgEffect, &QGSettings::changed, [this](const QString & key) {
                if (key == "effect") {
                    if (m_bgEffect->get("effect").toBool()) {
                        setOpacityEffect(m_bgEffect->get("transparency").toReal());
                    } else {
                        setOpacityEffect(m_bgEffect->get("transparency").toReal());
                    }
                }
            });
        }
    }
}

void TabletWindow::initStatusManager()
{
    m_usrInterface = new QDBusInterface("com.kylin.statusmanager.interface",
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
}

void TabletWindow::setBackground()
{
    if (QGSettings::isSchemaInstalled(QString("org.mate.background").toLocal8Bit())) {
        m_bgSetting = new QGSettings(QString("org.mate.background").toLocal8Bit());

        if (m_bgSetting->keys().contains("pictureFilename")) {
            m_bgPath = m_bgSetting->get("pictureFilename").toString();
        }

        if (m_bgSetting->keys().contains("pictureOptions")) {
            m_bgOption = m_bgSetting->get("pictureOptions").toString();
        }

        connect(m_bgSetting, &QGSettings::changed, this, [ = ](const QString & key) {
            if (key == "pictureFilename") {
                //在每个屏幕上绘制背景
                if (m_bgSetting->keys().contains("pictureFilename") &&
                    m_bgSetting->keys().contains("pictureOptions")) {
                    m_bgPath = m_bgSetting->get("pictureFilename").toString();
                    m_bgOption = m_bgSetting->get("pictureOptions").toString();
                }

                ways();//壁纸显示方式
            }

            if (key == "pictureOptions") {
                //在每个屏幕上绘制背景
                if (m_bgSetting->keys().contains("pictureOptions")) {
                    m_bgOption = m_bgSetting->get("pictureOptions").toString();
                }

                ways();
            }
        });
    }
}

void TabletWindow::registDbusService()
{
//    m_dbus = new DBus;
//    new MenuAdaptor(m_dbus);
//    QDBusConnection con = QDBusConnection::sessionBus();
//    if (!con.registerService("org.ukui.menu") ||
//        !con.registerObject("/org/ukui/menu", m_dbus)) {
//        qDebug() << "error:" << con.lastError().message();
//    }
//    connect(m_dbus, &DBus::winKeyResponseSignal, this, [ = ] {
//        if (QGSettings::isSchemaInstalled(QString("org.ukui.session").toLocal8Bit()))
//        {
//            QGSettings gsetting(QString("org.ukui.session").toLocal8Bit());
//            if (gsetting.keys().contains("winKeyRelease")) {
//                if (gsetting.get(QString("winKeyRelease")).toBool()) {
//                    return;
//                }
//            }
//        }
//        if (QApplication::activeWindow() == this)
//        {
//            myDebug() << "win键触发窗口隐藏事件";
//            this->hide();
//        } else
//        {
//            myDebug() << "win键触发窗口显示事件";
//            this->showPCMenu();
//        }
//    });
}

bool TabletWindow::checkapplist()
{
    qDebug() << "MainWindow   checkapplist";
    QString path = QDir::homePath() + "/.config/ukui/ukui-menu.ini";
    QSettings *setting = new QSettings(path, QSettings::IniFormat);
    setting->beginGroup("application");
    QStringList keyList = setting->allKeys();
    setting->sync();
    setting->endGroup();
    delete setting;
//    if(keyList.count() == UkuiMenuInterface::desktopfpVector.count())
//    {
//        return false;
//    }else
    {
        UkuiMenuInterface::desktopfpVector.clear();

        for (int i = 0; i < keyList.count(); i++) {
            QString tmp;

            if (UkuiMenuInterface::androidDesktopfnList.contains(keyList.at(i))) {
                tmp = QString(QDir::homePath() + "/.local/share/applications/" + keyList.at(i));
            } else {
                tmp = QString("%1%2").arg("/usr/share/applications/").arg(keyList.at(i));
            }

            UkuiMenuInterface::desktopfpVector.append(tmp);
        }

        return true;
    }
}

bool TabletWindow::eventFilter(QObject *target, QEvent *event)
{
    if (target == m_scrollArea->viewport()) {
        if (event->type() == QEvent::Wheel) {
            event->ignore();
            return true;
        }
    }

    if (target == m_firstPageWidget || target == m_buttonWidget) {
        if (event->type() == QEvent::MouseMove) {
            return true;
        }
    }

    if (target == m_firstPageWidget) {
        if (event->type() == QEvent::MouseButtonRelease) {//因触控拖拽会触发press事件，所以改为release时隐藏
            recvHideMainWindowSlot();
        }
    }

    return false;
}


void TabletWindow::wheelEvent(QWheelEvent *e)
{
    if (!(m_scrollAnimation->state() == QPropertyAnimation::Running)) {
        if (qAbs(e->angleDelta().y()) >= qAbs(e->angleDelta().x())) {
            if ((e->angleDelta().y() >= 120)) {
                on_pageNumberChanged(false);
            } else if ((e->angleDelta().y() <= -120)) {
                on_pageNumberChanged(true);
            }
        } else if (qAbs(e->angleDelta().y()) < qAbs(e->angleDelta().x())) {
            if ((e->angleDelta().x() >= 120)) {
                on_pageNumberChanged(false);
            } else if ((e->angleDelta().x() <= -120)) {
                on_pageNumberChanged(true);
            }
        }
    }

    e->ignore();
}

/**
 * 初始化应用列表界面
 */
void TabletWindow::initAppListWidget()
{
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(m_layout);
    m_firstPageLayout = new QHBoxLayout();
//    m_scrollAreaWid->setStyleSheet("border:0px; background:transparent;");
    m_scrollArea->setWidget(m_scrollAreaWid);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setStyleSheet("border:0px; background:transparent;");
    m_scrollArea->setProperty("notUseSlideGesture", true);
    m_scrollArea->setFocusPolicy(Qt::NoFocus);
    m_scrollArea->viewport()->installEventFilter(this);
    m_scrollAreaWidLayout = new QHBoxLayout(m_scrollAreaWid);
    m_scrollAreaWidLayout->setContentsMargins(0, 0, 0, 0);
    m_scrollAreaWidLayout->setSpacing(0);
    m_layout->addWidget(m_scrollArea);
    m_layout->addWidget(m_buttonWidget);
    m_buttonWidget->installEventFilter(this);
    fillAppList();
}

void TabletWindow::modelChanged(bool value)
{
    if (value) {
        ways();
        recvHideMainWindowSlot();
    }

    QEventLoop loop;
    QTimer::singleShot(500, &loop, SLOT(quit()));
    loop.exec();
    QDBusReply<bool> res = m_usrInterface->call("get_current_tabletmode");

    if (!res) {
        initSize();
        reloadWidget();
    }

//    reloadAppList();
    myDebug() << "平板模式切换";
}

//打开PC模式下的开始菜单
void TabletWindow::showPCMenu()
{
//    this->setAttribute(Qt::WA_TranslucentBackground,true);
//    this->setAttribute(Qt::WA_X11NetWmWindowTypeDesktop,false);
//    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->show();
    this->raise();
    this->activateWindow();
    g_menuStatus = true;
    menuStatusChange();
    myDebug() << "showMenu显示开始菜单";
}

//改变搜索框及工具栏透明度
void TabletWindow::setOpacityEffect(const qreal &num)
{
    m_leftWidget->setDownOpacityEffect(num); //全局搜索框透明度
}

void TabletWindow::reloadAppList()
{
//    qDebug() << "void TabletWindow::reloadAppList()";
    QVector<QStringList> vector;
    m_data.clear();
    m_keyVector.clear();
    m_keyValueVector.clear();
    vector = m_pagemanager->getAppPageVector();

    if (!vector.at(0).isEmpty()) {
        QLayoutItem *widItem = m_firstPageLayout->itemAt(1);
        QWidget *wid = widItem->widget();
        TabletListView *m_listview = qobject_cast<TabletListView *>(wid);
        m_listview->updateData(vector.at(0));
    }

    for (int i = 1; i < vector.size(); i++) {
        if (!vector.at(i).isEmpty()) {
            QLayoutItem *widItem = m_scrollAreaWidLayout->itemAt(i);
            QWidget *wid = widItem->widget();
            TabletListView *m_listview = qobject_cast<TabletListView *>(wid);
            m_listview->updateData(vector.at(i));
        }
    }
}

void TabletWindow::reloadWidget()
{
    QLayoutItem *child;

    if (m_firstPageLayout->count() == 2) {
        QLayoutItem *widItem = m_firstPageLayout->itemAt(1);
        QWidget *wid = widItem->widget();
        TabletListView *m_listview = qobject_cast<TabletListView *>(wid);
        delete m_listview;
        m_firstPageLayout->removeWidget(m_leftWidget);
    }

    while ((child = m_scrollAreaWidLayout->takeAt(1)) != 0) {
        if (child->widget() != 0) {
            delete child->widget();
        }

        delete child;
    }

    m_isFirstPage = true;
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
    m_keyVector.clear();
    m_keyValueVector.clear();
    vector = m_pagemanager->getAppPageVector();

    for (int i = 0; i < vector.size(); i++) {
        QStringList applist = vector.at(i);

        if (!applist.isEmpty()) {
//            if (!m_isFirstPage) {
//                insertAppList(QStringList());
//            }
            insertAppList(applist);
        }
    }
}
bool TabletWindow::cmpApp(QString &arg_1, QString &arg_2)
{
    if (m_keyValueVector.at(m_keyVector.indexOf(arg_1)) < m_keyValueVector.at(m_keyVector.indexOf(arg_2))) {
        return true;
    } else {
        return false;
    }
}

void TabletWindow::directoryChangedSlot()
{
    myDebug() << "监测到desktop文件变化";
    m_appFileWatcher->addPaths(QStringList() << QString("/usr/share/applications")
                               << QString(QDir::homePath() + "/.local/share/applications/"));
    QEventLoop loop;
    QTimer::singleShot(100, &loop, SLOT(quit()));
    loop.exec();
    m_directoryChangedThread->start();
}
void TabletWindow::requestUpdateSlot(QString desktopfp)
{
    m_directoryChangedThread->quit();
    reloadWidget();
    connect(m_appListFileWatcher, &QFileSystemWatcher::fileChanged, this, &TabletWindow::directoryChangedSlot);
}

void TabletWindow::requestDeleteAppSlot()
{
    m_directoryChangedThread->quit();
    reloadWidget();
    connect(m_appListFileWatcher, &QFileSystemWatcher::fileChanged, this, &TabletWindow::directoryChangedSlot);
}

void TabletWindow::on_pageNumberChanged(bool nextPage)
{
//    qDebug() << "void TabletWindow::on_pageNumberChanged(bool nextPage)";
//    if (!(m_scrollAnimation->state() == QPropertyAnimation::Running)) {
    int preNum = m_curPageNum;

    if (nextPage) {
        m_curPageNum++;

        if (m_curPageNum > (m_scrollAreaWidLayout->count() - 1)) {
            m_curPageNum = (m_scrollAreaWidLayout->count() - 1);
            return;
        }
    } else {
        m_curPageNum--;

        if (m_curPageNum < 0) {
            m_curPageNum = 0;
            return;
        }
    }

    m_scrollArea->horizontalScrollBar()->setMaximum(m_scrollAreaWidLayout->count() * 1920);
    btnGroupClickedSlot(preNum, m_curPageNum);
    pageNumberChanged(m_curPageNum + 1);
//    }
}

bool TabletWindow::event(QEvent *event)
{
    if (event->type() == QEvent::ActivationChange)
        //if(QEvent::WindowDeactivate == event->type())//窗口停用
    {
        if (QApplication::activeWindow() != this) {
            myDebug() << " * 鼠标点击窗口外部事件";
            this->hide();
            g_menuStatus = false;
            menuStatusChange();
        }
    }

    if (event->type() == QEvent::MouseMove) {
        qDebug() << "bool TabletWindow::event ( QEvent * event ) 鼠标移动";
        //return true;
        event->ignore();
    }

    if (event->type() == QEvent::MouseButtonPress) {
        if (!(m_scrollAnimation->state() == QPropertyAnimation::Running)) {
            this->hide();
            g_menuStatus = false;
            myDebug() << "鼠标点击事件触发隐藏";
        }
    }

    return QWidget::event(event);
}

void TabletWindow::insertAppList(QStringList desktopfplist)
{
    TabletListView *listview = nullptr;

    if (m_isFirstPage) {
        listview = new TabletListView(this, 0);
        m_firstPageLayout->setSpacing(0);
        m_firstPageLayout->setContentsMargins(0, 0, 0, 0);
        m_firstPageWidget->setLayout(m_firstPageLayout);
        m_firstPageLayout->addWidget(m_leftWidget);
        listview->setFixedSize(Style::FirsPageViewWidth, Style::CenterWindHeight);
        m_firstPageLayout->addWidget(listview);
        m_scrollAreaWidLayout->addWidget(m_firstPageWidget);
        listview->setGridSize(QSize(Style::TabletItemSizeWidthFirst, Style::AppListItemSizeHeight));
        m_isFirstPage = false;
    } else {
        listview = new TabletListView(this, 1);
        listview->setFixedSize(Style::OtherPageViewWidth, Style::CenterWindHeight);
        listview->setGridSize(QSize(Style::TabletItemSizeWidthOther, Style::AppListItemSizeHeight));
        m_scrollAreaWidLayout->addWidget(listview);
    }

//    //修复异常黑框问题
//    connect(m_scrollArea, &ScrollArea::requestUpdate, listview->viewport(), [=](){
//        listview->repaint(listview->rect());
//    });
    connect(listview, &TabletListView::pagenumchanged, this, &TabletWindow::on_pageNumberChanged);
    listview->setProperty("notUseSlideGesture", true);
    m_data.clear();

    for (int i = 0; i < desktopfplist.count(); i++) {
        m_data.append(desktopfplist.at(i));
    }

    listview->addData(m_data);
    connect(listview, &TabletListView::sendItemClickedSignal, this, &TabletWindow::execApplication);
    connect(listview, &TabletListView::sendHideMainWindowSignal, this, &TabletWindow::recvHideMainWindowSlot);
    connect(listview, &TabletListView::sendUpdateAppListSignal, this, &TabletWindow::reloadAppList);
}

//void TabletWindow::recvStartMenuSlot()
//{
//    QDBusReply<bool> res = usrInterface->call("get_current_tabletmode");

//    if (this->isVisible()) {
//        if (!res) { //平板模式 下禁止wi隐藏菜单
//            this->hide();
//        }
//    } else {
//        if (!res) { //平板模式 下禁止win隐藏菜单
//            this->showPCMenu();
//        }
//    }
//}

/**
 * 执行应用程序
 */
void TabletWindow::execApplication(QString desktopfp)
{
    Q_EMIT sendHideMainWindowSignal();

    if (!g_subProjectCodeName.contains("mavis")) {
        execApp(desktopfp);
        QString str;
        //打开文件.desktop
        GError **error = nullptr;
        GKeyFileFlags flags = G_KEY_FILE_NONE;
        GKeyFile *keyfile = g_key_file_new();
        QByteArray fpbyte = desktopfp.toLocal8Bit();
        char *filepath = fpbyte.data();
        g_key_file_load_from_file(keyfile, filepath, flags, error);
        char *name = g_key_file_get_locale_string(keyfile, "Desktop Entry", "Exec", nullptr, nullptr);
        //取出value值
        QString execnamestr = QString::fromLocal8Bit(name);
        str = execnamestr;
        //qDebug()<<"2 exec"<<str;
        //关闭文件
        g_key_file_free(keyfile);
        //打开ini文件
        QString pathini = QDir::homePath() + "/.cache/ukui-menu/ukui-menu.ini";
        m_disableAppSet = new QSettings(pathini, QSettings::IniFormat);
        m_disableAppSet->beginGroup("application");
        QString desktopfp1 = str;
        //判断
        bool bo = m_disableAppSet->contains(desktopfp1.toLocal8Bit().data()); // iskey
        bool bo1 = m_disableAppSet->QSettings::value(desktopfp1.toLocal8Bit().data()).toBool(); //isvalue
        m_disableAppSet->endGroup();

        if (bo && bo1 == false) { //都存在//存在并且为false，从filepathlist中去掉
            //qDebug()<<"bool"<<bo<<bo1;
            return;
        }

        QString exe = execnamestr.simplified();
        QStringList parameters;

        if (exe.indexOf("%") != -1) {
            exe = exe.left(exe.indexOf("%") - 1);
        }

        if (exe.indexOf(" ") != -1) {
            parameters = exe.split(" ");
            exe = parameters[0];
            parameters.removeAt(0);
        }

        if (exe == "/usr/bin/indicator-china-weather") {
            parameters.removeAt(0);
            parameters.append("showmainwindow");
        }

        qDebug() << "5 exe" << exe << parameters;
        QDBusInterface session("org.gnome.SessionManager", "/com/ukui/app", "com.ukui.app");

        if (parameters.isEmpty()) {
            session.call("app_open", exe, parameters);
        } else {
            session.call("app_open", exe, parameters);
        }
    } else {
        QDBusInterface iface("com.kylin.AppManager",
                             "/com/kylin/AppManager",
                             "com.kylin.AppManager",
                             QDBusConnection::sessionBus());
        QDBusReply<bool> ret = iface.call("LaunchApp", desktopfp);
    }

    //Q_EMIT sendHideMainWindowSignal();
    return;
}

void TabletWindow::paintEvent(QPaintEvent *event)
{
    backgroundPic();//bgPath,picrect
    //在每个屏幕上绘制背景
    return QWidget::paintEvent(event);
}

QImage TabletWindow::applyEffectToImage(QImage src, QGraphicsEffect *effect, int extent)
{
    //qDebug()<<"6、绘制特效";
    if (src.isNull()) {
        return QImage();    //No need to do anything else!
    }

    if (!effect) {
        return src;    //No need to do anything else!
    }

    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(QPixmap::fromImage(src));
    item.setGraphicsEffect(effect);
    scene.addItem(&item);
    QImage res(src.size() + QSize(extent * 2, extent * 2), QImage::Format_ARGB32);
    res.fill(Qt::transparent);//transparent
    QPainter ptr(&res);
    scene.render(&ptr, QRectF(-extent, -extent, src.width() + extent * 2, src.height() + extent * 2), QRectF());
    return res;
}

QPixmap *TabletWindow::blurPixmap(QPixmap *pixmap)
{
    //qDebug()<<"blurPixmap";
    QPainter painter(pixmap);
    QImage srcImg = pixmap->toImage();
    //qt_blurImage(&painter,srcImg,2,true,false);//top 27000//150
    QGraphicsBlurEffect *blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(70);
    QImage result = applyEffectToImage(srcImg, blur, 70);
    qt_blurImage(&painter, result, 1, false, false); //top 27000//150
    painter.end();
    return pixmap;
}

void TabletWindow::pageNumberChanged(int pageNum)
{
    if (m_pagemanager->getAppPageVector().size() != 1) {
        for (int page = 1; page <= m_pagemanager->getAppPageVector().size(); page++) {
            if (pageNum == page) {
                m_buttonGroup->button(page)->setStyleSheet("QPushButton{border-image:url(:/data/img/mainviewwidget/selected.svg);}"
                        "QPushButton:hover{border-image: url(:/data/img/mainviewwidget/selected.svg);}"
                        "QPushButton:pressed{border-image: url(:/data/img/mainviewwidget/selected.svg);}");
            } else {
                m_buttonGroup->button(page)->setStyleSheet("QPushButton{border-image:url(:/data/img/mainviewwidget/select.svg);}"
                        "QPushButton:hover{border-image: url(:/data/img/mainviewwidget/select.svg);}"
                        "QPushButton:pressed{border-image: url(:/data/img/mainviewwidget/select.svg);}");
            }
        }
    }
}

void TabletWindow::ways()
{
    m_pixmap = QPixmap(m_bgPath);

    if (m_bgOption == "zoom" || m_bgOption == "" || m_bgOption == NULL) {
        m_bgOption = "scaled";
    }

    if (m_bgOption == "centered") { //居中
        m_backPixmap->load(m_bgPath);
    } else if (m_bgOption == "stretched") { //拉伸
        m_pixmap = m_pixmap.scaled(this->size());
        m_backPixmap = &m_pixmap;
    } else if (m_bgOption == "scaled") { //填充
        m_backPixmap->load(m_bgPath);
    } else if (m_bgOption == "wallpaper") { //平铺
        m_backPixmap->load(m_bgPath);
    } else {
        m_pixmap = m_pixmap.scaled(this->size());
        m_backPixmap = &m_pixmap;
    }

    m_backPixmap = blurPixmap(m_backPixmap);
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
    if (pixmap.isNull() || pixmap.width() == 0 || pixmap.height() == 0) {
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
        paddingPixmap = scaledPixmap.copy((pixmap.width() * scaled - width) / 2, 0, width, height);
    } else {
        scaled = float(width) / float(pixmap.width());
        scaledPixmap = pixmap.scaled(width, pixmap.height() * scaled);
        paddingPixmap = scaledPixmap.copy(0, (pixmap.height() * scaled - height) / 2, width, height);
    }

    return paddingPixmap;
}

void TabletWindow::backgroundPic()   //const QString &bgPath,QRect rect
{
//    qDebug()<<"5、绘制背景";
    QPainter painter(this);

    if (/*hideBackground*/false) {
        QColor cor;
        cor = "#252729";
        painter.setBrush(cor);
        painter.drawRect(this->rect());
    } else if (m_bgOption == "zoom" || m_bgOption == "" || m_bgOption == NULL) {
        m_bgOption = "scaled";
    } else if (m_bgOption == "centered") {
        QColor cor;
        cor = "#000000";
        painter.setBrush(cor);
        painter.drawRect(this->rect());
        painter.drawPixmap(this->width() / 2 - m_backPixmap->width() / 2,
                           this->height() / 2 - m_backPixmap->height() / 2,
                           *m_backPixmap);
    } else if (m_bgOption == "stretched") {
        //qDebug() << "---------" << "stretched" << "----------";
        painter.drawPixmap(this->rect(), *m_backPixmap);
    } else if (m_bgOption == "scaled") {
        painter.setPen(QColor(0, 0, 0, 64));//蒙层效果
        painter.setBrush(QColor(0, 0, 0, 64));
        painter.drawPixmap(this->geometry(), getPaddingPixmap(*m_backPixmap, this->size().width(), this->size().height()));
        painter.drawRect(this->geometry());
    } else if (m_bgOption == "wallpaper") {
        //qDebug() << "---------" << "wallpaper" << "----------";
        int drawedWidth = 0;
        int drawedHeight = 0;

        while (1) {
            drawedWidth = 0;

            while (1) {
                painter.drawPixmap(drawedWidth, drawedHeight, *m_backPixmap);
                drawedWidth += m_backPixmap->width();

                if (drawedWidth >= this->width()) {
                    break;
                }
            }

            drawedHeight += m_backPixmap->height();

            if (drawedHeight >= this->height()) {
                break;
            }
        }
    } else {
        painter.drawPixmap(this->rect(), *m_backPixmap);
    }
}

void TabletWindow::recvHideMainWindowSlot()
{
//    this->setAttribute(Qt::WA_TranslucentBackground,true);
//    this->setAttribute(Qt::WA_X11NetWmWindowTypeDesktop,false);
//    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->hide();
    g_menuStatus = false;
    myDebug() << "信号触发隐藏窗口";
}

void TabletWindow::btnGroupClickedSlot(int prePageNum, int pageNum)
{
    qDebug() << "void TabletWindow::btnGroupClickedSlot(int pageNum)";
    int durationTime = 200;

    if (prePageNum == pageNum) {
        durationTime = 1;
    }

    if (!(m_scrollAnimation->state() == QPropertyAnimation::Running)) {
        QPixmap p = m_scrollAreaWid->grab(m_scrollAreaWid->rect());
        m_animationPage.setFixedSize(m_scrollAreaWid->size());
        m_animationPage.setPixmap(p);
        m_beginPos = m_scrollArea->horizontalScrollBar()->sliderPosition();
        m_endPos = m_scrollAreaWidLayout->itemAt(pageNum)->widget()->x();
        m_scrollArea->horizontalScrollBar()->setValue(m_endPos);
        m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_scrollAnimation->setDuration(durationTime);
        m_scrollAnimation->setStartValue(QPoint(-m_beginPos, m_scrollArea->y()));
        m_scrollAnimation->setEndValue(QPoint(-m_endPos, m_scrollArea->y()));
        m_scrollAnimation->start();
        m_animationPage.show();
        m_animationPage.raise();
        m_scrollAreaWid->hide();
    }
}

void TabletWindow::buttonWidgetShow()
{
    //qDebug()<<"4、buttonWidgetShow";
    delete m_buttonBoxLayout;
    m_buttonBoxLayout = new QHBoxLayout;
    m_buttonWidget->setLayout(m_buttonBoxLayout);
    m_buttonBoxLayout->setAlignment(Qt::AlignHCenter);
    m_buttonBoxLayout->setSpacing(16);
    m_buttonBoxLayout->setContentsMargins(0, 0, 0, 0);

    for (auto button : m_buttonGroup->buttons()) {
        m_buttonGroup->removeButton(button);
        button->deleteLater();
    }

    QDBusReply<bool> var = m_usrInterface->call("get_current_tabletmode");

//    res = var;
    for (int page = 1; page <= m_pagemanager->getAppPageVector().size(); page++) {
        m_pageButton = new QPushButton;
        m_pageButton->setFocusPolicy(Qt::NoFocus);
        m_pageButton->setFixedSize(24, 24);
        m_pageButton->setStyleSheet("QPushButton{border-image: url(:/data/img/mainviewwidget/select.svg);}"
                                    "QPushButton:hover{border-image: url(:/img/mainviewwidget/select.svg);}"
                                    "QPushButton:pressed{border-image:url(:/img/mainviewwidget/select.svg);}");

        if (page == 1) {
            m_pageButton->setStyleSheet("QPushButton{border-image:url(:/data/img/mainviewwidget/selected.svg);}"
                                        "QPushButton:hover{border-image: url(:/data/img/mainviewwidget/selected.svg);}"
                                        "QPushButton:pressed{border-image: url(:/data/img/mainviewwidget/selected.svg);}");
        }

        m_buttonBoxLayout->addWidget(m_pageButton);
        m_buttonGroup->addButton(m_pageButton, page);
    }

    btnGroupClickedSlot(0, 0);
    m_curPageNum = 0;
    connect(m_buttonGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), this, &TabletWindow::buttonClicked);
}

void TabletWindow::buttonClicked(QAbstractButton *button)
{
    if (!(m_scrollAnimation->state() == QPropertyAnimation::Running)) {
        int idd = m_buttonGroup->id(button);
        int preNum = m_curPageNum;
        Style::nowpagenum = idd;

//    QDBusReply<bool> res = usrInterface->call("get_current_tabletmode");
        for (int page = 1; page <= m_pagemanager->getAppPageVector().size(); page++) {
            if (idd == page) {
                m_buttonGroup->button(page)->setStyleSheet("QPushButton{border-image:url(:/data/img/mainviewwidget/selected.svg);}"
                        "QPushButton:hover{border-image: url(:/data/img/mainviewwidget/selected.svg);}"
                        "QPushButton:pressed{border-image: url(:/data/img/mainviewwidget/selected.svg);}");
            } else {
                m_buttonGroup->button(page)->setStyleSheet("QPushButton{border-image:url(:/data/img/mainviewwidget/select.svg);}"
                        "QPushButton:hover{border-image: url(:/data/img/mainviewwidget/select.svg);}"
                        "QPushButton:pressed{border-image: url(:/data/img/mainviewwidget/select.svg);}");
            }
        }

        m_curPageNum = idd - 1;
        btnGroupClickedSlot(preNum, m_curPageNum);
    }
}

void TabletWindow::animationFinishSlot()
{
    m_scrollAreaWid->show();
    m_scrollAreaWid->raise();
    m_animationPage.hide();
//    if(m_scrollArea->horizontalScrollBar()->value()==m_endPos ||
//            m_scrollArea->horizontalScrollBar()->value()==m_scrollArea->horizontalScrollBar()->maximum())
//    {
//        m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
//    }
}

void TabletWindow::animationValueChangedSlot(const QVariant &value)
{
    Q_UNUSED(value);

    if (sender() != m_scrollAnimation) {
        return;
    }

    QPropertyAnimation *ani = qobject_cast<QPropertyAnimation *>(sender());

    if (m_endPos != ani->endValue()) {
        ani->setEndValue(m_endPos);
    }
}

void TabletWindow::xkbEventsPress(const QString &keycode)
{
    myDebug() << keycode;
    QString KeyName;

    if (keycode.length() >= 8) {
        KeyName = keycode.left(8);
    }

    if (KeyName.compare("Super_L+") == 0) {//组合按键
        m_winFlag = true;
    }

    if (keycode == "Super_L") {//win键
        m_winFlag = false;
    }
}

void TabletWindow::xkbEventsRelease(const QString &keycode)
{
    myDebug() << "触发按键释放" << keycode;
    QString KeyName;

    if (keycode.length() >= 8) {
        KeyName = keycode.left(8);
    }

    if (KeyName.compare("Super_L+") == 0 ||  //组合释放
        (m_winFlag && keycode == "Super_L")) { //组合按下单独释放
        return;
    }

    QDBusReply<bool> res = m_usrInterface->call("get_current_tabletmode");

    if (m_usrInterface && res) {
        qWarning() << QTime::currentTime()
                   << " Now is tablet mode, and it is forbidden to hide or show the menu after 'win'.'Esc'";
        return;
    }

    /**以下代码是非平板模式需要处理的键盘按键**/
    if ((keycode == "Super_L") || (keycode == "Super_R")) {
        qDebug() << "(ActiveWindow, SelfWindow):(" << QApplication::activeWindow() << ", " << this << ")";

        if (QApplication::activeWindow() == this) {
//            if (m_CommonUseWidget->m_listView->isDraging()) {
//                qWarning() << "Icon is been draging";
//            }
            qDebug() << "win键触发窗口隐藏事件";
            this->hide();
            g_menuStatus = false;
        } else {
            qDebug() << "win键触发窗口显示事件";
            this->showPCMenu();
            g_menuStatus = true;
        }
    }

    if (keycode == "Escape") {
        this->hide();
    }
}

void TabletWindow::winKeyReleaseSlot(const QString &key)
{
    if (key == "winKeyRelease" || key == "win-key-release") {
        if (QGSettings::isSchemaInstalled(QString("org.ukui.session").toLocal8Bit())) {
            QGSettings gsetting(QString("org.ukui.session").toLocal8Bit());

            if (gsetting.keys().contains("winKeyRelease")) {
                if (gsetting.get(QString("winKeyRelease")).toBool()) {
                    disconnect(XEventMonitor::instance(), SIGNAL(keyRelease(QString)),
                               this, SLOT(xkbEventsRelease(QString)));
                    disconnect(XEventMonitor::instance(), SIGNAL(keyPress(QString)),
                               this, SLOT(xkbEventsPress(QString)));
                } else {
                    connect(XEventMonitor::instance(), SIGNAL(keyRelease(QString)),
                            this, SLOT(xkbEventsRelease(QString)));
                    connect(XEventMonitor::instance(), SIGNAL(keyPress(QString)),
                            this, SLOT(xkbEventsPress(QString)));
                }
            }
        }
    }
}

void TabletWindow::menuStatusChange()
{
    QDBusMessage message = QDBusMessage::createSignal("/com/ukui/menu", "com.ukui.menu", "statusChangeSignal");
    //给信号赋值
    message << g_menuStatus;
    //发射
    QDBusConnection::sessionBus().send(message);
}
