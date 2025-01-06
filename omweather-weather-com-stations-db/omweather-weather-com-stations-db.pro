#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------


TARGET = weathercom
#TEMPLATE = app


TEMPLATE = lib

HEADERS += src/main.h \
	       src/json/json.h

SOURCES += src/main.cpp \
           src/jsoncpp.cpp

FORMS    +=

OTHER_FILES += \


CONFIG += link_pkgconfig staticlib -qt
PKGCONFIG += libxml-2.0 \
             glib-2.0

LIBS += -L meecast/libxml2/liblibxml2.a
db.files = data/weather.com.db
db.path =  /opt/com.meecast.omweather/share/db/

icon.files = data/weather.com.png
icon.path = /opt/com.meecast.omweather/share/copyright_icons/

source.files = data/weather.com.xml
source.path = /opt/com.meecast.omweather/share/sources/

#install
target.path = /opt/com.meecast.omweather/lib
INSTALLS += db icon source


