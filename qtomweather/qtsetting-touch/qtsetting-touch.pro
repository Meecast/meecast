#-------------------------------------------------
#
# Project created by QtCreator 2011-03-03T01:00:05
#
#-------------------------------------------------

TARGET = qtsetting-touch
TEMPLATE = app

SOURCES += main.cpp

#HEADERS  += mainwindow.h

CONFIG += meegotouch

INCLUDEPATH += ../core
LIBS += -L ../core -lomweather-core

