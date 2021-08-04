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
    ,ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Style::initWidStyle();
    openDataBase("MainThread");
    initDatabase();
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
        ui->minAllListView->updateData(m_data);
        m_data = modaldata->getMinFuncData();
        ui->minFuncListView->updateData(m_data);
        m_data = modaldata->getMinLetterData();
        ui->minLetterListView->updateData(m_data);
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

    connect(ui->lineEdit, &QLineEdit::textChanged, this,&MainWindow::searchAppSlot);
    connect(this,&MainWindow::sendSearchKeyword,m_searchAppThread,&SearchAppThread::recvSearchKeyword);
    connect(m_searchAppThread,&SearchAppThread::sendSearchResult,this,&MainWindow::recvSearchResult);
    connect(fullWindow,&FullMainWindow::showNormalWindow,this,&MainWindow::showNormalWindow);
}

MainWindow::~MainWindow()
{
    delete ui;
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

    ui->minAllListView->addData(modaldata->getMinAllData(),0);
    ui->minFuncListView->addData(modaldata->getMinFuncData(),1);
    ui->minLetterListView->addData(modaldata->getMinLetterData(),2);
    ui->collectListView->addData(modaldata->getcollectData());

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
    ui->selectMenuButton->setMenu(m_menu);
    ui->collectPushButton->setStyleSheet("color:#3790FA");
    ui->recentPushButton->setStyleSheet("color:white");

    QAction *action = new QAction(this);
    action->setIcon(QIcon(":/data/img/mainviewwidget/DM-icon-search.svg"));
    ui->lineEdit->addAction(action,QLineEdit::LeadingPosition);
    connect(ui->minAllListView,&ListView::sendUpdateAppListSignal,this,&MainWindow::updateMinAllView);
    connect(ui->minAllListView,&ListView::sendUpdateCollectSignal,this,&MainWindow::updateCollectView);
    connect(ui->collectListView,&RightListView::sendCollectViewUpdate,this,&MainWindow::updateCollectView);
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
    if(ui->leftStackedWidget->currentIndex() == 0)
    {
        on_selectMenuButton_triggered(m_letterAction);
    }
    else if(ui->leftStackedWidget->currentIndex() == 1)
    {
        on_selectMenuButton_triggered(m_funcAction);
    }
    else if(ui->leftStackedWidget->currentIndex() == 2)
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
            ui->topStackedWidget->setCurrentIndex(1);
            ui->lineEdit->setFocus();
            ui->lineEdit->setText(e->text());
        }
        if(e->key() == Qt::Key_Backspace)
        {
            if(!ui->lineEdit->text().isEmpty())
            {
                ui->lineEdit->setText("");
            }
            else
            {
                ui->topStackedWidget->setCurrentIndex(0);
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
    ui->minSearchResultListView->addData(m_data, 3);
    Q_FOREACH(QStringList appinfo,arg)
        m_data.append(QStringList()<<appinfo.at(0)<<"1");
    ui->minSearchResultListView->updateData(m_data);
}

void MainWindow::searchAppSlot(QString arg)
{
    if(!arg.isEmpty())//切换至搜索模块
    {
        ui->leftStackedWidget->setCurrentIndex(3);
        Q_EMIT sendSearchKeyword(arg);
        m_searchAppThread->start();
    }
    else
    {
        ui->leftStackedWidget->setCurrentIndex(m_state);
    }
}

void MainWindow::on_selectMenuButton_triggered(QAction *arg1)
{
    qDebug() << "void MainWindow::on_selectMenuButton_triggered(QAction *arg1)" << m_allAction << arg1;
    if(arg1 == m_allAction)
    {
        ui->leftStackedWidget->setCurrentIndex(0);
        m_state = 0;
        ui->minSelectButton->setIcon(QIcon(":/data/img/mainviewwidget/DM-icon-所有应用.svg"));
        ui->minSelectTextLabel->setText(tr("All"));
        m_allAction->setChecked(true);
        m_letterAction->setChecked(false);
        m_funcAction->setChecked(false);
    }
    else if(arg1 == m_letterAction)
    {
        ui->leftStackedWidget->setCurrentIndex(1);
        m_state = 1;
        ui->minSelectButton->setIcon(QIcon(":/data/img/mainviewwidget/DM-icon-字母排序.svg"));
        ui->minSelectTextLabel->setText(tr("Letter"));
        m_allAction->setChecked(false);
        m_letterAction->setChecked(true);
        m_funcAction->setChecked(false);
    }
    else if(arg1 == m_funcAction)
    {
        ui->leftStackedWidget->setCurrentIndex(2);
        m_state = 2;
        ui->minSelectButton->setIcon(QIcon(":/data/img/mainviewwidget/DM-icon-功能排序.svg"));
        ui->minSelectTextLabel->setText(tr("Function"));
        m_allAction->setChecked(false);
        m_letterAction->setChecked(false);
        m_funcAction->setChecked(true);
    }
}

void MainWindow::updateCollectView()
{
    modaldata->loadDesktopVercor();
    ui->collectListView->updateData(modaldata->getcollectData());
}

void MainWindow::updateMinAllView()
{
    modaldata->loadDesktopVercor();
    ui->minAllListView->updateData(modaldata->getMinAllData());
}

void MainWindow::on_collectPushButton_clicked()
{
    ui->rightStackedWidget->setCurrentIndex(0);
    ui->collectPushButton->setStyleSheet("color:#3790FA");
    ui->recentPushButton->setStyleSheet("color:white");
}

void MainWindow::on_recentPushButton_clicked()
{
    ui->rightStackedWidget->setCurrentIndex(1);
    ui->collectPushButton->setStyleSheet("color:white");
    ui->recentPushButton->setStyleSheet("color:#3790FA");
}

void MainWindow::on_cancelSearchPushButton_clicked()
{
    ui->topStackedWidget->setCurrentIndex(0);
    ui->lineEdit->clear();
}

void MainWindow::on_searchPushButton_clicked()
{
    ui->topStackedWidget->setCurrentIndex(1);
    ui->lineEdit->setFocus();
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
    int ret = m_otherMenu.showShutdownMenu(this->mapToGlobal(ui->centralwidget->rect().bottomRight()));
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
