INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
TEMPLATE += fakelib
LIBINTERFACE_NAME = $$qtLibraryTarget(interface)
TEMPLATE -= fakelib
include(../common.pri)
!interface-buildlib{
    LIBS += -L$$PROJECT_LIBDIR -l$$LIBINTERFACE_NAME
}else{
    SOURCES += \
        chineseletterhelper.cpp \
        kylin-start-menu-interface.cpp

    HEADERS += \
        chineseletterhelper.h \
        kylin-start-menu-interface.h

    PKGCONFIG+=glib-2.0
    CONFIG += no_keywords link_pkgconfig

}
