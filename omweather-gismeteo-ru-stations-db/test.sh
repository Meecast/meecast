#!/bin/bash
 ./gismeteoru ./examples/weekly.html ./examples/out.xml ./examples/hourly.html
#valgrind --tool=memcheck --log-file=1.log -v --track-origins=yes  --leak-check=full --show-reachable=yes ./forecacom ./examples/forecast.html ./examples/out.xml ./examples/forecast_detail.html  
