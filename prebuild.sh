#!/bin/bash
echo "prebuild.sh"


cp ../omweather-openweathermap-org-stations-db/data/openweathermap.org.db ../data/db
cp ../omweather-openweathermap-org-stations-db/data/openweathermap.org.xml ../data/sources
cp ../omweather-gismeteo-ru-stations-db/data/gismeteo.ru.db ../data/db
cp ../omweather-gismeteo-ru-stations-db/data/gismeteo.ru.xml ../data/sources

rm -rf ../src/core
cp -r ../meecast/core ../src/core 
rm -rf ../src/tizen2
cp -r ../meecast/tizen2 ../src/tizen2
