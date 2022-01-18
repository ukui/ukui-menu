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

#include "full_function_widget.h"
#include <QDesktopWidget>
#include <QHeaderView>
#include "style.h"
#include <QDebug>
#include <QSvgRenderer>
#include <QPainter>

FullFunctionWidget::FullFunctionWidget(QWidget *parent) :
    QWidget(parent)
{
    initUi();
}

FullFunctionWidget::~FullFunctionWidget()
{
    delete m_ukuiMenuInterface;
}

void FullFunctionWidget::initUi()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFocusPolicy(Qt::NoFocus);
    m_applistWid = new QWidget(this);
    m_iconListWid = new QWidget(this);
    m_iconListWid->setAttribute(Qt::WA_TranslucentBackground);
    m_iconListWid->setAutoFillBackground(false);
    m_applistWid->setFixedSize(Style::m_applistWidWidth, Style::m_applistWidHeight);
    m_iconListWid->setFixedSize(Style::m_leftWidWidth, Style::m_applistWidHeight);
    m_verticalScrollBar = new QScrollBar(m_scrollArea);
    m_verticalScrollBar->installEventFilter(this);
    m_verticalScrollBar->setOrientation(Qt::Vertical);
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 40, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(m_iconListWid);
    mainLayout->addWidget(m_applistWid);
    QSpacerItem *m_spaceItem1 = nullptr;
    m_spaceItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    mainLayout->addItem(m_spaceItem1);
    QVBoxLayout *rightButtonLayout = new QVBoxLayout(this);
    rightButtonLayout->setContentsMargins(0, 0, 0, 20);
    rightButtonLayout->setSpacing(0);
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
    QSpacerItem *m_spaceItem2 = nullptr;
    m_spaceItem2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    rightButtonLayout->addItem(m_spaceItem2);
    rightButtonLayout->addWidget(m_verticalScrollBar);
    QSpacerItem *m_spaceItem3 = nullptr;
    m_spaceItem3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    rightButtonLayout->addItem(m_spaceItem3);
    rightButtonLayout->addWidget(m_powerOffButton);
    rightButtonLayout->setAlignment(m_verticalScrollBar, Qt::AlignHCenter);
    mainLayout->addLayout(rightButtonLayout);
    //    mainLayout->addWidget(verticalScrollBar);
    //    this->setLayout(mainLayout);
    m_ukuiMenuInterface = new UkuiMenuInterface;
    initAppListWidget();
    initIconListWidget();
    flag = true;
    //翻页灵敏度时间调节
    time = new QTimer(this);
    connect(time, &QTimer::timeout, [ = ]() {
        if (flag == false) {
            flag = true;
            time->stop();
        }
    });
    connect(m_scrollArea->verticalScrollBar(), &QScrollBar::valueChanged, this, &FullFunctionWidget::on_setScrollBarValue);
    connect(m_verticalScrollBar, &QScrollBar::valueChanged, this, &FullFunctionWidget::on_setAreaScrollBarValue);
    connect(m_powerOffButton, &QPushButton::customContextMenuRequested, this, &FullFunctionWidget::on_powerOffButton_customContextMenuRequested);
    connect(m_powerOffButton, &QPushButton::clicked, this, &FullFunctionWidget::on_powerOffButton_clicked);
}

/**
 * 初始化应用列表界面
 */
void FullFunctionWidget::initAppListWidget()
{
    QHBoxLayout *layout = new QHBoxLayout(m_applistWid);
    layout->setContentsMargins(0, 0, 0, 0);
    m_applistWid->setLayout(layout);
    m_scrollArea = new ScrollArea;
    m_scrollArea->setStyleSheet("background:transparent");
    m_scrollAreaWid = new ScrollAreaWid(this);
    m_scrollAreaWid->setAttribute(Qt::WA_TranslucentBackground);
    m_scrollArea->setFixedSize(m_applistWid->width(), m_applistWid->height());
    m_scrollArea->setWidget(m_scrollAreaWid);
    m_scrollArea->setWidgetResizable(true);
    m_scrollAreaWidLayout = new QVBoxLayout(m_scrollAreaWid);
    m_scrollAreaWidLayout->setContentsMargins(0, 0, 0, 0);
    m_scrollAreaWidLayout->setSpacing(10);
    layout->addWidget(m_scrollArea);
    connect(m_scrollArea->verticalScrollBar(), &QScrollBar::valueChanged,
            this, &FullFunctionWidget::valueChangedSlot);
    fillAppList();
    m_scrollAreaWidHeight = m_scrollAreaWid->height();
    initVerticalScrollBar();
}

void FullFunctionWidget::initVerticalScrollBar()
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

void FullFunctionWidget::on_powerOffButton_clicked()
{
    QProcess::startDetached(QString("ukui-session-tools"));
}

void FullFunctionWidget::on_powerOffButton_customContextMenuRequested(const QPoint &pos)
{
    RightClickMenu m_otherMenu(this);
    m_otherMenu.showShutdownMenu(m_powerOffButton->mapToGlobal(pos));
    myDebug() << "SideBarWidget::shutdownBtnRightClickSlot() 开始";
}


/**
 * 填充应用列表
 */
void FullFunctionWidget::fillAppList()
{
    m_classificationList.clear();
    QVector<QStringList> vector = UkuiMenuInterface::functionalVector;
    QStringList functionList = m_ukuiMenuInterface->getFunctionClassName();

    for (int i = 0; i < vector.size(); i++) {
        if (!(vector.at(i).isEmpty())) {
            insertClassificationBtn(functionList.at(i));
            insertAppList(vector.at(i));
        }
    }

    resizeScrollAreaControls();
}

void FullFunctionWidget::insertClassificationBtn(QString category)
{
    SplitBarFrame *classificationbtn = new SplitBarFrame(this, category, m_scrollArea->width() - 12, 30, 2);
    classificationbtn->setAttribute(Qt::WA_TranslucentBackground);
    classificationbtn->setAutoFillBackground(false);
    m_scrollAreaWidLayout->addWidget(classificationbtn);
    m_classificationList.append(category);
}

void FullFunctionWidget::insertAppList(QStringList desktopfplist)
{
    FullListView *listview = new FullListView(this, 2);
    //修复异常黑框问题
    connect(m_scrollArea, &ScrollArea::requestUpdate, listview->viewport(), [ = ]() {
        listview->repaint(listview->rect());
    });
    connect(listview, &FullListView::sendSetslidebar, this, &FullFunctionWidget::onSetSlider);
    connect(this, &FullFunctionWidget::selectFirstItem, listview, &FullListView::selectFirstItem);
    listview->installEventFilter(this);
    m_scrollAreaWidLayout->addWidget(listview);
    m_data.clear();

    for (int i = 0; i < desktopfplist.count(); i++) {
        m_data.append(desktopfplist.at(i));
    }

    listview->addData(m_data);
    connect(listview, &FullListView::sendItemClickedSignal, this, &FullFunctionWidget::execApplication);
    connect(listview, &FullListView::sendHideMainWindowSignal, this, &FullFunctionWidget::sendHideMainWindowSignal);
}

/**
 * 执行应用程序
 */
void FullFunctionWidget::execApplication(QString desktopfp)
{
    Q_EMIT sendHideMainWindowSignal();
    execApp(desktopfp);
}

void FullFunctionWidget::on_setAreaScrollBarValue(int value)
{
    //    m_scrollArea->verticalScrollBar()->setMaximum(maxmumValue);
    m_scrollArea->verticalScrollBar()->setValue(value);
}

/**
 * 更新应用列表
 */
void FullFunctionWidget::updateAppListView()
{
    //刷新应用列表界面
    QLayoutItem *child;

    while ((child = m_scrollAreaWidLayout->takeAt(0)) != 0) {
        QWidget *wid = child->widget();
        m_scrollAreaWidLayout->removeWidget(wid);
        wid->setParent(nullptr);
        delete wid;
        delete child;
    }

    fillAppList();

    //刷新图标列表界面
    Q_FOREACH (QAbstractButton *button, m_buttonList) {
        m_btnGroup->removeButton(button);
    }

    m_buttonList.clear();
    m_iconListWidLayout->removeItem(m_topSpacerItem);
    m_iconListWidLayout->removeItem(m_bottomSpacerItem);

    while ((child = m_iconListWidLayout->takeAt(0)) != 0) {
        QWidget *wid = child->widget();
        m_iconListWidLayout->removeWidget(wid);
        wid->setParent(nullptr);
        delete wid;
        delete child;
    }

    initIconListScrollArea();
    m_scrollAreaWidHeight  = m_scrollAreaWid->height();
    initVerticalScrollBar();
}

/**
 * 设置scrollarea所填充控件大小
 */
void FullFunctionWidget::resizeScrollAreaControls()
{
    int row = 0;
    int areaHeight = 0;

    while (row < m_scrollAreaWidLayout->count() / 2) {
        //应用界面
        QLayoutItem *widItem = m_scrollAreaWidLayout->itemAt(row * 2 + 1);
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

        listview->setFixedSize(m_scrollArea->width(), listview->gridSize().height()*rowcount);
        areaHeight += listview->height() + 50;
        row++;
    }

    m_scrollArea->widget()->setFixedSize(m_scrollArea->width(), areaHeight - 10);
}

/**
 * 初始化图标列表界面
 */
void FullFunctionWidget::initIconListWidget()
{
    m_iconListWidLayout = new QVBoxLayout(m_iconListWid);
    m_iconListWidLayout->setSpacing(Style::LeftSpaceBetweenItem);
    m_iconListWidLayout->setContentsMargins(45, 0, 0, 0);
    m_topSpacerItem = new QSpacerItem(20, 40, QSizePolicy::Fixed, QSizePolicy::Expanding);
    m_bottomSpacerItem = new QSpacerItem(20, 40, QSizePolicy::Fixed, QSizePolicy::Expanding);
    m_btnGroup = new QButtonGroup(m_iconListWid);
    m_animation = new QPropertyAnimation(m_iconListWid, "geometry");
    m_scrollAnimation = new QPropertyAnimation(m_scrollArea->verticalScrollBar(), "value");
    m_scrollAnimation->setEasingCurve(QEasingCurve::OutQuad);
    connect(m_scrollAnimation, &QPropertyAnimation::finished, this, &FullFunctionWidget::animationFinishSlot);
    connect(m_scrollAnimation, &QPropertyAnimation::valueChanged, this, &FullFunctionWidget::animationValueChangedSlot);
    initIconListScrollArea();
}

void FullFunctionWidget::on_setScrollBarValue(int value)
{
    m_verticalScrollBar->setMaximum(m_scrollAreaWidHeight - Style::m_applistWidHeight);
    m_verticalScrollBar->setValue(value);
}

/**
 * 初始化图标列表界面数据表格iconlisttableWid
 */
void FullFunctionWidget::initIconListScrollArea()
{
    m_iconListWidLayout->addItem(m_topSpacerItem);

    for (int i = 0; i < m_classificationList.size(); i++) {
        FunctionClassifyButton *iconbtn = new FunctionClassifyButton(
            Style::LeftBtnWidth,
            Style::LeftBtnHeight,
            Style::LeftIconSize,
            m_classificationList.at(i),
            true,
            true);
        iconbtn->setChecked(false);
        m_buttonList.append(iconbtn);
        m_iconListWidLayout->addWidget(iconbtn);
        m_iconListWidLayout->setAlignment(iconbtn, Qt::AlignLeft);
    }

    m_iconListWidLayout->addItem(m_bottomSpacerItem);
    int id = 0;

    Q_FOREACH (QAbstractButton *btn, m_buttonList) {
        m_btnGroup->addButton(btn, id++);
    }

    connect(m_btnGroup, static_cast<void(QButtonGroup::*)(QAbstractButton *)>(&QButtonGroup::buttonClicked), this, &FullFunctionWidget::btnGroupClickedSlot);

    //    m_iconListScrollArea->widget()->adjustSize();
    if (m_btnGroup->button(0) != nullptr) {
        m_btnGroup->button(0)->click();
    }
}

void FullFunctionWidget::btnGroupClickedSlot(QAbstractButton *btn)
{
    disconnect(m_scrollArea->verticalScrollBar(), &QScrollBar::valueChanged,
               this, &FullFunctionWidget::valueChangedSlot);

    Q_FOREACH (QAbstractButton *button, m_buttonList) {
        if (m_btnGroup->id(btn) == m_buttonList.indexOf(button)) {
            m_beginPos = m_scrollArea->verticalScrollBar()->sliderPosition();
            m_endPos = m_scrollAreaWidLayout->itemAt(m_btnGroup->id(btn) * 2)->widget()->y();
            m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            m_scrollAnimation->stop();
            m_scrollAnimation->setStartValue(m_beginPos);
            m_scrollAnimation->setEndValue(m_endPos);
            m_scrollAnimation->start();
            button->setChecked(true);
        } else {
            button->setChecked(false);
        }
    }
}

void FullFunctionWidget::animationFinishSlot()
{
    //    if(m_scrollArea->verticalScrollBar()->value()==m_endPos ||
    //            m_scrollArea->verticalScrollBar()->value()==m_scrollArea->verticalScrollBar()->maximum())
    //    {
    //        m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    connect(m_scrollArea->verticalScrollBar(), &QScrollBar::valueChanged,
            this, &FullFunctionWidget::valueChangedSlot);
    //    }
}

void FullFunctionWidget::animationValueChangedSlot(const QVariant &value)
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

void FullFunctionWidget::valueChangedSlot(int value)
{
    int index = 0;

    while (index <= m_classificationList.count() - 1) {
        int min = m_scrollAreaWidLayout->itemAt(2 * index)->widget()->y();
        int max = 0;

        if (index == m_classificationList.count() - 1) {
            max = m_scrollAreaWid->height();
        } else {
            max = m_scrollAreaWidLayout->itemAt(2 * (index + 1))->widget()->y();
        }

        if (value >= min && value < max) {
            Q_FOREACH (QAbstractButton *button, m_buttonList) {
                FunctionClassifyButton *fcbutton = qobject_cast<FunctionClassifyButton *>(button);

                if (index == m_buttonList.indexOf(button)) {
                    fcbutton->setChecked(true);
                } else {
                    fcbutton->setChecked(false);
                }
            }

            break;
        } else {
            index++;
        }
    }
}

QAbstractButton *FullFunctionWidget::getCurLetterButton(int value)
{
    return m_buttonList.at(value);
}


void FullFunctionWidget::enterAnimation()
{
    m_animation->setDuration(200);//动画总时间
    m_animation->setStartValue(QRect(0, 0,
                                     0, m_iconListWid->height()));
    m_animation->setEndValue(QRect(Style::LeftMargin,
                                   0,
                                   Style::LeftBtnWidth,
                                   m_iconListWid->height()));
    m_animation->setEasingCurve(QEasingCurve::InQuart);
    m_animation->start();
    m_iconListScrollAreaWid->show();
}

void FullFunctionWidget::setFunctionBtnGeometry()
{
    //    int height=m_classificationList.size()*Style::LeftBtnHeight+(m_classificationList.size()-1)*Style::LeftSpaceBetweenItem;
    m_iconListScrollAreaWid->setGeometry(QRect(Style::LeftMargin,
                                         0,
                                         Style::LeftBtnWidth,
                                         m_iconListWid->height()));
    m_iconListScrollAreaWid->show();
}

void FullFunctionWidget::repaintWidget()
{
    m_applistWid->setFixedSize(Style::m_applistWidWidth, Style::m_applistWidHeight);
    m_iconListWid->setFixedSize(Style::m_leftWidWidth, Style::m_applistWidHeight);
    m_scrollArea->setFixedSize(m_applistWid->width(), m_applistWid->height());
    updateAppListView();
}

void FullFunctionWidget::widgetMakeZero()
{
    if (m_btnGroup->button(0) != nullptr) {
        m_btnGroup->button(0)->click();
    }

    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

void FullFunctionWidget::onSetSlider(int value)
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

bool FullFunctionWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_verticalScrollBar) {
        if (watched == m_verticalScrollBar) {
            if (event->type() == QEvent::Enter) {
                m_verticalScrollBar->setStyleSheet(m_scrollBarStyle.arg(8).arg(4));
            }

            if (event->type() == QEvent::Leave) {
                m_verticalScrollBar->setStyleSheet(m_scrollBarStyle.arg(4).arg(2));
            }
        }
    } else {
        if (event->type() == QEvent::KeyPress) {
            QLayoutItem *widItem = m_scrollAreaWidLayout->itemAt(2 * m_buttonList.size() - 1);
            QWidget *wid = widItem->widget();
            FullListView *m_listview = qobject_cast<FullListView *>(wid);
            QLayoutItem *widItemTop = m_scrollAreaWidLayout->itemAt(1);
            QWidget *widTop = widItemTop->widget();
            FullListView *m_listviewTop = qobject_cast<FullListView *>(widTop);
            QKeyEvent *ke = (QKeyEvent *)event;

            if (ke->key() == Qt::Key_Tab) {
                Q_EMIT setFocusToSideWin();
                return true;
            }

            if (ke->key() == Qt::Key_Up) {
                if (!m_listviewTop->hasFocus()) {
                    QAbstractButton *buttonTop = getCurLetterButton((--m_index) % m_buttonList.size());
                    btnGroupClickedSlot(buttonTop);
                    this->m_scrollArea->setFocusToPreChild();
                } else {
                    m_listview->setFocus();
                    QAbstractButton *button = getCurLetterButton(m_buttonList.size() - 1);
                    btnGroupClickedSlot(button);
                    m_index = m_buttonList.size() - 1;
                }

                Q_EMIT selectFirstItem();
                return true;
            }

            if (ke->key() == Qt::Key_Down) {
                if (!m_listview->hasFocus()) {
                    QAbstractButton *button = getCurLetterButton((++m_index) % m_buttonList.size());
                    btnGroupClickedSlot(button);
                    this->m_scrollArea->setFocusToNextChild();
                } else {
                    m_listviewTop->setFocus();
                    QAbstractButton *buttonTop = getCurLetterButton(0);
                    btnGroupClickedSlot(buttonTop);
                    m_listviewTop->setCurrentIndex(m_listviewTop->model()->index(0, 0));
                    m_index = 0;
                }

                Q_EMIT selectFirstItem();
                return true;
            }
        }
    }

    return QWidget::eventFilter(watched, event);
}

void FullFunctionWidget::functionButtonClick()
{
    if (m_btnGroup->button(0) != nullptr) {
        m_btnGroup->button(0)->click();
    }

    m_index = 0;
}

void FullFunctionWidget::setFocusToThis()
{
    QLayoutItem *widItemTop = m_scrollAreaWidLayout->itemAt(1);
    QWidget *widTop = widItemTop->widget();
    FullListView *m_listviewTop = qobject_cast<FullListView *>(widTop);
    functionButtonClick();
    m_listviewTop->setFocus();
    Q_EMIT selectFirstItem();
}
