#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------


TARGET = predaemon 
TEMPLATE = app


TEXTDOMAIN = "omweather"
CATALOUGE  = omweather.por



HEADERS  += dbusadaptor_applet.h
SOURCES += dbusadaptor_applet.cpp predeamon.cpp 


FORMS    +=

OTHER_FILES += \


INCLUDEPATH += ../core
#LIBS += -L ../core -lomweather-core 
#LIBS += -L ../core  ../core/libomweather-core.a
LIBS += ../../omweather-fmi-fi-stations-db/tz.o ../../omweather-fmi-fi-stations-db/meego-main.o ../../omweather-fmi-fi-stations-db/jsoncpp.o ../../omweather-openweathermap-org-stations-db/meego-main.o ../../omweather-openweathermap-org-stations-db/hash.o ../../omweather-gismeteo-ru-stations-db/meego-main.o ../../omweather-gismeteo-ru-stations-db/hash.o ../../omweather-foreca-com-stations-db/meego-main.o ../../omweather-foreca-com-stations-db/hash.o ../../omweather-hko-gov-hk-stations-db/meego-main.o ../../omweather-hko-gov-hk-stations-db/hash.o ../../omweather-bom-gov-au-stations-db/meego-main.o ../../omweather-bom-gov-au-stations-db/hash.o ../../omweather-yr-no-stations-db/meego-main.o ../../omweather-yr-no-stations-db/hash.o ../../omweather-weather-com-stations-db/main.o


LIBS += -L ../core ../core/libomweather-core.a
CONFIG += link_pkgconfig \
         dbus \
         qdbus
PKGCONFIG += libcurl
PKGCONFIG += sqlite3
PKGCONFIG += libxml-2.0

system(pkg-config --exists Qt5Core) {
 PKGCONFIG += Qt5Core
}else {
 PKGCONFIG += QtCore
}

system(pkg-config --exists Qt5Xml) {
 PKGCONFIG += Qt5Xml
}else {
 PKGCONFIG += QtXml
}


#install
target.path = /opt/com.meecast.omweather/bin
INSTALLS += target 

