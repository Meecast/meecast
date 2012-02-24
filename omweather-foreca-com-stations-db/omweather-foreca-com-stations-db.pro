#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------


TARGET = forecacom 
TEMPLATE = app



HEADERS += src/meego-main.h \
           src/hash.h

SOURCES += src/meego-main.c \
           src/hash.c

FORMS    +=

OTHER_FILES += \


CONFIG = link_pkgconfig -qt
PKGCONFIG += libxml-2.0 \
             glib-2.0

db.files = data/foreca.com.db
db.path = /opt/com.meecast.omweather/share/db/

source.files = data/foreca.com.xml
source.path = /opt/com.meecast.omweather/share/sources/

#install
target.path = /opt/com.meecast.omweather/lib
INSTALLS += target db source


