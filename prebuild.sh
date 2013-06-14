#!/bin/bash
echo "prebuild.sh"


cp ../omweather-openweathermap-org-stations-db/data/openweathermap.org.db ../res/db
cp ../omweather-openweathermap-org-stations-db/data/openweathermap.org.xml ../res/sources
cp ../omweather-gismeteo-ru-stations-db/data/gismeteo.ru.db ../res/db
cp ../omweather-gismeteo-ru-stations-db/data/gismeteo.ru.xml ../res/sources

cp ../meecast/core/data/source.xsd ../res/data/
mkdir  ../res/screen-size-normal/images
cp ../meecast/core/data/images/mask_background_main.png ../res/screen-size-normal/images
cp ../meecast/core/data/images/arrow_left.png ../res/screen-size-normal/images
cp ../meecast/core/data/images/arrow_right.png ../res/screen-size-normal/images
rm -rf ../src/core
cp -r ../meecast/core ../src/core 
rm -rf ../src/tizen2
cp -r ../meecast/tizen2 ../src/tizen2
