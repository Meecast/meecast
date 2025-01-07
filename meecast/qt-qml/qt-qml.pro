#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------

#QT += sql xml xmlpatterns network qml 
QT += sql xml xmlpatterns network qml positioning

TARGET = harbour-meecast 
TEMPLATE = app


exists($$QMAKE_INCDIR_QT"/../qmsystem2/qmkeys.h"):contains(MEEGO_EDITION,harmattan): {
    DEFINES += MEEGO_EDITION_HARMATTAN
}

exists("/usr/lib/qt5/qml/Sailfish/Silica/SilicaGridView.qml"): {
    DEFINES += SAILFISHOS 
}

exists("/usr/lib64/qt5/qml/Sailfish/Silica/SilicaGridView.qml"): {
    DEFINES += SAILFISHOS
}

SOURCES += main.cpp \
    dataqml.cpp \
    configqml.cpp \
    dataitem.cpp \
    datamodel.cpp \
    dbusadaptor.cpp \
    dbusadaptor_applet.cpp \
    controller.cpp \
    networkingcontrol.cpp \
    selectmodel.cpp \
    updatethread.cpp \
    countrymodel.cpp \
    regionmodel.cpp \
    citymodel.cpp \
    gpsposition.cpp \
    meecastcover.cpp 

HEADERS  += \
    dataqml.h \
    configqml.h \
    dataitem.h \
    datamodel.h \
    dbusadaptor.h \
    dbusadaptor_applet.h \
    controller.h \
    networkingcontrol.h \
    selectmodel.h \
    updatethread.h \
    countrymodel.h \
    regionmodel.h \
    citymodel.h \
    gpsposition.h \
    meecastcover.h


FORMS    +=
#RESOURCES += weatherlayoutitem.qrc

OTHER_FILES += \
    qml/layoutitem.qml \
    qml/weatherlayoutitem.qml \
    qml/Details.qml \
    qml/ImageButton.qml \
    qml/TextButton.qml \
    qml/omweather.qml \
    qml/omweatherpanel.qml \
    qml/main.qml \
    qml/WeatherPage.qml \
    qml/SettingsPage.qml \
    qml/FullWeatherPage.qml \
    qml/AboutPage.qml \
    qml/StationsPage.qml \
    qml/NewStationPage.qml \
    qml/UnitsPage.qml \
    qml/VisualsPage.qml \
    qml/ListPage.qml \
    qml/SourcePage.qml \
    qml/CountryPage.qml \
    qml/RegionPage.qml \
    qml/CityPage.qml \
    qml/IconsetPage.qml \
    qml/StandbyScreen.qml \
    qml/ColorCell.qml \
    qml/LanguagesPage.qml \
    qml/wrapper.qml \
    qml/NeedDaemonDialog.qml \
    translations/omweather_ru_RU.qm




CONFIG(localdebug):DEFINES += LOCALDEBUG

CONFIG += mobility
MOBILITY += location
CONFIG += qdeclarative-boostable
CONFIG += meegotouch 

QMAKE_CXXFLAGS += -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
QMAKE_LFLAGS += -pie -rdynamic 


DATADIR=/usr/share/harbour-meecast
INCLUDEPATH += ../core ../sqlite3 



LIBS += ../../omweather-fmi-fi-stations-db/tz.o ../../omweather-fmi-fi-stations-db/meego-main.o ../../omweather-fmi-fi-stations-db/jsoncpp.o ../../omweather-openweathermap-org-stations-db/meego-main.o ../../omweather-openweathermap-org-stations-db/hash.o ../../omweather-gismeteo-ru-stations-db/meego-main.o ../../omweather-gismeteo-ru-stations-db/hash.o ../../omweather-foreca-com-stations-db/meego-main.o ../../omweather-foreca-com-stations-db/hash.o ../../omweather-hko-gov-hk-stations-db/meego-main.o ../../omweather-hko-gov-hk-stations-db/hash.o ../../omweather-bom-gov-au-stations-db/meego-main.o ../../omweather-bom-gov-au-stations-db/hash.o ../../omweather-yr-no-stations-db/meego-main.o ../../omweather-yr-no-stations-db/hash.o ../../omweather-weather-com-stations-db/main.o  ../sqlite3/sqlite3.o





LIBS += -L ../core ../core/libomweather-core.a  ../libxml2/liblibxml2.a  -lsailfishapp  
CONFIG += dbus
CONFIG += qdbus
CONFIG += link_pkgconfig
CONFIG += sailfishapp
PKGCONFIG += glib-2.0
PKGCONFIG += libcurl
PKGCONFIG += zlib
#CONFIG += static
#CONFIG += staticlib

target.path = /usr/bin
DEPLOYMENT_PATH = /usr/share/harbour-meecast

desktop_.files = harbour-meecast.desktop
desktop_.path = /usr/share/applications
icon64.path = /usr/share/pixmaps
icon64.files += omweather.png
qml.files = qml/*.qml *.js
qml.path = $$DEPLOYMENT_PATH/qml
translations.files = translations/*.qm
translations.path = $$DEPLOYMENT_PATH/locale/
# Please do not modify the following line.
#include(sailfishapplication/sailfishapplication.pri)

#INSTALLS += desktop qml icon64
INSTALLS += target desktop_ translations qml

