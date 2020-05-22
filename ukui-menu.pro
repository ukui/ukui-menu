#-------------------------------------------------
#
# Project created by QtCreator 2020-02-10T06:33:14
#
#-------------------------------------------------

QT       += core gui svg dbus x11extras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
include(src/QtSingleApplication/qtsingleapplication.pri)
QMAKE_CXXFLAGS += -g

TARGET = ukui-menu
TEMPLATE = app

target.path = /usr/bin

TRANSLATIONS+=\
    ukui_menu_ch.ts

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
    src/RightClickMenu/rightclickmenu.cpp \
    src/SearchResultWidget/fullsearchresultwidget.cpp \
    src/SearchResultWidget/searchappthread.cpp \
    src/SearchResultWidget/searchresultwidget.cpp \
    src/SideBarWidget/sidebarwidget.cpp \
    src/Style/style.cpp \
    src/UtilityFunction/classifyscrollarea.cpp \
    src/UtilityFunction/fullitemdelegate.cpp \
    src/UtilityFunction/fulllistview.cpp \
    src/UtilityFunction/functionclassifybutton.cpp \
    src/UtilityFunction/itemdelegate.cpp \
    src/UtilityFunction/letterclassifybutton.cpp \
    src/UtilityFunction/listview.cpp \
    src/UtilityFunction/pushbutton.cpp \
    src/UtilityFunction/scrollarea.cpp \
    src/UtilityFunction/toolbutton.cpp \
    src/Interface/ukuichineseletter.cpp \
    src/Interface/ukuimenuinterface.cpp \
    main.cpp \
    src/UtilityFunction/utility.cpp \
    src/XEventMonitor/xeventmonitor.cpp

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
    src/RightClickMenu/rightclickmenu.h \
    src/SearchResultWidget/fullsearchresultwidget.h \
    src/SearchResultWidget/searchappthread.h \
    src/SearchResultWidget/searchresultwidget.h \
    src/SideBarWidget/sidebarwidget.h \
    src/Style/style.h \
    src/UtilityFunction/classifyscrollarea.h \
    src/UtilityFunction/fullitemdelegate.h \
    src/UtilityFunction/fulllistview.h \
    src/UtilityFunction/functionclassifybutton.h \
    src/UtilityFunction/itemdelegate.h \
    src/UtilityFunction/letterclassifybutton.h \
    src/UtilityFunction/listview.h \
    src/UtilityFunction/pushbutton.h \
    src/UtilityFunction/scrollarea.h \
    src/UtilityFunction/toolbutton.h \
    src/Interface/ukuichineseletter.h \
    src/Interface/ukuimenuinterface.h \
    src/UtilityFunction/utility.h \
    src/XEventMonitor/xeventmonitor.h

FORMS    += \
    src/CommonUseWidget/commonusewidget.ui \
    src/CommonUseWidget/fullcommonusewidget.ui \
    src/FunctionWidget/fullfunctionwidget.ui \
    src/FunctionWidget/functionbuttonwidget.ui \
    src/FunctionWidget/functionwidget.ui \
    src/LetterWidget/fullletterwidget.ui \
    src/LetterWidget/letterbuttonwidget.ui \
    src/LetterWidget/letterwidget.ui \
    src/MainViewWidget/mainviewwidget.ui \
    src/MainWindow/mainwindow.ui \
    src/SearchResultWidget/fullsearchresultwidget.ui \
    src/SearchResultWidget/searchresultwidget.ui \
    src/SideBarWidget/sidebarwidget.ui

RESOURCES += \
    res.qrc

PKGCONFIG+=glib-2.0 gio-unix-2.0 gsettings-qt libbamf3 x11 xrandr xtst
CONFIG += no_keywords link_pkgconfig

desktop_file.files = ukui-menu.desktop
desktop_file.path = /etc/xdg/autostart

INSTALLS += \
    target desktop_file
