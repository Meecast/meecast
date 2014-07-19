#!/bin/bash
valgrind --tool=memcheck --log-file=1.log -v --track-origins=yes  --leak-check=full --show-reachable=yes ./fmifi examples/weatherdata.json out.xml  
