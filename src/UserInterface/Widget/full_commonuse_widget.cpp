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

#include "full_commonuse_widget.h"
#include <QDesktopWidget>
#include "style.h"
#include "utility.h"
#include "buriedpointdatasend.h"
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
    m_scrollAreaWid = new ScrollAreaWid(this);
    m_scrollAreaWid->setAttribute(Qt::WA_TranslucentBackground);
    m_scrollArea->setFixedSize(Style::m_applistWidWidth, Style::m_applistWidHeight);
    m_scrollArea->setWidget(m_scrollAreaWid);
    m_scrollArea->setStyleSheet("QWidget{background:transparent;}");
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
    m_verticalScrollBar = new QScrollBar(this);
    m_verticalScrollBar->installEventFilter(this);
    m_verticalScrollBar->setOrientation(Qt::Vertical);
    mainLayout->addWidget(m_scrollArea);
    QSpacerItem *m_spaceItem1 = nullptr;
    m_spaceItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    mainLayout->addItem(m_spaceItem1);
    m_powerOffButton = new QPushButton(this);
    m_powerOffButton->setFixedSize(QSize(40, 40));
    m_powerOffButton->setContextMenuPolicy(Qt::CustomContextMenu);
    QIcon powerIcon;
    powerIcon.addFile(QString::fromUtf8(":/data/img/mainviewwidget/power.svg"), QSize(), QIcon::Normal, QIcon::Off);
    m_powerOffButton->setIcon(powerIcon);
    m_powerOffButton->setIconSize(QSize(28, 28));
    m_powerOffButton->setFlat(true);
    m_powerOffButton->setStyleSheet("QPushButton {padding: 0px;}"
                                    "QPushButton:hover {border-radius   :20px; background: rgba(255, 255, 255, 0.2);}"
                                    "QPushButton:pressed {border-radius:20px; background: rgba(255, 255, 255, 0.3);}");
    rightButtonLayout->addWidget(m_verticalScrollBar);
    QSpacerItem *m_spaceItem3 = nullptr;
    m_spaceItem3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    rightButtonLayout->addItem(m_spaceItem3);
    rightButtonLayout->addWidget(m_powerOffButton);
    rightButtonLayout->setAlignment(m_verticalScrollBar, Qt::AlignHCenter);
    mainLayout->addLayout(rightButtonLayout);
    m_ukuiMenuInterface = new UkuiMenuInterface;
    initAppListWidget();
    fillAppList();
    m_scrollAreaWid->adjustSize();
    flag = true;
    //翻页灵敏度时间调节
    time = new QTimer(this);
    connect(time, &QTimer::timeout, [ = ]() {
        if (flag == false) {
            flag = true;
            time->stop();
        }
    });
    m_scrollAreaWidHeight = m_scrollAreaWid->height();
    initVerticalScrollBar();
    connect(m_scrollArea->verticalScrollBar(), &QScrollBar::valueChanged, this, &FullCommonUseWidget::on_setScrollBarValue);
    connect(m_verticalScrollBar, &QScrollBar::valueChanged, this, &FullCommonUseWidget::on_setAreaScrollBarValue);
    connect(m_powerOffButton, &QPushButton::customContextMenuRequested, this, &FullCommonUseWidget::on_powerOffButton_customContextMenuRequested);
    connect(m_powerOffButton, &QPushButton::clicked, this, &FullCommonUseWidget::on_powerOffButton_clicked);
}

void FullCommonUseWidget::initVerticalScrollBar()
{
    m_verticalScrollBar->setFixedHeight(200);
    int scrollBarSize = 200 * Style::m_applistWidHeight / m_scrollAreaWidHeight + 1;

    if (scrollBarSize >= 200) {
        m_verticalScrollBar->hide();
    } else {
        m_verticalScrollBar->show();
    }

    m_scrollBarStyle = QString("QScrollBar:vertical{width: %2px; background: rgba(12, 12, 12, 1); "
                               "margin: 0px,0px,0px,0px; border-radius: %3px;}"
                               "QScrollBar::handle:vertical{width: %2px; background: rgba(255, 255, 255, 1);"
                               "border-radius: %3px; min-height: %1;}"
                               "QScrollBar::add-line:vertical{ height: 0px; width: 0px; subcontrol-position: bottom;}"
                               "QScrollBar::sub-line:vertical{ height: 0px; width: 0px; subcontrol-position:top;}").arg(scrollBarSize);
    m_verticalScrollBar->setStyleSheet(m_scrollBarStyle.arg(4).arg(2));
}

void FullCommonUseWidget::on_powerOffButton_clicked()
{
    QProcess::startDetached(QString("ukui-session-tools"));
    pointDataStruct pointData;
    pointData.module = "fullWindow/commonUseWidget/powerOffButton";
    pointData.function = "Clicked";
    pointData.functionNum = "";
    BuriedPointDataSend::getInstance()->setPoint(pointData);
}

void FullCommonUseWidget::on_powerOffButton_customContextMenuRequested(const QPoint &pos)
{
    RightClickMenu m_otherMenu(this);
    m_otherMenu.showShutdownMenu(m_powerOffButton->mapToGlobal(pos));
    myDebug() << "SideBarWidget::shutdownBtnRightClickSlot() 开始";
    pointDataStruct pointData;
    pointData.module = "fullWindow/commonUseWidget/powerOffButton";
    pointData.function = "RightClicked";
    pointData.functionNum = "";
    BuriedPointDataSend::getInstance()->setPoint(pointData);
}

void FullCommonUseWidget::initAppListWidget()
{
    m_listView = new FullListView(this, 0);
    m_listView->installEventFilter(this);
    m_scrollAreaWidLayout->addWidget(m_listView);
    m_listView->setFixedWidth(m_scrollArea->width());
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
    int dividend = m_scrollArea->width() / Style::m_applistGridSizeWidth;
    int rowcount = 0;

    if (listview->model()->rowCount() % dividend > 0) {
        rowcount = listview->model()->rowCount() / dividend + 1;
    } else {
        rowcount = listview->model()->rowCount() / dividend;
    }

    listview->setFixedSize(m_listView->width(), listview->gridSize().height() * rowcount);
    m_scrollArea->widget()->setFixedSize(listview->size());
}

void FullCommonUseWidget::fillAppList()
{
    m_data.clear();

    Q_FOREACH (QString desktopfp, UkuiMenuInterface::allAppVector) {
        m_data.append(desktopfp);
    }

    m_listView->addData(m_data);
    resizeScrollAreaControls();
}

void FullCommonUseWidget::selectFirstItem()
{
    m_listView->setCurrentIndex(m_listView->model()->index(0, 0));
}

void FullCommonUseWidget::on_setScrollBarValue(int value)
{
    m_verticalScrollBar->setMaximum(m_scrollAreaWidHeight - Style::m_applistWidHeight);
    m_verticalScrollBar->setValue(value);
}

bool FullCommonUseWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_listView) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *ke = (QKeyEvent *)event;

            if (ke->key() == Qt::Key_Tab) {
                Q_EMIT setFocusToSideWin();
                return true;
            }
        }
    }

    if (watched == m_verticalScrollBar) {
        if (event->type() == QEvent::Enter) {
            m_verticalScrollBar->setStyleSheet(m_scrollBarStyle.arg(8).arg(4));
        }

        if (event->type() == QEvent::Leave) {
            m_verticalScrollBar->setStyleSheet(m_scrollBarStyle.arg(4).arg(2));
        }
    }

    return QWidget::eventFilter(watched, event);
}

void FullCommonUseWidget::onSetSlider(int value)
{
    if (value == 0) {
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
    Q_EMIT sendUpdateOtherView();
}

void FullCommonUseWidget::updateListView()
{
    m_data.clear();

    Q_FOREACH (QString desktopfp, m_ukuiMenuInterface->getAllClassification()) {
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
    m_scrollArea->setFixedSize(Style::m_applistWidWidth, Style::m_applistWidHeight);
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
    int height = Style::m_availableScreenHeight;

    if (type == 0) {
        m_listView->verticalScrollBar()->setSliderPosition(m_listView->verticalScrollBar()->sliderPosition() - height * 100 / 1080);
    } else {
        m_listView->verticalScrollBar()->setSliderPosition(m_listView->verticalScrollBar()->sliderPosition() + height * 100 / 1080);
    }
}
