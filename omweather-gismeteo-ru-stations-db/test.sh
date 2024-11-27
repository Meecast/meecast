#!/bin/bash
 # ./gismeteoru ./examples/weekly.html ./examples/out.xml ./examples/hourly.html
 ./gismeteoru ./examples/weekly.html ./examples/out.xml 
#valgrind --tool=memcheck --log-file=1.log -v --track-origins=yes  --leak-check=full --show-reachable=yes ./gismeteoru ./examples/weekly.html ./examples/out.xml ./examples/hourly.html

