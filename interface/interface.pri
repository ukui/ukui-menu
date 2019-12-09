INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
TEMPLATE += fakelib
LIBINTERFACE_NAME = $$qtLibraryTarget(ukui-menu)
TEMPLATE -= fakelib
include(../common.pri)
!interface-buildlib{
    LIBS += -L$$PROJECT_LIBDIR -l$$LIBINTERFACE_NAME
}else{
    SOURCES += \
        chineseletterhelper.cpp \
        ukuimenuinterface.cpp

    HEADERS += \
        chineseletterhelper.h \
        ukuimenuinterface.h

    PKGCONFIG+=glib-2.0
    CONFIG += no_keywords link_pkgconfig

}
