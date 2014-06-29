#!/bin/bash
wget -O examples/forecast.xml http://www.yr.no/sted/Belarus/Vitsyebsk/Vitebsk/varsel.xml
wget -O examples/forecast_detail.xml http://www.yr.no/sted/Hviterussland/Vitsiebsk/Vitebsk/varsel_time_for_time.xml
 ./yrno examples/forecast.xml out.xml examples/forecast_detail.xml 
#valgrind --tool=memcheck --log-file=1.log -v --track-origins=yes  --leak-check=full --show-reachable=yes ./yrno examples/varsel.xml out.xml examples/varsel.xml 

