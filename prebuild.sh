#!/bin/bash
echo "prebuild.sh"


cp ../omweather-openweathermap-org-stations-db/data/openweathermap.org.db ../res/db
cp ../omweather-openweathermap-org-stations-db/data/openweathermap.org.xml ../res/sources
cp ../omweather-gismeteo-ru-stations-db/data/gismeteo.ru.db ../res/db
cp ../omweather-gismeteo-ru-stations-db/data/gismeteo.ru.xml ../res/sources

cp ../src/core/data/source.xsd ../res/data/
rm -rf ../src/core
cp -r ../meecast/core ../src/core 
rm -rf ../src/tizen2
cp -r ../meecast/tizen2 ../src/tizen2
