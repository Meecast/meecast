VERSION = 1.1.17
TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += meecast/sqlite3 meecast/libxml2 meecast/core omweather-weather-com-stations-db omweather-gismeteo-ru-stations-db omweather-yr-no-stations-db omweather-foreca-com-stations-db omweather-bom-gov-au-stations-db omweather-hko-gov-hk-stations-db omweather-openweathermap-org-stations-db omweather-fmi-fi-stations-db meecast 
# meecast/predeamon meecast/meecastdaemon meecast/qmlplugin meecast/event-components

SUBDIRS += meecast/predeamon meecast/meecastdaemon meecast/qmlplugin meecast/event-components
