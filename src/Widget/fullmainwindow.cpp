#include "fullmainwindow.h"
#include <KWindowEffects>

FullMainWindow::FullMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAutoFillBackground(false);

    Style::initWidStyle();
    this->resize(1920, 1024);

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
    lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
    lineEdit->setMinimumSize(QSize(372, 36));
    lineEdit->setMaximumSize(QSize(372, 36));
    lineEdit->setLayoutDirection(Qt::LeftToRight);

    horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    fullSelectToolButton = new QToolButton(centralwidget);
    fullSelectToolButton->setObjectName(QString::fromUtf8("fullSelectToolButton"));
    fullSelectToolButton->setMinimumSize(QSize(48, 48));
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/data/img/mainviewwidget/\345\205\250\345\261\217 icon-\345\212\237\350\203\275\346\216\222\345\272\217.png"), QSize(), QIcon::Normal, QIcon::Off);
    fullSelectToolButton->setIcon(icon);

    fullSelectMenuButton = new QToolButton(centralwidget);
    fullSelectMenuButton->setObjectName(QString::fromUtf8("fullSelectMenuButton"));
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/data/img/mainviewwidget/\345\205\250\345\261\217 icon- \344\270\213\346\213\211.svg"), QSize(), QIcon::Normal, QIcon::Off);
    icon1.addFile(QString::fromUtf8(":/data/img/mainviewwidget/\345\205\250\345\261\217 icon- \344\270\213\346\213\211.svg"), QSize(), QIcon::Normal, QIcon::On);
    icon1.addFile(QString::fromUtf8(":/data/img/mainviewwidget/\345\205\250\345\261\217 icon- \344\270\213\346\213\211.png"), QSize(), QIcon::Active, QIcon::On);
    fullSelectMenuButton->setIcon(icon1);
    fullSelectMenuButton->setPopupMode(QToolButton::InstantPopup);
    fullSelectMenuButton->setToolButtonStyle(Qt::ToolButtonIconOnly);

    minPushButton = new QPushButton(centralwidget);
    minPushButton->setObjectName(QString::fromUtf8("minPushButton"));
    minPushButton->setMinimumSize(QSize(48, 48));
    QIcon icon2;
    icon2.addFile(QString::fromUtf8(":/data/img/mainviewwidget/\345\205\250\345\261\217 icon-\347\274\251\345\260\217.png"), QSize(), QIcon::Normal, QIcon::Off);
    minPushButton->setIcon(icon2);
    minPushButton->setFlat(true);

    fullStackedWidget = new QStackedWidget(centralwidget);
    fullStackedWidget->setFixedSize(1900,910);
    fullCommonPage = new FullCommonUseWidget();
    fullStackedWidget->addWidget(fullCommonPage);
    fullLetterPage = new FullLetterWidget();
    fullStackedWidget->addWidget(fullLetterPage);
    fullFunctionPage = new FullFunctionWidget();
    fullStackedWidget->addWidget(fullFunctionPage);



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

    connect(minPushButton,&QPushButton::clicked,this,&FullMainWindow::on_minPushButton_clicked);
    connect(fullSelectToolButton,&QToolButton::clicked,this,&FullMainWindow::on_fullSelectToolButton_clicked);
    connect(fullSelectMenuButton,&QToolButton::triggered,this,&FullMainWindow::on_fullSelectMenuButton_triggered);
    connect(fullCommonPage,&FullCommonUseWidget::changeScrollValue,this,&FullMainWindow::on_setScrollBarValue);
    connect(fullFunctionPage,&FullFunctionWidget::changeScrollValue,this,&FullMainWindow::on_setScrollBarValue);
    connect(fullLetterPage,&FullLetterWidget::changeScrollValue,this,&FullMainWindow::on_setScrollBarValue);
//    connect(verticalScrollBar,&QScrollBar::valueChanged,fullCommonPage,&FullCommonUseWidget::on_setScrollBarValue);
//    connect(verticalScrollBar,&QScrollBar::valueChanged,fullFunctionPage,&FullFunctionWidget::on_setScrollBarValue);
//    connect(verticalScrollBar,&QScrollBar::valueChanged,fullLetterPage,&FullLetterWidget::on_setScrollBarValue);
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
    double transparency=getTransparency();
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

bool FullMainWindow::event ( QEvent * event )
{
   if (event->type() == QEvent::ActivationChange)
   {
       qDebug() << " * 鼠标点击窗口外部事件";
        if(QApplication::activeWindow() != this)
        {
            this->hide();
        }
   }
   return QWidget::event(event);
}

void FullMainWindow::on_fullSelectToolButton_clicked()
{
    if(fullStackedWidget->currentIndex() == 0)
    {
        on_fullSelectMenuButton_triggered(m_letterAction);
    }
    else if(fullStackedWidget->currentIndex() == 1)
    {
        on_fullSelectMenuButton_triggered(m_funcAction);
    }
    else if(fullStackedWidget->currentIndex() == 2)
    {
        on_fullSelectMenuButton_triggered(m_allAction);
    }
}

void FullMainWindow::on_fullSelectMenuButton_triggered(QAction *arg1)
{
    qDebug() << "void MainWindow::on_selectMenuButton_triggered(QAction *arg1)" << m_allAction << arg1;
    if(arg1 == m_allAction)
    {
        fullStackedWidget->setCurrentIndex(0);
        fullCommonPage->repaintWidget();
//        m_state = 0;
        fullSelectToolButton->setIcon(QIcon(":/data/img/mainviewwidget/fullicon-all.svg"));
        m_allAction->setChecked(true);
        m_letterAction->setChecked(false);
        m_funcAction->setChecked(false);
    }
    else if(arg1 == m_letterAction)
    {
        fullStackedWidget->setCurrentIndex(1);
        fullLetterPage->repaintWidget();
//        m_state = 1;
        fullSelectToolButton->setIcon(QIcon(":/data/img/mainviewwidget/全屏 icon-字母排序.svg"));
        m_allAction->setChecked(false);
        m_letterAction->setChecked(true);
        m_funcAction->setChecked(false);
    }
    else if(arg1 == m_funcAction)
    {
        fullStackedWidget->setCurrentIndex(2);
        fullFunctionPage->repaintWidget();
        fullSelectToolButton->setIcon(QIcon(":/data/img/mainviewwidget/全屏 icon-功能排序.svg"));
        m_allAction->setChecked(false);
        m_letterAction->setChecked(false);
        m_funcAction->setChecked(true);
    }
}
