#include "fullcommonusewidget.h"
#include <QDesktopWidget>
#include "src/Style/style.h"
#include <QDebug>
#include "src/Interface/ukuimenuinterface.h"
#include "src/UtilityFunction/fulllistview.h"


FullCommonUseWidget::FullCommonUseWidget(QWidget *parent) : QWidget(parent)
{
    QString path=QDir::homePath()+"/.config/ukui/ukui-menu-panel.ini";
    setting=new QSettings(path,QSettings::IniFormat);
    initUi();
}

QVector<QString> FullCommonUseWidget::keyVector=QVector<QString>();
QVector<int> FullCommonUseWidget::keyValueVector=QVector<int>();

void FullCommonUseWidget::initUi()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAutoFillBackground(false);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setStyleSheet("border:1px solid #ff0000;background:transparent;");
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setFixedSize(1409,QApplication::primaryScreen()->geometry().height()-46);

    QHBoxLayout* mainLayout=new QHBoxLayout;
    mainLayout->setContentsMargins(105,96,0,0);
    this->setLayout(mainLayout);
    m_spaceItem=new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);
    mainLayout->addItem(m_spaceItem);

    m_ukuiMenuInterface=new UkuiMenuInterface;

    initAppListWidget();
//   fillAppList();
}

void FullCommonUseWidget::initAppListWidget()
{
    m_listView=new FullListView(this,0);
    m_listView->setFixedSize(this->width()-105,this->height()-96);

    QHBoxLayout *mainLayout=qobject_cast<QHBoxLayout*>(this->layout());
    mainLayout->insertWidget(1,m_listView);
    connect(m_listView,&FullListView::sendItemClickedSignal,this,&FullCommonUseWidget::execApplication);
   // connect(m_listView,&FullListView::sendUpdateAppListSignal,this,&FullCommonUseWidget::updateListViewSlot);


}

void FullCommonUseWidget::fillAppList()
{
    m_data.clear();
    keyVector.clear();
    keyValueVector.clear();
    if(!setting->childGroups().contains("position"))
    {
        Q_FOREACH(QString desktopfp,UkuiMenuInterface::allAppVector)
            m_data.append(desktopfp);
        m_listView->addData(m_data);

        setting->beginGroup("position");
        for (int index = 0; index < m_data.size(); index++) {

    //        QString data_info = m_data.at(index).split("\/").last();

            QFileInfo fileInfo(m_data.at(index));
            QString desktopfn=fileInfo.fileName();
            setting->setValue(desktopfn,index);
            setting->sync();
        }

        setting->endGroup();
    }
    else
    {
        setting->beginGroup("position");
        QStringList keyList=setting->childKeys();
        Q_FOREACH(QString desktopfn,keyList)
        {
            keyVector.append(desktopfn);
            keyValueVector.append(setting->value(desktopfn).toInt());
        }
        setting->endGroup();
        qSort(keyList.begin(),keyList.end(),cmpApp);

        Q_FOREACH(QString desktopfn,keyList)
            m_data.append("/usr/share/applications/"+desktopfn);
        m_listView->addData(m_data);

    }


}

bool FullCommonUseWidget::cmpApp(QString &arg_1, QString &arg_2)
{
    if(keyValueVector.at(keyVector.indexOf(arg_1)) < keyValueVector.at(keyVector.indexOf(arg_2)))
        return true;
    else
        return false;
}


/**
 * 执行应用程序
 */
void FullCommonUseWidget::execApplication(QString desktopfp)
{
    Q_EMIT sendHideMainWindowSignal();
    GDesktopAppInfo * desktopAppInfo=g_desktop_app_info_new_from_filename(desktopfp.toLocal8Bit().data());
    g_app_info_launch(G_APP_INFO(desktopAppInfo),nullptr, nullptr, nullptr);
    g_object_unref(desktopAppInfo);
}

/**
 * 更新应用列表
 */
void FullCommonUseWidget::updateListViewSlot()
{
//    updateListView();
}

void FullCommonUseWidget::updateListView(QString desktopfp)
{
//    m_data.clear();
//    Q_FOREACH(QString desktopfp,m_ukuiMenuInterface->getAllApp())
//        m_data.append(desktopfp);
//    m_listView->updateData(m_data);
    m_listView->insertData(desktopfp);

}

FullCommonUseWidget::~FullCommonUseWidget()
{
    delete m_ukuiMenuInterface;
}
void FullCommonUseWidget::deleteAppListView()
{
    m_data.clear();
    keyVector.clear();
    keyValueVector.clear();

    setting->beginGroup("position");
    QStringList keyList=setting->childKeys();
    Q_FOREACH(QString desktopfn,keyList)
    {
        keyVector.append(desktopfn);
        keyValueVector.append(setting->value(desktopfn).toInt());
    }
    setting->endGroup();
    qSort(keyList.begin(),keyList.end(),cmpApp);

    Q_FOREACH(QString desktopfn,keyList)
        m_data.append("/usr/share/applications/"+desktopfn);
    m_listView->addData(m_data);
}
