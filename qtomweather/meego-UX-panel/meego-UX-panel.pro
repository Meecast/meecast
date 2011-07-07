#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------


TARGET = meego-UX-panel
TEMPLATE = app


TEXTDOMAIN = "omweather"
CATALOUGE  = omweather.por



SOURCES += main.c 


FORMS    +=

OTHER_FILES += \



#install
panel.files = data/omweather.panel
panel.path = /usr/share/meego-ux-panels/panels

INSTALLS += panel

