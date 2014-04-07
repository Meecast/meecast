#!/bin/bash
#Vladivostok
#wget -O examples/forecast_detail.html "http://foreca.mobi/spot.php?l=102013348"
#wget -O examples/forecast_detail.html "http://foreca.mobi/spot.php?l=101850147"
#Vitebsk
wget -O examples/forecast_detail.html "http://foreca.mobi/spot.php?l=100620127"
#wget -O examples/forecast_detail.html "http://foreca.mobi/spot.php?l=105368361"
# ./forecacom ./examples/forecast.html ./examples/out.xml ./examples/forecast_detail.html
# ./forecacom ./examples/forecast_detail.html.old ./examples/out.xml ./examples/forecast_detail.html
valgrind --tool=memcheck --log-file=1.log -v --track-origins=yes  --leak-check=full --show-reachable=yes ./forecacom ./examples/forecast.html ./examples/out.xml ./examples/forecast_detail.html  
#valgrind --tool=memcheck --log-file=1.log -v --track-origins=yes  --leak-check=full --show-reachable=yes ./forecacom ./examples/foreca.com_100658225.orig ./examples/out.xml ./examples/foreca.com_100658225.detail.orig  
