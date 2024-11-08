#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------


TARGET = bomgovau 
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
LIBS += -L ../meecast/libxml2/liblibxml2.a
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


#CONFIG += staticlib

#PKGCONFIG += glib-2.0
#QMAKE_CXXFLAGS += `pkg-config libxml-2.0 --cflags`
#QMAKE_CXXFLAGS += `pkg-config glib-2.0 --cflags`
#QMAKE_LFLAGS +=   -Wl,-Bstatic -lxml2  -Wl,-Bdynamic -lgcc_s  
#QMAKE_LFLAGS +=   -Wl,-Bstatic -lxml2 -glib-2.0 -Wl,--as-needed
#-Wl,-Bstatic -lz -lfoo -Wl,-Bdynamic -lbar -Wl,--as-needed
db.files = data/bom.gov.au.db
db.path = /usr/share/org.meecast.MeeCast/db/

source.files = data/bom.gov.au.xml
source.path = /usr/share/org.meecast.MeeCast/sources/

#install
#target.path = /usr/share/org.meecast.MeeCast/lib
INSTALLS += db source


