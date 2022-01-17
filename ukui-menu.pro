#-------------------------------------------------
#
# ukui-menu
#
#-------------------------------------------------

QT       += core gui svg dbus x11extras KWindowSystem sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(./src/BackProcess/backprocess.pri)
include(./src/QtSingleApplication/qtsingleapplication.pri)
include(./src/UserInterface/userinterface.pri)
include(./src/UtilityFunction/utilityfunction.pri)
QMAKE_CXXFLAGS += -g

TARGET = ukui-menu
TEMPLATE = app

target.path = /usr/bin

INCLUDEPATH  += \
    src/BackProcess/DBus \
    src/BackProcess/FileWatcher \
    src/BackProcess/Interface \
    src/BackProcess/Search \
    src/BackProcess/tablet \
    src/BackProcess/XEventMonitor \
    src/QtSingleApplication \
    src/UserInterface \
    src/UserInterface/Button \
    src/UserInterface/ListView \
    src/UserInterface/Other \
    src/UserInterface/RightClickMenu \
    src/UserInterface/ViewItem \
    src/UserInterface/Widget \
    src/UtilityFunction \
    src/UtilityFunction/Style

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
    res.qrc

PKGCONFIG+=glib-2.0 gio-unix-2.0 gsettings-qt libbamf3 x11 xtst

CONFIG += no_keywords link_pkgconfig

LIBS+=-pthread -luchardet -lukui-log4qt -lukui-com4c -lukui-com4cxx -lpeony

desktop_file.files = ukui-menu.desktop
desktop_file.path = /etc/xdg/autostart

INSTALLS += \
    target desktop_file

FORMS +=

SOURCES += \
    main.cpp
