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
cp ../omweather-yr-no-stations-db/data/yr.no.db ../res/db
cp ../omweather-yr-no-stations-db/data/yr.no.xml ../res/sources


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
#rm -rf ../src/core
#cp -r ../meecast/core ../src/core 
#rm -rf ../src/tizen2
#cp -r ../meecast/tizen2 ../src/tizen2
#cp ../po/*.po ../res/locales




mkdir -p ../res/locales/ar/LC_MESSAGES
mkdir -p ../res/locales/bg/LC_MESSAGES
mkdir -p ../res/locales/ca/LC_MESSAGES
mkdir -p ../res/locales/de/LC_MESSAGES
mkdir -p ../res/locales/en_GB/LC_MESSAGES
mkdir -p ../res/locales/en_US/LC_MESSAGES
mkdir -p ../res/locales/es_ES/LC_MESSAGES
mkdir -p ../res/locales/es_MX/LC_MESSAGES
mkdir -p ../res/locales/fi/LC_MESSAGES
mkdir -p ../res/locales/fr/LC_MESSAGES
mkdir -p ../res/locales/hu/LC_MESSAGES
mkdir -p ../res/locales/it/LC_MESSAGES
mkdir -p ../res/locales/lv/LC_MESSAGES
mkdir -p ../res/locales/nl/LC_MESSAGES
mkdir -p ../res/locales/no/LC_MESSAGES
mkdir -p ../res/locales/ru/LC_MESSAGES
mkdir -p ../res/locales/pl/LC_MESSAGES
mkdir -p ../res/locales/pt/LC_MESSAGES
mkdir -p ../res/locales/sk/LC_MESSAGES
mkdir -p ../res/locales/sq/LC_MESSAGES
mkdir -p ../res/locales/sr/LC_MESSAGES
mkdir -p ../res/locales/sv/LC_MESSAGES
mkdir -p ../res/locales/tr/LC_MESSAGES
mkdir -p ../res/locales/vi/LC_MESSAGES
mkdir -p ../res/locales/zh_ZH/LC_MESSAGES
mkdir -p ../res/locales/zh_HK/LC_MESSAGES
mkdir -p ../res/locales/zh_TW/LC_MESSAGES
mkdir -p ../res/locales/zh/LC_MESSAGES

msgfmt -o ../res/locales/ar/LC_MESSAGES/omweather.mo ../po/ar_AR.po 
msgfmt -o ../res/locales/bg/LC_MESSAGES/omweather.mo ../po/bg_BG.po 
msgfmt -o ../res/locales/ca/LC_MESSAGES/omweather.mo ../po/ca_CA.po 
msgfmt -o ../res/locales/de/LC_MESSAGES/omweather.mo ../po/de_DE.po 
msgfmt -o ../res/locales/en_GB/LC_MESSAGES/omweather.mo ../po/en_GB.po 
msgfmt -o ../res/locales/en_US/LC_MESSAGES/omweather.mo ../po/en_US.po 
msgfmt -o ../res/locales/es_ES/LC_MESSAGES/omweather.mo ../po/es_ES.po 
msgfmt -o ../res/locales/es_MX/LC_MESSAGES/omweather.mo ../po/es_MX.po 
msgfmt -o ../res/locales/fi/LC_MESSAGES/omweather.mo ../po/fi_FI.po 
msgfmt -o ../res/locales/fr/LC_MESSAGES/omweather.mo ../po/fr_FR.po 
msgfmt -o ../res/locales/hu/LC_MESSAGES/omweather.mo ../po/hu_HU.po 
msgfmt -o ../res/locales/it/LC_MESSAGES/omweather.mo ../po/it_IT.po 
msgfmt -o ../res/locales/lv/LC_MESSAGES/omweather.mo ../po/lv_LV.po 
msgfmt -o ../res/locales/nl/LC_MESSAGES/omweather.mo ../po/nl_NL.po 
msgfmt -o ../res/locales/no/LC_MESSAGES/omweather.mo ../po/no_NO.po 
msgfmt -o ../res/locales/ru/LC_MESSAGES/omweather.mo ../po/ru_RU.po 
msgfmt -o ../res/locales/pl/LC_MESSAGES/omweather.mo ../po/pl_PL.po 
msgfmt -o ../res/locales/pt/LC_MESSAGES/omweather.mo ../po/pt_PT.po 
msgfmt -o ../res/locales/sk/LC_MESSAGES/omweather.mo ../po/sk_SK.po 
msgfmt -o ../res/locales/sq/LC_MESSAGES/omweather.mo ../po/sq_SQ.po 
msgfmt -o ../res/locales/sr/LC_MESSAGES/omweather.mo ../po/sr_SR.po 
msgfmt -o ../res/locales/sv/LC_MESSAGES/omweather.mo ../po/sv_SV.po 
msgfmt -o ../res/locales/tr/LC_MESSAGES/omweather.mo ../po/tr_TR.po 
msgfmt -o ../res/locales/vi/LC_MESSAGES/omweather.mo ../po/vi_VI.po 
msgfmt -o ../res/locales/zh_HK/LC_MESSAGES/omweather.mo ../po/zh_HK.po 
msgfmt -o ../res/locales/zh_TW/LC_MESSAGES/omweather.mo ../po/zh_TW.po 
msgfmt -o ../res/locales/zh_ZH/LC_MESSAGES/omweather.mo ../po/zh_ZH.po 
msgfmt -o ../res/locales/zh/LC_MESSAGES/omweather.mo ../po/zh_ZH.po 

