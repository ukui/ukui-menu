#include "fullsearchresultwidget.h"
#include "ui_fullsearchresultwidget.h"
#include "src/color.h"
#include <QDebug>

FullSearchResultWidget::FullSearchResultWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FullSearchResultWidget)
{
    ui->setupUi(this);
    init_widget();
}

FullSearchResultWidget::~FullSearchResultWidget()
{
    delete ui;
}

void FullSearchResultWidget::init_widget()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground,true);
    char widgetcolor[100];
    sprintf(widgetcolor, "border:0px;background-color:%s;",MAINVIEWWIDGETCOLOR);
    this->setStyleSheet(QString::fromLocal8Bit(widgetcolor));
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setFixedSize(QApplication::desktop()->availableGeometry().width()-160,
                                  QApplication::desktop()->availableGeometry().height()-70);

    mainLayout=new QHBoxLayout;
    mainLayout->setContentsMargins(30,0,2,0);
    searchResultTableWid=new QTableWidget;
    mainLayout->addWidget(searchResultTableWid);
    this->setLayout(mainLayout);

    init_search_result_table();
}

void FullSearchResultWidget::init_search_result_table()
{
    QStringList header;
    for(int i=0;i<4;i++)
        header.append("");
    searchResultTableWid->setHorizontalHeaderLabels(header);
    searchResultTableWid->setFixedSize(this->width()-32,this->height());
    searchResultTableWid->setColumnCount(4);
    searchResultTableWid->verticalHeader()->setDefaultSectionSize(50);
    searchResultTableWid->verticalHeader()->setHidden(true);
    searchResultTableWid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    searchResultTableWid->setFocusPolicy(Qt::NoFocus);
    searchResultTableWid->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    searchResultTableWid->horizontalHeader()->setFixedHeight(0);
    searchResultTableWid->setEditTriggers(QAbstractItemView::NoEditTriggers);
    searchResultTableWid->setSelectionMode(QAbstractItemView::SingleSelection);
    searchResultTableWid->setSelectionBehavior(QAbstractItemView::SelectColumns);
    searchResultTableWid->setShowGrid(false);
    searchResultTableWid->setStyleSheet("QTableWidget{border:0px;background:transparent;}"
                                    "QTableWidget::Item{background:transparent;;padding-left:0px;}\
                                     QTableWidget::Item:selected{background-color:#0e1316;}");
    searchResultTableWid->verticalScrollBar()->setStyleSheet(
                                            "QScrollBar{width:12px;padding-top:15px;padding-bottom:15px;background-color:#283138;}"
                                            "QScrollBar::handle{background-color:#414e59; width:12px;}"
                                            "QScrollBar::handle:hover{background-color:#697883; }"
                                            "QScrollBar::handle:pressed{background-color:#8897a3;}"
                                            "QScrollBar::sub-line{background-color:#283138;height:15px;width:12px;border-image:url(:/data/img/mainviewwidget/滑动条-向上箭头.svg);subcontrol-position:top;}"
                                            "QScrollBar::sub-line:hover{background-color:#697883;}"
                                            "QScrollBar::sub-line:pressed{background-color:#8897a3;border-image:url(:/data/img/mainviewwidget/滑动条-向上箭头-点击.svg);}"
                                            "QScrollBar::add-line{background-color:#283138;height:15px;width:12px;border-image:url(:/data/img/mainviewwidget/滑动条-向下箭头.svg);subcontrol-position:bottom;}"
                                            "QScrollBar::add-line:hover{background-color:#697883;}"
                                            "QScrollBar::add-line:pressed{background-color:#8897a3;border-image:url(:/data/img/mainviewwidget/滑动条-向下箭头-点击.svg);}"
                                            );

}

void FullSearchResultWidget::fill_search_result_table()
{
    if(!searchResultList.isEmpty())
    {
        int count=searchResultList.count()%4;
        int row=0;
        if(count==0)
            row=searchResultList.count()/4;
        else{
            row=searchResultList.count()/4+1;
        }
        searchResultTableWid->setRowCount(row);

        for(int r=0;r<row;r++)
        {
            for(int c=0;r*4+c<searchResultList.count();c++)
            {
                QString style="QToolButton{background:transparent;border:0px;}\
                               QToolButton:hover{background-color:#414e59;}\
                               QToolButton:pressed{background-color:#2a343e;}";
                int width=(searchResultTableWid->width()-12*3)/4;
                QToolButton* btn=new QToolButton;
                btn->setFixedSize(width,50);
                btn->setStyleSheet(style);
                QHBoxLayout* layout=new QHBoxLayout;
                layout->setContentsMargins(10,0,0,0);
                layout->setSpacing(15);
                QLabel* labelicon=new QLabel;
                labelicon->setFixedSize(32,32);
                labelicon->setStyleSheet("QLabel{background:transparent;}");
                QLabel* labeltext=new QLabel;
                labeltext->setText(searchResultList.at(r*4+c));
                labeltext->adjustSize();
                labeltext->setStyleSheet("QLabel{background:transparent;color:#ffffff;font-size:16px;}");
                layout->addWidget(labelicon);
                layout->addWidget(labeltext);
                btn->setLayout(layout);
                btn->setFocusPolicy(Qt::NoFocus);
                searchResultTableWid->setCellWidget(r,c,btn);
            }
        }
    }
    else{
        searchResultTableWid->clear();
        searchResultTableWid->setRowCount(0);
    }
}

void FullSearchResultWidget::recv_search_keyword(QString arg)
{
    searchResultList.clear();
    //搜索符合arg关键字的程序名
    if(!arg.isEmpty())
    {
        for(int i=0;i<10;i++)
            searchResultList.append(arg);
    }
    else{
        searchResultList.clear();
    }

    fill_search_result_table();

}
