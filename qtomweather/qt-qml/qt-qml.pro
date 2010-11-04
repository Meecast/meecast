#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------

QT       += declarative

TARGET = qt-qml
TEMPLATE = app


SOURCES += main.cpp \
    dataqml.cpp

HEADERS  += \
    dataqml.h

FORMS    +=
RESOURCES += layoutitem.qrc

OTHER_FILES += \
    layoutitem.qml


INCLUDEPATH += ../core
LIBS += -L ../core -lomweather-core
CONFIG += link_pkgconfig
PKGCONFIG += libxml++-2.6 libcurl
