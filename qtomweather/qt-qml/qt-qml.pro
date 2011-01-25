#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------

QT       += declarative

TARGET = omweather-qml
TEMPLATE = app


SOURCES += main.cpp \
    dataqml.cpp \
    configqml.cpp \
    qmllayoutitem.cpp \
    dataitem.cpp \
    datamodel.cpp 

HEADERS  += \
    dataqml.h \
    configqml.h \
    qmllayoutitem.h \
    dataitem.h \
    datamodel.h 

FORMS    +=
RESOURCES += weatherlayoutitem.qrc

OTHER_FILES += \
    layoutitem.qml \
    weatherlayoutitem.qml \
    Details.qml

CONFIG(localdebug):DEFINES += LOCALDEBUG

INCLUDEPATH += ../core
LIBS += -L ../core -lomweather-core
CONFIG += link_pkgconfig
PKGCONFIG +=
target.path = /usr/bin
INSTALLS += target
