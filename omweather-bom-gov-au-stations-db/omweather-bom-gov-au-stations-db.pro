#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------


TARGET = bomgovau 
TEMPLATE = app


HEADERS += src/meego-main.h \
           src/hash.h

SOURCES += src/meego-main.cpp \
           src/hash.cpp

FORMS    +=

OTHER_FILES += \


CONFIG = link_pkgconfig 
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

db.files = data/bom.gov.au.db
#db.path = /opt/com.meecast.omweather/share/db/
db.path = /usr/share/db/

source.files = data/bom.gov.au.xml
#source.path = /opt/com.meecast.omweather/share/sources/
source.path = /usr/share/sources/

#install
#target.path = /opt/com.meecast.omweather/lib
target.path = /usr/lib
INSTALLS += target db source


