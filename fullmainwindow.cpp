#include "fullmainwindow.h"
#include "ui_fullmainwindow.h"
#include <KWindowEffects>

FullMainWindow::FullMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FullMainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAutoFillBackground(false);

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
    ui->fullSelectMenuButton->setMenu(m_menu);
    ui->fullFunctionPage->setFunctionBtnGeometry();
    ui->fullLetterPage->setLetterBtnGeometry();
}

FullMainWindow::~FullMainWindow()
{
    delete ui;
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
    if(ui->fullStackedWidget->currentIndex() == 0)
    {
        on_fullSelectMenuButton_triggered(m_letterAction);
    }
    else if(ui->fullStackedWidget->currentIndex() == 1)
    {
        on_fullSelectMenuButton_triggered(m_funcAction);
    }
    else if(ui->fullStackedWidget->currentIndex() == 2)
    {
        on_fullSelectMenuButton_triggered(m_allAction);
    }
}

void FullMainWindow::on_fullSelectMenuButton_triggered(QAction *arg1)
{
    qDebug() << "void MainWindow::on_selectMenuButton_triggered(QAction *arg1)" << m_allAction << arg1;
    if(arg1 == m_allAction)
    {
        ui->fullStackedWidget->setCurrentIndex(0);
//        m_state = 0;
        ui->fullSelectToolButton->setIcon(QIcon(":/data/img/mainviewwidget/fullicon-all.svg"));
        m_allAction->setChecked(true);
        m_letterAction->setChecked(false);
        m_funcAction->setChecked(false);
    }
    else if(arg1 == m_letterAction)
    {
        ui->fullStackedWidget->setCurrentIndex(1);
//        m_state = 1;
        ui->fullSelectToolButton->setIcon(QIcon(":/data/img/mainviewwidget/全屏 icon-字母排序.svg"));
        m_allAction->setChecked(false);
        m_letterAction->setChecked(true);
        m_funcAction->setChecked(false);
    }
    else if(arg1 == m_funcAction)
    {
        ui->fullStackedWidget->setCurrentIndex(2);
        ui->fullSelectToolButton->setIcon(QIcon(":/data/img/mainviewwidget/全屏 icon-功能排序.svg"));
        m_allAction->setChecked(false);
        m_letterAction->setChecked(false);
        m_funcAction->setChecked(true);
    }
}
