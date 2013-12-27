#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------


TARGET = weathercom
TEMPLATE = app



HEADERS += src/main.h

SOURCES += src/main.c


FORMS    +=

OTHER_FILES += \


CONFIG = link_pkgconfig -qt
PKGCONFIG += libxml-2.0 \
             glib-2.0

db.files = data/weather.com.db
db.path =  /usr/share/db/

icon.files = data/weather.com.png
icon.path = /usr/share/copyright_icons/

source.files = data/weather.com.xml
source.path = /usr/share/sources/

#install
target.path = /usr/lib
INSTALLS += target db icon source


