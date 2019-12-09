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

QMAKE_CXXFLAGS += -g

DESTDIR = $$PROJECT_BINDIR
unix:QMAKE_RPATHDIR+=$$PROJECT_LIBDIR

SOURCES += main.cpp \
    src/MainWindow/mainwindow.cpp \
    src/SideBarWidget/sidebarwidget.cpp \
    src/MainViewWidget/mainviewwidget.cpp \
    src/LetterWidget/letterwidget.cpp \
    src/FunctionWidget/functionwidget.cpp \
    src/CommonUseWidget/commonusewidget.cpp \
    src/FunctionWidget/functionbuttonwidget.cpp \
    src/LetterWidget/letterbuttonwidget.cpp \
    src/CommonUseWidget/fullcommonusewidget.cpp \
    src/FunctionWidget/fullfunctionbuttonwidget.cpp \
    src/FunctionWidget/fullfunctionwidget.cpp \
    src/LetterWidget/fullletterbuttonwidget.cpp \
    src/LetterWidget/fullletterwidget.cpp \
    src/SearchResultWidget/searchresultwidget.cpp \
    src/SearchResultWidget/fullsearchresultwidget.cpp \
    src/RightClickMenu/rightclickmenu.cpp \
    src/SearchResultWidget/searchappthread.cpp \
    src/AttributeDialog/attributedialog.cpp \
    src/UtilityFunction/toolbutton.cpp \
    src/UtilityFunction/itemdelegate.cpp \
    src/UtilityFunction/listview.cpp \
    src/UtilityFunction/fulllistview.cpp \
    src/UtilityFunction/pushbutton.cpp \
    src/UtilityFunction/scrollarea.cpp \
    src/UtilityFunction/classifybutton.cpp \
    src/UtilityFunction/classifyscrollarea.cpp \
    src/UtilityFunction/fullitemdelegate.cpp \
    src/UtilityFunction/listmodel.cpp \
    src/UtilityFunction/fulllistmodel.cpp

HEADERS  += \
    src/MainWindow/mainwindow.h \
    src/SideBarWidget/sidebarwidget.h \
    src/MainViewWidget/mainviewwidget.h \
    src/LetterWidget/letterwidget.h \
    src/FunctionWidget/functionwidget.h \
    src/CommonUseWidget/commonusewidget.h \
    src/FunctionWidget/functionbuttonwidget.h \
    src/LetterWidget/letterbuttonwidget.h \
    src/CommonUseWidget/fullcommonusewidget.h \
    src/FunctionWidget/fullfunctionbuttonwidget.h \
    src/FunctionWidget/fullfunctionwidget.h \
    src/LetterWidget/fullletterbuttonwidget.h \
    src/LetterWidget/fullletterwidget.h \
    src/SearchResultWidget/searchresultwidget.h \
    src/SearchResultWidget/fullsearchresultwidget.h \
    src/color.h \
    src/RightClickMenu/rightclickmenu.h \
    src/SearchResultWidget/searchappthread.h \
    src/AttributeDialog/attributedialog.h \
    src/UtilityFunction/toolbutton.h \
    src/UtilityFunction/itemdelegate.h \
    src/UtilityFunction/listview.h \
    src/UtilityFunction/fulllistview.h \
    src/UtilityFunction/pushbutton.h \
    src/UtilityFunction/scrollarea.h \
    src/UtilityFunction/classifybutton.h \
    src/UtilityFunction/classifyscrollarea.h \
    src/UtilityFunction/fullitemdelegate.h \
    src/UtilityFunction/listmodel.h \
    src/UtilityFunction/fulllistmodel.h

FORMS    += \
    src/MainWindow/mainwindow.ui \
    src/SideBarWidget/sidebarwidget.ui \
    src/MainViewWidget/mainviewwidget.ui \
    src/LetterWidget/letterwidget.ui \
    src/FunctionWidget/functionwidget.ui \
    src/CommonUseWidget/commonusewidget.ui \
    src/FunctionWidget/functionbuttonwidget.ui \
    src/LetterWidget/letterbuttonwidget.ui \
    src/CommonUseWidget/fullcommonusewidget.ui \
    src/FunctionWidget/fullfunctionbuttonwidget.ui \
    src/FunctionWidget/fullfunctionwidget.ui \
    src/LetterWidget/fullletterbuttonwidget.ui \
    src/LetterWidget/fullletterwidget.ui \
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
