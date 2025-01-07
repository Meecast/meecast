#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------


TARGET = weathercom
#TEMPLATE = app


TEMPLATE = lib

HEADERS += src/main.h \
	       src/json/json.h

SOURCES += src/main.cpp \
           src/jsoncpp.cpp

FORMS    +=

OTHER_FILES += \


CONFIG += link_pkgconfig staticlib -qt
PKGCONFIG += libxml-2.0 \
             glib-2.0

LIBS += -L meecast/libxml2/liblibxml2.a
<<<<<<< HEAD
#CONFIG += static
=======
>>>>>>> master
db.files = data/weather.com.db
db.path =  /usr/share/harbour-meecast/db/

#icon.files = data/weather.com.png
#icon.path = /usr/share/copyright_icons/

source.files = data/weather.com.xml
source.path = /usr/share/harbour-meecast/sources/

#install
#target.path = /usr/share/harbour-meecast/lib
INSTALLS += db source
 


