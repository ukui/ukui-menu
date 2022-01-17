DISTFILES += \
    $$PWD/DBus/org.ukui.menu.xml \
    $$PWD/FileWatcher/filewatcher.pri \
    $$PWD/Search/pinyinWithTone.txt \
    $$PWD/Search/pinyinWithoutTone.txt

HEADERS += \
    $$PWD/DBus/dbus-adaptor.h \
    $$PWD/DBus/dbus.h \
    $$PWD/FileWatcher/convertwinidtodesktop.h \
    $$PWD/FileWatcher/desktopwatcher.h \
    $$PWD/FileWatcher/directorychangedthread.h \
    $$PWD/FileWatcher/softwaredatabaseupdatethread.h \
    $$PWD/FileWatcher/tabletdirectorychangedthread.h \
    $$PWD/Interface/currenttimeinterface.h \
    $$PWD/Interface/ukuichineseletter.h \
    $$PWD/Interface/ukuimenuinterface.h \
    $$PWD/Search/file-utils.h \
    $$PWD/Search/searchappthread.h \
    $$PWD/XEventMonitor/xeventmonitor.h \
    $$PWD/tablet/getmodeldata.h \
    $$PWD/tablet/pagemanager.h

SOURCES += \
    $$PWD/DBus/dbus-adaptor.cpp \
    $$PWD/DBus/dbus.cpp \
    $$PWD/FileWatcher/convertwinidtodesktop.cpp \
    $$PWD/FileWatcher/desktopwatcher.cpp \
    $$PWD/FileWatcher/directorychangedthread.cpp \
    $$PWD/FileWatcher/softwaredatabaseupdatethread.cpp \
    $$PWD/FileWatcher/tabletdirectorychangedthread.cpp \
    $$PWD/Interface/currenttimeinterface.cpp \
    $$PWD/Interface/ukuichineseletter.cpp \
    $$PWD/Interface/ukuimenuinterface.cpp \
    $$PWD/Search/file-utils.cpp \
    $$PWD/Search/searchappthread.cpp \
    $$PWD/XEventMonitor/xeventmonitor.cpp \
    $$PWD/tablet/getmodeldata.cpp \
    $$PWD/tablet/pagemanager.cpp
