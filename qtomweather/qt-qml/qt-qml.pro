#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------

QT       += declarative

TARGET = qt-qml
TEMPLATE = app


SOURCES += main.cpp \
    dataqml.cpp \
    configqml.cpp

HEADERS  += \
    dataqml.h \
    configqml.h

FORMS    +=
RESOURCES += weatherlayoutitem.qrc

OTHER_FILES += \
    layoutitem.qml \
    weatherlayoutitem.qml


INCLUDEPATH += ../core
LIBS += -L ../core -lomweather-core
CONFIG += link_pkgconfig
PKGCONFIG += libxml++-2.6 libcurl
