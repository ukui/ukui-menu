#-------------------------------------------------
#
# Project created by QtCreator 2019-07-08T09:41:44
#
#-------------------------------------------------

QT       += core gui svg dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(../interface/interface.pri)
include(src/QtSingleApplication/qtsingleapplication.pri)

TARGET = ukui-menu
TEMPLATE = app

DESTDIR = $$PROJECT_BINDIR
QMAKE_RPATHDIR+=$$PROJECT_LIBDIR

SOURCES += main.cpp \
    src/MainWindow/mainwindow.cpp \
    src/SideBarWidget/sidebarwidget.cpp \
    src/MainViewWidget/mainviewwidget.cpp \
    src/LetterWidget/letterwidget.cpp \
    src/FunctionWidget/functionwidget.cpp \
    src/CommonUseWidget/commonusewidget.cpp \
    src/FunctionWidget/functionbuttonwidget.cpp \
    src/LetterWidget/letterbuttonwidget.cpp \
    src/SearchResultWidget/searchresultwidget.cpp \
    src/SearchResultWidget/fullsearchresultwidget.cpp \
    src/RightClickMenu/rightclickmenu.cpp \
    src/SearchResultWidget/searchappthread.cpp \
    src/AttributeDialog/attributedialog.cpp \
    src/UtilityFunction/toolbutton.cpp \
    src/UtilityFunction/qflowlayout.cpp

HEADERS  += \
    src/MainWindow/mainwindow.h \
    src/SideBarWidget/sidebarwidget.h \
    src/MainViewWidget/mainviewwidget.h \
    src/LetterWidget/letterwidget.h \
    src/FunctionWidget/functionwidget.h \
    src/CommonUseWidget/commonusewidget.h \
    src/FunctionWidget/functionbuttonwidget.h \
    src/LetterWidget/letterbuttonwidget.h \
    src/SearchResultWidget/searchresultwidget.h \
    src/SearchResultWidget/fullsearchresultwidget.h \
    src/color.h \
    src/RightClickMenu/rightclickmenu.h \
    src/SearchResultWidget/searchappthread.h \
    src/AttributeDialog/attributedialog.h \
    src/UtilityFunction/toolbutton.h \
    src/UtilityFunction/qflowlayout.h

FORMS    += \
    src/MainWindow/mainwindow.ui \
    src/SideBarWidget/sidebarwidget.ui \
    src/MainViewWidget/mainviewwidget.ui \
    src/LetterWidget/letterwidget.ui \
    src/FunctionWidget/functionwidget.ui \
    src/CommonUseWidget/commonusewidget.ui \
    src/FunctionWidget/functionbuttonwidget.ui \
    src/LetterWidget/letterbuttonwidget.ui \
    src/SearchResultWidget/searchresultwidget.ui \
    src/SearchResultWidget/fullsearchresultwidget.ui \
    src/AttributeDialog/attributedialog.ui

RESOURCES += \
    res.qrc

DISTFILES +=

#LIBS+= -L/usr/lib/ukui-menu/ -lkylin-start-menu-interface
#INCLUDEPATH+=-I/usr/include/ukui-menu/
#LIBS+=-L ../lib -lukui-start-menu-interface
#QMAKE_RPATHDIR+=-L ../lib -lukui-start-menu-interface
#INCLUDEPATH+=-I ../lib
