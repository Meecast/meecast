#-------------------------------------------------
#
# Project created by QtCreator 2010-10-03T21:25:41
#
#-------------------------------------------------

QT       += core gui

TARGET = qtomweather
TEMPLATE = app

CONFIG += link_pkgconfig
PKGCONFIG += glib-2.0


SOURCES += main.cpp\
        mainwindow.cpp \
    source.cpp \
    sources.cpp

HEADERS  += mainwindow.h \
    source.h

FORMS    += mainwindow.ui
