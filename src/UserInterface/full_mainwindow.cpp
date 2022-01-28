#include "full_mainwindow.h"
#include <KWindowEffects>
#include <QAction>
#include <QTranslator>
#include "src/UtilityFunction/utility.h"

FullMainWindow::FullMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAutoFillBackground(false);
    m_searchAppThread = new SearchAppThread;
    Style::initWidStyle();
    m_buttonStyle = QString("%1{border-radius:24px; background: rgba(255, 255, 255, 0.1);}"
                            "%1:hover {border-radius:24px; background: rgba(255, 255, 255, 0.2);}"
                            "%1:pressed {border-radius:24px; background: rgba(255, 255, 255, 0.3);}");
    this->setFixedSize(Style::m_availableScreenWidth, Style::m_availableScreenHeight);
    centralwidget = new QWidget(this);
    verticalLayout = new QVBoxLayout(centralwidget);
    verticalLayout->setSpacing(0);
    verticalLayout->setContentsMargins(0, 48, 0, 0);
    topHorizontalLayout = new QHBoxLayout();
    topHorizontalLayout->setSpacing(8);
    topHorizontalLayout->setContentsMargins(155, 0, 35, 0);
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    bottomHorizonLayout = new QHBoxLayout();
    bottomHorizonLayout->setSpacing(0);
    bottomHorizonLayout->setContentsMargins(0, 0, 0, 0);
    m_lineEdit = new QLineEdit(centralwidget);
    m_lineEdit->setFixedSize(QSize(372, 36));
    m_lineEdit->setLayoutDirection(Qt::LeftToRight);
    m_lineEdit->installEventFilter(this);
    m_lineEdit->setFrame(false);
    m_queryWid = new QWidget;
    m_queryWid->setParent(m_lineEdit);
    m_queryWid->setFocusPolicy(Qt::NoFocus);
    m_queryWid->setFixedSize(372, 36);
    QHBoxLayout *queryWidLayout = new QHBoxLayout;
    queryWidLayout->setContentsMargins(0, 0, 0, 0);
    queryWidLayout->setSpacing(5);
    m_queryWid->setLayout(queryWidLayout);
    char style[200];
    QPixmap pixmap = loadSvg(QString(":/data/img/mainviewwidget/search.svg"), 16);

    if (QGSettings::isSchemaInstalled(QString("org.ukui.style").toLocal8Bit())) {
        QGSettings *gsetting = new QGSettings("org.ukui.style", QByteArray(), this);

        if (gsetting->keys().contains("styleName")) {
            if (gsetting->get("styleName").toString() == "ukui-light") {
                pixmap = drawSymbolicBlackColoredPixmap(pixmap);
                sprintf(style, "QLineEdit{border:1px solid %s;background-color:%s;border-radius:17px;color:#000000;}",
                        QueryLineEditClickedBorderDefault, QueryLineEditDefaultBackground);
            } else {
                pixmap = drawSymbolicColoredPixmap(pixmap); //反白
                sprintf(style, "QLineEdit{border:1px solid %s;background-color:%s;border-radius:17px;color:#ffffff;}",
                        QueryLineEditClickedBorderDefault, QueryLineEditBackground);
            }
        }

        connect(gsetting, &QGSettings::changed, [ = ](QString key) {
            if ("systemFont" == key || "systemFontSize" == key) {
                m_queryWid->layout()->removeWidget(m_queryText);
                m_queryText->setParent(nullptr);
                resetEditline();
                m_fullCommonPage->repaintWidget();
                m_fullFunctionPage->repaintWidget();
                m_fullLetterPage->repaintWidget();
            }
        });
    }

    m_lineEdit->setStyleSheet(style);
    pixmap.setDevicePixelRatio(qApp->devicePixelRatio());
    m_queryIcon = new QLabel;
    m_queryIcon->setFixedSize(pixmap.size());
    m_queryIcon->setPixmap(pixmap);
    m_queryText = new QLabel;
    m_queryText->setText(tr("Search"));
    m_queryText->adjustSize();
    queryWidLayout->addWidget(m_queryIcon);
    queryWidLayout->addWidget(m_queryText);
    queryWidLayout->setAlignment(Qt::AlignCenter);
    m_lineEdit->setFocusPolicy(Qt::StrongFocus);
    horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    fullSelectToolButton = new QToolButton(centralwidget);
    fullSelectToolButton->setStyleSheet(m_buttonStyle.arg("QToolButton"));
    fullSelectToolButton->setObjectName(QString::fromUtf8("fullSelectToolButton"));
    fullSelectToolButton->setMinimumSize(QSize(48, 48));
    QIcon selectIcon;
    selectIcon.addFile(QString::fromUtf8(":/data/img/mainviewwidget/full-function.svg"), QSize(), QIcon::Normal, QIcon::Off);
    fullSelectToolButton->setIcon(selectIcon);
    fullSelectToolButton->installEventFilter(this);
    fullSelectToolButton->setFocus();
    fullSelectMenuButton = new QToolButton(centralwidget);
    fullSelectMenuButton->setStyleSheet("QToolButton{background: transparent;}");
    fullSelectMenuButton->setFixedSize(20, 20);
    fullSelectMenuButton->setObjectName(QString::fromUtf8("fullSelectMenuButton"));
    QIcon menuBottonIcon;
    menuBottonIcon.addFile(QString::fromUtf8(":/data/img/mainviewwidget/full-drop-down.svg"), QSize(), QIcon::Normal, QIcon::Off);
    fullSelectMenuButton->setIcon(menuBottonIcon);
    fullSelectMenuButton->setPopupMode(QToolButton::InstantPopup);
    fullSelectMenuButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    fullSelectMenuButton->installEventFilter(this);
    minPushButton = new QPushButton(centralwidget);
    minPushButton->setObjectName(QString::fromUtf8("minPushButton"));
    minPushButton->setMinimumSize(QSize(48, 48));
    QIcon minPushButtonIcon;
    minPushButtonIcon.addFile(QString::fromUtf8(":/data/img/mainviewwidget/full-min.svg"), QSize(), QIcon::Normal, QIcon::Off);
    minPushButton->setIcon(minPushButtonIcon);
    minPushButton->setFlat(true);
    minPushButton->installEventFilter(this);
    m_fullStackedWidget = new QStackedWidget(centralwidget);
    m_fullCommonPage = new FullCommonUseWidget(m_fullStackedWidget);
    m_fullStackedWidget->addWidget(m_fullCommonPage);
    m_fullLetterPage = new FullLetterWidget(m_fullStackedWidget);
    m_fullStackedWidget->addWidget(m_fullLetterPage);
    m_fullFunctionPage = new FullFunctionWidget(m_fullStackedWidget);
    m_fullStackedWidget->addWidget(m_fullFunctionPage);
    m_fullResultPage = new FullSearchResultWidget(m_fullStackedWidget);
    m_fullStackedWidget->addWidget(m_fullResultPage);
    bottomHorizonLayout->addWidget(m_fullStackedWidget);
    //    bottomHorizonLayout->addWidget(verticalScrollBar);
    topHorizontalLayout->addItem(horizontalSpacer);
    topHorizontalLayout->addWidget(m_lineEdit);
    topHorizontalLayout->addItem(horizontalSpacer_2);
    topHorizontalLayout->addWidget(fullSelectToolButton);
    topHorizontalLayout->addWidget(fullSelectMenuButton);
    topHorizontalLayout->addWidget(minPushButton);
    verticalLayout->addLayout(topHorizontalLayout);
    verticalLayout->addLayout(bottomHorizonLayout);
    this->setCentralWidget(centralwidget);
    m_fullStackedWidget->setCurrentIndex(0);
    m_menu = new QMenu;
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
    fullSelectMenuButton->setMenu(m_menu);
    setTabOrder(m_lineEdit, fullSelectToolButton);
    setTabOrder(fullSelectToolButton, fullSelectMenuButton);
    setTabOrder(fullSelectMenuButton, minPushButton);
    //    QAction *action = new QAction(this);
    //    action->setIcon(QIcon(":/data/img/mainviewwidget/DM-icon-search.svg"));
    //    lineEdit->addAction(action,QLineEdit::TrailingPosition);
    connect(m_lineEdit, &QLineEdit::textChanged, this, &FullMainWindow::searchAppSlot);
    connect(this, &FullMainWindow::sendSearchKeyword, m_searchAppThread, &SearchAppThread::recvSearchKeyword);
    connect(m_searchAppThread, &SearchAppThread::sendSearchResult, this, &FullMainWindow::recvSearchResult);
    connect(minPushButton, &QPushButton::clicked, this, &FullMainWindow::on_minPushButton_clicked);
    connect(fullSelectToolButton, &QToolButton::clicked, this, &FullMainWindow::on_fullSelectToolButton_clicked);
    connect(fullSelectMenuButton, &QToolButton::triggered, this, &FullMainWindow::on_fullSelectMenuButton_triggered);
    connect(m_fullCommonPage, &FullCommonUseWidget::changeScrollValue, this, &FullMainWindow::setScrollBarValue);
    connect(m_fullFunctionPage, &FullFunctionWidget::changeScrollValue, this, &FullMainWindow::setScrollBarValue);
    connect(m_fullLetterPage, &FullLetterWidget::changeScrollValue, this, &FullMainWindow::setScrollBarValue);
    connect(m_fullFunctionPage, &FullFunctionWidget::setFocusToSideWin, this, &FullMainWindow::setFocusToButton);
    connect(m_fullLetterPage, &FullLetterWidget::setFocusToSideWin, this, &FullMainWindow::setFocusToButton);
    connect(m_fullCommonPage, &FullCommonUseWidget::setFocusToSideWin, this, &FullMainWindow::setFocusToButton);
    connect(m_fullResultPage, &FullSearchResultWidget::setFocusToSideWin, this, &FullMainWindow::setFocusToButton);
    connect(this, &FullMainWindow::sendSetFocusToCom, m_fullCommonPage, &FullCommonUseWidget::selectFirstItemTab);
    connect(this, &FullMainWindow::sendSetFocusToLet, m_fullLetterPage, &FullLetterWidget::setFocusToThis);
    connect(this, &FullMainWindow::sendSetFocusToFun, m_fullFunctionPage, &FullFunctionWidget::setFocusToThis);
    connect(this, &FullMainWindow::sendSetFocusToResult, m_fullResultPage, &FullSearchResultWidget::selectFirstItemTab);
    connect(fullSelectMenuButton, &QToolButton::clicked, this, &FullMainWindow::on_fullSelectMenuButton_clicked);
    connect(m_fullCommonPage, &FullCommonUseWidget::sendUpdateOtherView, this, &FullMainWindow::sendUpdateOtherView);
}

FullMainWindow::~FullMainWindow()
{
}

void FullMainWindow::updateView()
{
    m_fullCommonPage->updateListView();
    m_fullFunctionPage->updateAppListView();
    m_fullLetterPage->updateAppListView();
}

void FullMainWindow::setScrollBarValue(int value, int maximumValue)
{
    //    verticalScrollBar->setMaximum(maximumValue);
    //    verticalScrollBar->setValue(value);
}

void FullMainWindow::itemHide(bool flag)
{
    if (flag) {
        m_fullStackedWidget->hide();
        centralwidget->hide();
    } else {
        centralwidget->show();
        m_fullStackedWidget->show();
    }
}

void FullMainWindow::on_minPushButton_clicked()
{
//    this->hide();
    Q_EMIT showNormalWindow();
}

void FullMainWindow::paintEvent(QPaintEvent *event)
{
    double transparency = getTransparency();
    QPainterPath path;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    painter.setPen(Qt::transparent);
    painter.setBrush(this->palette().base());
    painter.setPen(Qt::transparent);
    painter.setOpacity(transparency);
    painter.drawRect(this->rect());
    path.addRect(this->rect());
    KWindowEffects::enableBlurBehind(this->winId(), true, QRegion(path.toFillPolygon().toPolygon()));
    QMainWindow::paintEvent(event);
}

bool FullMainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_lineEdit) {
        isSearching = true;
        char style[200];

        if (event->type() == QEvent::FocusIn) {
            sprintf(style, "QLineEdit{border:2px solid %s;background-color:%s;border-radius:17px;color:#ffffff;}",
                    QueryLineEditClickedBorder, QueryLineEditClickedBackground);
            m_lineEdit->setStyleSheet(style);

            if (m_lineEdit->text().isEmpty()) {
                qDebug() << "bool FullMainWindow::eventFilter(QObject *watched, QEvent *event)" << m_queryWid->layout()->count();

                if (m_queryWid->layout()->count() == 2) {
                    m_queryWid->layout()->removeWidget(m_queryText);
                    m_queryText->setParent(nullptr);
                }

                m_queryWid->setGeometry(6, 2, m_queryIcon->width() + 5, Style::QueryLineEditHeight);
                m_queryWid->layout()->setAlignment(Qt::AlignVCenter);
                m_lineEdit->setTextMargins(26, 0, 0, 0);
            }
        } else if (event->type() == QEvent::FocusOut && m_lineEdit->text().isEmpty()) {
            sprintf(style, "QLineEdit{border:1px solid %s;background-color:%s;border-radius:17px;color:#ffffff;}",
                    QueryLineEditClickedBorderDefault, QueryLineEditClickedBackground);
            m_lineEdit->setStyleSheet(style);
            resetEditline();
        }

        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *ke = (QKeyEvent *)event;

            if (ke->key() == Qt::Key_Enter || ke->key() == Qt::Key_Return) {
                Q_EMIT sendSetFocusToResult();
            }
        }
    }

    if (watched == minPushButton) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *ke = (QKeyEvent *)event;

            if (ke->key() == Qt::Key_Tab) {
                if (m_fullStackedWidget->currentIndex() == 0) {
                    Q_EMIT sendSetFocusToCom();
                } else if (m_fullStackedWidget->currentIndex() == 1) {
                    Q_EMIT sendSetFocusToLet();
                } else if (m_fullStackedWidget->currentIndex() == 2) {
                    Q_EMIT sendSetFocusToFun();
                } else {
                    Q_EMIT sendSetFocusToResult();
                }

                return true;
            }
        }
    }

    if (watched == minPushButton || watched == fullSelectToolButton || watched == fullSelectMenuButton) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *ke = (QKeyEvent *)event;

            if (ke->key() == Qt::Key_Up || ke->key() == Qt::Key_Down ||
                ke->key() == Qt::Key_Right || ke->key() == Qt::Key_Left) {
                return true;
            }
        }
    }

    return QWidget::eventFilter(watched, event);
}

void FullMainWindow::resetEditline()
{
    m_lineEdit->clear();
    m_lineEdit->clearFocus();
    m_queryWid->layout()->addWidget(m_queryIcon);
    m_queryWid->layout()->addWidget(m_queryText);
    m_queryIcon->adjustSize();
    m_queryText->adjustSize();
    m_queryWid->layout()->setAlignment(Qt::AlignCenter);
    m_queryWid->setFixedSize(372, 36);
}

bool FullMainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::ActivationChange) {
        qDebug() << " * 鼠标点击窗口外部事件";

        if (QApplication::activeWindow() != this) {
            this->hide();
        } /*else {

            fullLetterPage->repaintWidget();
            fullFunctionPage->repaintWidget();
        }*/
    }

    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = (QKeyEvent *) event;

        if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) {
            if (fullSelectToolButton->hasFocus()) {
                fullSelectToolButton->click();
            } else if (fullSelectMenuButton->hasFocus()) {
                fullSelectMenuButton->click();
            }

            if (m_lineEdit->hasFocus()) {
                m_fullResultPage->setFocus();
            } else {
                QWidget *current_focus_widget;
                current_focus_widget = QWidget::focusWidget();
                QPushButton *le = qobject_cast<QPushButton *>(current_focus_widget);

                if (le != nullptr) {
                    le->clicked();
                }
            }
        }

        if (keyEvent->key() == Qt::Key_Escape) {
            this->hide();
        }
    }

    return QWidget::event(event);
}

void FullMainWindow::setFocusToButton()
{
    qDebug() << "设置lineEdit焦点";
    m_lineEdit->setFocus();
}

void FullMainWindow::searchAppSlot(QString arg)
{
    if (!arg.isEmpty()) { //切换至搜索模块
        Q_EMIT sendSearchKeyword(arg);
        m_searchAppThread->start();
        m_fullStackedWidget->setCurrentIndex(3);
    } else {
        m_fullStackedWidget->setCurrentIndex(m_state);
        m_fullFunctionPage->repaintWidget();
        m_fullLetterPage->repaintWidget();
    }
}

void FullMainWindow::mousePressEvent(QMouseEvent *event)
{
    resetEditline();
    return QMainWindow::mousePressEvent(event);
}

void FullMainWindow::recvSearchResult(QVector<QStringList> arg)
{
    m_searchAppThread->quit();
    m_fullResultPage->updateAppListView(arg);
}

void FullMainWindow::on_fullSelectToolButton_clicked()
{
    resetEditline();
    fullSelectToolButton->setFocus();

    if (m_fullStackedWidget->currentIndex() == 0) {
        on_fullSelectMenuButton_triggered(m_letterAction);
    } else if (m_fullStackedWidget->currentIndex() == 1) {
        on_fullSelectMenuButton_triggered(m_funcAction);
    } else if (m_fullStackedWidget->currentIndex() == 2) {
        on_fullSelectMenuButton_triggered(m_allAction);
    }
}

void FullMainWindow::on_fullSelectMenuButton_clicked()
{
    resetEditline();
}

void FullMainWindow::on_fullSelectMenuButton_triggered(QAction *arg1)
{
    if (arg1 == m_allAction) {
        m_fullStackedWidget->setCurrentIndex(0);
        m_fullCommonPage->repaintWidget();
        //fullCommonPage->updateListView();
        m_state = 0;
        fullSelectToolButton->setIcon(QIcon(":/data/img/mainviewwidget/full-all-2x.png"));
        m_allAction->setChecked(true);
        m_letterAction->setChecked(false);
        m_funcAction->setChecked(false);
    } else if (arg1 == m_letterAction) {
        m_fullStackedWidget->setCurrentIndex(1);
        m_fullLetterPage->repaintWidget();
        m_state = 1;
        fullSelectToolButton->setIcon(QIcon(":/data/img/mainviewwidget/full-letter.svg"));
        m_allAction->setChecked(false);
        m_letterAction->setChecked(true);
        m_funcAction->setChecked(false);
    } else if (arg1 == m_funcAction) {
        m_fullStackedWidget->setCurrentIndex(2);
        m_fullFunctionPage->repaintWidget();
        m_state = 2;
        fullSelectToolButton->setIcon(QIcon(":/data/img/mainviewwidget/full-function.svg"));
        m_allAction->setChecked(false);
        m_letterAction->setChecked(false);
        m_funcAction->setChecked(true);
    }
}

void FullMainWindow::repaintWidget()
{
    m_fullCommonPage->repaintWidget();
    m_fullFunctionPage->repaintWidget();
    m_fullLetterPage->repaintWidget();
    m_fullResultPage->repaintWidget();
}
