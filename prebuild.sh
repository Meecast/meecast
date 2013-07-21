#!/bin/bash
echo "prebuild.sh"


cp ../omweather-openweathermap-org-stations-db/data/openweathermap.org.db ../res/db
cp ../omweather-openweathermap-org-stations-db/data/openweathermap.org.xml ../res/sources
cp ../omweather-gismeteo-ru-stations-db/data/gismeteo.ru.db ../res/db
cp ../omweather-gismeteo-ru-stations-db/data/gismeteo.ru.xml ../res/sources

cp ../omweather-foreca-com-stations-db/data/foreca.com.db ../res/db
cp ../omweather-foreca-com-stations-db/data/foreca.com.xml ../res/sources
cp ../omweather-hko-gov-hk-stations-db/data/hko.gov.hk.db ../res/db
cp ../omweather-hko-gov-hk-stations-db/data/hko.gov.hk.xml ../res/sources

#cp ../omweather-bom-gov-au-stations-db/data/bom.gov.au.db ../res/db
#cp ../omweather-bom-gov-au-stations-db/data/bom.gov.au.xml ../res/sources



cp ../meecast/core/data/source.xsd ../res/data/
mkdir  ../res/screen-size-normal/images
mkdir  ../res/screen-size-normal/icons
cp ../meecast/core/data/images/mask_background_main.png ../res/screen-size-normal/images
cp ../meecast/core/data/images/humidity.png ../res/screen-size-normal/images
cp -r ../meecast/core/data/icons/Atmos/* ../res/screen-density-xhigh/icons/Atmos
cp ../meecast/core/data/images/arrow_left.png ../res/screen-size-normal/images
cp ../meecast/core/data/images/arrow_right.png ../res/screen-size-normal/images
rm -rf ../src/core
cp -r ../meecast/core ../src/core 
rm -rf ../src/tizen2
cp -r ../meecast/tizen2 ../src/tizen2
#cp ../po/*.po ../res/locales
