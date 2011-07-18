#!/bin/sh

qmake CONFIG+=localdebug
make
#cd ../../omweather-weather-com-stations-db/src
#make 
#wget -O BOXX0014.xml "http://ff.1click.weather.com/weather/local/BOXX0014?cm_ven=1CW&site=wx.com-bar&cm_ite=wx-cc&par=1CWFFv1.1.8&cm_pla=wx.com-bar&cm_cat=FFv1.1.8&unit=m&dayf=10&cc=*" && ./weathercom ./BOXX0014.xml ../../qtomweather/qt-qml/data.xml
#cd -

export LD_LIBRARY_PATH=$PWD/../core:$LD_LIBRARY_PATH
./qt-qml
#valgrind --leak-check=summary --log-file=/tmp/1.log ./omweather-qml
