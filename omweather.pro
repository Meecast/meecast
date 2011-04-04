VERSION = 0.3.12
TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += qtomweather omweather-weather-com-stations-db omweather-gismeteo-ru-stations-db

desktop.files = omweather.desktop
desktop.path = /usr/share/applications

icon.files = Icon/omweather.png
icon.path = /usr/share/icons/

icon16.files = Icon/16/omweather.png
icon16.path = /usr/share/icons/hicolor/16x16/apps/

icon32.files = Icon/32/omweather.png
icon32.path = /usr/share/icons/hicolor/32x32/apps/

icon64.files = Icon/64/omweather.png
icon64.path = /usr/share/icons/hicolor/64x64/apps/

icon128.files = Icon/128/omweather.png
icon128.path = /usr/share/icons/hicolor/128x128/apps/

INSTALLS += icon icon16 icon32 icon64 icon128 desktop
