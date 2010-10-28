VERSION = 1.0
TARGET = omweather-core
TEMPLATE = app 
CONFIG = link_pkgconfig \
    -qt
PKGCONFIG += libxml++-2.6 \
    libcurl
SOURCES = config.cpp \
    station.cpp \
    data.cpp \
    parser.cpp \
    source.cpp \
    downloader.cpp \
    connection.cpp \
    test.cpp
HEADERS = config.h \
    station.h \
    data.h \
    parser.h \
    source.h \
    downloader.h \
    connection.h \
    core.h
target.path = /usr/lib/omweather
INSTALLS += target
QMAKE_CLEAN += lib$$TARGET* \
    *.pro.user
