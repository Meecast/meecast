VERSION = 1.1.17
TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += omweather-weather-com-stations-db omweather-gismeteo-ru-stations-db omweather-yr-no-stations-db omweather-foreca-com-stations-db omweather-bom-gov-au-stations-db omweather-hko-gov-hk-stations-db omweather-openweathermap-org-stations-db omweather-fmi-fi-stations-db meecast meecast/sqlite3 meecast/libxml2 meecast/core 
# meecast/libxml2 meecast/core meecast/predeamon meecast/meecastdaemon meecast/qmlplugin meecast/event-components

android: include(/home/vlad/android-sdk/android_openssl/openssl.pri)
