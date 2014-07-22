#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------


TARGET = fmifi 
TEMPLATE = lib



HEADERS += src/meego-main.h \
<<<<<<< HEAD
	   src/json/json.h
=======
	       src/json/json.h
>>>>>>> master

SOURCES += src/meego-main.cpp \
           src/jsoncpp.cpp

FORMS    +=

OTHER_FILES += \


CONFIG = link_pkgconfig 
PKGCONFIG += libxml-2.0 
QMAKE_CXXFLAGS += -fPIC

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
target.path = /usr/share/harbour-meecast/lib
INSTALLS += target db source


