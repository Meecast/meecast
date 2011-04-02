#-------------------------------------------------
#
# Project created by QtCreator 2011-03-03T01:00:05
#
#-------------------------------------------------

TARGET = omweather-settouch
TEMPLATE = app

SOURCES += main.cpp \
    mainpage.cpp \
    stationpage.cpp

QT += sql

CONFIG += link_pkgconfig
CONFIG += meegotouch

PKGCONFIG += sqlite3
PKGCONFIG += libcurl

INCLUDEPATH += ../core
LIBS += -L ../core -lomweather-core

HEADERS += \
    mainpage.h \
    stationpage.h

#install

target.path = /opt/com.meecast.omweather/bin

INSTALLS += target
