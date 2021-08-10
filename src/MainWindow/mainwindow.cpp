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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    Style::initWidStyle();
    openDataBase("MainThread");
    initDatabase();

    this->resize(652, 590);
    this->setAutoFillBackground(false);

    centralwidget = new QWidget(this);
    centerLayout_0 = new QHBoxLayout(centralwidget);
    centerLayout_0->setSpacing(0);
    centerLayout_0->setContentsMargins(0, 0, 0, 0);

    //左侧
    widget = new MainViewWidget(centralwidget);
    widget->resize(QSize(308, 540));

    mainLeftVerticalLayout_1 = new QVBoxLayout(widget);
    mainLeftVerticalLayout_1->setSpacing(0);
    mainLeftVerticalLayout_1->setContentsMargins(8, 8, 0, 0);

    //搜索框部分
    topStackedWidget = new QStackedWidget(widget);
    topStackedWidget->setFixedHeight(48);

    //搜索框收起页
    minMenuPage = new QWidget();
    minMenuPage->setMinimumSize(QSize(0, 48));

    letfTopSelectHorizontalLayout = new QHBoxLayout(minMenuPage);
    letfTopSelectHorizontalLayout->setSpacing(8);
    letfTopSelectHorizontalLayout->setContentsMargins(8, 0, 8, 12);

    minSelectTextLabel = new QLabel(minMenuPage);


    horizontalSpacer = new QSpacerItem(68, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);


    searchPushButton = new QPushButton(minMenuPage);
    searchPushButton->setFixedSize(QSize(26, 26));
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/data/img/mainviewwidget/search.svg"), QSize(), QIcon::Normal, QIcon::Off);
    searchPushButton->setIcon(icon1);

    minSelectButton = new QToolButton(minMenuPage);
    minSelectButton->setFixedSize(QSize(26, 26));
    QIcon icon2;
    icon2.addFile(QString::fromUtf8(":/data/img/mainviewwidget/DM-icon-\345\212\237\350\203\275\346\216\222\345\272\217.png"), QSize(), QIcon::Normal, QIcon::Off);
    minSelectButton->setIcon(icon2);

    selectMenuButton = new QToolButton(minMenuPage);
    selectMenuButton->setFixedSize(QSize(16, 26));
    selectMenuButton->setAcceptDrops(true);
    QIcon icon3;
    icon3.addFile(QString::fromUtf8(":/data/img/mainviewwidget/DM-icon- arrow.svg"), QSize(), QIcon::Normal, QIcon::Off);
    selectMenuButton->setIcon(icon3);
    selectMenuButton->setPopupMode(QToolButton::InstantPopup);

    //搜索框展开页
    minSearchPage = new QWidget();
    leftTopSearchHorizontalLayout = new QHBoxLayout(minSearchPage);
    leftTopSearchHorizontalLayout->setContentsMargins(8, 0, 8, 12);

    lineEdit = new QLineEdit(minSearchPage);
    lineEdit->setMinimumSize(QSize(30, 26));
    lineEdit->setStyleSheet(QString::fromUtf8("border-radius: 12px;/*\350\256\276\347\275\256\345\234\206\350\247\222\347\232\204\345\244\247\345\260\217*/"));
    lineEdit->setFrame(false);


    cancelSearchPushButton = new QPushButton(minSearchPage);
    cancelSearchPushButton->setFixedSize(QSize(26, 26));
    cancelSearchPushButton->setStyleSheet(QString::fromUtf8("border-radius: 10px;/*\350\256\276\347\275\256\345\234\206\350\247\222\347\232\204\345\244\247\345\260\217*/\n"
"background-color: darkgray;"));
    QIcon icon4;
    icon4.addFile(QString::fromUtf8(":/data/img/mainviewwidget/DM-icon-\345\205\263\351\227\255.png"), QSize(), QIcon::Normal, QIcon::Off);
    cancelSearchPushButton->setIcon(icon4);

    topStackedWidget->addWidget(minMenuPage);
    topStackedWidget->addWidget(minSearchPage);

    //左侧列表区
    leftStackedWidget = new QStackedWidget(widget);
    leftStackedWidget->setFixedSize(QSize(300, 517));

    minAllPage = new QWidget();
    minAllPage->setFixedSize(QSize(292, 517));
    minAllListView = new ListView(minAllPage);
    minAllListView->setFixedSize(QSize(292, 517));
    minAllListView->setFrameShape(QFrame::NoFrame);
    leftStackedWidget->addWidget(minAllPage);

    minLetterPage = new QWidget();
    minLetterPage->setFixedSize(QSize(292, 517));
    minLetterListView = new ListView(minLetterPage);
    minLetterListView->setFixedSize(QSize(292, 517));
    minLetterListView->setFrameShape(QFrame::NoFrame);
    leftStackedWidget->addWidget(minLetterPage);

    minFuncPage = new QWidget();
    minFuncPage->setFixedSize(QSize(292, 517));
    minFuncListView = new ListView(minFuncPage);
    minFuncListView->setFixedSize(QSize(292, 517));
    minFuncListView->setFrameShape(QFrame::NoFrame);
    leftStackedWidget->addWidget(minFuncPage);
    minSearchResultPage = new QWidget();
    minSearchResultPage->setFixedSize(QSize(292, 517));
    minSearchResultListView = new ListView(minSearchResultPage);
    minSearchResultListView->setFixedSize(QSize(292, 517));
    leftStackedWidget->addWidget(minSearchResultPage);

    //右侧窗口
    mainRightVerticalLayout_1 = new QVBoxLayout();
    mainRightVerticalLayout_1->setSpacing(0);
    mainRightVerticalLayout_1->setContentsMargins(8, 8, 8, 8);
    mainRightVerticalLayout_2 = new QVBoxLayout();
    rightTopHorizontalLayout_3 = new QHBoxLayout();
    rightTopHorizontalLayout_3->setSpacing(24);
    rightTopHorizontalLayout_3->setContentsMargins(8, 0, 8, 0);

    //收藏按键
    collectPushButton = new QPushButton(centralwidget);
    collectPushButton->setFixedSize(QSize(40, 34));
    collectPushButton->setFlat(true);
    //最近按键
    recentPushButton = new QPushButton(centralwidget);
    recentPushButton->setFixedSize(QSize(40, 34));
    recentPushButton->setFlat(true);

    horizontalSpacer_3 = new QSpacerItem(332, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    //放大缩小按键
    minMaxChangeButton = new QPushButton(centralwidget);
    minMaxChangeButton->setFixedSize(QSize(24, 24));
    QIcon icon5;
    icon5.addFile(QString::fromUtf8(":/data/img/mainviewwidget/DM-icon-\346\224\276\345\244\247.svg"), QSize(), QIcon::Normal, QIcon::Off);
    minMaxChangeButton->setIcon(icon5);
    minMaxChangeButton->setFlat(true);

    rightTopHorizontalLayout_3->addWidget(collectPushButton);
    rightTopHorizontalLayout_3->addWidget(recentPushButton);
    rightTopHorizontalLayout_3->addItem(horizontalSpacer_3);
    rightTopHorizontalLayout_3->addWidget(minMaxChangeButton);

    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    //右侧列表区
    rightStackedWidget = new QStackedWidget(centralwidget);
    rightStackedWidget->setFixedSize(QSize(324, 490));

    collectPage = new QWidget();
    collectPage->setFixedSize(QSize(324, 480));
    rightCollectLayout = new QVBoxLayout(collectPage);
    rightCollectLayout->setContentsMargins(0,0,0,0);

    //收藏视图
    collectListView = new RightListView(collectPage);

    collectListView->setFixedSize(QSize(324, 428));
    collectListView->setAcceptDrops(true);
    collectListView->setAutoFillBackground(false);
    collectListView->setProperty("showDropIndicator", QVariant(true));
    collectListView->setDragEnabled(true);

    collectListView->setDragDropMode(QAbstractItemView::InternalMove);
    collectListView->setDefaultDropAction(Qt::MoveAction);
    collectListView->setMovement(QListView::Snap);
    collectListView->setProperty("isWrapping", QVariant(true));
    collectListView->setWordWrap(true);
    collectListView->setSelectionRectVisible(true);

    recentPage = new QWidget();
    recentPage->setFixedSize(QSize(324, 480));
    //最近视图
    recentListView = new ListView(recentPage);

    recentListView->setFixedSize(QSize(324, 470));

    rightStackedWidget->addWidget(collectPage);
    rightStackedWidget->addWidget(recentPage);

    verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    rightBottomHorizontalLayout_2 = new QHBoxLayout();
    rightBottomHorizontalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
    rightBottomHorizontalLayout_2->setContentsMargins(0, 0, 8, 0);
    horizontalSpacer_2 = new QSpacerItem(332, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    powerOffButton = new QPushButton(centralwidget);
    powerOffButton->setMinimumSize(QSize(24, 24));
    powerOffButton->setContextMenuPolicy(Qt::CustomContextMenu);
    QIcon icon6;
    icon6.addFile(QString::fromUtf8(":/data/img/sidebarwidget/shutdown.svg"), QSize(), QIcon::Normal, QIcon::Off);
    powerOffButton->setIcon(icon6);
    powerOffButton->setFlat(true);

    letfTopSelectHorizontalLayout->addWidget(minSelectTextLabel);
    letfTopSelectHorizontalLayout->addItem(horizontalSpacer);
    letfTopSelectHorizontalLayout->addWidget(searchPushButton);
    letfTopSelectHorizontalLayout->addWidget(minSelectButton);
    letfTopSelectHorizontalLayout->addWidget(selectMenuButton);
    leftTopSearchHorizontalLayout->addWidget(lineEdit);
    leftTopSearchHorizontalLayout->addWidget(cancelSearchPushButton);

    mainLeftVerticalLayout_1->addWidget(topStackedWidget);
    mainLeftVerticalLayout_1->addWidget(leftStackedWidget);

    mainRightVerticalLayout_2->addLayout(rightTopHorizontalLayout_3);
    rightCollectLayout->addItem(verticalSpacer);
    rightCollectLayout->addWidget(collectListView);
    rightCollectLayout->addItem(verticalSpacer_2);
    mainRightVerticalLayout_2->addWidget(rightStackedWidget);

    rightBottomHorizontalLayout_2->addItem(horizontalSpacer_2);
    rightBottomHorizontalLayout_2->addWidget(powerOffButton);

    mainRightVerticalLayout_1->addLayout(mainRightVerticalLayout_2);
    mainRightVerticalLayout_1->addLayout(rightBottomHorizontalLayout_2);

    centerLayout_0->addWidget(widget);
    centerLayout_0->addLayout(mainRightVerticalLayout_1);

    this->setCentralWidget(centralwidget);

    minSelectTextLabel->setText(QApplication::translate("MainWindow", "All", nullptr));
    searchPushButton->setText(QString());
    minSelectButton->setText(QApplication::translate("MainWindow", "...", nullptr));
    selectMenuButton->setText(QString());
    cancelSearchPushButton->setText(QString());
    collectPushButton->setText(QApplication::translate("MainWindow", "collection", nullptr));
    recentPushButton->setText(QApplication::translate("MainWindow", "recent", nullptr));
    minMaxChangeButton->setToolTip(QApplication::translate("MainWindow", "Max", nullptr));
    minMaxChangeButton->setText(QString());
    powerOffButton->setToolTip(QApplication::translate("MainWindow", "PowerOff", nullptr));
    powerOffButton->setText(QString());

    topStackedWidget->setCurrentIndex(0);
    leftStackedWidget->setCurrentIndex(0);
    minMaxChangeButton->setDefault(false);
    rightStackedWidget->setCurrentIndex(0);

    initUi();
    m_searchAppThread=new SearchAppThread;
    m_dbus=new DBus;
    new MenuAdaptor(m_dbus);
    fullWindow = new FullMainWindow;
    QDBusConnection con=QDBusConnection::sessionBus();
    if(!con.registerService("org.ukui.menu") ||
            !con.registerObject("/org/ukui/menu",m_dbus))
    {
        qDebug()<<"error:"<<con.lastError().message();
    }
    connect(m_dbus,&DBus::sendReloadSignal,this,[=]
    {
        modaldata->loadDesktopVercor();
        QVector<QStringList> m_data = modaldata->getMinAllData();
        minAllListView->updateData(m_data);
        m_data = modaldata->getMinFuncData();
        minFuncListView->updateData(m_data);
        m_data = modaldata->getMinLetterData();
        minLetterListView->updateData(m_data);
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
            this->show();
            this->raise();
            this->activateWindow();
        }
    });

    connect(lineEdit, &QLineEdit::textChanged, this,&MainWindow::searchAppSlot);
    connect(this,&MainWindow::sendSearchKeyword,m_searchAppThread,&SearchAppThread::recvSearchKeyword);
    connect(m_searchAppThread,&SearchAppThread::sendSearchResult,this,&MainWindow::recvSearchResult);
    connect(fullWindow,&FullMainWindow::showNormalWindow,this,&MainWindow::showNormalWindow);
    connect(minSelectButton,&QToolButton::clicked,this,&MainWindow::on_minSelectButton_clicked);
    connect(selectMenuButton,&QToolButton::triggered,this,&MainWindow::on_selectMenuButton_triggered);
    connect(powerOffButton,&QPushButton::customContextMenuRequested,this,&MainWindow::on_powerOffButton_customContextMenuRequested);
    connect(collectPushButton,&QPushButton::clicked,this,&MainWindow::on_collectPushButton_clicked);
    connect(recentPushButton,&QPushButton::clicked,this,&MainWindow::on_recentPushButton_clicked);
    connect(cancelSearchPushButton,&QPushButton::clicked,this,&MainWindow::on_cancelSearchPushButton_clicked);
    connect(searchPushButton,&QPushButton::clicked,this,&MainWindow::on_searchPushButton_clicked);
    connect(minMaxChangeButton,&QPushButton::clicked,this,&MainWindow::on_minMaxChangeButton_clicked);
}

MainWindow::~MainWindow()
{
    closeDataBase("MainThread");
}

void MainWindow::initUi()
{
    qDebug() <<"init UI";
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAutoFillBackground(false);
    this->setFocusPolicy(Qt::NoFocus);
    modaldata = new GetModelData;

    minAllListView->addData(modaldata->getMinAllData(),0);
    minFuncListView->addData(modaldata->getMinFuncData(),1);
    minLetterListView->addData(modaldata->getMinLetterData(),2);
    collectListView->addData(modaldata->getcollectData());
    recentListView->addData(modaldata->getMinAllData(),0);

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
    selectMenuButton->setMenu(m_menu);
    collectPushButton->setStyleSheet("color:#3790FA");
    recentPushButton->setStyleSheet("color:white");

    QAction *action = new QAction(this);
    action->setIcon(QIcon(":/data/img/mainviewwidget/DM-icon-search.svg"));
    lineEdit->addAction(action,QLineEdit::LeadingPosition);
    connect(minAllListView,&ListView::sendUpdateAppListSignal,this,&MainWindow::updateMinAllView);
    connect(minAllListView,&ListView::sendUpdateCollectSignal,this,&MainWindow::updateCollectView);
    connect(collectListView,&RightListView::sendCollectViewUpdate,this,&MainWindow::updateCollectView);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    double transparency=getTransparency();

    QRect rect = this->rect();
    QPainterPath path;
//    rect.setTopLeft(QPoint(rect.x()+320,rect.y()));

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    painter.setPen(Qt::transparent);
    if(1)
    {
        qreal radius=10;
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

        painter.setBrush(this->palette().base());
        painter.setPen(Qt::transparent);
        painter.setOpacity(transparency);
        painter.drawRect(this->rect());

        QPainterPath path;
        path.addRect(this->rect());
        KWindowEffects::enableBlurBehind(this->winId(), true, QRegion(path.toFillPolygon().toPolygon()));
    }
    QMainWindow::paintEvent(event);
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
           //m_mainViewWid->widgetMakeZero();
        }
   }

   if (event->type() == QEvent::KeyPress)
   {
       QKeyEvent *keyEvent = (QKeyEvent *) event;
//       if (keyEvent->key() == Qt::Key_Tab)
//       {
//           m_mainViewWid->setFocus();
//           Q_EMIT setFocusSignal();
//           return true;
//       }
       if(keyEvent->key() == Qt::Key_Up || keyEvent->key() == Qt::Key_Down ||
               keyEvent->key() == Qt::Key_Left || keyEvent->key() == Qt::Key_Right ||
               keyEvent->key() == Qt::Key_Tab)
       {
 //        m_mainViewWid->setFocus();
//         Q_EMIT setFocusSignal();
        // return true;
       }
   }
   return QWidget::event(event);
}

void MainWindow::on_minSelectButton_clicked()
{
    if(leftStackedWidget->currentIndex() == 0)
    {
        on_selectMenuButton_triggered(m_letterAction);
    }
    else if(leftStackedWidget->currentIndex() == 1)
    {
        on_selectMenuButton_triggered(m_funcAction);
    }
    else if(leftStackedWidget->currentIndex() == 2)
    {
        on_selectMenuButton_triggered(m_allAction);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if(e->type() == QEvent::KeyPress)
    {
        if((e->key() >= 0x30 && e->key() <= 0x39) || (e->key() >= 0x41 && e->key() <= 0x5a))
        {
            qDebug() << "void MainWindow::keyPressEvent(QKeyEvent *e)" << e->text();
            topStackedWidget->setCurrentIndex(1);
            lineEdit->setFocus();
            lineEdit->setText(e->text());
        }
        if(e->key() == Qt::Key_Backspace)
        {
            if(!lineEdit->text().isEmpty())
            {
                lineEdit->setText("");
            }
            else
            {
                topStackedWidget->setCurrentIndex(0);
            }
        }
        if(e->key() == Qt::Key_Escape)
        {
            this->hide();
        }
    }
}

void MainWindow::recvSearchResult(QVector<QStringList> arg)
{
    m_searchAppThread->quit();
    QVector<QStringList> m_data;
    m_data.clear();
    minSearchResultListView->addData(m_data, 3);
    Q_FOREACH(QStringList appinfo,arg)
        m_data.append(QStringList()<<appinfo.at(0)<<"1");
    minSearchResultListView->updateData(m_data);
}

void MainWindow::searchAppSlot(QString arg)
{
    if(!arg.isEmpty())//切换至搜索模块
    {
        leftStackedWidget->setCurrentIndex(3);
        Q_EMIT sendSearchKeyword(arg);
        m_searchAppThread->start();
    }
    else
    {
        leftStackedWidget->setCurrentIndex(m_state);
    }
}

void MainWindow::on_selectMenuButton_triggered(QAction *arg1)
{
    qDebug() << "void MainWindow::on_selectMenuButton_triggered(QAction *arg1)" << m_allAction << arg1;
    if(arg1 == m_allAction)
    {
        leftStackedWidget->setCurrentIndex(0);
        m_state = 0;
        minSelectButton->setIcon(QIcon(":/data/img/mainviewwidget/DM-icon-所有应用.svg"));
        minSelectTextLabel->setText(tr("All"));
        m_allAction->setChecked(true);
        m_letterAction->setChecked(false);
        m_funcAction->setChecked(false);
    }
    else if(arg1 == m_letterAction)
    {
        leftStackedWidget->setCurrentIndex(1);
        m_state = 1;
        minSelectButton->setIcon(QIcon(":/data/img/mainviewwidget/DM-icon-字母排序.svg"));
        minSelectTextLabel->setText(tr("Letter"));
        m_allAction->setChecked(false);
        m_letterAction->setChecked(true);
        m_funcAction->setChecked(false);
    }
    else if(arg1 == m_funcAction)
    {
        leftStackedWidget->setCurrentIndex(2);
        m_state = 2;
        minSelectButton->setIcon(QIcon(":/data/img/mainviewwidget/DM-icon-功能排序.svg"));
        minSelectTextLabel->setText(tr("Function"));
        m_allAction->setChecked(false);
        m_letterAction->setChecked(false);
        m_funcAction->setChecked(true);
    }
}

void MainWindow::updateCollectView()
{
    modaldata->loadDesktopVercor();
    collectListView->updateData(modaldata->getcollectData());
}

void MainWindow::updateMinAllView()
{
    modaldata->loadDesktopVercor();
    minAllListView->updateData(modaldata->getMinAllData());
}

void MainWindow::on_collectPushButton_clicked()
{
    rightStackedWidget->setCurrentIndex(0);
    collectPushButton->setStyleSheet("color:#3790FA");
    recentPushButton->setStyleSheet("color:white");
}

void MainWindow::on_recentPushButton_clicked()
{
    rightStackedWidget->setCurrentIndex(1);
    collectPushButton->setStyleSheet("color:white");
    recentPushButton->setStyleSheet("color:#3790FA");
}

void MainWindow::on_cancelSearchPushButton_clicked()
{
    topStackedWidget->setCurrentIndex(0);
    lineEdit->clear();
}

void MainWindow::on_searchPushButton_clicked()
{
    topStackedWidget->setCurrentIndex(1);
    lineEdit->setFocus();
}

void MainWindow::on_minMaxChangeButton_clicked()
{
//    this->setGeometry(0,0,1920,1080);
//    this->setFixedSize(1920,1080);
    this->hide();
    fullWindow->show();
}

void MainWindow::showNormalWindow()
{
    this->show();
    this->activateWindow();
}

void MainWindow::on_powerOffButton_customContextMenuRequested(const QPoint &pos)
{
    RightClickMenu m_otherMenu(this);
   // connect(&m_otherMenu, &RightClickMenu::sendMainWinActiveSignal, this, &SideBarWidget::sendShowMainWindowSignal);
  //  Q_EMIT sendShowMainWindowSignal(false);
    int ret = m_otherMenu.showShutdownMenu(this->mapToGlobal(centralwidget->rect().bottomRight()));
    qDebug() << "SideBarWidget::shutdownBtnRightClickSlot() 开始";
    if(ret>=10 && ret<=17)
    {
//        Q_EMIT sendHideMainWindowSignal();
        switch (ret) {
        case 10:
            QProcess::startDetached(QString("ukui-screensaver-command -l"));
            break;
        case 11:
            QProcess::startDetached(QString("ukui-session-tools --switchuser"));
            break;
        case 12:
            QProcess::startDetached(QString("ukui-session-tools --logout"));
            break;
        case 13:
            QProcess::startDetached(QString("ukui-session-tools --reboot"));
            break;
        case 14:
            QProcess::startDetached(QString("ukui-session-tools --shutdown"));
            break;
        case 16:
            QProcess::startDetached(QString("ukui-session-tools --suspend"));
            break;
        case 17:
            QProcess::startDetached(QString("ukui-session-tools --sleep"));
            break;
        default:
            break;
        }

    }
    qDebug() << "SideBarWidget::shutdownBtnRightClickSlot() 结束";
}
