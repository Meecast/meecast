VERSION = 1.0
TARGET = omweather-core
TEMPLATE = lib
PKGCONFIG += libcurl
PKGCONFIG += sqlite3
CONFIG += link_pkgconfig staticlib

CONFIG += qdeclarative-boostable
CONFIG += qt-boostable 

DEPLOYMENT_PATH = /usr/share/org.meecast.MeeCast/



DEFINES += AURORA 

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

QMAKE_CXXFLAGS += -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
QMAKE_LFLAGS += -pie -rdynamic

 
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


#icon.files = data/desktop/Icon/meecast.svg
#icon.path =  /usr/share/icons/
#
#icon50.files = data/desktop/Icon/omweather.png
#icon50.path = /usr/share/icons/
#
#icon255.files = data/desktop/Icon/omweather255.png
#icon255.path = /usr/share/icons/
#
#iconpanel.files = data/desktop/Icon/pnl_icn_omweather.png
#iconpanel.path = /usr/share/icons/
#
#icon16.files = data/desktop/Icon/16/omweather.png
#icon16.path = /usr/share/icons/hicolor/16x16/apps/
#
#icon32.files = data/desktop/Icon/32/omweather.png
#icon32.path = /usr/share/icons/hicolor/32x32/apps/
#
#icon64.files = data/desktop/Icon/64/omweather.png
#icon64.path = /usr/share/icons/hicolor/64x64/apps/
#
#icon128.files = data/desktop/Icon/128/org.meecast.MeeCast.png
#icon128.path = /usr/share/icons/hicolor/128x128/apps/
#
#icon86.files = data/desktop/Icon/86/org.meecast.MeeCast.png
#icon86.path = /usr/share/icons/hicolor/86x86/apps/
#
#icon108.files = data/desktop/Icon/108/org.meecast.MeeCast.png
#icon108.path = /usr/share/icons/hicolor/108x108/apps/
#
#icon172.files = data/desktop/Icon/172/org.meecast.MeeCast.png
#icon172.path = /usr/share/icons/hicolor/172x172/apps/
#
#
#icon256.files = data/desktop/Icon/256/org.meecast.MeeCast.png
#icon256.path = /usr/share/icons/hicolor/256x256/apps/

#target.path = /usr/share/org.meecast.MeeCast/lib

#data.files = data/icons/
databeginning.files += data/icons/Beginning/*.png
databeginning.path = $$DATAPATH/share/iconsets/Beginning
datacontour.files += data/icons/Contour/*.png
datacontour.path = /usr/share/org.meecast.MeeCast/iconsets/Contour
dataepona.files += data/icons/Epona/*.png
dataepona.path = /usr/share/org.meecast.MeeCast/iconsets/Epona
dataglance.files += data/icons/Glance/*.png
dataglance.path = /usr/share/org.meecast.MeeCast/iconsets/Glance
datameecast.files += data/icons/Meecast/*.png
datameecast.path = /usr/share/org.meecast.MeeCast/iconsets/Meecast
datagrzankas.files += data/icons/Grzankas/*.png
datagrzankas.path = /usr/share/org.meecast.MeeCast/iconsets/Grzankas
datashiny.files += data/icons/Shiny/*.png
datashiny.path = /usr/share/org.meecast.MeeCast/iconsets/Shiny
datatango.files += data/icons/Tango/*.png
datatango.path = /usr/share/org.meecast.MeeCast/iconsets/Tango
datasimpelmee.files += data/icons/SimpelMee/*.png
datasimpelmee.path = /usr/share/org.meecast.MeeCast/iconsets/SimpelMee
datanoun.files += data/icons/Noun/*.png
datanoun.path = /usr/share/org.meecast.MeeCast/iconsets/Noun
dataatmos.files += data/icons/Atmos/*.png
dataatmos.path = /usr/share/org.meecast.MeeCast/iconsets/Atmos

donation.files += data/donation.html
donation.path = /usr/share/org.meecast.MeeCast/html


#data.path = $$DATAPATH
xsd.files += data/config.xsd
xsd.files += data/data.xsd
xsd.files += data/source.xsd
xsd.path = /usr/share/org.meecast.MeeCast/xsd
buttonicons.path = /usr/share/org.meecast.MeeCast/buttons_icons
buttonicons.files += data/button_icons/*.png
images.path = /usr/share/org.meecast.MeeCast/images
images.files += data/images/*.png
#alib.files = libomweather-core.a
#alib.path = /usr/share/org.meecast.MeeCast/lib

#INSTALLS += target databeginning datacontour dataepona dataglance datameecast datagrzankas datashiny datatango datasimpelmee datanoun dataatmos xsd buttonicons images  icon50  icon16 icon32 icon64 icon108 icon128 icon172 icon256 icon86 donation
INSTALLS += target databeginning datacontour dataepona dataglance datameecast datagrzankas datashiny datatango datasimpelmee datanoun dataatmos xsd buttonicons images donation
QMAKE_CLEAN += lib$$TARGET* \
    *.pro.user


