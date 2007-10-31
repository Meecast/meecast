#!/bin/bash

xgettext --keyword=_ -f po/POTFILES.in -s -C -o po/omweather.pot
cd po
for language in ru_RU fi_FI fr_FR en_GB en_US; do
    echo -n "$language: "
    msgmerge -U $language.po omweather.pot
done
cd ..