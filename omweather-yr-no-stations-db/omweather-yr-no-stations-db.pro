#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------


TARGET = yrno 
TEMPLATE = app



HEADERS += src/meego-main.h \
           src/hash.h

SOURCES += src/meego-main.cpp \
           src/hash.cpp

FORMS    +=

OTHER_FILES += \


CONFIG = link_pkgconfig 
CONFIG += warn_on debug_and_release
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
    DEFINES += QT
#}

device {
	CONFIG(release, debug|release) {
		DESTDIR = o.le-v7
	}
	CONFIG(debug, debug|release) {
		DESTDIR = o.le-v7-g
	}
}

simulator {
	CONFIG(release, debug|release) {
		DESTDIR = o
	}
	CONFIG(debug, debug|release) {
		DESTDIR = o-g
	}
}

db.files = data/yr.no.db
db.path = /opt/com.meecast.omweather/share/db/

icon.files = data/yr.no.png
icon.path = /opt/com.meecast.omweather/share/copyright_icons/

source.files = data/yr.no.xml
source.path = /opt/com.meecast.omweather/share/sources/

#install
target.path = /opt/com.meecast.omweather/lib
INSTALLS += target db icon source


