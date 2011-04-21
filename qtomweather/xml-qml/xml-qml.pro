#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------


TARGET = xml-qml
TEMPLATE = app


TEXTDOMAIN = "omweather"
CATALOUGE  = omweather.por


#HEADERS += xml-qml.h 

SOURCES += xml-qml.cpp


FORMS    +=

OTHER_FILES += \


INCLUDEPATH += ../core
#LIBS += -L ../core -lomweather-core 
LIBS += -L ../core  ../core/libomweather-core.a


CONFIG = link_pkgconfig 
PKGCONFIG += sqlite3
PKGCONFIG += libcurl
PKGCONFIG += dbus-glib-1 \
             QtCore \
             QtXml


#install
target.path = /opt/com.meecast.omweather/bin
INSTALLS += target 

