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
    omweatherpanel.qml \
    omweatherpanel.dev.qml \
    Details.qml



#install
panel.files = data/omweather.panel
panel.path = /usr/share/meego-ux-panels/panels
qml.files = omweatherpanel.qml
qml.path = /opt/com.meecast.omweather/share/omweather/qml

INSTALLS += panel qml
