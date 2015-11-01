VERSION = 1.0
TARGET = libxml2 
TEMPLATE = lib
CONFIG += link_pkgconfig staticlib


#target.path = /usr/share/harbour-meecast/lib

SOURCES += *.c
HEADERS  += *.h

INCLUDEPATH += include                                                                                                        

#system(./autogen.sh)
#system(make)


QMAKE_CXXFLAGS += -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
QMAKE_LFLAGS += -pie -rdynamic
INSTALLS += target 
QMAKE_CLEAN += lib$$TARGET* \
    *.pro.user


