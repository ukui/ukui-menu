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

#include "mainviewwidget.h"
#include "src/Style/style.h"
#include <QSvgRenderer>
#include <QPainter>
#include <syslog.h>
#include <QDebug>
#include <thread>
#include <KWindowInfo>
#include <KWindowSystem>
#include <KWindowEffects>
#include <QCheckBox>

MainViewWidget::MainViewWidget(QWidget *parent) :
    QWidget(parent)
{
    initUi();
}

MainViewWidget::~MainViewWidget()
{
    delete m_ukuiMenuInterface;
    delete m_fileWatcher;
    delete m_directoryChangedThread;
    delete m_animation;
    delete m_searchAppThread;
}

void MainViewWidget::initUi()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setAutoFillBackground(false);
    this->setAttribute(Qt::WA_TranslucentBackground);

    //监控应用进程开启
    connect(KWindowSystem::self(),&KWindowSystem::windowAdded,[=](WId id)
    {
       KWindowInfo info(id,NET::WMPid);
       int pid=info.pid();
       QString desktopfp=getEnvOverriddenDesktopFile(pid);
       if(!desktopfp.isEmpty())
           ViewOpenedSlot(desktopfp);
       else
       {
           //获取软件名(2种方式)
           //法1
           QStringList nameList;
           KWindowInfo info(id,NET::WMName);
           nameList.append(info.name());
           //法2
           QString path=QString("/proc/%1/status").arg(pid);
           QFile file(path);
           if(file.open(QIODevice::ReadOnly))
           {
               char buf[1024];
               qint64 len=file.readLine(buf,sizeof(buf));
               if(len!=-1)
               {
                   QString name=QString::fromLocal8Bit(buf).remove("Name:").remove("\t").remove("\n");
                   nameList.append(name);
               }
           }

           QVector<QStringList> appInfoVector=UkuiMenuInterface::appInfoVector;
           Q_FOREACH(QStringList list , appInfoVector)
           {
               if(list.at(1).contains(nameList.at(0),Qt::CaseInsensitive) ||
                       list.at(2).contains(nameList.at(0),Qt::CaseInsensitive) ||
                       list.at(5).contains(nameList.at(0),Qt::CaseInsensitive) ||
                       nameList.at(0).contains(list.at(1),Qt::CaseInsensitive) ||
                       nameList.at(0).contains(list.at(2),Qt::CaseInsensitive))
               {
                   ViewOpenedSlot(list.at(0));
                   break;
               }
               else if(nameList.size()==2 &&
                       (
                           list.at(1).contains(nameList.at(1),Qt::CaseInsensitive) ||
                           list.at(2).contains(nameList.at(1),Qt::CaseInsensitive) ||
                           list.at(5).contains(nameList.at(1),Qt::CaseInsensitive) ||
                           nameList.at(0).contains(list.at(1),Qt::CaseInsensitive) ||
                           nameList.at(0).contains(list.at(2),Qt::CaseInsensitive))
                       )
               {
                   ViewOpenedSlot(list.at(0));
                   break;
               }
           }
       }
    });

    //监控图标主题
    if(QGSettings::isSchemaInstalled(QString("org.ukui.style").toLocal8Bit()))
    {
        m_gsetting=new QGSettings(QString("org.ukui.style").toLocal8Bit());
        connect(m_gsetting,&QGSettings::changed,this,&MainViewWidget::iconThemeChangeSlot);
    }
}

/**
 * 添加顶部控件
 */
void MainViewWidget::addTopControl()
{
//    m_topLayout=new QHBoxLayout;
//    m_topSearchLayout = new QHBoxLayout;
////    m_topSearchLayout->setSpacing(0);
////    m_topLayout->setSpacing(10);
//    m_topLayout->setContentsMargins(0,0,200,0);
//    m_queryLineEdit=new QLineEdit;
//    m_selectButton = new QToolButton;
    m_topSearchLayout->addWidget(m_queryLineEdit);
//    m_topSearchLayout->addWidget(&m_searchCloseButton);
//    m_topLayout->addWidget(&m_label);
//    m_label.setContentsMargins(20,0,0,0);
//    m_topLayout->addWidget(&m_searchButton);
//    m_topLayout->addWidget(m_selectButton);
//    QMenu *m_menu = new QMenu;
//    m_allAction = new QAction(m_menu);
//    m_letterAction = new QAction(m_menu);
//    m_funcAction = new QAction(m_menu);
//    m_allAction->setText(tr("All"));
//    m_allAction->setCheckable(true);
//    m_letterAction->setText(tr("Letter"));
//    m_letterAction->setCheckable(true);
//    m_funcAction->setText(tr("Function"));
//    m_funcAction->setCheckable(true);
//    m_menu->addAction(m_allAction);
//    m_menu->addAction(m_letterAction);
//    m_menu->addAction(m_funcAction);
//    m_selectButton->setMenu(m_menu);
//    m_selectButton->setPopupMode(QToolButton::MenuButtonPopup);
//    m_selectButton->setFixedSize(60,30);
//    m_searchButton.setFixedSize(30,30);
//    m_searchButton.setIcon(QIcon(":/data/img/mainviewwidget/DM-icon-search.svg"));
//    m_searchCloseButton.setFixedSize(30,30);
//    m_searchCloseButton.setIcon(QIcon(":/data/img/mainviewwidget/DM-icon-关闭.png"));
//    m_topWidget->setLayout(m_topLayout);
//    m_topWidget->setContentsMargins(0,0,10,0);
//    m_topSearchWidget->setLayout(m_topSearchLayout);

//    connect(m_allAction,&QAction::triggered,this,&MainViewWidget::loadCommonUseWidget);
//    connect(m_letterAction,&QAction::triggered,this,&MainViewWidget::loadLetterWidget);
//    connect(m_funcAction,&QAction::triggered,this,&MainViewWidget::loadFunctionWidget);
//    connect(m_selectButton,&QToolButton::clicked,this,&MainViewWidget::switchWidget);
//    connect(&m_searchButton,&QPushButton::clicked,this,&MainViewWidget::onSearchButtClicked);
//    connect(&m_searchCloseButton,&QPushButton::clicked,this,&MainViewWidget::onSearchCloseButtonClicked);
//    char style[200];
//    QGSettings gsetting(QString("org.ukui.style").toLocal8Bit());
//    if(gsetting.get("style-name").toString()=="ukui-light")
//    sprintf(style, "QLineEdit{border:1px solid %s;background-color:%s;border-radius:4px;color:#000000;}",
//            QueryLineEditClickedBorderDefault,QueryLineEditDefaultBackground);
//    else
//        sprintf(style, "QLineEdit{border:1px solid %s;background-color:%s;border-radius:4px;color:#ffffff;}",
//                QueryLineEditClickedBorder,QueryLineEditBackground);
//    m_queryLineEdit->setStyleSheet(style);

//    initQueryLineEdit();

}

//void MainViewWidget::focusInEvent(QFocusEvent *event)
//{
//    Q_UNUSED(event);
//    if(this->hasFocus())
//    {
//        this->focusNextChild();
//    }
//    return QWidget::focusInEvent(event);
//}

/**
 * 添加搜索框
 */
void MainViewWidget::initQueryLineEdit()
{
//    m_queryWid=new QWidget;
//    m_queryWid->setParent(m_queryLineEdit);
//    m_queryWid->setFocusPolicy(Qt::NoFocus);
//    QHBoxLayout* queryWidLayout=new QHBoxLayout;
//    queryWidLayout->setContentsMargins(0,0,0,0);
//    queryWidLayout->setSpacing(5);
//    m_queryWid->setLayout(queryWidLayout);
//    QPixmap pixmap=loadSvg(QString(":/data/img/mainviewwidget/search.svg"),16);
//    QGSettings gsetting(QString("org.ukui.style").toLocal8Bit());
//    if(gsetting.get("style-name").toString()=="ukui-light")//反黑
//        pixmap=drawSymbolicBlackColoredPixmap(pixmap);
//    else
//        pixmap=drawSymbolicColoredPixmap(pixmap);//反白
//    pixmap.setDevicePixelRatio(qApp->devicePixelRatio());
//    m_queryIcon=new QLabel;
//    m_queryIcon->setFixedSize(pixmap.size());
//    m_queryIcon->setPixmap(pixmap);
//    m_queryText=new QLabel;
//    m_queryText->setText(tr("Search"));
//    m_queryText->adjustSize();
//    queryWidLayout->addWidget(m_queryIcon);
//    queryWidLayout->addWidget(m_queryText);
//    queryWidLayout->setAlignment(m_queryIcon,Qt::AlignVCenter);
//    queryWidLayout->setAlignment(m_queryText,Qt::AlignVCenter);
//    m_queryLineEdit->setFocusPolicy(Qt::ClickFocus);
//    m_queryLineEdit->installEventFilter(this);
//    m_queryLineEdit->setContextMenuPolicy(Qt::NoContextMenu);

//    m_animation= new QPropertyAnimation(m_queryWid,"geometry");
//    m_animation->setDuration(100);
//    connect(m_animation,&QPropertyAnimation::finished,this,&MainViewWidget::animationFinishedSlot);

//    m_searchAppThread=new SearchAppThread;
//    connect(this,&MainViewWidget::sendSearchKeyword,
//            m_searchAppThread,&SearchAppThread::recvSearchKeyword);
//    connect(m_searchAppThread,&SearchAppThread::sendSearchResult,
//            this,&MainViewWidget::recvSearchResult);
//    connect(m_queryLineEdit, &QLineEdit::textChanged, this, &MainViewWidget::searchAppSlot);
}

void MainViewWidget::switchWidget()
{
    if(m_curretWidget == 1)
    {
        loadLetterWidget();
    }
    else if(m_curretWidget ==2)
    {
        loadFunctionWidget();
    }
    else if(m_curretWidget == 3)
    {
        loadCommonUseWidget();
    }
}

void MainViewWidget::paintEvent(QPaintEvent *event)
{
    double transparency=getTransparency();

    QRect rect = this->rect();
    QPainterPath path;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    painter.setPen(Qt::transparent);
    if(!m_isFullScreen)
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
        painter.setOpacity(transparency + 0.1);
        painter.fillPath(path,this->palette().base());
//        setProperty("blurRegion", QRegion(path.toFillPolygon().toPolygon()));
      //  KWindowEffects::enableBlurBehind(this->winId(), true, QRegion(path.toFillPolygon().toPolygon()));
    }
    else//全屏固定背景色(黑底白字)
    {
//        if(QGSettings::isSchemaInstalled(QString("org.ukui.control-center.personalise").toLocal8Bit()))
//        {
//            QGSettings gsetting(QString("org.ukui.control-center.personalise").toLocal8Bit());
//            if(gsetting.keys().contains(QString("effect")))
//            {
//                if(gsetting.get("effect").toBool())
//                {
//                    painter.setBrush(Qt::black);
//                    painter.setPen(Qt::transparent);
//                    painter.setOpacity(0.25);
//                    painter.drawRect(this->rect());
//                }
//                else
//                {
//                    painter.setBrush(this->palette().base());
//                    painter.setPen(Qt::transparent);
//                    painter.setOpacity(transparency);
//                    painter.drawRect(this->rect());
//                }
//            }
//            else
//            {
//                painter.setBrush(this->palette().base());
//                painter.setPen(Qt::transparent);
//                painter.setOpacity(transparency);
//                painter.drawRect(this->rect());
//            }
//        }
//        else
//        {
            painter.setBrush(this->palette().base());
            painter.setPen(Qt::transparent);
            painter.setOpacity(transparency);
            painter.drawRect(this->rect());
//        }

        QPainterPath path;
        path.addRect(this->rect());
        KWindowEffects::enableBlurBehind(this->winId(), true, QRegion(path.toFillPolygon().toPolygon()));
    }
    QWidget::paintEvent(event);
}

void MainViewWidget::onSearchButtClicked()
{
    m_topSearchWidget->show();
    m_topWidget->hide();
    m_queryLineEdit->setFocus();
}

void MainViewWidget::onSearchCloseButtonClicked()
{
    m_topSearchWidget->hide();
    m_topWidget->show();
    m_queryLineEdit->clear();
}

bool MainViewWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==m_queryLineEdit)
    {
        char style[200];
        if(event->type()==QEvent::FocusIn)
        {
            qDebug() << "MainViewWidget::eventFilter(QObject *watched, QEvent *event)iiiiiiiiiiiiii";
            if(!m_isFullScreen)
            {
                QGSettings gsetting(QString("org.ukui.style").toLocal8Bit());
                if(gsetting.get("style-name").toString()=="ukui-light")
                    sprintf(style, "QLineEdit{border:1px solid %s;background-color:%s;border-radius:4px;color:#000000;}",
                            QueryLineEditClickedBorderDefault,QueryLineEditClickedDefaultBackground);
                else
                    sprintf(style, "QLineEdit{border:1px solid %s;background-color:%s;border-radius:4px;color:#ffffff;}",
                            QueryLineEditClickedBorder,QueryLineEditClickedBackground);
            }
            else
                sprintf(style, "QLineEdit{border:1px solid %s;background-color:%s;border-radius:4px;color:#ffffff;}",
                        QueryLineEditClickedBorder,QueryLineEditClickedBackground);
            m_queryLineEdit->setStyleSheet(style);
             if(!m_queryLineEdit->text().isEmpty())
             {
                 if(m_searchKeyWords.isEmpty())
                    searchAppSlot(m_queryLineEdit->text());
             }
             else
             {
                 m_queryWid->layout()->removeWidget(m_queryText);
                 m_queryText->setParent(nullptr);
                 m_animation->stop();
                 m_animation->setStartValue(QRect((m_queryLineEdit->width()-(m_queryIcon->width()+m_queryText->width()+10))/2,0,
                                                m_queryIcon->width()+m_queryText->width()+10,Style::QueryLineEditHeight));
                 m_animation->setEndValue(QRect(0,0,
                                               m_queryIcon->width()+5,Style::QueryLineEditHeight));
                 m_animation->setEasingCurve(QEasingCurve::OutQuad);
                 m_animation->start();
                 m_queryLineEdit->setTextMargins(-5,0,0,0);
             }
             m_isSearching=true;
        }
        else if(event->type()==QEvent::FocusOut)
        {
            qDebug() << "MainViewWidget::eventFilter(QObject *watched, QEvent *event)ooooooooooo";
            m_searchKeyWords.clear();
            if(m_queryLineEdit->text().isEmpty())
            {
                if(m_isSearching)
                {
                    if(!m_isFullScreen)
                    {
                        QGSettings gsetting(QString("org.ukui.style").toLocal8Bit());
                        if(gsetting.get("style-name").toString()=="ukui-light")
                        sprintf(style, "QLineEdit{border:1px solid %s;background-color:%s;border-radius:4px;color:#000000;}",
                                QueryLineEditClickedBorderDefault,QueryLineEditDefaultBackground);
                        else
                            sprintf(style, "QLineEdit{border:1px solid %s;background-color:%s;border-radius:4px;color:#ffffff;}",
                                    QueryLineEditClickedBorder,QueryLineEditBackground);
                    }
                    else
                        sprintf(style, "QLineEdit{border:1px solid %s;background-color:%s;border-radius:4px;color:#ffffff;}",
                                QueryLineEditClickedBorder,QueryLineEditBackground);
                    m_queryLineEdit->setStyleSheet(style);
                    m_animation->stop();
                    m_queryText->adjustSize();
                    m_animation->setStartValue(QRect(0,0,
                                                    m_queryIcon->width()+5,Style::QueryLineEditHeight));
                    m_animation->setEndValue(QRect((m_queryLineEdit->width()-(m_queryIcon->width()+m_queryText->width()+10))/2,0,
                                                 m_queryIcon->width()+m_queryText->width()+10,Style::QueryLineEditHeight));
                    m_animation->setEasingCurve(QEasingCurve::InQuad);
                    m_animation->start();
                }
            }
            else
            {
                if(!m_isFullScreen)
                {
                    QGSettings gsetting(QString("org.ukui.style").toLocal8Bit());
                    if(gsetting.get("style-name").toString()=="ukui-light")
                    sprintf(style, "QLineEdit{border:1px solid %s;background-color:%s;border-radius:4px;color:#000000;}",
                            QueryLineEditClickedDefaultBackground,QueryLineEditDefaultBackground);
                    else
                        sprintf(style, "QLineEdit{border:1px solid %s;background-color:%s;border-radius:4px;color:#ffffff;}",
                                QueryLineEditClickedBorder,QueryLineEditBackground);
                }
                else
                    sprintf(style, "QLineEdit{border:1px solid %s;background-color:%s;border-radius:4px;color:#ffffff;}",
                            QueryLineEditClickedBorder,QueryLineEditBackground);
                m_queryLineEdit->setStyleSheet(style);
            }
            m_isSearching=false;
        }
    }

     return QWidget::eventFilter(watched,event);     // 最后将事件交给上层对话框
}

void MainViewWidget::setLineEditFocus(QString arg)
{
    if(!m_queryLineEdit->hasFocus())
    {
        m_searchKeyWords=arg;
        m_queryLineEdit->setFocus();
        if(!m_queryLineEdit->text().isEmpty())
            m_queryLineEdit->setText(arg);
    }
}

/**
 * 搜索程序和文件槽函数
 */
void MainViewWidget::searchAppSlot(QString arg)
{
    if(!arg.isEmpty())//切换至搜索模块
    {
        if(m_widgetState!=0)
        {
            m_widgetState=0;
            if(!m_isFullScreen)
                loadSearchResultWidget();
            else
                loadFullSearchResultWidget();
        }
        Q_EMIT sendSearchKeyword(arg);
        m_searchAppThread->start();
    }
    else{//搜索模块显示所有的软件
        if(m_isFullScreen)
        {
            switch (m_saveCurrentWidState) {
            case 1:
                loadFullCommonUseWidget();
                break;
            case 2:
                loadFullLetterWidget();
                break;
            case 3:
                loadFullFunctionWidget();
            default:
                break;
            }
        }
        else {
            switch (m_saveCurrentWidState) {
            case 1:
                loadCommonUseWidget();
                break;
            case 2:
                loadLetterWidget();
                break;
            case 3:
                loadFunctionWidget();
                break;
            default:
                break;
            }
        }
    }
}

void MainViewWidget::recvSearchResult(QVector<QStringList> arg)
{
    m_searchAppThread->quit();
//    m_searchAppThread->wait();
    m_fullSearchResultWid->updateAppListView(arg);
    m_searchResultWid->updateAppListView(arg);
}

void MainViewWidget::animationFinishedSlot()
{
    if(m_isSearching)//进入搜索状态
    {
//        m_queryWid->layout()->removeWidget(m_queryText);
//        m_queryText->setParent(nullptr);
        m_queryLineEdit->setTextMargins(20,0,0,0);
        if(!m_searchKeyWords.isEmpty())
        {
            m_queryLineEdit->setText(m_searchKeyWords);
            m_searchKeyWords.clear();
        }
    }
    else//退出搜索状态
    {
        m_queryWid->layout()->addWidget(m_queryText);
        m_queryWid->layout()->setAlignment(m_queryIcon,Qt::AlignVCenter);
        m_queryWid->layout()->setAlignment(m_queryText,Qt::AlignVCenter);
        QPalette pe = m_queryText->palette();
        if(!m_isFullScreen)
        {
            QGSettings gsetting(QString("org.ukui.style").toLocal8Bit());
            if(gsetting.get("style-name").toString()=="ukui-light")//反黑
            {
                pe.setColor(QPalette::Text,QColor(Qt::black));
            }
            else
            {
                pe.setColor(QPalette::Text,QColor(Qt::white));
            }
        }
        else
            pe.setColor(QPalette::Text,QColor(Qt::white));

        m_queryText->setPalette(pe);
    }
}

/**
 * 加载默认主视图
 */
void MainViewWidget::loadMinMainView()
{
    this->setFixedSize(Style::defaultMainViewWidWidth,Style::minh);
    //搜索区
    m_topWidget->setFixedSize(this->width(),Style::defaultTopWidHeight);
    m_topLayout->setContentsMargins(0,0,0,0);
    m_topLayout->setAlignment(m_queryLineEdit,Qt::AlignCenter);
    m_queryLineEdit->setFixedSize(Style::defaultQueryLineEditWidth,Style::defaultQueryLineEditHeight);
    if(m_queryLineEdit->text().isEmpty())
    {
        if(m_queryWid->layout()->count()==1)
        {
            m_queryWid->layout()->addWidget(m_queryText);
            m_queryWid->layout()->setAlignment(m_queryIcon,Qt::AlignVCenter);
            m_queryWid->layout()->setAlignment(m_queryText,Qt::AlignVCenter);
        }
        m_queryText->setFixedWidth(42);
        m_queryWid->setGeometry(QRect((m_queryLineEdit->width()-(m_queryIcon->width()+m_queryText->width()+10))/2,0,
                                      m_queryIcon->width()+m_queryText->width()+10,Style::QueryLineEditHeight));
        m_queryWid->show();
    }

    char style[200];
    QPalette pe = m_queryText->palette();
    QPixmap pixmap=loadSvg(QString(":/data/img/mainviewwidget/search.svg"),16);
    QGSettings gsetting(QString("org.ukui.style").toLocal8Bit());
    if(gsetting.get("style-name").toString()=="ukui-light")
    {
        pixmap=drawSymbolicBlackColoredPixmap(pixmap);//反黑
        pe.setColor(QPalette::Text,QColor(Qt::black));
        sprintf(style, "QLineEdit{border:1px solid %s;background-color:%s;border-radius:4px;color:#000000;}",
                QueryLineEditClickedBorderDefault,QueryLineEditDefaultBackground);
    }
    else
    {
        pixmap=drawSymbolicColoredPixmap(pixmap);//反白
        pe.setColor(QPalette::Text,QColor(Qt::white));
        sprintf(style, "QLineEdit{border:1px solid %s;background-color:%s;border-radius:4px;color:#ffffff;}",
                QueryLineEditClickedBorder,QueryLineEditBackground);
    }
    m_queryLineEdit->setStyleSheet(style);
    pixmap.setDevicePixelRatio(qApp->devicePixelRatio());
    m_queryIcon->setPixmap(pixmap);
    m_queryText->setPalette(pe);

    //内容区
    m_contentWid->setFixedSize(this->width(),this->height()-m_topWidget->height());
    if(m_widgetState==0)
    {
        loadSearchResultWidget();
    }
    else if(m_widgetState==1)
        loadCommonUseWidget();
    else if(m_widgetState==2)
        loadLetterWidget();
    else if(m_widgetState==3)
        loadFunctionWidget();
    m_isFullScreen=false;
}

/**
 * 加载全屏主视图
 */
void MainViewWidget::loadMaxMainView()
{
    this->setFixedSize(Style::MainViewWidWidth,
                       Style::heightavailable);
    //搜索区
    m_topWidget->setFixedSize(this->width(),Style::TopWidgetHeight);
    m_queryLineEdit->setFixedSize(Style::QueryLineEditWidth,Style::QueryLineEditHeight);

    m_topLayout->setContentsMargins((m_topWidget->width()-Style::LeftWidWidth-m_queryLineEdit->width())/2+Style::LeftWidWidth,
                                  0,
                                  (m_topWidget->width()-Style::LeftWidWidth-m_queryLineEdit->width())/2,
                                  0);

    if(m_queryLineEdit->text().isEmpty())
    {
        if(m_queryWid->layout()->count()==1)
        {
            m_queryWid->layout()->addWidget(m_queryText);
            m_queryWid->layout()->setAlignment(m_queryIcon,Qt::AlignVCenter);
            m_queryWid->layout()->setAlignment(m_queryText,Qt::AlignVCenter);
        }
        m_queryText->adjustSize();
        m_queryWid->setGeometry(QRect((m_queryLineEdit->width()-(m_queryIcon->width()+m_queryText->width()+10))/2,0,
                                      m_queryIcon->width()+m_queryText->width()+10,Style::QueryLineEditHeight));
        m_queryWid->show();
    }

    char style[200];
    sprintf(style, "QLineEdit{border:1px solid %s;background-color:%s;border-radius:4px;color:#ffffff;}",
            QueryLineEditClickedBorder,QueryLineEditBackground);
    m_queryLineEdit->setStyleSheet(style);

    QPixmap pixmap=loadSvg(QString(":/data/img/mainviewwidget/search.svg"),16);
    pixmap=drawSymbolicColoredPixmap(pixmap);//反白
    QPalette pe = m_queryText->palette();
    pe.setColor(QPalette::Text,QColor(Qt::white));
    m_queryText->setPalette(pe);

    pixmap.setDevicePixelRatio(qApp->devicePixelRatio());
    m_queryIcon->setPixmap(pixmap);

    //内容区
    m_contentWid->setFixedSize(this->width(),this->height()-m_topWidget->height());
    if(m_widgetState==0)
    {
        loadFullSearchResultWidget();
    }
    else if(m_widgetState==1)
        loadFullCommonUseWidget();
    else if(m_widgetState==2)
        loadFullLetterWidget();
    else if(m_widgetState==3)
        loadFullFunctionWidget();
    m_isFullScreen=true;
}

void MainViewWidget::resizeControl()
{
    if(m_isFullScreen)
    {
        if(m_widgetState==2)
            m_fullLetterWid->setLetterBtnGeometry();
        if(m_widgetState==3)
            m_fullFunctionWid->setFunctionBtnGeometry();

        this->setFixedSize(Style::MainViewWidWidth,
                           Style::heightavailable);
        m_topWidget->setFixedSize(this->width(),Style::TopWidgetHeight);
        m_queryLineEdit->setFixedSize(Style::QueryLineEditWidth,Style::QueryLineEditHeight);

        m_topLayout->setContentsMargins((m_topWidget->width()-Style::LeftWidWidth-m_queryLineEdit->width())/2+Style::LeftWidWidth,
                                      0,
                                      (m_topWidget->width()-Style::LeftWidWidth-m_queryLineEdit->width())/2,
                                      0);
        m_contentWid->setFixedSize(this->width(),this->height()-m_topWidget->height());
    }
    else
    {
        this->setFixedSize(Style::defaultMainViewWidWidth,Style::minh);
        m_topWidget->setFixedSize(Style::defaultMainViewWidWidth,Style::defaultTopWidHeight);
        m_topLayout->setContentsMargins(0,0,0,0);
        m_topLayout->setAlignment(m_queryLineEdit,Qt::AlignCenter);
        m_contentWid->setFixedSize(this->width(),this->height()-m_topWidget->height());
    }

}

void MainViewWidget::resetQueryLine()
{
    if(!m_queryLineEdit->text().isEmpty())
    {
        m_queryLineEdit->clearFocus();
        m_queryLineEdit->clear();
        m_isSearching = false;

        if(m_queryWid->layout()->count()==1)
        {
            m_queryWid->layout()->addWidget(m_queryText);
            m_queryWid->layout()->setAlignment(m_queryIcon,Qt::AlignVCenter);
            m_queryWid->layout()->setAlignment(m_queryText,Qt::AlignVCenter);
        }
        m_queryText->adjustSize();
        m_queryWid->setGeometry(QRect((m_queryLineEdit->width()-(m_queryIcon->width()+m_queryText->width()+10))/2,0,
                                      m_queryIcon->width()+m_queryText->width()+10,Style::QueryLineEditHeight));
        char style[200];
        QPalette pe = m_queryText->palette();
        QPixmap pixmap=loadSvg(QString(":/data/img/mainviewwidget/search.svg"),16);
        if(!m_isFullScreen)
        {

            QGSettings gsetting(QString("org.ukui.style").toLocal8Bit());
            if(gsetting.get("style-name").toString()=="ukui-light")
            {
                pixmap=drawSymbolicBlackColoredPixmap(pixmap);//反黑
                pe.setColor(QPalette::Text,QColor(Qt::black));
                sprintf(style, "QLineEdit{border:1px solid %s;background-color:%s;border-radius:4px;color:#000000;}",
                        QueryLineEditClickedBorderDefault,QueryLineEditDefaultBackground);
            }
            else
            {
                pixmap=drawSymbolicColoredPixmap(pixmap);//反白
                pe.setColor(QPalette::Text,QColor(Qt::white));
                sprintf(style, "QLineEdit{border:1px solid %s;background-color:%s;border-radius:4px;color:#ffffff;}",
                        QueryLineEditClickedBorder,QueryLineEditBackground);
            }
        }
        else
        {
            sprintf(style, "QLineEdit{border:1px solid %s;background-color:%s;border-radius:4px;color:#ffffff;}",
                    QueryLineEditClickedBorder,QueryLineEditBackground);
            pixmap=drawSymbolicColoredPixmap(pixmap);//反白
            pe.setColor(QPalette::Text,QColor(Qt::white));
        }
        m_queryLineEdit->setStyleSheet(style);
        pixmap.setDevicePixelRatio(qApp->devicePixelRatio());
        m_queryIcon->setPixmap(pixmap);
        m_queryText->setPalette(pe);
    }

}

/**
 * 加载常用分类界面
 */
void MainViewWidget::loadCommonUseWidget()
{
    m_curretWidget = 1;
    m_label.setText(tr("All"));
    m_allAction->setChecked(true);
    m_letterAction->setChecked(false);
    m_funcAction->setChecked(false);
    m_selectButton->setIcon(QIcon(QString(":/data/img/mainviewwidget/DM-icon-所有应用.svg")));
    m_fullCommonUseWid->widgetMakeZero();
    m_letterWid->widgetMakeZero();
    m_fullLetterWid->widgetMakeZero();
    m_functionWid->widgetMakeZero();
    m_fullFunctionWid->widgetMakeZero();
    hideWidget();
    m_commonUseWid->setGeometry(0,0,this->width(),this->height()-m_topWidget->height());
    m_commonUseWid->show();
    m_commonUseWid->updateListView();
    this->layout()->update();
    m_widgetState=1;
    m_saveCurrentWidState=1;
    resetQueryLine();
}

/**
 * 加载字母分类界面
 */
void MainViewWidget::loadLetterWidget()
{
    m_curretWidget = 2;
    m_label.setText(tr("Letter"));
    m_letterAction->setChecked(true);
    m_allAction->setChecked(false);
    m_funcAction->setChecked(false);
    m_selectButton->setIcon(QIcon(QString(":/data/img/mainviewwidget/DM-icon-字母排序.svg")));
    m_commonUseWid->widgetMakeZero();
    m_fullCommonUseWid->widgetMakeZero();
    m_fullLetterWid->widgetMakeZero();
    m_functionWid->widgetMakeZero();
    m_fullFunctionWid->widgetMakeZero();
    hideWidget();
    m_letterWid->setGeometry(0,0,this->width(),this->height()-m_topWidget->height());
    m_letterWid->show();
    m_widgetState=2;
    m_saveCurrentWidState=2;
    resetQueryLine();
}

/**
 * 加载功能分类界面
 */
void MainViewWidget::loadFunctionWidget()
{
    m_curretWidget = 3;
    m_label.setText(tr("Function"));
    m_funcAction->setChecked(true);
    m_letterAction->setChecked(false);
    m_allAction->setChecked(false);
    m_selectButton->setIcon(QIcon(QString(":/data/img/mainviewwidget/DM-icon-功能排序.svg")));
    m_commonUseWid->widgetMakeZero();
    m_fullCommonUseWid->widgetMakeZero();
    m_letterWid->widgetMakeZero();
    m_fullLetterWid->widgetMakeZero();
    m_fullFunctionWid->widgetMakeZero();
    hideWidget();
    m_functionWid->setGeometry(0,0,this->width(),this->height()-m_topWidget->height());
    m_functionWid->show();
    m_widgetState=3;
    m_saveCurrentWidState=3;
    resetQueryLine();
}

/**
 * 加载全屏常用分类界面
 */
void MainViewWidget::loadFullCommonUseWidget()
{
    m_commonUseWid->widgetMakeZero();
    m_letterWid->widgetMakeZero();
    m_fullLetterWid->widgetMakeZero();
    m_functionWid->widgetMakeZero();
    m_fullFunctionWid->widgetMakeZero();
    hideWidget();
    m_fullCommonUseWid->setGeometry(0,0,this->width(),this->height()-m_topWidget->height());
    m_fullCommonUseWid->show();
    m_fullCommonUseWid->updateListView();
    m_widgetState=1;
    m_saveCurrentWidState=1;
    resetQueryLine();
}

/**
 * 加载全屏字母分类界面
 */
void MainViewWidget::loadFullLetterWidget()
{
    m_commonUseWid->widgetMakeZero();
    m_fullCommonUseWid->widgetMakeZero();
    m_letterWid->widgetMakeZero();
    m_functionWid->widgetMakeZero();
    m_fullFunctionWid->widgetMakeZero();
    hideWidget();
    m_fullLetterWid->setGeometry(0,0,this->width(),this->height()-m_topWidget->height());
    m_fullLetterWid->show();
//    if(!m_isFullScreen || (m_isFullScreen && m_saveCurrentWidState!=2))
    m_fullLetterWid->enterAnimation();
    m_widgetState=2;
    m_saveCurrentWidState=2;
    resetQueryLine();
}

/**
 * 加载全屏功能分类界面
 */
void MainViewWidget::loadFullFunctionWidget()
{
    m_commonUseWid->widgetMakeZero();
    m_fullCommonUseWid->widgetMakeZero();
    m_letterWid->widgetMakeZero();
    m_fullLetterWid->widgetMakeZero();
    m_functionWid->widgetMakeZero();
    hideWidget();
    m_fullFunctionWid->setGeometry(0,0,this->width(),this->height()-m_topWidget->height());
    m_fullFunctionWid->show();
//    if(!m_isFullScreen || (m_isFullScreen && m_saveCurrentWidState!=3))
    m_fullFunctionWid->enterAnimation();
    m_widgetState=3;
    m_saveCurrentWidState=3;
    resetQueryLine();
}

void MainViewWidget::loadSearchResultWidget()
{
    hideWidget();
    m_topWidget->hide();
    m_topSearchWidget->show();
    m_searchResultWid->setGeometry(0,0,this->width(),this->height()-m_topWidget->height());
    m_searchResultWid->show();
}

void MainViewWidget::loadFullSearchResultWidget()
{
    hideWidget();
    m_fullSearchResultWid->setGeometry(0,0,this->width(),this->height()-m_topWidget->height());
    m_fullSearchResultWid->show();
}

void MainViewWidget::hideWidget()
{
    m_commonUseWid->hide();
    m_fullCommonUseWid->hide();
    m_letterWid->hide();
    m_fullLetterWid->hide();
    m_functionWid->hide();
    m_fullFunctionWid->hide();
    m_searchResultWid->hide();
    m_fullSearchResultWid->hide();
}

void MainViewWidget::changeFocuUp()
{
    this->focusPreviousChild();
}

void MainViewWidget::changeFocuDown()
{
    this->focusNextChild();
}

/**
 * 进程开启监控槽函数
 */
void MainViewWidget::ViewOpenedSlot(QString desktopfp)
{
    qDebug()<<"open software:"<<desktopfp;
    QVector<QString> desktopfpVec=UkuiMenuInterface::desktopfpVector;
    if(desktopfpVec.contains(desktopfp))
    {
        QFileInfo fileInfo(desktopfp);
        QString desktopfn=fileInfo.fileName();

        QString dateTimeKey;
        dateTimeKey.clear();
        if(!desktopfn.isEmpty())
        {
            updateDataBaseTableTimes(desktopfn);
        }
    }
}

/**
 * desktop文件目录改变信号槽
 */
void MainViewWidget::directoryChangedSlot(const QString &path)
{
    m_directoryChangedThread->start();
}

void MainViewWidget::requestUpdateSlot()
{
    m_directoryChangedThread->quit();
    Q_EMIT directoryChangedSignal();
}

void MainViewWidget::iconThemeChangeSlot(QString key)
{
    if(key=="iconThemeName" || key=="icon-theme-name")
        Q_EMIT directoryChangedSignal();
}

void MainViewWidget::reloadUkuiMenu()
{
    Q_EMIT directoryChangedSignal();
}

void MainViewWidget::repaintWidget()
{
    this->setMinimumSize(Style::minw,Style::minh);
    m_commonUseWid->repaintWidget();
    m_fullCommonUseWid->repaintWidget();
    m_letterWid->repaintWidget();
    m_fullLetterWid->repaintWidget();
    m_functionWid->repaintWidget();
    m_fullFunctionWid->repaintWidget();
    m_searchResultWid->repaintWidget();
    m_fullSearchResultWid->repaintWidget();
}

void MainViewWidget::widgetMakeZero()
{
    m_isSearching=false;
    m_widgetState=m_saveCurrentWidState;
    m_commonUseWid->widgetMakeZero();
    m_fullCommonUseWid->widgetMakeZero();
    m_letterWid->widgetMakeZero();
    m_fullLetterWid->widgetMakeZero();
    m_functionWid->widgetMakeZero();
    m_fullFunctionWid->widgetMakeZero();
    m_queryLineEdit->blockSignals(true);
    m_queryLineEdit->clear();
    m_queryLineEdit->clearFocus();
    m_queryLineEdit->blockSignals(false);
    m_queryLineEdit->setTextMargins(0,0,0,0);
}

void MainViewWidget::moveScrollBar(int type)
{
    if(m_widgetState==0)
    {
        if(m_isFullScreen)
            m_fullSearchResultWid->moveScrollBar(type);
        else
            m_searchResultWid->moveScrollBar(type);
    }
    if(m_widgetState==1)
    {
        if(m_isFullScreen)
            m_fullCommonUseWid->moveScrollBar(type);
        else
            m_commonUseWid->moveScrollBar(type);
    }
    if(m_widgetState==2)
    {
        if(m_isFullScreen)
            m_fullLetterWid->moveScrollBar(type);
        else
            m_letterWid->moveScrollBar(type);
    }
    if(m_widgetState==3)
    {
        if(m_isFullScreen)
            m_fullFunctionWid->moveScrollBar(type);
        else
            m_functionWid->moveScrollBar(type);
    }
}

void MainViewWidget::keyPressEvent(QKeyEvent *e)
{
    if(e->type()==QEvent::KeyPress)
    {
        switch(e->key())
        {
            case Qt::Key_Enter:
            case Qt::Key_Return:
            case Qt::Key_Down:
            {
                if(m_queryLineEdit->hasFocus() && !m_queryLineEdit->text().isEmpty())
                {
                    if(m_isFullScreen)
                    {
                        m_fullSearchResultWid->setViewFocus();
                    }
                    else
                    {
                        m_searchResultWid->setViewFocus();
                    }
                }
            }
                break;
            default:
                break;
        }
    }
}

void MainViewWidget::setFocusToThis()
{
    this->setFocus();
    ////   this->focusProxy();
//        m_fullLetterWid->letterButtonClick();
//        m_fullFunctionWid->functionButtonClick();
}
