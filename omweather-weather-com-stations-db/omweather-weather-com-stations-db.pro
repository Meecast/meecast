#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------


TARGET = weathercom
TEMPLATE = app



HEADERS += src/main.h

SOURCES += src/main.c


FORMS    +=

OTHER_FILES += \


CONFIG = link_pkgconfig -qt
PKGCONFIG += libxml-2.0 

INCLUDEPATH += ${QNX_TARGET}/usr/include/glib-2.0 ${QNX_TARGET}/usr/lib/glib-2.0/include 

db.files = data/weather.com.db
db.path =  /opt/com.meecast.omweather/share/db/

icon.files = data/weather.com.png
icon.path = /opt/com.meecast.omweather/share/copyright_icons/

source.files = data/weather.com.xml
source.path = /opt/com.meecast.omweather/share/sources/

#install
target.path = /opt/com.meecast.omweather/lib
INSTALLS += target db icon source


