#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------


TARGET = OmweatherPlugin
TEMPLATE = lib

CONFIG += qt plugin
QT += declarative

TEXTDOMAIN = "omweather"
CATALOUGE  = omweather.por

SOURCES += omweatherplugin.cpp \
    configqml.cpp

HEADERS += \
    omweatherplugin.h \
    configqml.h

FORMS    +=

OTHER_FILES += \
    omweatherpanel.qml \
    omweatherpanel.dev.qml \
    Details.qml \
    qmldir

INCLUDEPATH += ../core
#LIBS += -L ../core -lomweather-core
LIBS += -L ../core  ../core/libomweather-core.a


#install
target.path = /opt/com.meecast.omweather/lib/OmweatherPlugin

panel.files = data/omweather.panel
panel.path = /usr/share/meego-ux-panels/panels
qml.files = omweatherpanel.qml
qml.path = /opt/com.meecast.omweather/share/omweather/qml
qmldir.files = qmldir
qmldir.path = /opt/com.meecast.omweather/lib/OmweatherPlugin

INSTALLS += panel qml target qmldir
