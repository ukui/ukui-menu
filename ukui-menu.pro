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

SOURCES += \
    src/CommonUseWidget/commonusewidget.cpp \
    src/CommonUseWidget/fullcommonusewidget.cpp \
    src/FunctionWidget/fullfunctionwidget.cpp \
    src/FunctionWidget/functionbuttonwidget.cpp \
    src/FunctionWidget/functionwidget.cpp \
    src/LetterWidget/fullletterwidget.cpp \
    src/LetterWidget/letterbuttonwidget.cpp \
    src/LetterWidget/letterwidget.cpp \
    src/MainViewWidget/directorychangedthread.cpp \
    src/MainViewWidget/mainviewwidget.cpp \
    src/MainWindow/mainwindow.cpp \
    src/RightClickMenu/menubox.cpp \
    src/RightClickMenu/rightclickmenu.cpp \
    src/SearchResultWidget/file-utils.cpp \
    src/SearchResultWidget/fullsearchresultwidget.cpp \
    src/SearchResultWidget/searchappthread.cpp \
    src/SearchResultWidget/searchresultwidget.cpp \
    src/SideBarWidget/sidebarwidget.cpp \
    src/Style/style.cpp \
    src/UtilityFunction/classifybtnscrollarea.cpp \
    src/UtilityFunction/fullitemdelegate.cpp \
    src/UtilityFunction/fulllistview.cpp \
    src/UtilityFunction/functionclassifybutton.cpp \
    src/UtilityFunction/itemdelegate.cpp \
    src/UtilityFunction/letterclassifybutton.cpp \
    src/UtilityFunction/listview.cpp \
    src/UtilityFunction/proxystyle.cpp \
    src/UtilityFunction/scrollarea.cpp \
    src/UtilityFunction/splitbarframe.cpp \
    src/UtilityFunction/toolbutton.cpp \
    src/Interface/ukuichineseletter.cpp \
    src/Interface/ukuimenuinterface.cpp \
    main.cpp \
    src/UtilityFunction/utility.cpp \
    src/DBus/dbus.cpp \
    src/DBus/dbus-adaptor.cpp

HEADERS  += \
    src/CommonUseWidget/commonusewidget.h \
    src/CommonUseWidget/fullcommonusewidget.h \
    src/FunctionWidget/fullfunctionwidget.h \
    src/FunctionWidget/functionbuttonwidget.h \
    src/FunctionWidget/functionwidget.h \
    src/LetterWidget/fullletterwidget.h \
    src/LetterWidget/letterbuttonwidget.h \
    src/LetterWidget/letterwidget.h \
    src/MainViewWidget/directorychangedthread.h \
    src/MainViewWidget/mainviewwidget.h \
    src/MainWindow/mainwindow.h \
    src/RightClickMenu/menubox.h \
    src/RightClickMenu/rightclickmenu.h \
    src/SearchResultWidget/file-utils.h \
    src/SearchResultWidget/fullsearchresultwidget.h \
    src/SearchResultWidget/searchappthread.h \
    src/SearchResultWidget/searchresultwidget.h \
    src/SideBarWidget/sidebarwidget.h \
    src/Style/style.h \
    src/UtilityFunction/classifybtnscrollarea.h \
    src/UtilityFunction/fullitemdelegate.h \
    src/UtilityFunction/fulllistview.h \
    src/UtilityFunction/functionclassifybutton.h \
    src/UtilityFunction/itemdelegate.h \
    src/UtilityFunction/letterclassifybutton.h \
    src/UtilityFunction/listview.h \
    src/UtilityFunction/proxystyle.h \
    src/UtilityFunction/scrollarea.h \
    src/UtilityFunction/splitbarframe.h \
    src/UtilityFunction/toolbutton.h \
    src/Interface/ukuichineseletter.h \
    src/Interface/ukuimenuinterface.h \
    src/UtilityFunction/utility.h \
    src/DBus/dbus.h \
    src/DBus/dbus-adaptor.h

RESOURCES += \
    res.qrc

PKGCONFIG+=glib-2.0 gio-unix-2.0 gsettings-qt

CONFIG += no_keywords link_pkgconfig

LIBS+=-pthread -luchardet -lukui-log4qt


desktop_file.files = ukui-menu.desktop
desktop_file.path = /etc/xdg/autostart

INSTALLS += \
    target desktop_file
