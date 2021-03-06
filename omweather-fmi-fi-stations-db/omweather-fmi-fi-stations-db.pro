#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------


TARGET = fmifi 
TEMPLATE = app



HEADERS += src/meego-main.h \
	       src/json/json.h

SOURCES += src/meego-main.cpp \
           src/jsoncpp.cpp

FORMS    +=

OTHER_FILES += \


CONFIG = link_pkgconfig 
PKGCONFIG += libxml-2.0 

#QMAKE_CXXFLAGS += -std=c++11
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


db.files = data/fmi.fi.db
db.path = /opt/com.meecast.omweather/share/db/

source.files = data/fmi.fi.xml
source.path = /opt/com.meecast.omweather/share/sources/

#install
target.path = /opt/com.meecast.omweather/lib
INSTALLS += target db source


