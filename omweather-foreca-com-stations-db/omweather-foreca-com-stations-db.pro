#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------


TARGET = forecacom 
#TEMPLATE = app


TEMPLATE = lib

HEADERS += src/meego-main.h \
           src/hash.h

SOURCES += src/meego-main.cpp \
           src/hash.cpp

FORMS    +=

OTHER_FILES += \


CONFIG = link_pkgconfig 

linux:!android {
    message("* Using settings for Unix/Linux.")
    PKGCONFIG += libxml-2.0
}
             
INCLUDEPATH += ../meecast/libxml2/include
LIBS += $$PWD/../meecast/libxml2/liblibxml2.a
CONFIG += link_pkgconfig staticlib

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


db.files = data/foreca.com.db
db.path = /opt/com.meecast.omweather/share/db/

source.files = data/foreca.com.xml
source.path = /opt/com.meecast.omweather/share/sources/

#install
target.path = /opt/com.meecast.omweather/lib
INSTALLS += db source


