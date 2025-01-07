#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------


TARGET = openweathermaporg 
#TEMPLATE = app

TEMPLATE = lib


HEADERS += src/meego-main.h \
           src/hash.h

SOURCES += src/meego-main.cpp \
           src/hash.cpp

FORMS    +=

OTHER_FILES += \


CONFIG += link_pkgconfig staticlib
PKGCONFIG += libxml-2.0 
system(pkg-config --exists glib-2.0){
    PKGCONFIG += glib-2.0
    message(GLIB-2.0 is exist)
    CONFIG += -qt
    DEFINES += GLIB 
}else{
    message(GLIB-2.0 is not exist)
    CONFIG += qt
    QT += core
    DEFINES += QT
}

db.files = data/openweathermap.org.db
db.path = /opt/com.meecast.omweather/share/db/

#icon.files = data/yr.no.png
#icon.path = /opt/com.meecast.omweather/share/copyright_icons/

source.files = data/openweathermap.org.xml
source.path = /opt/com.meecast.omweather/share/sources/

#install
target.path = /opt/com.meecast.omweather/lib
INSTALLS += db icon source


