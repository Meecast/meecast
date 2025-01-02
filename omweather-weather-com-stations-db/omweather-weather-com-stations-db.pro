#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------


TARGET = weathercom
TEMPLATE = app



HEADERS += src/main.h \
	       src/json/json.h

SOURCES += src/main.cpp \
           src/jsoncpp.cpp

FORMS    +=

OTHER_FILES += \


CONFIG = link_pkgconfig -qt

linux:!android {
    message("* Using settings for Unix/Linux.")
    PKGCONFIG += libxml-2.0 \
                 glib-2.0
}
             
INCLUDEPATH += ../meecast/libxml2/include

db.files = data/weather.com.db
db.path =  /opt/com.meecast.omweather/share/db/

icon.files = data/weather.com.png
icon.path = /opt/com.meecast.omweather/share/copyright_icons/

source.files = data/weather.com.xml
source.path = /opt/com.meecast.omweather/share/sources/

#install
target.path = /opt/com.meecast.omweather/lib
INSTALLS += target db icon source


