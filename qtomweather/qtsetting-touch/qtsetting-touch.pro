#-------------------------------------------------
#
# Project created by QtCreator 2011-03-03T01:00:05
#
#-------------------------------------------------

TARGET = qtsetting-touch
TEMPLATE = app

SOURCES += main.cpp \
    mainpage.cpp \
    stationpage.cpp

QT += sql

CONFIG += meegotouch

INCLUDEPATH += ../core
LIBS += -L ../core -lomweather-core

HEADERS += \
    mainpage.h \
    stationpage.h

#install

target.path = /usr/bin

INSTALLS += target
