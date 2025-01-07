#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------


TARGET = fmifi 
TEMPLATE = lib
#TEMPLATE = app



HEADERS += src/meego-main.h \
	       src/json/json.h

SOURCES += src/meego-main.cpp \
           src/tz.cpp \
           src/jsoncpp.cpp

FORMS    +=

DEFINES += "USE_OS_TZDB=1"
OTHER_FILES += \

DEFINES += "USE_OS_TZDB=1"

CONFIG = link_pkgconfig 
PKGCONFIG += libxml-2.0 
PKGCONFIG += libcurl
#QMAKE_CXXFLAGS += -fPIC -std=c++20
#QMAKE_CXXFLAGS += -fPIC -std=c++2a

QMAKE_CXXFLAGS += -fPIC -std=c++14
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


CONFIG += static
db.files = data/fmi.fi.db
db.path = /usr/share/harbour-meecast/db/



source.files = data/fmi.fi.xml
source.path = /usr/share/harbour-meecast/sources/

#install
#target.path = /usr/share/harbour-meecast/lib
INSTALLS += db source


