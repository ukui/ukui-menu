#-------------------------------------------------
#
# Project created by QtCreator 2020-02-07T10:42:50
#
#-------------------------------------------------

QT       += core gui svg dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
include(src/QtSingleApplication/qtsingleapplication.pri)

TARGET = ukui-menu
TEMPLATE = app

TRANSLATIONS+=\
    ukui_menu_ch.ts

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    src/CommonUseWidget/commonusewidget.cpp \
    src/CommonUseWidget/fullcommonusewidget.cpp \
    src/FunctionWidget/fullfunctionwidget.cpp \
    src/FunctionWidget/functionbuttonwidget.cpp \
    src/FunctionWidget/functionwidget.cpp \
    src/LetterWidget/fullletterwidget.cpp \
    src/LetterWidget/letterbuttonwidget.cpp \
    src/LetterWidget/letterwidget.cpp \
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
    src/UtilityFunction/fulllistmodel.cpp \
    src/UtilityFunction/fulllistview.cpp \
    src/UtilityFunction/functionclassifybutton.cpp \
    src/UtilityFunction/itemdelegate.cpp \
    src/UtilityFunction/listmodel.cpp \
    src/UtilityFunction/listview.cpp \
    src/UtilityFunction/pushbutton.cpp \
    src/UtilityFunction/scrollarea.cpp \
    src/UtilityFunction/toolbutton.cpp \
    main.cpp \
    src/interface/chineseletterhelper.cpp \
    src/interface/ukuimenuinterface.cpp

HEADERS += \
    src/CommonUseWidget/commonusewidget.h \
    src/CommonUseWidget/fullcommonusewidget.h \
    src/FunctionWidget/fullfunctionwidget.h \
    src/FunctionWidget/functionbuttonwidget.h \
    src/FunctionWidget/functionwidget.h \
    src/LetterWidget/fullletterwidget.h \
    src/LetterWidget/letterbuttonwidget.h \
    src/LetterWidget/letterwidget.h \
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
    src/UtilityFunction/fulllistmodel.h \
    src/UtilityFunction/fulllistview.h \
    src/UtilityFunction/functionclassifybutton.h \
    src/UtilityFunction/itemdelegate.h \
    src/UtilityFunction/listmodel.h \
    src/UtilityFunction/listview.h \
    src/UtilityFunction/pushbutton.h \
    src/UtilityFunction/scrollarea.h \
    src/UtilityFunction/toolbutton.h \
    src/interface/chineseletterhelper.h \
    src/interface/ukuimenuinterface.h

FORMS += \
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

PKGCONFIG+=glib-2.0
CONFIG += no_keywords link_pkgconfig
