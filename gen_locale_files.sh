#!/bin/bash

ALL_LANGUAGES="ru_RU fi_FI fr_FR en_GB en_US it_IT nl_NL de_DE es_ES es_MX pl_PL pt_PT bg_BG lv_LV sk_SK sv_SV sr_SR tr_TR sq_SQ vi_VI zh_ZH zh_HK zh_TW ca_CA ar_AR no_NO hu_HU uk_UA cs_CS sl_SL da_DK gl_GL"

#xgettext  --from-code=UTF-8 --keyword=_ --keyword=Q_ --keyword=tr -f po/POTFILES.in -s -C -o po/omweather.pot
#cd po
#for language in $ALL_LANGUAGES; do
#    echo -n "$language: "
#    msgmerge -U $language.po omweather.pot
#done
#cd ..


for language in $ALL_LANGUAGES; do
    cp po/$language.po meecast/qt-qml/po/locale/$language
#   cp po/$language.po meecast/qt-qml/po/$language.po
    po2ts -c QObject po/$language.po meecast/qt-qml/translations/omweather_$language.ts
done


cp -r po/*.po omweather/po
cp -r po/*.pot omweather/po

cd meecast/qt-qml/translations/
lrelease -nounfinished omweather_*.ts
cd -

exit 0
