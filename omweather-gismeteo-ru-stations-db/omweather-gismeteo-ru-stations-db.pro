#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------


TARGET = gismeteoru 
#TEMPLATE = app


TEMPLATE = lib

HEADERS += src/meego-main.h \
           src/hash.h \
           src/json/json.h

SOURCES += src/meego-main.cpp \
           src/hash.cpp \
           src/jsoncpp.cpp

FORMS    +=

OTHER_FILES += \

CONFIG += link_pkgconfig staticlib
PKGCONFIG += libxml-2.0 

QMAKE_CXXFLAGS += -fPIC -std=c++14 -g
#system(pkg-config --exists glib-2.0){
#    PKGCONFIG += glib-2.0
#    message(GLIB-2.0 is exist)
#    CONFIG += -qt
#    DEFINES += GLIB 
#}else{
#    message(GLIB-2.0 is not exist)
#    CONFIG += qt
#    QT += core
#    DEFINES += QT
#}


db.files = data/gismeteo.ru.db
db.path = /opt/com.meecast.omweather/share/db/

icon.files = data/gismeteo.ru.png
icon.path = /opt/com.meecast.omweather/share/copyright_icons/

source.files = data/gismeteom.ru.xml
source.path = /opt/com.meecast.omweather/share/sources/

#install
target.path = /opt/com.meecast.omweather/lib
INSTALLS += db source


