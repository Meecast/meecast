
TEMPLATE = lib
CONFIG += plugin \
    gui \
    link_pkgconfig \
    meegotouch

HEADERS = meegotouchplugin.h
SOURCES = meegotouchplugin.cpp


TARGET = $$qtLibraryTarget(events-meecast)
DESTDIR = ../../../lib
target.path += /usr/lib/meegotouch/applicationextensions/ 
INSTALLS += target desktop_entry

desktop_entry.path =  /usr/share/meegotouch/applicationextensions/ 
desktop_entry.files = *.desktop

