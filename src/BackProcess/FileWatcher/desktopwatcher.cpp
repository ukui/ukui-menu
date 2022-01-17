#include "desktopwatcher.h"
#include <QDir>
#include <QEventLoop>

DesktopWatcher::DesktopWatcher()
{
    //监控.desktop文件目录
    m_fileWatcher = new QFileSystemWatcher;
    m_fileWatcher->addPaths(QStringList() << QString("/usr/share/applications")
                            << QString(QDir::homePath() + "/.local/share/applications/")
                            << QString("/var/lib/snapd/desktop/applications/")
                            << QString("/var/lib/flatpak/exports/share/applications/"));
    connect(m_fileWatcher, &QFileSystemWatcher::directoryChanged, this, &DesktopWatcher::directoryChangedSlot);
    //    m_recentFile = new QFileSystemWatcher;
    //    m_recentFile->addPath(QString(QDir::homePath()+"/.local/share/recently-used.xbel"));
    //    connect(m_recentFile,&QFileSystemWatcher::fileChanged,this,[=](){
    //        m_recentFile->addPath(QDir::homePath()+"/.local/share/recently-used.xbel");
    //    Q_EMIT updateRecentList();});
    m_directoryChangedThread = new DirectoryChangedThread;
    connect(m_directoryChangedThread, &DirectoryChangedThread::requestUpdateSignal, this, &DesktopWatcher::requestUpdateSlot);
}

/**
 * desktop文件目录改变信号槽
 */
void DesktopWatcher::directoryChangedSlot(const QString &path)
{
    QEventLoop loop;
    QTimer::singleShot(100, &loop, SLOT(quit()));
    loop.exec();
    m_directoryChangedThread->start();
}

void DesktopWatcher::requestUpdateSlot()
{
    m_directoryChangedThread->quit();
    Q_EMIT directoryChangedSignal();
}
