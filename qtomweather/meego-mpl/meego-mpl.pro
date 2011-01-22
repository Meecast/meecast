#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------


TARGET = meego-mpl
TEMPLATE = app


SOURCES += panel.c


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
