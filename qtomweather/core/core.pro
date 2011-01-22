VERSION = 1.0
TARGET = omweather-core
TEMPLATE = lib
PKGCONFIG += libcurl
CONFIG += link_pkgconfig 



system(pkg-config --exists libxml++-2.6) {
    CONFIG += -qt
    PKGCONFIG += libxml++-2.6
    DEFINES += LIBXML
    QT += xml
    
} else {
    message("Not libxml++-2.6")
    QT += xml
}
 
SOURCES = \
    abstractconfig.cpp \ 
    config.cpp \
    station.cpp \
    data.cpp \
    parser.cpp \
    parserqt.cpp \
    source.cpp \
    downloader.cpp \
    connection.cpp \
    dataparser.cpp \
    temperature.cpp \
    datalist.cpp \ 
    dataparserqt.cpp
HEADERS = \ 
    abstractconfig.h \ 
    config.h \
    station.h \
    data.h \
    parser.h \
    parserqt.h \
    source.h \
    downloader.h \
    connection.h \
    datalist.h \
    core.h \
    dataparser.h \
    temperature.h \
    dataparserqt.h
target.path = /usr/lib/omweather
INSTALLS += target
QMAKE_CLEAN += lib$$TARGET* \
    *.pro.user
