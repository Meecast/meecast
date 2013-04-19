#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------

QT       += declarative sql xml xmlpatterns network script svg  

#QT       += sql xml xmlpatterns network cascades

CONFIG += qt warn_on debug_and_release cascades
#CONFIG += qt warn_on debug_and_release 
TARGET = omweather-qml
TEMPLATE = app


SOURCES += main.cpp \
    dataqml.cpp \
    configqml.cpp \
    qmllayoutitem.cpp \
    dataitem.cpp \
    datamodel.cpp \
    controller.cpp \
    networkingcontrol.cpp \
    selectmodel.cpp \
    updatethread.cpp \
    countrymodel.cpp \
    regionmodel.cpp \
    citymodel.cpp \
    gpsposition.cpp \
    active.cpp

HEADERS  += \
    dataqml.h \
    configqml.h \
    qmllayoutitem.h \
    dataitem.h \
    datamodel.h \
    controller.h \
    networkingcontrol.h \
    selectmodel.h \
    updatethread.h \
    countrymodel.h \
    regionmodel.h \
    citymodel.h \
    gpsposition.h \
    active.h

#RESOURCES += weatherlayoutitem.qrc

OTHER_FILES += \
    qml/layoutitem.qml \
    qml/active_frame.qml \
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
    qml/SearchField.qml


lupdate_inclusion {
    SOURCES += ../assets/*.qml
}


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

OBJECTS_DIR = $${DESTDIR}/.obj
MOC_DIR = $${DESTDIR}/.moc
RCC_DIR = $${DESTDIR}/.rcc
UI_DIR = $${DESTDIR}/.ui
INSTALL_ROOT = $${DESTDIR}/.rcc

suredelete.target = sureclean
suredelete.commands = $(DEL_FILE) $${MOC_DIR}/*; $(DEL_FILE) $${RCC_DIR}/*; $(DEL_FILE) $${UI_DIR}/*
suredelete.depends = distclean

QMAKE_EXTRA_TARGETS += suredelete


#CONFIG(localdebug):DEFINES += LOCALDEBUG
#CONFIG += warn_on debug_and_release cascades
#CONFIG += qt warn_on  cascades
#CONFIG += mobility
#MOBILITY += location
#CONFIG += qdeclarative-boostable
#CONFIG += meegotouch 

QMAKE_CXXFLAGS += -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
#QMAKE_LFLAGS += -pie -rdynamic


INCLUDEPATH += ../core                                                                                                        
LIBS += -lsqlite3 -lcurl -lintl -L ../core lib/$${DESTDIR}/libomweather-core.a  
#CONFIG += qdbus
#CONFIG += link_pkgconfig
#PKGCONFIG += glib-2.0
#PKGCONFIG += sqlite3
#PKGCONFIG += libcurl
target.path = /opt/com.meecast.omweather/bin
INSTALLS += target

desktop.files = meecast.desktop
desktop.path = /usr/share/applications
icon64.path = /usr/share/pixmaps
icon64.files += omweather.png
qml.files = qml/*.qml *.js
qml.path = /opt/com.meecast.omweather/share/omweather/qml
searchicon.files += gfx/*.png
searchicon.path += /opt/com.meecast.omweather/share/omweather/qml/gfx

#INSTALLS += desktop qml icon64
INSTALLS += desktop qml searchicon

DATADIR=/opt/com.meecast.omweather/share

