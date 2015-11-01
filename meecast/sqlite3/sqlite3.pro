VERSION = 1.0
TARGET = sqlite3 
TEMPLATE = lib
CONFIG += link_pkgconfig staticlib

QMAKE_CFLAGS += -DSQLITE_OMIT_LOAD_EXTENSION=1

#target.path = /usr/share/harbour-meecast/lib

SOURCES += sqlite3.c
HEADERS  += sqlite3.h




QMAKE_CXXFLAGS += -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
QMAKE_LFLAGS += -pie -rdynamic
INSTALLS += target 
QMAKE_CLEAN += lib$$TARGET* \
    *.pro.user


