#include "attributedialog.h"
#include "ui_attributedialog.h"
#include <QDebug>

AttributeDialog::AttributeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AttributeDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint|Qt::Dialog);
//    this->setAttribute(Qt::WA_DeleteOnClose);
    init_widget();
}

AttributeDialog::~AttributeDialog()
{
    delete ui;
    delete pUkuiMenuInterface;
}

void AttributeDialog::init_widget()
{
    this->resize(436,334);
    this->setStyleSheet("background-color:#f5f6f7;border:1px solid #2a6b9f;");
    mainLayout=new QVBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    this->setLayout(mainLayout);

    //标题栏
    titlebarWid=new QWidget(this);
    titlebarWid->setStyleSheet("border:0px;background:transparent;");
    titlebarWid->setFixedSize(this->width(),30);
    titlebarwidLayout=new QHBoxLayout;
    titlebarwidLayout->setContentsMargins(0,0,0,0);
    titlebarwidLayout->setSpacing(0);
    leftSpacer=new QSpacerItem(40,20,QSizePolicy::Fixed,QSizePolicy::Fixed);
    appnameLabel=new QLabel(titlebarWid);
    appnameLabel->setStyleSheet("color:#000000;font-size:14px;");
    titlebarLabel=new QLabel(titlebarWid);
    titlebarLabel->setText(" "+tr("属性"));
    titlebarLabel->setStyleSheet("color:#000000;font-size:14px;");
    titlebarLabel->adjustSize();
//    minbtn=new ToolButton(QString(":/data/img/attributedialog/min-black.svg"),QString(":/data/img/attributedialog/min-black.svg"),
//                          QString("#1a000000"),QString("#33000000"));
    closebtn=new ToolButton(45,30,QString(":/data/img/attributedialog/close-black.svg"),QString(":/data/img/attributedialog/close-white.svg"),
                            ATTRIBUTEDIALOGHOVER,ATTRIBUTEDIALOGPRESSED,3);
//    minbtn->setFixedSize(45,30);
    closebtn->setFixedSize(45,30);
//    connect(minbtn,SIGNAL(clicked()),this,SLOT(showMinimized()));
    connect(closebtn,SIGNAL(clicked()),this,SLOT(close()));

    rightSpacer=new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);
    titlebarwidLayout->addItem(leftSpacer);
    titlebarwidLayout->addWidget(appnameLabel);
    titlebarwidLayout->addWidget(titlebarLabel);
    titlebarwidLayout->addItem(rightSpacer);
//    titlebarwidLayout->addWidget(minbtn);
    titlebarwidLayout->addWidget(closebtn);
    titlebarWid->setLayout(titlebarwidLayout);

    //上部控件
    upWid=new QWidget(this);
    upWid->setStyleSheet("border:0px;background:transparent;");
    upWid->setFixedSize(this->width(),90);
    upwidLayout=new QHBoxLayout;
    upwidLayout->setContentsMargins(22,22,0,20);
    upwidLayout->setSpacing(22);
    labelIcon=new QLabel(upWid);
    labelIcon->setFixedSize(48,48);
    labelappName=new QLabel(upWid);
    labelappName->setStyleSheet("color:#000000;font-size:14px;");
    upwidLayout->addWidget(labelIcon);
    upwidLayout->addWidget(labelappName);
    upWid->setLayout(upwidLayout);

    //分割线
    line=new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFixedSize(this->width(),1);
    line->setStyleSheet("border:0px;background-color:#cccccc");

    //下部控件
    downWid=new QWidget(this);
    downWid->setStyleSheet("border:0px;background:transparent;");
    downWid->setFixedSize(this->width(),this->height()-titlebarWid->height()-upWid->height()-1-50);
    downwidLayout=new QVBoxLayout;
    downwidLayout->setContentsMargins(22,20,10,0);
//    downwidLayout->setSpacing(32);
    downWid->setLayout(downwidLayout);

    tableWid=new QTableWidget;
    tableWid->setFixedSize(downWid->width()-32,downWid->height()-20);
    QStringList header;
    header.append("");
    header.append("");
    tableWid->setHorizontalHeaderLabels(header);
    tableWid->setRowCount(3);
    tableWid->setColumnCount(2);
//    tableWid->verticalHeader()->setDefaultSectionSize(tableWid->height()/3);
//    tableWid->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    qDebug()<<tableWid->height()<<tableWid->height()/3<<tableWid->verticalHeader()->defaultSectionSize();
    tableWid->setRowHeight(0,32);
    tableWid->setRowHeight(1,32);
    tableWid->setRowHeight(2,tableWid->height()-64);
    tableWid->setColumnWidth(0,70);
    tableWid->setColumnWidth(1,tableWid->width()-70);
    tableWid->verticalHeader()->setHidden(true);
    tableWid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tableWid->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tableWid->setFocusPolicy(Qt::NoFocus);
    tableWid->horizontalHeader()->setFixedHeight(0);
    tableWid->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWid->setSelectionMode(QAbstractItemView::NoSelection);
    tableWid->setShowGrid(false);
    tableWid->verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
    tableWid->setStyleSheet("QTableWidget{border:0px;background:transparent;}"
                            "QTableWidget:Item{color:#000000;font-size:12px;}");
    typeEdit=new QTextEdit;
    execEdit=new QTextEdit;
    commentEdit=new QTextEdit;
    typevalueEdit=new QTextEdit;
    execvalueEdit=new  QTextEdit;
    commentvalueEdit=new QTextEdit;
    set_controls_style(typeEdit,typevalueEdit,"类型",32);
    set_controls_style(execEdit,execvalueEdit,"命令",32);
    set_controls_style(commentEdit,commentvalueEdit,"备注",tableWid->height()-64);
    tableWid->setCellWidget(0,0,typeEdit);
    tableWid->setCellWidget(1,0,execEdit);
    tableWid->setCellWidget(2,0,commentEdit);
    tableWid->setCellWidget(0,1,typevalueEdit);
    tableWid->setCellWidget(1,1,execvalueEdit);
    tableWid->setCellWidget(2,1,commentvalueEdit);
    downwidLayout->addWidget(tableWid);

    //关闭按钮
    closeWid=new QWidget(this);
    closeWid->setFixedSize(this->width(),50);
    closeWid->setStyleSheet("border:0px;background:transparent;");
    closewidLayout=new QHBoxLayout;
    closewidLayout->setContentsMargins(0,0,20,20);
    btnleftSpacer=new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);
    btn=new QPushButton(closeWid);
    btn->setFixedSize(90,30);
    btn->setText(tr("关闭"));
    btn->setFocusPolicy(Qt::NoFocus);
    closewidLayout->addItem(btnleftSpacer);
    closewidLayout->addWidget(btn);
    closeWid->setLayout(closewidLayout);
    btn->setStyleSheet("QPushButton{background:#fbfbfb;border:1px solid #9e9e9e;font-size:14px;color:#000000;}\
                        QPushButton:hover{background:#fbfbfb;border:2px solid #629ee8;font-size:14px;color:#000000;}\
                        QPushButton:pressed{background:#e2e2e2;border:1px solid #9e9e9e;font-size:14px;color:#000000;}");
    connect(btn,SIGNAL(clicked(bool)),this,SLOT(close()));

    mainLayout->addWidget(titlebarWid);
    mainLayout->addWidget(upWid);
    mainLayout->addWidget(line);
    mainLayout->addWidget(downWid);
    mainLayout->addWidget(closeWid);

    pUkuiMenuInterface=new UkuiMenuInterface;

}

void AttributeDialog::set_controls_style(QTextEdit *edit, QTextEdit *valueEdit, QString str, int height)
{
    QByteArray byte=str.toLocal8Bit();
    char* arg=byte.data();
    edit->setText(tr(arg)+":");
    edit->setStyleSheet("background:transparent;color:#000000;font-size:14px;");
    edit->setFixedSize(70,height);
    edit->setAlignment(Qt::AlignLeft);
    edit->setEnabled(false);
    edit->setContextMenuPolicy(Qt::NoContextMenu);

    valueEdit->setFixedSize(tableWid->width()-70,height);
    valueEdit->setStyleSheet("background:transparent;color:#000000;font-size:14px;");
    valueEdit->setContextMenuPolicy(Qt::NoContextMenu);
    valueEdit->setReadOnly(true);
//    valueEdit->setAcceptDrops(true);
    valueEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}

void AttributeDialog::recv_attribute_value(QString appname)
{
    QString desktopfp=pUkuiMenuInterface->get_desktop_path_by_app_name(appname);
    appnameLabel->setText(appname);
    appnameLabel->adjustSize();
//    titlebarwidLayout->setContentsMargins((titlebarWid->width()-appnameLabel->width()-titlebarLabel->width())/2,0,0,0);
    leftSpacer->changeSize((titlebarWid->width()-appnameLabel->width()-titlebarLabel->width())/2,20,QSizePolicy::Fixed,QSizePolicy::Fixed);
    QString iconstr=pUkuiMenuInterface->get_app_icon(desktopfp);
    QIcon::setThemeName("ukui-icon-theme");
    QIcon icon=QIcon::fromTheme(iconstr);
    QPixmap pixmapicon(icon.pixmap(QSize(48,48)));
    labelIcon->setPixmap(pixmapicon);
    this->labelappName->setText(appname);
    QString type=pUkuiMenuInterface->get_app_type(desktopfp);
    QString exec=pUkuiMenuInterface->get_app_exec(desktopfp);
    QString comment=pUkuiMenuInterface->get_app_comment(desktopfp);
    typevalueEdit->setText(type);
    execvalueEdit->setText(exec);
    commentvalueEdit->setText(comment);
//    typevalueEdit->setAlignment(Qt::AlignTop);
//    execvalueEdit->setAlignment(Qt::AlignTop);
//    commentvalueEdit->setAlignment(Qt::AlignTop);
//    typevalueLabel->setWordWrap(true);
//    typevalueEdit->adjustSize();
//    typevalueEdit->setAlignment(Qt::AlignTop);
//    execvalueLabel->setWordWrap(true);
//    execvalueLabel->adjustSize();
//    commentvalueLabel->setWordWrap(true);
//    commentvalueLabel->adjustSize();
//    commentvalueLabel->setAlignment(Qt::AlignTop);

}
