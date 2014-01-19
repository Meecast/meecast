#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------

QT       += declarative sql xml xmlpatterns network positioning qml

TARGET = harbour-meecast 
TEMPLATE = app




exists($$QMAKE_INCDIR_QT"/../qmsystem2/qmkeys.h"):contains(MEEGO_EDITION,harmattan): {
    DEFINES += MEEGO_EDITION_HARMATTAN
}

exists("/usr/lib/qt5/qml/Sailfish/Silica/SilicaGridView.qml"): {
    DEFINES += SAILFISHOS 
}
SOURCES += main.cpp \
    dataqml.cpp \
    configqml.cpp \
    qmllayoutitem.cpp \
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
    qmllayoutitem.h \
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
    translations/omweather_ru_RU.qm




CONFIG(localdebug):DEFINES += LOCALDEBUG

CONFIG += mobility
MOBILITY += location
CONFIG += qdeclarative-boostable
CONFIG += meegotouch 

QMAKE_CXXFLAGS += -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
QMAKE_LFLAGS += -pie -rdynamic 


DATADIR=/usr/share/harbour-meecast
INCLUDEPATH += ../core                                                                                                        

#include(sailfishapplication/sailfishapplication.pri)


LIBS += -L ../core ../core/libomweather-core.a  
CONFIG += dbus
CONFIG += qdbus
CONFIG += link_pkgconfig
CONFIG += sailfishapp
PKGCONFIG += glib-2.0
PKGCONFIG += sqlite3
PKGCONFIG += libcurl
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
include(sailfishapplication/sailfishapplication.pri)

#INSTALLS += desktop qml icon64
INSTALLS += target desktop_ translations qml

