VERSION = 1.0
TARGET = omweather-core
TEMPLATE = lib
CONFIG += link_pkgconfig staticlib

CONFIG += qdeclarative-boostable
CONFIG += qt-boostable 


system(pkg-config --exists libxml++-2.6) {
    CONFIG += -qt
    PKGCONFIG += libxml++-2.6
    DEFINES += LIBXML1
    QT += xml
    
} else {
    message("Not libxml++-2.6")
    QT += xml
}


linux:!android {
    message("* Using settings for Unix/Linux.")
    PKGCONFIG += sqlite3
    PKGCONFIG += libcurl
    DEFINES += LIBCURL

}
android {
    DEFINES += INTERNALSQLITE3
}

CONFIG(localdebug):DEFINES += LOCALDEBUG

QMAKE_CXXFLAGS += -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
QMAKE_LFLAGS += -pie -rdynamic

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
    windspeed.cpp \
    visible.cpp \
    pressure.cpp


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
    windspeed.h \
    visible.h \
    pressure.h


icon.files = data/desktop/Icon/meecast.svg
icon.path =  /opt/com.meecast.omweather/share/icons/

icon50.files = data/desktop/Icon/omweather.png
icon50.path = /opt/com.meecast.omweather/share/icons/

icon255.files = data/desktop/Icon/omweather255.png
icon255.path = /opt/com.meecast.omweather/share/icons/

iconpanel.files = data/desktop/Icon/pnl_icn_omweather.png
iconpanel.path = /opt/com.meecast.omweather/share/icons/

icon16.files = data/desktop/Icon/16/omweather.png
icon16.path = /usr/share/icons/hicolor/16x16/apps/

icon32.files = data/desktop/Icon/32/omweather.png
icon32.path = /usr/share/icons/hicolor/32x32/apps/

icon64.files = data/desktop/Icon/64/omweather.png
icon64.path = /usr/share/icons/hicolor/64x64/apps/

icon128.files = data/desktop/Icon/128/omweather.png
icon128.path = /usr/share/icons/hicolor/128x128/apps/

target.path = /opt/com.meecast.omweather/lib
#data.files = data/icons/
databeginning.files += data/icons/Beginning/*.png
databeginning.path = /opt/com.meecast.omweather/share/iconsets/Beginning
datacontour.files += data/icons/Contour/*.png
datacontour.path = /opt/com.meecast.omweather/share/iconsets/Contour
dataepona.files += data/icons/Epona/*.png
dataepona.path = /opt/com.meecast.omweather/share/iconsets/Epona
dataglance.files += data/icons/Glance/*.png
dataglance.path = /opt/com.meecast.omweather/share/iconsets/Glance
datameecast.files += data/icons/Meecast/*.png
datameecast.path = /opt/com.meecast.omweather/share/iconsets/Meecast
datagrzankas.files += data/icons/Grzankas/*.png
datagrzankas.path = /opt/com.meecast.omweather/share/iconsets/Grzankas
datashiny.files += data/icons/Shiny/*.png
datashiny.path = /opt/com.meecast.omweather/share/iconsets/Shiny
datatango.files += data/icons/Tango/*.png
datatango.path = /opt/com.meecast.omweather/share/iconsets/Tango
datasimpelmee.files += data/icons/SimpelMee/*.png
datasimpelmee.path = /opt/com.meecast.omweather/share/iconsets/SimpelMee
datanoun.files += data/icons/Noun/*.png
datanoun.path = /opt/com.meecast.omweather/share/iconsets/Noun
dataatmos.files += data/icons/Atmos/*.png
dataatmos.path = /opt/com.meecast.omweather/share/iconsets/Atmos

donation.files += data/donation.html
donation.path = /opt/com.meecast.omweather/share/html


#data.path = /opt/com.meecast.omweather/share
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

INSTALLS += target databeginning datacontour dataepona dataglance datameecast datagrzankas datashiny datatango datasimpelmee datanoun dataatmos xsd buttonicons images alib icon icon50 icon255 icon16 icon32 icon64 icon128 iconpanel donation
QMAKE_CLEAN += lib$$TARGET* \
    *.pro.user


