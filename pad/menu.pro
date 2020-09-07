#-------------------------------------------------
#
# Project created by QtCreator 2020-08-17T11:42:29
#
#-------------------------------------------------

QT       += core gui dbus


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
include(src/QtSingleApplication/qtsingleapplication.pri)
TARGET = ukui-menu-tablet
TEMPLATE = app
target.path = /usr/bin

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
        src/Interface/ukuichineseletter.cpp \
        src/Interface/ukuimenuinterface.cpp \
        src/UtilityFunction/fullcommonusewidget.cpp \
        src/UtilityFunction/fullitemdelegate.cpp \
        src/UtilityFunction/fulllistview.cpp \
        src/MainViewWidget/directorychangedthread.cpp



HEADERS += \
        mainwindow.h \
        src/Interface/ukuichineseletter.h \
        src/Interface/ukuimenuinterface.h \
        src/UtilityFunction/fullcommonusewidget.h \
        src/UtilityFunction/fullitemdelegate.h \
        src/UtilityFunction/fulllistview.h \
        src/Style/style.h \
        src/MainViewWidget/directorychangedthread.h



PKGCONFIG+=glib-2.0 gio-unix-2.0 gsettings-qt
CONFIG += no_keywords link_pkgconfig

desktop_file.files=

RESOURCES += \
    img.qrc \
    translation.qrc

TRANSLATIONS +=\
    translation/tr_en.ts \
    translation/tr_zh.ts \
    translation/tr_zh.qm


desktop_file.files = ukui-menu-tablet.desktop
desktop_file.path = /etc/xdg/autostart

INSTALLS += \
    target desktop_file



