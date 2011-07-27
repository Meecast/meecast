VERSION = 1.0
TARGET = omweather-core
TEMPLATE = lib
PKGCONFIG += libcurl
PKGCONFIG += sqlite3
CONFIG += link_pkgconfig staticlib



system(pkg-config --exists libxml++-2.6) {
    CONFIG += -qt
    PKGCONFIG += libxml++-2.6
    DEFINES += LIBXML1
    QT += xml
    
} else {
    message("Not libxml++-2.6")
    QT += xml
}

CONFIG(localdebug):DEFINES += LOCALDEBUG

system(pkg-config --exists meego-panel) {
 DEFINES += MEEGO_MPL
} 
 
SOURCES = \
    abstractconfig.cpp \ 
    config.cpp \
    station.cpp \
    stationlist.cpp \
    sourcelist.cpp \
    data.cpp \
    parser.cpp \
    source.cpp \
    connection.cpp \
    dataparser.cpp \
    temperature.cpp \
    datalist.cpp \
    databasesqlite.cpp \
    downloader.cpp \
    windspeed.cpp


HEADERS = \ 
    abstractconfig.h \ 
    config.h \
    station.h \
    stationlist.h \
    sourcelist.h \
    data.h \
    parser.h \
    source.h \
    connection.h \
    datalist.h \
    core.h \
    dataparser.h \
    temperature.h \
    databasesqlite.h \
    databaseabstract.h \
    downloader.h \
    windspeed.h


#installs
desktop.files = data/desktop/omweather-settings.desktop
desktop.path = /usr/share/applications

icon.files = data/desktop/Icon/omweather.png
icon.path = /usr/share/icons/

icon50.files = data/desktop/Icon/omweather.png
icon50.path = /opt/com.meecast.omweather/share/icons/

icon255.files = data/desktop/Icon/omweather255.png
icon255.path = /opt/com.meecast.omweather/share/icons/

icon16.files = data/desktop/Icon/16/omweather.png
icon16.path = /usr/share/icons/hicolor/16x16/apps/

icon32.files = data/desktop/Icon/32/omweather.png
icon32.path = /usr/share/icons/hicolor/32x32/apps/

icon64.files = data/desktop/Icon/64/omweather.png
icon64.path = /usr/share/icons/hicolor/64x64/apps/

icon128.files = data/desktop/Icon/128/omweather.png
icon128.path = /usr/share/icons/hicolor/128x128/apps/

target.path = /opt/com.meecast.omweather/lib
data.files = data/icons
data.path = /opt/com.meecast.omweather/share
xsd.files += data/config.xsd
xsd.files += data/data.xsd
xsd.files += data/source.xsd
xsd.path = /opt/com.meecast.omweather/share/xsd
buttonicons.path = /opt/com.meecast.omweather/share/buttons_icons
buttonicons.files += data/button_icons/*.png
images.path = /opt/com.meecast.omweather/share/images
images.files += data/images/*.png
alib.files = libomweather-core.a
alib.path = /opt/com.meecast.omweather/lib

INSTALLS += target data xsd buttonicons images alib icon icon50 icon255 icon16 icon32 icon64 icon128 desktop
QMAKE_CLEAN += lib$$TARGET* \
    *.pro.user
