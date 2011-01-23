#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------


TARGET = meego-panel-omweather
TEMPLATE = app


SOURCES += meego-panel-omweather.c


FORMS    +=

OTHER_FILES += \


#INCLUDEPATH += ../core
#LIBS += -L ../core -lomweather-core
CONFIG = link_pkgconfig -qt
PKGCONFIG += dbus-glib-1 \
             mx-1.0 \
             gtk+-2.0 \
             mutter-plugins \
             meego-panel \
             gio-unix-2.0

#install
target.path = $$[install_prefix]/libexec
desktop.files = data/meego-panel-omweather.desktop
desktop.path = $$[install_prefix]/share/mutter-meego/panels
autostart.files = data/meego-panel-omweather.desktop
autostart.path = /etc/xdg

INSTALLS += target desktop autostart
