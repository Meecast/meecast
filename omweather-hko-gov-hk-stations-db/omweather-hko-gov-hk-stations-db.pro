#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------


TARGET = hkogovhk 
#TEMPLATE = app


TEMPLATE = lib

HEADERS += src/meego-main.h \
           src/hash.h

SOURCES += src/meego-main.cpp \
           src/hash.cpp

FORMS    +=

OTHER_FILES += \


CONFIG += link_pkgconfig staticlib -qt
PKGCONFIG += libxml-2.0 

#system(pkg-config --exists glib-2.0){
#    PKGCONFIG += glib-2.0
#    message(GLIB-2.0 is exist)
#    CONFIG += -qt
#    DEFINES += GLIB 
#}else{
#    message(GLIB-2.0 is not exist)
    CONFIG += qt
    QT += core
#    DEFINES += QT
#}


#CONFIG += static

db.files = data/hko.gov.hk.db
db.path = /usr/share/harbour-meecast/db/

source.files = data/hko.gov.hk.xml
source.path = /usr/share/harbour-meecast/sources/

#install
#target.path = /usr/share/harbour-meecast/lib
INSTALLS += db source


