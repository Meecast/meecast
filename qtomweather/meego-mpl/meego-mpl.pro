#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------


TARGET = meego-panel-omweather
TEMPLATE = app


SOURCES += meego-panel-omweather.cpp


FORMS    +=

OTHER_FILES += \


INCLUDEPATH += ../core
LIBS += -L ../core -lomweather-core
CONFIG = link_pkgconfig 
PKGCONFIG += dbus-glib-1 \
             mx-1.0 \
             mutter-plugins \
             meego-panel \
             QtCore

#install
target.path = /usr/libexec
desktop.files = data/meego-panel-omweather.desktop
desktop.path = /usr/share/mutter-meego/panels
autostart.files = data/meego-panel-omweather.desktop
autostart.path = /etc/xdg
css.files = data/omweather-panel.css
css.path = /usr/share/meego-panel-omweather/theme
INSTALLS += target desktop autostart css
