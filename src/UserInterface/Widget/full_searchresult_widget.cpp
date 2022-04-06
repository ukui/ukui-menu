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
#include "full_searchresult_widget.h"
#include <QHeaderView>
#include "style.h"
#include "utility.h"
#include <QDebug>
#include "buriedpointdatasend.h"

FullSearchResultWidget::FullSearchResultWidget(QWidget *parent) :
    QWidget(parent)
{
    initUi();
}

FullSearchResultWidget::~FullSearchResultWidget()
{
    delete m_ukuiMenuInterface;
}

void FullSearchResultWidget::initUi()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->setFocusPolicy(Qt::NoFocus);
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(40, 0, 40, 0);
    mainLayout->setSpacing(0);
    m_spaceItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Fixed);
    mainLayout->addItem(m_spaceItem);
    m_scrollArea = new ScrollArea;
    m_scrollArea->setStyleSheet("background:transparent");
    m_scrollAreaWid = new ScrollAreaWid(this);
    m_scrollAreaWid->setAttribute(Qt::WA_TranslucentBackground);
    m_scrollArea->setFixedSize(Style::m_applistWidWidth, Style::m_applistWidHeight);
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
    m_verticalScrollBar = new QScrollBar(m_scrollArea);
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
                                    "QPushButton:hover {border-radius:20px; background: rgba(255, 255, 255, 0.2);}"
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
    m_scrollAreaWidHeight = m_scrollAreaWid->height() + 1;
    initVerticalScrollBar();
    connect(m_scrollArea->verticalScrollBar(), &QScrollBar::valueChanged, this, &FullSearchResultWidget::on_setScrollBarValue);
    connect(m_verticalScrollBar, &QScrollBar::valueChanged, this, &FullSearchResultWidget::on_setAreaScrollBarValue);
    connect(m_powerOffButton, &QPushButton::customContextMenuRequested, this, &FullSearchResultWidget::on_powerOffButton_customContextMenuRequested);
    connect(m_powerOffButton, &QPushButton::clicked, this, &FullSearchResultWidget::on_powerOffButton_clicked);
}

void FullSearchResultWidget::initVerticalScrollBar()
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

void FullSearchResultWidget::on_powerOffButton_clicked()
{
    QProcess::startDetached(QString("ukui-session-tools"));
    pointDataStruct pointData;
    pointData.module = "fullWindow/FullSearchResultWidget/powerOffButton";
    pointData.function = "Clicked";
    pointData.functionNum = "";
    BuriedPointDataSend::getInstance()->setPoint(pointData);
}

void FullSearchResultWidget::on_powerOffButton_customContextMenuRequested(const QPoint &pos)
{
    RightClickMenu m_otherMenu(this);
    m_otherMenu.showShutdownMenu(m_powerOffButton->mapToGlobal(pos));
    myDebug() << "SideBarWidget::shutdownBtnRightClickSlot() 开始";
    pointDataStruct pointData;
    pointData.module = "fullWindow/FullSearchResultWidget/powerOffButton";
    pointData.function = "RightClicked";
    pointData.functionNum = "";
    BuriedPointDataSend::getInstance()->setPoint(pointData);
}

void FullSearchResultWidget::initAppListWidget()
{
    m_listView = new FullListView(this, 0);
    m_listView->installEventFilter(this);
    m_scrollAreaWidLayout->addWidget(m_listView);
    m_listView->setFixedWidth(m_scrollArea->width());
    connect(m_listView, &FullListView::sendItemClickedSignal, this, &FullSearchResultWidget::execApplication);
    connect(m_listView, &FullListView::sendHideMainWindowSignal, this, &FullSearchResultWidget::sendHideMainWindowSignal);
    connect(m_listView, &FullListView::sendSetslidebar, this, &FullSearchResultWidget::onSetSlider);
}

void FullSearchResultWidget::fillAppList()
{
    m_data.clear();
    m_listView->addData(m_data);
    resizeScrollAreaControls();
}
/**
 * 执行应用程序
 */
void FullSearchResultWidget::execApplication(QString desktopfp)
{
    Q_EMIT sendHideMainWindowSignal();
    execApp(desktopfp);
    pointDataStruct pointData;
    pointData.module = "fullWindow/FullLetterWidget/execApplication";
    pointData.function = "Clicked";
    pointData.functionNum = "";
    pointData.otherFunction[0] = desktopfp;
    BuriedPointDataSend::getInstance()->setPoint(pointData);
}

void FullSearchResultWidget::updateAppListView(QVector<QStringList> arg)
{
    m_data.clear();

    Q_FOREACH (QStringList appinfo, arg) {
        m_data.append(appinfo.at(0));
    }

    m_listView->updateData(m_data);
    resizeScrollAreaControls();
    m_scrollAreaWidHeight  = m_scrollAreaWid->height() + 1;
    initVerticalScrollBar();
}

void FullSearchResultWidget::resizeScrollAreaControls()
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

    listview->setFixedSize(m_scrollArea->width(), listview->gridSize().height() * rowcount);
    m_scrollArea->widget()->setFixedSize(listview->size());
}

bool FullSearchResultWidget::eventFilter(QObject *watched, QEvent *event)
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

void FullSearchResultWidget::selectFirstItemTab()
{
    m_listView->setFocus();
    m_listView->setCurrentIndex(m_listView->model()->index(0, 0));
    m_scrollArea->verticalScrollBar()->setValue(0);
}

void FullSearchResultWidget::repaintWidget()
{
    m_scrollArea->setFixedSize(Style::m_applistWidWidth, Style::m_applistWidHeight);
    m_scrollAreaWidLayout->removeWidget(m_listView);
    m_listView->setParent(nullptr);
    delete m_listView;
    initAppListWidget();
    fillAppList();
    m_scrollAreaWidHeight  = m_scrollAreaWid->height() + 1;
    initVerticalScrollBar();
}

void FullSearchResultWidget::moveScrollBar(int type)
{
    int height = Style::m_availableScreenHeight;

    if (type == 0) {
        m_listView->verticalScrollBar()->setSliderPosition(m_listView->verticalScrollBar()->sliderPosition() - height * 100 / 1080);
    } else {
        m_listView->verticalScrollBar()->setSliderPosition(m_listView->verticalScrollBar()->sliderPosition() + height * 100 / 1080);
    }
}

void FullSearchResultWidget::onSetSlider(int value)
{
    myDebug() << value;

    if (value == 0) {
        m_scrollArea->verticalScrollBar()->setValue(0);
    } else {
        int curvalue = m_scrollArea->verticalScrollBar()->value();
        m_scrollArea->verticalScrollBar()->setValue(curvalue + value);
    }
}

void FullSearchResultWidget::setViewFocus()
{
    m_listView->setFocus();
}

void FullSearchResultWidget::on_setScrollBarValue(int value)
{
    m_verticalScrollBar->setMaximum(m_scrollAreaWidHeight - Style::m_applistWidHeight);
    m_verticalScrollBar->setValue(value);
}

void FullSearchResultWidget::on_setAreaScrollBarValue(int value)
{
    m_scrollArea->verticalScrollBar()->setValue(value);
}
