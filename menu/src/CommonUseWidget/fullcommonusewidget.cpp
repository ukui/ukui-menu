#include "fullcommonusewidget.h"
#include "ui_fullcommonusewidget.h"
#include <QDesktopWidget>
#include "src/color.h"

FullCommonUseWidget::FullCommonUseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FullCommonUseWidget)
{
    ui->setupUi(this);
    init_widget();
}

FullCommonUseWidget::~FullCommonUseWidget()
{
    delete ui;
}

void FullCommonUseWidget::init_widget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    char widgetcolor[100];
    sprintf(widgetcolor, "border:0px;background-color:%s;",MAINVIEWWIDGETCOLOR);
    this->setStyleSheet(QString::fromLocal8Bit(widgetcolor));
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setFixedSize(QApplication::desktop()->availableGeometry().width()-160,
                       QApplication::desktop()->availableGeometry().height());
}
