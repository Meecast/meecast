#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------


TARGET = hkogovhk 
TEMPLATE = app



HEADERS += src/meego-main.h \
           src/hash.h

SOURCES += src/meego-main.cpp \
           src/hash.cpp

FORMS    +=

OTHER_FILES += \

CONFIG = link_pkgconfig 
CONFIG += warn_on debug_and_release

#PKGCONFIG += libxml-2.0 

    CONFIG += qt
    QT += core

LIBS += -lxml2

db.files = data/hko.gov.hk.db
db.path = /opt/com.meecast.omweather/share/db/

source.files = data/hko.gov.hk.xml
source.path = /opt/com.meecast.omweather/share/sources/

#install
target.path = /opt/com.meecast.omweather/lib
INSTALLS += target db source


