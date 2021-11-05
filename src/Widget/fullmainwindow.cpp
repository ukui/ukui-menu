#include "fullmainwindow.h"
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
    QRect rect = getScreenAvailableGeometry();
    this->setFixedSize(QSize(rect.width(), rect.height()));
    centralwidget = new QWidget(this);
    verticalLayout = new QVBoxLayout(centralwidget);
    verticalLayout->setSpacing(32);
    verticalLayout->setContentsMargins(0, 48, 0, 0);
    topHorizontalLayout = new QHBoxLayout();
    topHorizontalLayout->setSpacing(8);
    topHorizontalLayout->setContentsMargins(155, 0, 35, 0);
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    bottomHorizonLayout = new QHBoxLayout();
    bottomHorizonLayout->setSpacing(0);
    bottomHorizonLayout->setContentsMargins(0, 0, 0, 0);
    lineEdit = new QLineEdit(centralwidget);
    lineEdit->setFixedSize(QSize(372, 36));
    lineEdit->setLayoutDirection(Qt::LeftToRight);
    lineEdit->installEventFilter(this);
    lineEdit->setStyleSheet(QString::fromUtf8("border-radius: 17px;"));
    lineEdit->setFrame(false);
    m_queryWid = new QWidget;
    m_queryWid->setParent(lineEdit);
    m_queryWid->setFocusPolicy(Qt::NoFocus);
    m_queryWid->setFixedSize(372, 36);
    QHBoxLayout *queryWidLayout = new QHBoxLayout;
    queryWidLayout->setContentsMargins(0, 0, 0, 0);
    queryWidLayout->setSpacing(5);
    m_queryWid->setLayout(queryWidLayout);
    char style[200];
    QPixmap pixmap = loadSvg(QString(":/data/img/mainviewwidget/search.svg"), 16);
    QGSettings gsetting(QString("org.ukui.style").toLocal8Bit());

    if(gsetting.get("style-name").toString() == "ukui-light") {
        pixmap = drawSymbolicBlackColoredPixmap(pixmap);
        sprintf(style, "QLineEdit{border:1px solid %s;background-color:%s;border-radius:4px;color:#000000;}",
                QueryLineEditClickedBorderDefault, QueryLineEditDefaultBackground);
    } else {
        pixmap = drawSymbolicColoredPixmap(pixmap); //反白
        sprintf(style, "QLineEdit{border:1px solid %s;background-color:%s;border-radius:4px;color:#ffffff;}",
                QueryLineEditClickedBorder, QueryLineEditBackground);
    }

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
    lineEdit->setFocusPolicy(Qt::StrongFocus);
    horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    fullSelectToolButton = new QToolButton(centralwidget);
    fullSelectToolButton->setObjectName(QString::fromUtf8("fullSelectToolButton"));
    fullSelectToolButton->setMinimumSize(QSize(48, 48));
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/data/img/mainviewwidget/fullicon-all.svg"), QSize(), QIcon::Normal, QIcon::Off);
    fullSelectToolButton->setIcon(icon);
    fullSelectToolButton->installEventFilter(this);
    fullSelectToolButton->setFocus();
    fullSelectMenuButton = new QToolButton(centralwidget);
    fullSelectMenuButton->setObjectName(QString::fromUtf8("fullSelectMenuButton"));
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/data/img/mainviewwidget/\345\205\250\345\261\217 icon- \344\270\213\346\213\211.svg"), QSize(), QIcon::Normal, QIcon::Off);
    icon1.addFile(QString::fromUtf8(":/data/img/mainviewwidget/\345\205\250\345\261\217 icon- \344\270\213\346\213\211.svg"), QSize(), QIcon::Normal, QIcon::On);
    icon1.addFile(QString::fromUtf8(":/data/img/mainviewwidget/\345\205\250\345\261\217 icon- \344\270\213\346\213\211.png"), QSize(), QIcon::Active, QIcon::On);
    fullSelectMenuButton->setIcon(icon1);
    fullSelectMenuButton->setPopupMode(QToolButton::InstantPopup);
    fullSelectMenuButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    fullSelectMenuButton->installEventFilter(this);
    minPushButton = new QPushButton(centralwidget);
    minPushButton->setObjectName(QString::fromUtf8("minPushButton"));
    minPushButton->setMinimumSize(QSize(48, 48));
    QIcon icon2;
    icon2.addFile(QString::fromUtf8(":/data/img/mainviewwidget/\345\205\250\345\261\217 icon-\347\274\251\345\260\217.png"), QSize(), QIcon::Normal, QIcon::Off);
    minPushButton->setIcon(icon2);
    minPushButton->setFlat(true);
    minPushButton->installEventFilter(this);
    fullStackedWidget = new QStackedWidget(centralwidget);
    fullCommonPage = new FullCommonUseWidget(fullStackedWidget);
    fullStackedWidget->addWidget(fullCommonPage);
    fullLetterPage = new FullLetterWidget(fullStackedWidget);
    fullStackedWidget->addWidget(fullLetterPage);
    fullFunctionPage = new FullFunctionWidget(fullStackedWidget);
    fullStackedWidget->addWidget(fullFunctionPage);
    fullResultPage = new FullSearchResultWidget(fullStackedWidget);
    fullStackedWidget->addWidget(fullResultPage);
    bottomHorizonLayout->addWidget(fullStackedWidget);
    //    bottomHorizonLayout->addWidget(verticalScrollBar);
    topHorizontalLayout->addItem(horizontalSpacer);
    topHorizontalLayout->addWidget(lineEdit);
    topHorizontalLayout->addItem(horizontalSpacer_2);
    topHorizontalLayout->addWidget(fullSelectToolButton);
    topHorizontalLayout->addWidget(fullSelectMenuButton);
    topHorizontalLayout->addWidget(minPushButton);
    verticalLayout->addLayout(topHorizontalLayout);
    verticalLayout->addLayout(bottomHorizonLayout);
    this->setCentralWidget(centralwidget);
    fullStackedWidget->setCurrentIndex(0);
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
    setTabOrder(lineEdit, fullSelectToolButton);
    setTabOrder(fullSelectToolButton, fullSelectMenuButton);
    setTabOrder(fullSelectMenuButton, minPushButton);
    //    QAction *action = new QAction(this);
    //    action->setIcon(QIcon(":/data/img/mainviewwidget/DM-icon-search.svg"));
    //    lineEdit->addAction(action,QLineEdit::TrailingPosition);
    connect(lineEdit, &QLineEdit::textChanged, this, &FullMainWindow::searchAppSlot);
    connect(this, &FullMainWindow::sendSearchKeyword, m_searchAppThread, &SearchAppThread::recvSearchKeyword);
    connect(m_searchAppThread, &SearchAppThread::sendSearchResult, this, &FullMainWindow::recvSearchResult);
    connect(minPushButton, &QPushButton::clicked, this, &FullMainWindow::on_minPushButton_clicked);
    connect(fullSelectToolButton, &QToolButton::clicked, this, &FullMainWindow::on_fullSelectToolButton_clicked);
    connect(fullSelectMenuButton, &QToolButton::triggered, this, &FullMainWindow::on_fullSelectMenuButton_triggered);
    connect(fullCommonPage, &FullCommonUseWidget::changeScrollValue, this, &FullMainWindow::on_setScrollBarValue);
    connect(fullFunctionPage, &FullFunctionWidget::changeScrollValue, this, &FullMainWindow::on_setScrollBarValue);
    connect(fullLetterPage, &FullLetterWidget::changeScrollValue, this, &FullMainWindow::on_setScrollBarValue);
    connect(fullFunctionPage, &FullFunctionWidget::setFocusToSideWin, this, &FullMainWindow::setFocusToButton);
    connect(fullLetterPage, &FullLetterWidget::setFocusToSideWin, this, &FullMainWindow::setFocusToButton);
    connect(fullCommonPage, &FullCommonUseWidget::setFocusToSideWin, this, &FullMainWindow::setFocusToButton);
    connect(fullResultPage, &FullSearchResultWidget::setFocusToSideWin, this, &FullMainWindow::setFocusToButton);
    connect(this, &FullMainWindow::sendSetFocusToCom, fullCommonPage, &FullCommonUseWidget::selectFirstItemTab);
    connect(this, &FullMainWindow::sendSetFocusToLet, fullLetterPage, &FullLetterWidget::setFocusToThis);
    connect(this, &FullMainWindow::sendSetFocusToFun, fullFunctionPage, &FullFunctionWidget::setFocusToThis);
    connect(this, &FullMainWindow::sendSetFocusToResult, fullResultPage, &FullSearchResultWidget::selectFirstItemTab);
    connect(fullSelectMenuButton, &QToolButton::clicked, this, &FullMainWindow::on_fullSelectMenuButton_clicked);
}

FullMainWindow::~FullMainWindow()
{
}

void FullMainWindow::updateView()
{
    fullCommonPage->updateListView();
    fullFunctionPage->updateAppListView();
    fullLetterPage->updateAppListView();
}

void FullMainWindow::on_setScrollBarValue(int value, int maximumValue)
{
    //    verticalScrollBar->setMaximum(maximumValue);
    //    verticalScrollBar->setValue(value);
}

void FullMainWindow::on_minPushButton_clicked()
{
    this->hide();
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
    if(watched == lineEdit) {
        isSearching = true;
        char style[200];

        if(event->type() == QEvent::FocusIn) {
            sprintf(style, "QLineEdit{border:1px solid %s;background-color:%s;border-radius:4px;color:#ffffff;}",
                    QueryLineEditClickedBorder, QueryLineEditClickedBackground);

            if(lineEdit->text().isEmpty()) {
                qDebug() << "bool FullMainWindow::eventFilter(QObject *watched, QEvent *event)" << m_queryWid->layout()->count();

                if(m_queryWid->layout()->count() == 2) {
                    m_queryWid->layout()->removeWidget(m_queryText);
                    m_queryText->setParent(nullptr);
                }

                m_queryWid->setGeometry(6, 2, m_queryIcon->width() + 5, Style::QueryLineEditHeight);
                m_queryWid->layout()->setAlignment(Qt::AlignVCenter);
                lineEdit->setTextMargins(26, 0, 0, 0);
            }
        } else if(event->type() == QEvent::FocusOut && lineEdit->text().isEmpty()) {
            resetEditline();
        }

        if( event->type() == QEvent::KeyPress ) {
            QKeyEvent *ke = (QKeyEvent *)event;

            if( ke->key() == Qt::Key_Enter || ke->key() == Qt::Key_Return) {
                Q_EMIT sendSetFocusToResult();
            }
        }
    }

    if(watched == minPushButton) {
        if( event->type() == QEvent::KeyPress ) {
            QKeyEvent *ke = (QKeyEvent *)event;

            if( ke->key() == Qt::Key_Tab) {
                if(fullStackedWidget->currentIndex() == 0) {
                    Q_EMIT sendSetFocusToCom();
                } else if(fullStackedWidget->currentIndex() == 1) {
                    Q_EMIT sendSetFocusToLet();
                } else if(fullStackedWidget->currentIndex() == 2) {
                    Q_EMIT sendSetFocusToFun();
                } else {
                    Q_EMIT sendSetFocusToResult();
                }

                return true;
            }
        }
    }

    if(watched == minPushButton || watched == fullSelectToolButton || watched == fullSelectMenuButton) {
        if( event->type() == QEvent::KeyPress ) {
            QKeyEvent *ke = (QKeyEvent *)event;

            if(ke->key() == Qt::Key_Up || ke->key() == Qt::Key_Down ||
               ke->key() == Qt::Key_Right || ke->key() == Qt::Key_Left) {
                return true;
            }
        }
    }

    return QWidget::eventFilter(watched, event);
}

void FullMainWindow::resetEditline()
{
    lineEdit->clear();
    lineEdit->clearFocus();
    m_queryWid->layout()->addWidget(m_queryIcon);
    m_queryWid->layout()->addWidget(m_queryText);
    m_queryIcon->adjustSize();
    m_queryText->adjustSize();
    m_queryWid->layout()->setAlignment(Qt::AlignCenter);
    m_queryWid->setFixedSize(372, 36);
}

bool FullMainWindow::event ( QEvent *event )
{
    if (event->type() == QEvent::ActivationChange) {
        qDebug() << " * 鼠标点击窗口外部事件";

        if(QApplication::activeWindow() != this) {
            this->hide();
        } else {
            fullLetterPage->repaintWidget();
            fullFunctionPage->repaintWidget();
        }
    }

    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = (QKeyEvent *) event;

        if(keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) {
            if(fullSelectToolButton->hasFocus()) {
                fullSelectToolButton->click();
            } else if(fullSelectMenuButton->hasFocus()) {
                fullSelectMenuButton->click();
            }

            if(lineEdit->hasFocus()) {
                fullResultPage->setFocus();
            } else {
                QWidget *current_focus_widget;
                current_focus_widget = QWidget::focusWidget();
                QPushButton *le = qobject_cast<QPushButton *>(current_focus_widget);

                if(le != nullptr) {
                    le->clicked();
                }
            }
        }
    }

    return QWidget::event(event);
}

void FullMainWindow::setFocusToButton()
{
    qDebug() << "设置lineEdit焦点";
    lineEdit->setFocus();
}

void FullMainWindow::searchAppSlot(QString arg)
{
    if(!arg.isEmpty()) { //切换至搜索模块
        Q_EMIT sendSearchKeyword(arg);
        m_searchAppThread->start();
        fullStackedWidget->setCurrentIndex(3);
    } else {
        fullStackedWidget->setCurrentIndex(m_state);
        fullFunctionPage->repaintWidget();
        fullLetterPage->repaintWidget();
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
    fullResultPage->updateAppListView(arg);
}

void FullMainWindow::on_fullSelectToolButton_clicked()
{
    resetEditline();
    fullSelectToolButton->setFocus();

    if(fullStackedWidget->currentIndex() == 0) {
        on_fullSelectMenuButton_triggered(m_letterAction);
    } else if(fullStackedWidget->currentIndex() == 1) {
        on_fullSelectMenuButton_triggered(m_funcAction);
    } else if(fullStackedWidget->currentIndex() == 2) {
        on_fullSelectMenuButton_triggered(m_allAction);
    }
}

void FullMainWindow::on_fullSelectMenuButton_clicked()
{
    resetEditline();
}

void FullMainWindow::on_fullSelectMenuButton_triggered(QAction *arg1)
{
    if(arg1 == m_allAction) {
        fullStackedWidget->setCurrentIndex(0);
        fullCommonPage->repaintWidget();
        m_state = 0;
        fullSelectToolButton->setIcon(QIcon(":/data/img/mainviewwidget/fullicon-all.svg"));
        m_allAction->setChecked(true);
        m_letterAction->setChecked(false);
        m_funcAction->setChecked(false);
    } else if(arg1 == m_letterAction) {
        fullStackedWidget->setCurrentIndex(1);
        fullLetterPage->repaintWidget();
        m_state = 1;
        fullSelectToolButton->setIcon(QIcon(":/data/img/mainviewwidget/全屏 icon-字母排序.svg"));
        m_allAction->setChecked(false);
        m_letterAction->setChecked(true);
        m_funcAction->setChecked(false);
    } else if(arg1 == m_funcAction) {
        fullStackedWidget->setCurrentIndex(2);
        fullFunctionPage->repaintWidget();
        m_state = 2;
        fullSelectToolButton->setIcon(QIcon(":/data/img/mainviewwidget/全屏 icon-功能排序.svg"));
        m_allAction->setChecked(false);
        m_letterAction->setChecked(false);
        m_funcAction->setChecked(true);
    }
}
