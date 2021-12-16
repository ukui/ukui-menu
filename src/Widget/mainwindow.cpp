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
#include <QGroupBox>
#include <QEventLoop>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    Style::initWidStyle();
    openDataBase("MainThreadDataBase");
    initDatabase();
    this->resize(Style::minw, Style::minh);
    this->setAutoFillBackground(false);
    m_buttonStyle = QString("%1{border-radius:13px; background: rgba(255, 255, 255, 0.1);}"
                            "%1:hover {border-radius:13px; background: rgba(255, 255, 255, 0.2);}"
                            "%1:pressed {border-radius:13px; background: rgba(255, 255, 255, 0.3);}");
    m_centralwidget = new QWidget(this);
    m_centerLayout = new QHBoxLayout(m_centralwidget);
    m_centerLayout->setSpacing(0);
    m_centerLayout->setContentsMargins(0, 0, 0, 0);
    //左侧
    m_viewWidget = new MainViewWidget(m_centralwidget);
    m_viewWidget->resize(QSize(Style::defaultMainViewWidWidth, Style::defaultMainViewWidHeight));
    m_mainLeftVerticalLayout = new QVBoxLayout(m_viewWidget);
    m_mainLeftVerticalLayout->setSpacing(0);
    m_mainLeftVerticalLayout->setContentsMargins(8, 8, 0, 0);
    //搜索框部分
    m_topStackedWidget = new QStackedWidget(m_viewWidget);
    m_topStackedWidget->setFixedHeight(48);
    //搜索框收起页
    m_minMenuPage = new QWidget();
    m_minMenuPage->setMinimumSize(QSize(0, 48));
    m_letfTopSelectHorizontalLayout = new QHBoxLayout(m_minMenuPage);
    m_letfTopSelectHorizontalLayout->setSpacing(8);
    m_letfTopSelectHorizontalLayout->setContentsMargins(8, 0, 8, 12);
    m_minSelectTextLabel = new QLabel(m_minMenuPage);
    m_horizontalSpacer = new QSpacerItem(68, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_searchPushButton = new QPushButton(m_minMenuPage);
    m_searchPushButton->setStyleSheet(m_buttonStyle.arg("QPushButton"));
    m_searchPushButton->setFixedSize(QSize(26, 26));
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/data/img/mainviewwidget/search.svg"), QSize(), QIcon::Normal, QIcon::Off);
    m_searchPushButton->setIcon(icon1);
    m_minSelectButton = new QToolButton(m_minMenuPage);
    m_minSelectButton->setStyleSheet(m_buttonStyle.arg("QToolButton"));
    m_minSelectButton->setFixedSize(QSize(26, 26));
    QIcon icon2;
    icon2.addFile(QString::fromUtf8(":/data/img/mainviewwidget/DM-icon-所有应用.png"), QSize(), QIcon::Normal, QIcon::Off);
    m_minSelectButton->setIcon(icon2);
    m_minSelectButton->installEventFilter(this);
    m_selectMenuButton = new QToolButton(m_minMenuPage);
    m_selectMenuButton->setStyleSheet("background: transparent;");
    m_selectMenuButton->setFixedSize(QSize(16, 26));
    m_selectMenuButton->setAcceptDrops(true);
    QIcon icon3;
    icon3.addFile(QString::fromUtf8(":/data/img/mainviewwidget/DM-icon- arrow.svg"), QSize(), QIcon::Normal, QIcon::Off);
    m_selectMenuButton->setIcon(icon3);
    m_selectMenuButton->setPopupMode(QToolButton::InstantPopup);
//    m_selectMenuButton->setStyleSheet(QString::fromUtf8("QToolButton::menu-indicator { image: None; }"));
    //搜索框展开页
    m_minSearchPage = new QWidget();
    m_leftTopSearchHorizontalLayout = new QHBoxLayout(m_minSearchPage);
    m_leftTopSearchHorizontalLayout->setContentsMargins(8, 0, 8, 12);
    m_lineEdit = new QLineEdit(m_minSearchPage);
    m_lineEdit->setMinimumSize(QSize(30, 26));
    m_lineEdit->setStyleSheet(QString::fromUtf8("border-radius: 13px; border:2px solid rgba(5, 151, 255, 1); background: transparent;"));
    m_lineEdit->setFrame(false);
    m_lineEdit->setPlaceholderText("搜索应用");
    m_cancelSearchPushButton = new QPushButton(m_minSearchPage);
    m_cancelSearchPushButton->setFixedSize(QSize(26, 26));
    m_cancelSearchPushButton->setStyleSheet(m_buttonStyle.arg("QPushButton"));
    m_cancelSearchPushButton->installEventFilter(this);
    QIcon icon4;
    icon4.addFile(QString::fromUtf8(":/data/img/mainviewwidget/DM-icon-\345\205\263\351\227\255.png"), QSize(), QIcon::Normal, QIcon::Off);
    m_cancelSearchPushButton->setIcon(icon4);
    m_topStackedWidget->addWidget(m_minMenuPage);
    m_topStackedWidget->addWidget(m_minSearchPage);
    //左侧列表区
    m_leftStackedWidget = new QStackedWidget(m_viewWidget);
    m_leftStackedWidget->setFixedSize(QSize(300, Style::leftPageHeight));
    m_minAllPage = new QWidget();
    m_minAllPage->setFixedSize(QSize(Style::leftPageWidth, Style::leftPageHeight));
    m_minAllListView = new ListView(m_minAllPage);
    m_minAllListView->setFixedSize(QSize(Style::leftPageWidth, Style::leftPageHeight));
    m_minAllListView->setFrameShape(QFrame::NoFrame);
    m_leftStackedWidget->addWidget(m_minAllPage);
    m_minAllListView->installEventFilter(this);
    m_minLetterPage = new QWidget();
    m_minLetterPage->setFixedSize(QSize(Style::leftPageWidth, Style::leftPageHeight));
    m_minLetterListView = new ListView(m_minLetterPage);
    m_minLetterListView->setFixedSize(QSize(Style::leftPageWidth, Style::leftPageHeight));
    m_minLetterListView->setFrameShape(QFrame::NoFrame);
    m_leftStackedWidget->addWidget(m_minLetterPage);
    m_minLetterListView->installEventFilter(this);
    m_minFuncPage = new QWidget();
    m_minFuncPage->setFixedSize(QSize(Style::leftPageWidth, Style::leftPageHeight));
    m_minFuncListView = new ListView(m_minFuncPage);
    m_minFuncListView->setFixedSize(QSize(Style::leftPageWidth, Style::leftPageHeight));
    m_minFuncListView->setFrameShape(QFrame::NoFrame);
    m_leftStackedWidget->addWidget(m_minFuncPage);
    m_minFuncListView->installEventFilter(this);
    m_minSearchResultPage = new QWidget();
    m_minSearchResultPage->setFixedSize(QSize(Style::leftPageWidth, Style::leftPageHeight));
    m_minSearchResultListView = new ListView(m_minSearchResultPage);
    m_minSearchResultListView->setFixedSize(QSize(Style::leftPageWidth, Style::leftPageHeight));
    m_minSearchResultListView->installEventFilter(this);
    m_leftStackedWidget->addWidget(m_minSearchResultPage);
    //右侧窗口
    m_mainRightVerticalLayout = new QVBoxLayout();
    m_mainRightVerticalLayout->setSpacing(0);
    m_mainRightVerticalLayout->setContentsMargins(8, 8, 8, 8);
    m_mainRightVerticalLayout_1 = new QVBoxLayout();
    m_rightTopHorizontalLayout = new QHBoxLayout();
    m_rightTopHorizontalLayout->setSpacing(24);
    m_rightTopHorizontalLayout->setContentsMargins(8, 0, 8, 0);
    //收藏按键
    m_collectPushButton = new QPushButton(m_centralwidget);
    m_collectPushButton->setFixedHeight(34);
    m_collectPushButton->setFlat(true);
    m_collectPushButton->installEventFilter(this);
    //最近按键
    m_recentPushButton = new QPushButton(m_centralwidget);
    m_recentPushButton->setFixedHeight(34);
    m_recentPushButton->setFlat(true);
    m_recentPushButton->installEventFilter(this);
    m_horizontalSpacer_3 = new QSpacerItem(332, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    //放大缩小按键
    m_minMaxChangeButton = new QPushButton(m_centralwidget);
    m_minMaxChangeButton->setFixedSize(QSize(24, 24));
    QIcon icon5;
    icon5.addFile(QString::fromUtf8(":/data/img/mainviewwidget/DM-icon-\346\224\276\345\244\247.svg"), QSize(), QIcon::Normal, QIcon::Off);
    m_minMaxChangeButton->setIcon(icon5);
    m_minMaxChangeButton->setFlat(true);
    m_rightTopHorizontalLayout->addWidget(m_collectPushButton);
    m_rightTopHorizontalLayout->addWidget(m_recentPushButton);
    m_rightTopHorizontalLayout->addItem(m_horizontalSpacer_3);
    m_rightTopHorizontalLayout->addWidget(m_minMaxChangeButton);
    m_verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    //右侧列表区
    m_rightStackedWidget = new QStackedWidget(m_centralwidget);
    m_rightStackedWidget->setFixedSize(QSize(324, 490));
    m_rightStackedWidget->setFocusPolicy(Qt::StrongFocus);
    m_collectPage = new QWidget();
    m_collectPage->setFixedSize(QSize(324, 480));
    m_rightCollectLayout = new QVBoxLayout(m_collectPage);
    m_rightCollectLayout->setContentsMargins(0, 0, 0, 0);
    //收藏视图
    m_collectListView = new RightListView(m_collectPage);
    m_collectListView->setFixedSize(QSize(324, 428));
    m_collectListView->setAcceptDrops(true);
    m_collectListView->setAutoFillBackground(false);
    m_collectListView->setProperty("showDropIndicator", QVariant(true));
    m_collectListView->setDragEnabled(true);
    m_collectListView->setDragDropMode(QAbstractItemView::InternalMove);
    m_collectListView->setDefaultDropAction(Qt::MoveAction);
    m_collectListView->setMovement(QListView::Snap);
    m_collectListView->setProperty("isWrapping", QVariant(true));
    m_collectListView->setWordWrap(true);
    m_collectListView->setSelectionRectVisible(true);
    m_collectListView->installEventFilter(this);
    m_recentPage = new QWidget();
    m_recentPage->setFixedSize(QSize(324, 490));
    m_rightRecentLayout = new QVBoxLayout(m_recentPage);
    m_rightRecentLayout->setContentsMargins(0, 20, 0, 0);
    //最近视图
    m_recentListView = new ListView(m_recentPage);
    m_recentListView->installEventFilter(this);
    m_recentListView->setFixedSize(QSize(324, 470));
    m_rightStackedWidget->addWidget(m_collectPage);
    m_rightStackedWidget->addWidget(m_recentPage);
    m_verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    m_rightBottomHorizontalLayout = new QHBoxLayout();
    m_rightBottomHorizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
    m_rightBottomHorizontalLayout->setContentsMargins(0, 0, 8, 0);
    m_horizontalSpacer_2 = new QSpacerItem(332, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_powerOffButton = new QPushButton(m_centralwidget);
    m_powerOffButton->setFixedSize(QSize(24, 24));
    m_powerOffButton->setStyleSheet("QPushButton:hover {border-radius:12px; background: rgba(255, 255, 255, 0.2);}"
                                    "QPushButton:pressed {border-radius:12px; background: rgba(255, 255, 255, 0.3);}");
    m_powerOffButton->setContextMenuPolicy(Qt::CustomContextMenu);
    QIcon icon6;
    icon6.addFile(QString::fromUtf8(":/data/img/sidebarwidget/shutdown.svg"), QSize(), QIcon::Normal, QIcon::Off);
    m_powerOffButton->setIcon(icon6);
    m_powerOffButton->setFlat(true);
    m_powerOffButton->installEventFilter(this);
    m_letfTopSelectHorizontalLayout->addWidget(m_minSelectTextLabel);
    m_letfTopSelectHorizontalLayout->addItem(m_horizontalSpacer);
    m_letfTopSelectHorizontalLayout->addWidget(m_searchPushButton);
    m_letfTopSelectHorizontalLayout->addWidget(m_minSelectButton);
    m_letfTopSelectHorizontalLayout->addWidget(m_selectMenuButton);
    m_leftTopSearchHorizontalLayout->addWidget(m_lineEdit);
    m_leftTopSearchHorizontalLayout->addWidget(m_cancelSearchPushButton);
    m_mainLeftVerticalLayout->addWidget(m_topStackedWidget);
    m_mainLeftVerticalLayout->addWidget(m_leftStackedWidget);
    m_mainRightVerticalLayout_1->addLayout(m_rightTopHorizontalLayout);
    m_rightCollectLayout->addItem(m_verticalSpacer);
    m_rightCollectLayout->addWidget(m_collectListView);
    m_rightCollectLayout->addItem(m_verticalSpacer_2);
    m_rightRecentLayout->addWidget(m_recentListView);
    m_mainRightVerticalLayout_1->addWidget(m_rightStackedWidget);
    m_rightBottomHorizontalLayout->addItem(m_horizontalSpacer_2);
    m_rightBottomHorizontalLayout->addWidget(m_powerOffButton);
    m_mainRightVerticalLayout->addLayout(m_mainRightVerticalLayout_1);
    m_mainRightVerticalLayout->addLayout(m_rightBottomHorizontalLayout);
    m_centerLayout->addWidget(m_viewWidget);
    m_centerLayout->addLayout(m_mainRightVerticalLayout);
    this->setCentralWidget(m_centralwidget);
    m_minSelectTextLabel->setText(QApplication::translate("MainWindow", "All", nullptr));
    m_searchPushButton->setText(QString());
    m_minSelectButton->setText(QApplication::translate("MainWindow", "...", nullptr));
    m_selectMenuButton->setText(QString());
    m_cancelSearchPushButton->setText(QString());
    m_collectPushButton->setText(QApplication::translate("MainWindow", "collection", nullptr));
    m_recentPushButton->setText(QApplication::translate("MainWindow", "recent", nullptr));
    m_minMaxChangeButton->setToolTip(QApplication::translate("MainWindow", "Max", nullptr));
    m_minMaxChangeButton->setText(QString());
    m_powerOffButton->setToolTip(QApplication::translate("MainWindow", "PowerOff", nullptr));
    m_powerOffButton->setText(QString());
    m_topStackedWidget->setCurrentIndex(0);
    m_leftStackedWidget->setCurrentIndex(0);
    m_minMaxChangeButton->setDefault(false);
    m_rightStackedWidget->setCurrentIndex(0);
    //    setTabOrder(widget, searchPushButton);
    setTabOrder(m_searchPushButton, m_minSelectButton);
    setTabOrder(m_minSelectButton, m_selectMenuButton);
    setTabOrder(m_selectMenuButton, m_collectPushButton);
    setTabOrder(m_collectPushButton, m_recentPushButton);
    setTabOrder(m_recentPushButton, m_minMaxChangeButton);
    setTabOrder(m_minMaxChangeButton, m_powerOffButton);
    setTabOrder(m_powerOffButton, m_collectListView);

    m_softwareDbThread = new SoftwareDatabaseUpdateThread;
    //获取软件商店类别信号
    QDBusConnection::sessionBus().connect("com.kylin.softwarecenter.getsearchresults",
                                          "/com/kylin/softwarecenter/getsearchresults",
                                          "com.kylin.getsearchresults",
                                          "get_app_category_list_signal",
                                          this,
                                          SLOT(updateAppCategorySlot(QString))
                                         );

    initUi();
    m_functionBtnWid = new FunctionButtonWidget(m_minFuncPage);
    m_functionBtnWid->hide();
    m_letterBtnWid = new LetterButtonWidget(m_minLetterPage);
    m_letterBtnWid->hide();
    m_enterAnimation = new QPropertyAnimation;
    m_enterAnimation->setPropertyName(QString("geometry").toLocal8Bit());
    m_leaveAnimation = new QPropertyAnimation;
    m_leaveAnimation->setPropertyName(QString("geometry").toLocal8Bit());
    connect(this, &MainWindow::sendClassificationbtnList, m_functionBtnWid, &FunctionButtonWidget::recvClassificationBtnList);
    connect(this, &MainWindow::sendLetterClassificationList, m_letterBtnWid, &LetterButtonWidget::recvLetterBtnList);
    //   connect(m_functionBtnWid, &FunctionButtonWidget::sendFunctionBtnSignal,this,&FunctionWidget::recvFunctionBtnSignal);
    connect(m_minFuncListView, &ListView::sendAppClassificationBtnClicked, this, &MainWindow::appClassificationBtnClickedSlot);
    connect(m_minLetterListView, &ListView::sendAppClassificationBtnClicked, this, &MainWindow::appClassificationBtnClickedSlot);
    connect(m_leaveAnimation, &QPropertyAnimation::finished, this, &MainWindow::animationFinishedSLot);
    connect(m_enterAnimation, &QPropertyAnimation::finished, this, &MainWindow::animationFinishedSLot);
    connect(m_functionBtnWid, &FunctionButtonWidget::sendFunctionBtnSignal, this, &MainWindow::recvFunctionBtnSignal);
    connect(m_letterBtnWid, &LetterButtonWidget::sendLetterBtnSignal, this, &MainWindow::recvFunctionBtnSignal);
    m_searchAppThread = new SearchAppThread;
    m_dbus = new DBus;
    new MenuAdaptor(m_dbus);
    m_fullWindow = new FullMainWindow;
    QDBusConnection con = QDBusConnection::sessionBus();

    if (!con.registerService("org.ukui.menu") ||
        !con.registerObject("/org/ukui/menu", m_dbus)) {
        qDebug() << "error:" << con.lastError().message();
    }

    connect(m_dbus, &DBus::sendReloadSignal, this, [ = ] {
        updateView();
    });
    connect(m_dbus, &DBus::winKeyResponseSignal, this, [ = ] {
        if (QGSettings::isSchemaInstalled(QString("org.ukui.session").toLocal8Bit()))
        {
            QGSettings gsetting(QString("org.ukui.session").toLocal8Bit());

            if (gsetting.keys().contains("winKeyRelease"))
                if (gsetting.get("winKeyRelease").toBool()) {
                    return;
                }
        }
        if (QGSettings::isSchemaInstalled(QString("org.ukui.screenshot").toLocal8Bit()))
        {
            QGSettings gsetting(QString("org.ukui.screenshot").toLocal8Bit());

            if (gsetting.keys().contains("isrunning"))
                if (gsetting.get("isrunning").toBool()) {
                    return;
                }
        }

        if (this->isVisible())
        {
            this->hide();
            this->clearFocus();
            m_isFullScreen = false;
        } else if (m_fullWindow->isVisible())
        {
            m_fullWindow->hide();
            m_fullWindow->clearFocus();
            m_isFullScreen = true;
        } else
        {
            if (!m_isFullScreen) {
                this->show();
                this->raise();
                this->activateWindow();
                m_collectPushButton->clicked(true);
                m_viewWidget->setFocus();
            } else {
                m_fullWindow->show();
                //                fullWindow->raise();
                m_fullWindow->activateWindow();
            }
        }
    });
    connect(m_lineEdit, &QLineEdit::textChanged, this, &MainWindow::searchAppSlot);
    connect(this, &MainWindow::sendSearchKeyword, m_searchAppThread, &SearchAppThread::recvSearchKeyword);
    connect(m_searchAppThread, &SearchAppThread::sendSearchResult, this, &MainWindow::recvSearchResult);
    connect(m_fullWindow, &FullMainWindow::showNormalWindow, this, &MainWindow::showNormalWindow);
    connect(m_fullWindow, &FullMainWindow::sendUpdateOtherView, this, &MainWindow::updateMinAllView);
    connect(m_minSelectButton, &QToolButton::clicked, this, &MainWindow::on_minSelectButton_clicked);
    connect(m_selectMenuButton, &QToolButton::triggered, this, &MainWindow::on_selectMenuButton_triggered);
    connect(m_powerOffButton, &QPushButton::customContextMenuRequested, this, &MainWindow::on_powerOffButton_customContextMenuRequested);
    connect(m_powerOffButton, &QPushButton::clicked, this, &MainWindow::on_powerOffButton_clicked);
    connect(m_collectPushButton, &QPushButton::clicked, this, &MainWindow::on_collectPushButton_clicked);
    connect(m_recentPushButton, &QPushButton::clicked, this, &MainWindow::on_recentPushButton_clicked);
    connect(m_cancelSearchPushButton, &QPushButton::clicked, this, &MainWindow::on_cancelSearchPushButton_clicked);
    connect(m_searchPushButton, &QPushButton::clicked, this, &MainWindow::on_searchPushButton_clicked);
    connect(m_minMaxChangeButton, &QPushButton::clicked, this, &MainWindow::on_minMaxChangeButton_clicked);
    QDBusConnection::sessionBus().connect(DBUS_NAME, DBUS_PATH, DBUS_INTERFACE, QString("PanelGeometryRefresh"), this, SLOT(primaryScreenChangeSlot()));

    //监听屏幕缩放
    if (QGSettings::isSchemaInstalled(QString("org.ukui.SettingsDaemon.plugins.xsettings").toLocal8Bit())) {
        QGSettings *m_gsetting = new QGSettings(QString("org.ukui.SettingsDaemon.plugins.xsettings").toLocal8Bit());
        connect(m_gsetting, &QGSettings::changed, this, [ = ](const QString & key) {
            if (key == "scalingFactor") {
                repaintWidget();
            }
        });
    }

    if (QGSettings::isSchemaInstalled(QString("org.ukui.panel.settings").toLocal8Bit())) {
        QGSettings *gsetting = new QGSettings(QString("org.ukui.panel.settings").toLocal8Bit());
        connect(gsetting, &QGSettings::changed,
                this, &MainWindow::repaintWidget);
    }

    QGSettings * gsetting = new QGSettings("org.ukui.style", QByteArray(), this);
    connect(gsetting, &QGSettings::changed,[=](QString key) {
        if ("systemFont" == key || "systemFontSize" == key) {
            m_leftTopSearchHorizontalLayout->removeWidget(m_lineEdit);
            m_leftTopSearchHorizontalLayout->removeWidget(m_cancelSearchPushButton);
            m_lineEdit->setParent(nullptr);
            m_leftTopSearchHorizontalLayout->addWidget(m_lineEdit);
            m_leftTopSearchHorizontalLayout->addWidget(m_cancelSearchPushButton);
            m_lineEdit->setPlaceholderText("搜索应用");
            m_fullWindow->updateView();
        }
    });

    //监控应用进程开启
    connect(KWindowSystem::self(), &KWindowSystem::windowAdded, [=](WId id) {
        ConvertWinidToDesktop reply;
        QString desktopfp = reply.tranIdToDesktop(id);
        if (!desktopfp.isEmpty()) {
            ViewOpenedSlot(desktopfp);
        }
    });
}

MainWindow::~MainWindow()
{
    closeDataBase("MainThread");
}

void MainWindow::initUi()
{
    qDebug() << "init UI";
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAutoFillBackground(false);
    this->setFocusPolicy(Qt::NoFocus);
    m_modaldata = new GetModelData;
    m_minAllListView->addData(m_modaldata->getMinAllData(), 0);
    m_minFuncListView->addData(m_modaldata->getMinFuncData(), 1);
    m_minLetterListView->addData(m_modaldata->getMinLetterData(), 2);
    m_collectListView->addData(m_modaldata->getcollectData());
    m_recentListView->addData(m_modaldata->getRecentData(), -1);
    QMenu *m_menu = new QMenu;
    m_allAction = new QAction(m_menu);
    m_letterAction = new QAction(m_menu);
    m_funcAction = new QAction(m_menu);
    m_allAction->setText(tr("All"));
    m_allAction->setCheckable(true);
    m_letterAction->setText(tr("Letter"));
    m_letterAction->setCheckable(true);
    m_funcAction->setText(tr("Function"));
    m_funcAction->setCheckable(true);
    m_menu->addAction(m_allAction);
    m_menu->addAction(m_letterAction);
    m_menu->addAction(m_funcAction);
    m_allAction->setChecked(true);
    m_selectMenuButton->setMenu(m_menu);
    m_collectPushButton->setStyleSheet("color:#3790FA");
    m_recentPushButton->setStyleSheet("color:white");
    QAction *action = new QAction(this);
    action->setIcon(QIcon(":/data/img/mainviewwidget/DM-icon-search.svg"));
    m_lineEdit->addAction(action, QLineEdit::LeadingPosition);
    m_desktopWatcher = new DesktopWatcher();
    connect(m_minAllListView, &ListView::sendUpdateAppListSignal, this, &MainWindow::updateView);
    connect(m_minFuncListView, &ListView::sendUpdateAppListSignal, this, &MainWindow::updateView);
    connect(m_minLetterListView, &ListView::sendUpdateAppListSignal, this, &MainWindow::updateView);
    connect(m_minSearchResultListView, &ListView::sendUpdateAppListSignal, this, &MainWindow::updateView);
    connect(m_minAllListView, &ListView::sendCollectViewUpdate, this, &MainWindow::updateCollectView);
    connect(m_minFuncListView, &ListView::sendCollectViewUpdate, this, &MainWindow::updateCollectView);
    connect(m_minSearchResultListView, &ListView::sendCollectViewUpdate, this, &MainWindow::updateCollectView);
    connect(m_minLetterListView, &ListView::sendCollectViewUpdate, this, &MainWindow::updateCollectView);
    connect(m_collectListView, &RightListView::sendCollectViewUpdate, this, &MainWindow::updateCollectView);
    connect(m_desktopWatcher, &DesktopWatcher::directoryChangedSignal, this, &MainWindow::updateView);
    connect(m_desktopWatcher, &DesktopWatcher::updateRecentList, this, &MainWindow::updateRecentView);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    double transparency = getTransparency();
    QRect rect = this->rect();
    QPainterPath path;
    //    rect.setTopLeft(QPoint(rect.x()+320,rect.y()));
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    painter.setPen(Qt::transparent);
    qreal radius = 10;
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
    QMainWindow::paintEvent(event);
}

/**
 * 鼠标点击窗口外部事件
 */
bool MainWindow::event(QEvent *event)
{
    if (QEvent::WindowDeactivate == event->type() && m_canHide) { //窗口停用
        if (QApplication::activeWindow() != this) {
            qDebug() << " * 鼠标点击窗口外部事件";
            this->hide();
        }
    }

    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = (QKeyEvent *) event;

        if (keyEvent->key() == Qt::Key_Up || keyEvent->key() == Qt::Key_Down ||
            keyEvent->key() == Qt::Key_Left || keyEvent->key() == Qt::Key_Right ||
            keyEvent->key() == Qt::Key_Tab) {
            if (!m_collectListView->hasFocus()) {
                m_viewWidget->setFocus();

                if (m_state == 0) {
                    m_minAllListView->setFocus();
                    m_minAllListView->setCurrentIndex(m_minAllListView->model()->index(0, 0));
                } else if (m_state == 1) {
                    m_minLetterListView->setFocus();
                    m_minLetterListView->setCurrentIndex(m_minLetterListView->model()->index(0, 0));
                } else {
                    m_minFuncListView->setFocus();
                    m_minFuncListView->setCurrentIndex(m_minFuncListView->model()->index(0, 0));
                }
            }

            return true;
        }

        if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) {
            if (m_minSelectButton->hasFocus()) {
                m_minSelectButton->click();
            } else if (m_selectMenuButton->hasFocus()) {
                m_selectMenuButton->click();
            }

            if (m_lineEdit->hasFocus()) {
                m_minSearchResultListView->setFocus();
            } else {
                QWidget *current_focus_widget;
                current_focus_widget = QWidget::focusWidget();
                QPushButton *le = qobject_cast<QPushButton *>(current_focus_widget);

                if (le != nullptr) {
                    le->clicked();
                }
            }
        }
    }

    return QWidget::event(event);
}

/**
 * 接收FunctionButtonWidget界面按钮信号
 */
void MainWindow::recvFunctionBtnSignal(QString btnname)
{
    //此处需实现将功能为btnname的应用列表移动到applistWid界面最顶端
    if (m_state == 1) {
        int index = m_modaldata->getLetterClassificationList().indexOf(btnname);

        if (index != -1) {
            int row = m_modaldata->getLetterClassificationBtnRowList().at(index).toInt();
            m_minLetterListView->verticalScrollBar()->setValue(row);
            m_leaveAnimation->setTargetObject(m_letterBtnWid);
            m_enterAnimation->setTargetObject(m_minLetterListView);
        }
    } else {
        int index = m_modaldata->getFuncClassificationList().indexOf(btnname);

        if (index != -1) {
            int row = m_modaldata->getFuncClassificationBtnRowList().at(index).toInt();
            m_minFuncListView->verticalScrollBar()->setValue(row);
            m_leaveAnimation->setTargetObject(m_functionBtnWid);
            m_enterAnimation->setTargetObject(m_minFuncListView);
        }
    }

    m_leaveAnimation->setStartValue(QRect(0, 0, m_minFuncPage->width(), m_minFuncPage->height()));
    m_leaveAnimation->setEndValue(QRect(0, 0, 0, 0));
    m_enterAnimation->setStartValue(QRect(0, 0, 0, 0));
    m_enterAnimation->setEndValue(QRect(0, 0, m_minFuncPage->width(), m_minFuncPage->height()));
    m_leaveAnimation->setDuration(10);
    m_enterAnimation->setDuration(100);
    m_leaveAnimation->start();
    m_widgetState = 0;
}

void MainWindow::primaryScreenChangeSlot()
{
    repaintWidget();
}

void MainWindow::appClassificationBtnClickedSlot()
{
    m_leaveAnimation->setStartValue(QRect(0, 0, m_minFuncPage->width(), m_minFuncPage->height()));
    m_leaveAnimation->setEndValue(QRect(0, 0, 0, 0));
    m_enterAnimation->setStartValue(QRect(-40, -40, m_minFuncPage->width() + 80, m_minFuncPage->height() + 80));
    m_enterAnimation->setEndValue(QRect(10, 0, m_minFuncPage->width() - 20, m_minFuncPage->height() - 60));
    m_leaveAnimation->setDuration(10);
    m_enterAnimation->setDuration(100);

    if (m_state == 1) {
        Q_EMIT sendLetterClassificationList(m_modaldata->getLetterClassificationList());
        m_leaveAnimation->setTargetObject(m_minLetterListView);
        m_enterAnimation->setTargetObject(m_letterBtnWid);
    } else if (m_state == 2) {
        //加载FunctionButtonWidget界面
        Q_EMIT sendClassificationbtnList();
        m_leaveAnimation->setTargetObject(m_minFuncListView);
        m_enterAnimation->setTargetObject(m_functionBtnWid);
    }

    m_leaveAnimation->start();
    m_widgetState = 1;
}

void MainWindow::animationFinishedSLot()
{
    if (m_widgetState == 1) {
        if (m_state == 1) {
            m_minLetterListView->hide();
            m_letterBtnWid->show();
        } else {
            m_minFuncListView->hide();
            m_functionBtnWid->show();
        }

        m_enterAnimation->start();
        m_widgetState = -1;
    }

    if (m_widgetState == 0) {
        if (m_state == 1) {
            m_letterBtnWid->hide();
            m_minLetterListView->show();
        } else {
            m_functionBtnWid->hide();
            m_minFuncListView->show();
        }

        m_enterAnimation->start();
        m_widgetState = -1;
    }
}

void MainWindow::on_minSelectButton_clicked()
{
    if (m_leftStackedWidget->currentIndex() == 0) {
        on_selectMenuButton_triggered(m_letterAction);
    } else if (m_leftStackedWidget->currentIndex() == 1) {
        on_selectMenuButton_triggered(m_funcAction);
    } else if (m_leftStackedWidget->currentIndex() == 2) {
        on_selectMenuButton_triggered(m_allAction);
    }
}

bool MainWindow::eventFilter(QObject *target, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = (QKeyEvent *)event;

        if (target == m_powerOffButton) {
            if (ke->key() == Qt::Key_Tab || ke->key() == Qt::Key_Right) {
                if (m_topStackedWidget->currentIndex() == 0) {
                    m_topStackedWidget->setFocus();
                } else {
                    m_lineEdit->setFocus();
                    return true;
                }
            }

            if (ke->key() == Qt::Key_Up || ke->key() == Qt::Key_Down) {
                return true;
            }
        }

        if (target == m_collectPushButton) {
            if (ke->key() == Qt::Key_Down) {
                if (m_rightStackedWidget->currentIndex() == 0) {
                    m_collectListView->setFocus();
                    return true;
                } else {
                    return true;
                }
            }

            if (ke->key() == Qt::Key_Up || ke->key() == Qt::Key_Right || ke->key() == Qt::Key_Left) {
                return true;
            }
        }

        if (target == m_collectListView) {
            if (ke->key() == Qt::Key_Tab) {
                m_collectPushButton->setFocus();
                return true;
            }
        }

        if (target == m_recentListView) {
            if (ke->key() == Qt::Key_Tab) {
                m_recentPushButton->setFocus();
                return true;
            }

            if (ke->key() == Qt::Key_Left || ke->key() == Qt::Key_Right) {
                return true;
            }
        }

        if (target == m_minAllListView || target == m_minLetterListView || target == m_minFuncListView) {
            if (ke->key() == Qt::Key_Tab) {
                m_searchPushButton->setFocus();
            }
        }

        if (target == m_cancelSearchPushButton) {
            if (ke->key() == Qt::Key_Tab || ke->key() == Qt::Key_Right) {
                m_collectPushButton->setFocus();
                return true;
            }

            if (ke->key() == Qt::Key_Up || ke->key() == Qt::Key_Down) {
                return true;
            }
        }

        if (target == m_minSearchResultListView) {
            if (ke->key() == Qt::Key_Tab) {
                m_lineEdit->setFocus();
                return true;
            }
        }

        if (target == m_minSelectButton) {
            if (ke->key() == Qt::Key_Down) {
                if (m_state == 0) {
                    m_minAllListView->setFocus();
                    m_minAllListView->setCurrentIndex(m_minAllListView->model()->index(0, 0));
                } else if (m_state == 1) {
                    m_minLetterListView->setFocus();
                    m_minLetterListView->setCurrentIndex(m_minLetterListView->model()->index(0, 0));
                } else {
                    m_minFuncListView->setFocus();
                    m_minFuncListView->setCurrentIndex(m_minFuncListView->model()->index(0, 0));
                }

                return true;
            }

            if (ke->key() == Qt::Key_Up || ke->key() == Qt::Key_Right || ke->key() == Qt::Key_Left) {
                return true;
            }
        }

        if (target == m_recentPushButton) {
            if (ke->key() == Qt::Key_Down) {
                if (m_rightStackedWidget->currentIndex() == 1) {
                    m_recentListView->setFocus();
                    return true;
                } else {
                    return true;
                }
            }

            if (ke->key() == Qt::Key_Up || ke->key() == Qt::Key_Right || ke->key() == Qt::Key_Left) {
                return true;
            }
        }
    }

    return QWidget::eventFilter(target, event);
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->type() == QEvent::KeyPress) {
        if ((e->key() >= 0x30 && e->key() <= 0x39) || (e->key() >= 0x41 && e->key() <= 0x5a)) {
            qDebug() << "void MainWindow::keyPressEvent(QKeyEvent *e)" << e->text();
            m_topStackedWidget->setCurrentIndex(1);
            m_lineEdit->setFocus();
            m_lineEdit->setText(e->text());
        }

        if (e->key() == Qt::Key_Backspace) {
            if (!m_lineEdit->text().isEmpty()) {
                m_lineEdit->setText("");
            } else {
                m_topStackedWidget->setCurrentIndex(0);
            }
        }

        if (e->key() == Qt::Key_Escape) {
            this->hide();
        }
    }
}

/**
 * 进程开启监控槽函数
 */
void MainWindow::ViewOpenedSlot(QString desktopfp)
{
    myDebug() << "open software:" << desktopfp;
    QVector<QString> desktopfpVec = UkuiMenuInterface::desktopfpVector;

    if (desktopfpVec.contains(desktopfp)) {
        QFileInfo fileInfo(desktopfp);
        QString desktopfn = fileInfo.fileName();
        QString dateTimeKey;
        dateTimeKey.clear();

        if (!desktopfn.isEmpty()) {
            updateDataBaseTableTimes(desktopfn);
            updateView();
        }
    }
}

void MainWindow::recvSearchResult(QVector<QStringList> arg)
{
    m_searchAppThread->quit();
    QVector<QStringList> m_data;
    m_data.clear();
    m_minSearchResultListView->addData(m_data, 3);

    Q_FOREACH (QStringList appinfo, arg) {
        m_data.append(QStringList() << appinfo.at(0) << "1");
    }

    m_minSearchResultListView->updateData(m_data);
}

void MainWindow::searchAppSlot(QString arg)
{
    if (!arg.isEmpty()) { //切换至搜索模块
        m_leftStackedWidget->setCurrentIndex(3);
        Q_EMIT sendSearchKeyword(arg);
        m_searchAppThread->start();
    } else {
        m_leftStackedWidget->setCurrentIndex(m_state);
    }
}

void MainWindow::on_selectMenuButton_triggered(QAction *arg1)
{
    qDebug() << "void MainWindow::on_selectMenuButton_triggered(QAction *arg1)" << m_allAction << arg1;

    if (arg1 == m_allAction) {
        m_leftStackedWidget->setCurrentIndex(0);
        m_state = 0;
        m_minSelectButton->setIcon(QIcon(":/data/img/mainviewwidget/DM-icon-所有应用.svg"));
        m_minSelectTextLabel->setText(tr("All"));
        m_allAction->setChecked(true);
        m_letterAction->setChecked(false);
        m_funcAction->setChecked(false);
    } else if (arg1 == m_letterAction) {
        m_leftStackedWidget->setCurrentIndex(1);
        m_state = 1;
        m_minSelectButton->setIcon(QIcon(":/data/img/mainviewwidget/DM-icon-字母排序.svg"));
        m_minSelectTextLabel->setText(tr("Letter"));
        m_allAction->setChecked(false);
        m_letterAction->setChecked(true);
        m_funcAction->setChecked(false);
    } else if (arg1 == m_funcAction) {
        m_leftStackedWidget->setCurrentIndex(2);
        m_state = 2;
        m_minSelectButton->setIcon(QIcon(":/data/img/mainviewwidget/DM-icon-功能排序.svg"));
        m_minSelectTextLabel->setText(tr("Function"));
        m_allAction->setChecked(false);
        m_letterAction->setChecked(false);
        m_funcAction->setChecked(true);
    }
}

void MainWindow::updateCollectView()
{
    m_modaldata->loadDesktopVercor();
    m_collectListView->updateData(m_modaldata->getcollectData());
}

void MainWindow::updateMinAllView()
{
    m_modaldata->loadDesktopVercor();
    m_minAllListView->updateData(m_modaldata->getMinAllData());
}

void MainWindow::updateRecentView()
{
    m_modaldata->loadDesktopVercor();
    m_recentListView->updateData(m_modaldata->getRecentData());
}

void MainWindow::updateView()
{
    m_modaldata->loadDesktopVercor();
    m_minAllListView->updateData(m_modaldata->getMinAllData());
    m_collectListView->updateData(m_modaldata->getcollectData());
    m_minFuncListView->updateData(m_modaldata->getMinFuncData());
    m_minLetterListView->updateData(m_modaldata->getMinLetterData());
    m_fullWindow->updateView();
}

void MainWindow::updateAppCategorySlot(QString category)
{
    m_softwareDbThread->getDatabaseList(category);
    m_softwareDbThread->start();
    connect(m_softwareDbThread, &SoftwareDatabaseUpdateThread::updateDatabaseSignal, this, &MainWindow::databaseThreadCloseSlot);
}

void MainWindow::databaseThreadCloseSlot()
{
    m_softwareDbThread->quit();
    updateView();
}

void MainWindow::on_collectPushButton_clicked()
{
    m_rightStackedWidget->setCurrentIndex(0);
    m_collectPushButton->setStyleSheet("color:#3790FA");
    m_recentPushButton->setStyleSheet("color:white");
}

void MainWindow::on_recentPushButton_clicked()
{
    m_rightStackedWidget->setCurrentIndex(1);
    m_collectPushButton->setStyleSheet("color:white");
    m_recentPushButton->setStyleSheet("color:#3790FA");
}

void MainWindow::on_cancelSearchPushButton_clicked()
{
    m_topStackedWidget->setCurrentIndex(0);
    m_lineEdit->clear();
}

void MainWindow::on_searchPushButton_clicked()
{
    m_topStackedWidget->setCurrentIndex(1);
    m_lineEdit->setFocus();
}

void MainWindow::on_minMaxChangeButton_clicked()
{
    m_canHide = false;
    this->hide();
    QEventLoop loop;
    QTimer::singleShot(10, &loop, SLOT(quit()));
    loop.exec();
    m_fullWindow->raise();
    m_fullWindow->showNormal();
    m_fullWindow->activateWindow();
    m_canHide = true;
    m_isFullScreen = true;
}

void MainWindow::showWindow()
{
    if (m_isFullScreen) {
        m_fullWindow->raise();
        m_fullWindow->showNormal();
        m_fullWindow->activateWindow();
    } else {
        this->raise();
        this->showNormal();
        this->activateWindow();
        m_collectPushButton->clicked(true);
        m_viewWidget->setFocus();
    }
}

void MainWindow::hideWindow()
{
    if (m_fullWindow->isVisible()) {
        m_fullWindow->hide();
        this->clearFocus();
        m_isFullScreen = true;
    } else {
        this->hide();
        this->clearFocus();
        m_isFullScreen = false;
    }
}

void MainWindow::repaintWidget()
{
    Style::initWidStyle();
    this->setMinimumSize(Style::minw, Style::minh);
    int position = Style::m_panelPosition;
    int x = Style::m_primaryScreenX;
    int y = Style::m_primaryScreenY;
    int width = Style::m_availableScreenWidth;
    int height = Style::m_availableScreenHeight;

    if (position == 0) {
        this->setGeometry(QRect(x + 4, y + height - Style::minh - 3, Style::minw, Style::minh));
    } else if (position == 1) {
        this->setGeometry(QRect(x + 4, y + 4, Style::minw, Style::minh));
    } else if (position == 2) {
        this->setGeometry(QRect(x + 4, y + 4, Style::minw, Style::minh));
    } else {
        this->setGeometry(QRect(x + width - Style::minw - 4, y + 4, Style::minw, Style::minh));
    }
    m_fullWindow->move(x, y);
    m_fullWindow->setFixedSize(width, height);
    m_fullWindow->repaintWidget();
}

void MainWindow::showNormalWindow()
{
    m_canHide = false;
    QEventLoop loop;
    QTimer::singleShot(10, &loop, SLOT(quit()));
    loop.exec();
    this->show();
    this->raise();
    this->activateWindow();
    m_viewWidget->setFocus();
    m_isFullScreen = false;
    m_canHide = true;
    m_collectPushButton->clicked(true);
}

void MainWindow::on_powerOffButton_clicked()
{
    QProcess::startDetached(QString("ukui-session-tools"));
}

void MainWindow::on_powerOffButton_customContextMenuRequested(const QPoint &pos)
{
    RightClickMenu m_otherMenu(this);
    m_otherMenu.showShutdownMenu(this->mapToGlobal(m_centralwidget->rect().bottomRight()));
    qDebug() << "SideBarWidget::shutdownBtnRightClickSlot() 开始";
}
