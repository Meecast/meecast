# -------------------------------------------------
# Project created by QtCreator 2010-10-03T21:25:41
# -------------------------------------------------
QT += core \
    gui
TARGET = qtomweather
TEMPLATE = app
CONFIG += link_pkgconfig
PKGCONFIG += glib-2.0
SOURCES += main.cpp \
    mainwindow.cpp \
    source.cpp \
    sources.cpp \
    core/station.cpp \
    core/source.cpp \
    core/parser.cpp \
    core/downloader.cpp \
    core/data.cpp \
    core/connection.cpp \
    core/config.cpp
HEADERS += mainwindow.h \
    source.h \
    core/station.h \
    core/source.h \
    core/parser.h \
    core/downloader.h \
    core/data.h \
    core/core.h \
    core/connection.h \
    core/config.h
FORMS += mainwindow.ui
OTHER_FILES += core/core.pro
