DISTFILES += \
    $$PWD/FileWatcher/filewatcher.pri

INCLUDEPATH  += \
    $$PWD/DBus \
    $$PWD/FileWatcher \
    $$PWD/Interface \
    $$PWD/Search \
    $$PWD/tablet \
    $$PWD/XEventMonitor

HEADERS += \
    $$PWD/DBus/dbus-adaptor.h \
    $$PWD/DBus/dbus.h \
    $$PWD/FileWatcher/convert_winid_to_desktop.h \
    $$PWD/FileWatcher/desktop_watcher.h \
    $$PWD/FileWatcher/directory_changed_thread.h \
    $$PWD/FileWatcher/software_database_update_thread.h \
    $$PWD/FileWatcher/tablet_directorychanged_thread.h \
    $$PWD/Interface/currenttime_interface.h \
    $$PWD/Interface/ukui_chineseletter.h \
    $$PWD/Interface/ukuimenuinterface.h \
    $$PWD/Search/file-utils.h \
    $$PWD/Search/searchappthread.h \
    $$PWD/XEventMonitor/xeventmonitor.h \
    $$PWD/tablet/getmodeldata.h \
    $$PWD/tablet/pagemanager.h

SOURCES += \
    $$PWD/DBus/dbus-adaptor.cpp \
    $$PWD/DBus/dbus.cpp \
    $$PWD/FileWatcher/convert_winid_to_desktop.cpp \
    $$PWD/FileWatcher/desktop_watcher.cpp \
    $$PWD/FileWatcher/directory_changed_thread.cpp \
    $$PWD/FileWatcher/software_database_update_thread.cpp \
    $$PWD/FileWatcher/tablet_directorychanged_thread.cpp \
    $$PWD/Interface/currenttime_interface.cpp \
    $$PWD/Interface/ukui_chineseletter.cpp \
    $$PWD/Interface/ukuimenuinterface.cpp \
    $$PWD/Search/file-utils.cpp \
    $$PWD/Search/searchappthread.cpp \
    $$PWD/XEventMonitor/xeventmonitor.cpp \
    $$PWD/tablet/getmodeldata.cpp \
    $$PWD/tablet/pagemanager.cpp
