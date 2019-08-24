#-------------------------------------------------
#
# Project created by QtCreator 2019-07-08T09:41:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = kylin-start-menu
TEMPLATE = app


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
    src/SearchResultWidget/fullsearchresultwidget.cpp

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
    src/color.h

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
    src/SearchResultWidget/fullsearchresultwidget.ui

RESOURCES += \
    res.qrc

DISTFILES +=
