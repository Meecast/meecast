
QT += declarative network

TEMPLATE = lib
#TEMPLATE = app
CONFIG += plugin \
    qdbus \
    gui \
    link_pkgconfig \
    mobility \
    meegotouch

MOBILITY = publishsubscribe

HEADERS = meegotouchplugin.h dbusadaptor.h eventfeedif.h  weatherdataif.h
SOURCES = meegotouchplugin.cpp dbusadaptor.cpp eventfeedif.cpp  weatherdataif.cpp


TARGET = $$qtLibraryTarget(events-meecast)
#TARGET = test
DESTDIR = lib
target.path += /usr/lib/meegotouch/applicationextensions/ 
INSTALLS += target desktop_entry applet package datasmallcontour

#desktop_entry.path =  /usr/share/meegotouch/applicationextensions/ 
desktop_entry.path =  /opt/com.meecast.omweather/share/applet 
desktop_entry.files = *.desktop


applet.path =  /opt/com.meecast.omweather/share/omweather/qml 
applet.files = *.qml

package.path =  /opt/com.meecast.omweather/share/packages
package.files = *.deb

datasmallcontour.files += data/smallcontour/*.png
datasmallcontour.path = /opt/com.meecast.omweather/share/images/smallcontour

contextreg.files = data/meecast.context
contextreg.path = /usr/share/contextkit/providers
INSTALLS += contextreg
