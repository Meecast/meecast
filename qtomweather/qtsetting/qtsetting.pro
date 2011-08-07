#-------------------------------------------------
#
# Project created by QtCreator 2011-01-27T14:47:03
#
#-------------------------------------------------

QT       += core gui dbus

TARGET = omweather-settings
TEMPLATE = app


SOURCES += main.cpp\
        setting.cpp \
    settingstations.cpp

HEADERS  += setting.h \
    settingstations.h

FORMS    += setting.ui \
    settingstations.ui

QT += sql xml xmlpatterns 

CONFIG(localdebug):DEFINES += LOCALDEBUG

CONFIG += link_pkgconfig
PKGCONFIG += sqlite3
PKGCONFIG += libcurl

INCLUDEPATH += ../core
LIBS += -L ../core ../core/libomweather-core.a

RESOURCES += omweather-settings.qrc

TRANSLATIONS += omweather-settings_ru.qm

#install
#installs
#desktop.files = data/desktop/omweather-settings.desktop
#desktop.path = /usr/share/applications



target.path = /opt/com.meecast.omweather/bin

INSTALLS += target

OTHER_FILES += \
    ComboBox.qml \
    setting.qml
