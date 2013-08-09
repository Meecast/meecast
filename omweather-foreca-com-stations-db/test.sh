#!/bin/bash
# ./forecacom ./examples/forcast.html ./examples/out.xml ./examples/forcast_detail.html
valgrind --tool=memcheck --log-file=1.log -v --track-origins=yes  --leak-check=full --show-reachable=yes ./forecacom ./examples/forcast.html ./examples/out.xml ./examples/forcast_detail.html  
