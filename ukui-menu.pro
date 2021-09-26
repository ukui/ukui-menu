#-------------------------------------------------
#
# ukui-menu
#
#-------------------------------------------------

QT       += core gui svg dbus x11extras KWindowSystem sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
include(src/QtSingleApplication/qtsingleapplication.pri)
QMAKE_CXXFLAGS += -g

TARGET = ukui-menu
TEMPLATE = app

target.path = /usr/bin

TRANSLATIONS+=\
    translations/ukui-menu_bo.ts \
    translations/ukui-menu_zh_CN.ts \
    translations/ukui-menu_tr.ts

QM_FILES_INSTALL_PATH = /usr/share/ukui-menu/translations/

# CONFIG += lrelase not work for qt5.6, add those from lrelease.prf for compatibility
qtPrepareTool(QMAKE_LRELEASE, lrelease)
lrelease.name = lrelease
lrelease.input = TRANSLATIONS
lrelease.output = ${QMAKE_FILE_IN_BASE}.qm
lrelease.commands = $$QMAKE_LRELEASE ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_OUT}
lrelease.CONFIG = no_link
QMAKE_EXTRA_COMPILERS += lrelease
PRE_TARGETDEPS += compiler_lrelease_make_all

for (translation, TRANSLATIONS) {
    translation = $$basename(translation)
    QM_FILES += $$OUT_PWD/$$replace(translation, \\..*$, .qm)
}
qm_files.files = $$QM_FILES
qm_files.path = $$QM_FILES_INSTALL_PATH
qm_files.CONFIG = no_check_exist
INSTALLS += qm_files

# So we can access it from main.cpp
DEFINES += QM_FILES_INSTALL_PATH='\\"$${QM_FILES_INSTALL_PATH}\\"'

RESOURCES += \
    res.qrc \
    res.qrc

PKGCONFIG+=glib-2.0 gio-unix-2.0 gsettings-qt libbamf3 x11 xtst

CONFIG += no_keywords link_pkgconfig

LIBS+=-pthread -luchardet -lukui-log4qt -lukui-com4c -lukui-com4cxx

desktop_file.files = ukui-menu.desktop
desktop_file.path = /etc/xdg/autostart

INSTALLS += \
    target desktop_file

FORMS +=

HEADERS += \
    src/DBus/dbus-adaptor.h \
    src/DBus/dbus.h \
    src/DataProcess/getmodeldata.h \
    src/FileWatcher/desktopwatcher.h \
    src/FileWatcher/directorychangedthread.h \
    src/FileWatcher/tabletdirectorychangedthread.h \
    src/Interface/currenttimeinterface.h \
    src/Interface/ukuichineseletter.h \
    src/Interface/ukuimenuinterface.h \
    src/ListView/fulllistview.h \
    src/ListView/klistview.h \
    src/ListView/listview.h \
    src/ListView/rightlistview.h \
    src/ListView/tabletlistview.h \
    src/QtSingleApplication/QtLockedFile \
    src/QtSingleApplication/QtSingleApplication \
    src/QtSingleApplication/qtlockedfile.h \
    src/QtSingleApplication/qtsinglecoreapplication.h \
    src/RightClickMenu/menubox.h \
    src/RightClickMenu/rightclickmenu.h \
    src/RightClickMenu/tabletrightclickmenu.h \
    src/Search/file-utils.h \
    src/Search/searchappthread.h \
    src/Style/style.h \
    src/UtilityFunction/utility.h \
    src/ViewItem/fullitemdelegate.h \
    src/ViewItem/itemdelegate.h \
    src/ViewItem/kitemdelegate.h \
    src/ViewItem/recentitemdelegate.h \
    src/ViewItem/rightitemdelegate.h \
    src/ViewItem/tabletfullitemdelegate.h \
    src/Widget/classifybtnscrollarea.h \
    src/Widget/fullcommonusewidget.h \
    src/Widget/fullfunctionwidget.h \
    src/Widget/fullletterwidget.h \
    src/Widget/fullmainwindow.h \
    src/Widget/fullsearchresultwidget.h \
    src/Widget/functionbuttonwidget.h \
    src/Widget/functionclassifybutton.h \
    src/Widget/letterbuttonwidget.h \
    src/Widget/letterclassifybutton.h \
    src/Widget/lettertooltip.h \
    src/Widget/mainviewwidget.h \
    src/Widget/mainwindow.h \
    src/Widget/scrollarea.h \
    src/Widget/splitbarframe.h \
    src/Widget/tabviewwidget.h \
    src/Widget/toolbutton.h \
    src/tablet/UtilityFunction/AbstractInterface.h \
    src/tablet/UtilityFunction/KySmallPluginInterface.h \
    src/tablet/UtilityFunction/abstractInterface.h \
    src/tablet/UtilityFunction/pluginwidget.h \
    src/tablet/UtilityFunction/thumbnail.h \
    src/tablet/UtilityFunction/timewidget.h \
    src/tablet/UtilityFunction/toolbox.h \
    src/tablet/XEventMonitor/xeventmonitor.h \
    src/tablet/pagemanager.h \
    src/tablet/tabletwindow.h

SOURCES += \
    main.cpp \
    src/DBus/dbus-adaptor.cpp \
    src/DBus/dbus.cpp \
    src/DataProcess/getmodeldata.cpp \
    src/FileWatcher/desktopwatcher.cpp \
    src/FileWatcher/directorychangedthread.cpp \
    src/FileWatcher/tabletdirectorychangedthread.cpp \
    src/Interface/currenttimeinterface.cpp \
    src/Interface/ukuichineseletter.cpp \
    src/Interface/ukuimenuinterface.cpp \
    src/ListView/fulllistview.cpp \
    src/ListView/klistview.cpp \
    src/ListView/listview.cpp \
    src/ListView/rightlistview.cpp \
    src/ListView/tabletlistview.cpp \
    src/QtSingleApplication/qtlockedfile.cpp \
    src/QtSingleApplication/qtlockedfile_unix.cpp \
    src/QtSingleApplication/qtlockedfile_win.cpp \
    src/QtSingleApplication/qtsinglecoreapplication.cpp \
    src/RightClickMenu/menubox.cpp \
    src/RightClickMenu/rightclickmenu.cpp \
    src/RightClickMenu/tabletrightclickmenu.cpp \
    src/Search/file-utils.cpp \
    src/Search/searchappthread.cpp \
    src/Style/style.cpp \
    src/UtilityFunction/utility.cpp \
    src/ViewItem/fullitemdelegate.cpp \
    src/ViewItem/itemdelegate.cpp \
    src/ViewItem/kitemdelegate.cpp \
    src/ViewItem/recentitemdelegate.cpp \
    src/ViewItem/rightitemdelegate.cpp \
    src/ViewItem/tabletfullitemdelegate.cpp \
    src/Widget/classifybtnscrollarea.cpp \
    src/Widget/fullcommonusewidget.cpp \
    src/Widget/fullfunctionwidget.cpp \
    src/Widget/fullletterwidget.cpp \
    src/Widget/fullmainwindow.cpp \
    src/Widget/fullsearchresultwidget.cpp \
    src/Widget/functionbuttonwidget.cpp \
    src/Widget/functionclassifybutton.cpp \
    src/Widget/letterbuttonwidget.cpp \
    src/Widget/letterclassifybutton.cpp \
    src/Widget/lettertooltip.cpp \
    src/Widget/mainviewwidget.cpp \
    src/Widget/mainwindow.cpp \
    src/Widget/scrollarea.cpp \
    src/Widget/splitbarframe.cpp \
    src/Widget/tabviewwidget.cpp \
    src/Widget/toolbutton.cpp \
    src/tablet/UtilityFunction/pluginwidget.cpp \
    src/tablet/UtilityFunction/thumbnail.cpp \
    src/tablet/UtilityFunction/timewidget.cpp \
    src/tablet/UtilityFunction/toolbox.cpp \
    src/tablet/XEventMonitor/xeventmonitor.cpp \
    src/tablet/pagemanager.cpp \
    src/tablet/tabletwindow.cpp
