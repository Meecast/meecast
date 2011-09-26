
QT += declarative 

TEMPLATE = lib
CONFIG += plugin \
    qdbus \
    gui \
    link_pkgconfig \
    meegotouch

HEADERS = meegotouchplugin.h dbusadaptor.h
SOURCES = meegotouchplugin.cpp dbusadaptor.cpp


TARGET = $$qtLibraryTarget(events-meecast)
DESTDIR = lib
target.path += /usr/lib/meegotouch/applicationextensions/ 
INSTALLS += target desktop_entry applet

#desktop_entry.path =  /usr/share/meegotouch/applicationextensions/ 
#desktop_entry.files = *.desktop
desktop_entry.path =  /opt/com.meecast.omweather/share/applet 
desktop_entry.files = *.desktop


applet.path =  /opt/com.meecast.omweather/share/omweather/qml 
applet.files = *.qml

