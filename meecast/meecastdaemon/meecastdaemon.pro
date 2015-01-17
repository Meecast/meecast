
QT += network quick
#QT += widgets
QT += concurrent
PKGCONFIG += mlite5 
PKGCONFIG += dconf
CONFIG += link_pkgconfig
PKGCONFIG += sailfishapp
PKGCONFIG += keepalive 
#

#TEMPLATE = lib
TEMPLATE = app
CONFIG += console
CONFIG += sailfishapp
CONFIG += dbus \
    gui \
    link_pkgconfig 


HEADERS = meecastdaemon.h dbusadaptor.h weatherdataif.h \
    ../dconf/dconfvalue.h \
    ../dconf/mdconf_p.h \
    ../dconf/mdconfitem.h \
    ../dconf/dconfmigration.h

SOURCES = meecastdaemon.cpp dbusadaptor.cpp weatherdataif.cpp \
    ../dconf/dconfvalue.cpp \
    ../dconf/mdconf.cpp \
    ../dconf/mdconfitem.cpp


TARGET = meecastd
DESTDIR = bin 
target.path += /usr/lib/meegotouch/applicationextensions/ 
INSTALLS += target meecastdservice

meecastdservice.files = $${TARGET}.service
meecastdservice.path = /usr/lib/systemd/user


#desktop_entry.path =  /usr/share/meegotouch/applicationextensions/ 
desktop_entry.path =  /opt/com.meecast.omweather/share/applet 
desktop_entry.files = *.desktop


applet.path =  /opt/com.meecast.omweather/share/omweather/qml 
applet.files = *.qml

package.path =  /opt/com.meecast.omweather/share/packages
package.files = *.deb

datasmallcontour.files += data/smallcontour/*.png
datasmallcontour.path = /opt/com.meecast.omweather/share/images/smallcontour

