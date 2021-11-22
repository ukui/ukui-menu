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

#include "fullcommonusewidget.h"
#include <QDesktopWidget>
#include "src/Style/style.h"
#include "src/UtilityFunction/utility.h"
#include <QDebug>

FullCommonUseWidget::FullCommonUseWidget(QWidget *parent) :
    QWidget(parent)
{
    Style::initWidStyle();
    initUi();
}

FullCommonUseWidget::~FullCommonUseWidget()
{
    delete m_ukuiMenuInterface;
}

void FullCommonUseWidget::initUi()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setFocusPolicy(Qt::NoFocus);
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(40, 0, 40, 0);
    mainLayout->setSpacing(0);
    m_spaceItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Fixed);
    mainLayout->addItem(m_spaceItem);
    m_scrollArea = new ScrollArea();
    m_scrollArea->setStyleSheet("background:transparent");
    m_scrollAreaWid = new ScrollAreaWid(this);
    m_scrollAreaWid->setAttribute(Qt::WA_TranslucentBackground);
    m_scrollArea->setFixedSize(Style::AppListWidWidth, Style::AppListWidHeight);
    m_scrollArea->setWidget(m_scrollAreaWid);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollAreaWidLayout = new QVBoxLayout(m_scrollAreaWid);
    m_scrollAreaWidLayout->setContentsMargins(0, 0, 0, 0);
    m_scrollAreaWidLayout->setSpacing(10);
    QVBoxLayout *rightButtonLayout = new QVBoxLayout(this);
    rightButtonLayout->setContentsMargins(0, 0, 0, 20);
    rightButtonLayout->setSpacing(0);
    QSpacerItem *m_spaceItem2 = nullptr;
    m_spaceItem2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    rightButtonLayout->addItem(m_spaceItem2);
    verticalScrollBar = new QScrollBar(m_scrollArea);
    verticalScrollBar->setOrientation(Qt::Vertical);
    mainLayout->addWidget(m_scrollArea);
    QSpacerItem *m_spaceItem1 = nullptr;
    m_spaceItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    mainLayout->addItem(m_spaceItem1);
    powerOffButton = new QPushButton(this);
    powerOffButton->setMinimumSize(QSize(24, 24));
    powerOffButton->setContextMenuPolicy(Qt::CustomContextMenu);
    QIcon icon6;
    icon6.addFile(QString::fromUtf8(":/data/img/mainviewwidget/icon-电源.svg"), QSize(), QIcon::Normal, QIcon::Off);
    powerOffButton->setIcon(icon6);
    powerOffButton->setIconSize(QSize(24, 24));
    powerOffButton->setFlat(true);
    powerOffButton->setStyleSheet("padding: 0px;");
    rightButtonLayout->addWidget(verticalScrollBar);
    QSpacerItem *m_spaceItem3 = nullptr;
    m_spaceItem3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    rightButtonLayout->addItem(m_spaceItem3);
    rightButtonLayout->addWidget(powerOffButton);
    rightButtonLayout->setAlignment(verticalScrollBar, Qt::AlignHCenter);
    mainLayout->addLayout(rightButtonLayout);
    m_ukuiMenuInterface = new UkuiMenuInterface;
    initAppListWidget();
    fillAppList();
    m_scrollAreaWid->adjustSize();
    flag = true;
    //翻页灵敏度时间调节
    time = new QTimer(this);
    connect(time, &QTimer::timeout, [ = ]() {
        if(flag == false) {
            flag = true;
            time->stop();
        }
    });
    m_scrollAreaWidHeight = m_scrollAreaWid->height();
    initVerticalScrollBar();
    connect(m_scrollArea->verticalScrollBar(), &QScrollBar::valueChanged, this, &FullCommonUseWidget::on_setScrollBarValue);
    connect(verticalScrollBar, &QScrollBar::valueChanged, this, &FullCommonUseWidget::on_setAreaScrollBarValue);
    connect(powerOffButton, &QPushButton::customContextMenuRequested, this, &FullCommonUseWidget::on_powerOffButton_customContextMenuRequested);
    connect(powerOffButton, &QPushButton::clicked, this, &FullCommonUseWidget::on_powerOffButton_clicked);
}

void FullCommonUseWidget::initVerticalScrollBar()
{
    verticalScrollBar->setFixedHeight(200);
    int scrollBarSize = 200 * Style::AppListWidHeight / m_scrollAreaWidHeight + 1;
    QString scrollBarStyle = QString("QScrollBar:vertical"
                                     "{"
                                     "width:4px;"
                                     "background:rgba(0,0,0,60%);"
                                     "margin:0px,0px,0px,0px;"
                                     "border-radius:2px;"
                                     "}"
                                     "QScrollBar::handle:vertical"
                                     "{"
                                     "width:8px;"
                                     "background:rgba(255,255,255,100%);"
                                     " border-radius:2px;"
                                     "min-height:%1;"
                                     "}"
                                     "QScrollBar::add-line:vertical"
                                     "{"
                                     "height:0px;width:0px;"
                                     "subcontrol-position:bottom;"
                                     "}"
                                     "QScrollBar::sub-line:vertical"
                                     "{"
                                     "height:0px;width:0px;"
                                     "subcontrol-position:top;"
                                     "}").arg(scrollBarSize);
    verticalScrollBar->setStyleSheet(scrollBarStyle);
}

void FullCommonUseWidget::on_powerOffButton_clicked()
{
    QProcess::startDetached(QString("ukui-session-tools"));
}

void FullCommonUseWidget::on_powerOffButton_customContextMenuRequested(const QPoint &pos)
{
    RightClickMenu m_otherMenu(this);
    // connect(&m_otherMenu, &RightClickMenu::sendMainWinActiveSignal, this, &SideBarWidget::sendShowMainWindowSignal);
    //  Q_EMIT sendShowMainWindowSignal(false);
    int ret = m_otherMenu.showShutdownMenu(powerOffButton->mapToGlobal(pos));
    qDebug() << "SideBarWidget::shutdownBtnRightClickSlot() 开始";

    if(ret >= 10 && ret <= 17) {
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

void FullCommonUseWidget::initAppListWidget()
{
    m_listView = new FullListView(this, 0);
    m_listView->installEventFilter(this);
    //    m_listView->setFixedSize(this->width()-Style::LeftWidWidth+3,this->height());
    //    QHBoxLayout *mainLayout=qobject_cast<QHBoxLayout*>(this->layout());
    //    mainLayout->insertWidget(1,m_listView);
    m_scrollAreaWidLayout->addWidget(m_listView);
    m_listView->setFixedWidth(m_scrollArea->width());
    connect(m_listView, &FullListView::sendItemClickedSignal, this, &FullCommonUseWidget::execApplication);
    connect(m_listView, &FullListView::sendUpdateAppListSignal, this, &FullCommonUseWidget::updateListViewSlot);
    connect(m_listView, &FullListView::sendHideMainWindowSignal, this, &FullCommonUseWidget::sendHideMainWindowSignal);
    connect(m_listView, &FullListView::sendSetslidebar, this, &FullCommonUseWidget::onSetSlider);
}

void FullCommonUseWidget::resizeScrollAreaControls()
{
    QLayoutItem *widItem = m_scrollAreaWidLayout->itemAt(0);
    QWidget *wid = widItem->widget();
    FullListView *listview = qobject_cast<FullListView *>(wid);
    listview->adjustSize();
    int dividend = m_scrollArea->width() / Style::AppListGridSizeWidth;
    int rowcount = 0;

    if(listview->model()->rowCount() % dividend > 0) {
        rowcount = listview->model()->rowCount() / dividend + 1;
    } else {
        rowcount = listview->model()->rowCount() / dividend;
    }

    listview->setFixedSize(m_listView->width(), listview->gridSize().height()*rowcount);
    m_scrollArea->widget()->setFixedSize(listview->size());
}

void FullCommonUseWidget::fillAppList()
{
    m_data.clear();

    Q_FOREACH(QString desktopfp, UkuiMenuInterface::allAppVector) {
        m_data.append(desktopfp);
    }

    m_listView->addData(m_data);
    resizeScrollAreaControls();
}

/**
 * 执行应用程序
 */
void FullCommonUseWidget::execApplication(QString desktopfp)
{
    Q_EMIT sendHideMainWindowSignal();
    execApp(desktopfp);
}

void FullCommonUseWidget::selectFirstItem()
{
    m_listView->setCurrentIndex(m_listView->model()->index(0, 0));
}

void FullCommonUseWidget::on_setScrollBarValue(int value)
{
    verticalScrollBar->setMaximum(m_scrollAreaWidHeight - Style::AppListWidHeight);
    verticalScrollBar->setValue(value);
}

bool FullCommonUseWidget::eventFilter(QObject *watched, QEvent *event)
{
    if( event->type() == QEvent::KeyPress ) {
        QKeyEvent *ke = (QKeyEvent *)event;

        if( ke->key() == Qt::Key_Tab ) {
            Q_EMIT setFocusToSideWin();
            return true;
        }
    }

    return QWidget::eventFilter(watched, event);
}

void FullCommonUseWidget::onSetSlider(int value)
{
    if(value == 0) {
        m_scrollArea->verticalScrollBar()->setValue(0);
    } else {
        int curvalue = m_scrollArea->verticalScrollBar()->value();
        m_scrollArea->verticalScrollBar()->setValue(curvalue + value);
    }
}

void FullCommonUseWidget::selectFirstItemTab()
{
    m_listView->setFocus();
    m_listView->setCurrentIndex(m_listView->model()->index(0, 0));
    onSetSlider(0);
}

void FullCommonUseWidget::on_setAreaScrollBarValue(int value)
{
    m_scrollArea->verticalScrollBar()->setValue(value);
}
/**
 * 更新应用列表
 */
void FullCommonUseWidget::updateListViewSlot()
{
    updateListView();
}

void FullCommonUseWidget::updateListView()
{
    m_data.clear();

    Q_FOREACH(QString desktopfp, m_ukuiMenuInterface->getAllClassification()) {
        m_data.append(desktopfp);
    }

    m_listView->updateData(m_data);
    m_data.clear();
    resizeScrollAreaControls();
    m_scrollAreaWidHeight  = m_scrollAreaWid->height();
    initVerticalScrollBar();
}

void FullCommonUseWidget::repaintWidget()
{
    m_scrollAreaWidLayout->removeWidget(m_listView);
    m_listView->setParent(nullptr);
    delete m_listView;
    initAppListWidget();
    fillAppList();
    m_scrollAreaWidHeight  = m_scrollAreaWid->height();
    initVerticalScrollBar();
    on_setAreaScrollBarValue(0);
}

void FullCommonUseWidget::widgetMakeZero()
{
    m_listView->verticalScrollBar()->setSliderPosition(0);
}

void FullCommonUseWidget::moveScrollBar(int type)
{
    int height = Style::primaryScreenHeight;

    if(type == 0) {
        m_listView->verticalScrollBar()->setSliderPosition(m_listView->verticalScrollBar()->sliderPosition() - height * 100 / 1080);
    } else {
        m_listView->verticalScrollBar()->setSliderPosition(m_listView->verticalScrollBar()->sliderPosition() + height * 100 / 1080);
    }
}
