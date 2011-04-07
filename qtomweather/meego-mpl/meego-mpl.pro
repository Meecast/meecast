#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------


TARGET = meego-panel-omweather
TEMPLATE = app


TEXTDOMAIN = "omweather"
CATALOUGE  = omweather.por


HEADERS += meego-netbookUX-omweather.h \
           meego-netbookUX-common.h 

SOURCES += meego-panel-omweather.cpp \
           meego-netbookUX-omweather.cpp 


FORMS    +=

OTHER_FILES += \


INCLUDEPATH += ../core
#LIBS += -L ../core -lomweather-core 
LIBS += -L ../core  ../core/libomweather-core.a


CONFIG = link_pkgconfig 
PKGCONFIG += sqlite3
PKGCONFIG += libcurl
PKGCONFIG += dbus-glib-1 \
             mx-1.0 \
             mutter-plugins \
             meego-panel \
             QtCore \
             QtXml


#install
target.path = /opt/com.meecast.omweather/libexec
desktop.files = data/meego-panel-omweather.desktop
desktop.path = /usr/share/mutter-meego/panels
autostart.files = data/meego-panel-omweather
autostart.path = /etc/xdg/autostart
css.files = data/omweather-panel.css
css.path = /opt/com.meecast.omweather/share/meego-panel-omweather/theme
service.files = data/com.meego.UX.Shell.Panels.omweather.service
service.path = /usr/share/dbus-1/services
INSTALLS += target desktop autostart css service

