#ifndef DESKTOPWATCHER_H
#define DESKTOPWATCHER_H
#include <QFileSystemWatcher>
#include "directorychangedthread.h"


class DesktopWatcher : public QObject
{
    Q_OBJECT
public:
    DesktopWatcher();

public:
Q_SIGNALS:
    /**
     * @brief Desktop file directory change signal
     */
    void directoryChangedSignal();
    void updateRecentList();

public Q_SLOTS:

    void directoryChangedSlot(const QString &path);
    void requestUpdateSlot();

private:
    QFileSystemWatcher *m_fileWatcher=nullptr;//Monitor desktop folder status
//    QFileSystemWatcher *m_recentFile=nullptr;
    DirectoryChangedThread *m_directoryChangedThread=nullptr;
};

#endif // DESKTOPWATCHER_H
