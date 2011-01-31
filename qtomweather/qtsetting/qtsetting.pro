#-------------------------------------------------
#
# Project created by QtCreator 2011-01-27T14:47:03
#
#-------------------------------------------------

QT       += core gui

TARGET = qtsetting
TEMPLATE = app


SOURCES += main.cpp\
        setting.cpp

HEADERS  += setting.h

FORMS    += setting.ui

QT += sql xml xmlpatterns 

INCLUDEPATH += ../core
LIBS += -L ../core -lomweather-core
