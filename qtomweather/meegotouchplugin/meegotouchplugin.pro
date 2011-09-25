
QT += declarative
TEMPLATE = lib
CONFIG += plugin \
    gui \
    link_pkgconfig \
    meegotouch

HEADERS = meegotouchplugin.h
SOURCES = meegotouchplugin.cpp

INCLUDEPATH += ../core
LIBS += -L ../core ../core/libomweather-core.a

TARGET = $$qtLibraryTarget(events-meecast)
DESTDIR = lib
target.path += /usr/lib/meegotouch/applicationextensions/ 
INSTALLS += target desktop_entry applet

desktop_entry.path =  /usr/share/meegotouch/applicationextensions/ 
desktop_entry.files = *.desktop

applet.path =  /opt/com.meecast.omweather/share/omweather/qml 
applet.files = *.qml

