#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------


TARGET = OmweatherPlugin
TEMPLATE = lib

CONFIG += qt plugin
QT += declarative sql

TEXTDOMAIN = "omweather"
CATALOUGE  = omweather.por

SOURCES += omweatherplugin.cpp \
    configqml.cpp \
    stationmodel.cpp

HEADERS += \
    omweatherplugin.h \
    configqml.h \
    stationmodel.h

FORMS    +=

OTHER_FILES += \
    omweatherpanel.qml \
    omweatherpanel.dev.qml \
    Details.qml \
    qmldir \
    main.qml

INCLUDEPATH += ../core
#LIBS += -L ../core -lomweather-core
LIBS += -L ../core  ../core/libomweather-core.a  
LIBS += -lsqlite3

#install
target.path = /opt/com.meecast.omweather/lib/OmweatherPlugin

panel.files = data/omweather.panel
panel.path = /usr/share/meego-ux-panels/panels
desktop.files = data/omweather-settings.desktop
desktop.path = /usr/share/applications
qml.files = omweatherpanel.qml
qml.path = /opt/com.meecast.omweather/share/omweather/qml
qmldir.files = qmldir
qmldir.path = /opt/com.meecast.omweather/lib/OmweatherPlugin
settings.files = main.qml
settings.path = /usr/share/omweather-settings

INSTALLS += panel qml target qmldir settings desktop

