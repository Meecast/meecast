#!/bin/bash
 ./openweathermaporg examples/10days.xml examples/out.xml examples/current.xml examples/hours.xml
#valgrind --tool=memcheck --log-file=1.log -v --track-origins=yes  --leak-check=full --show-reachable=yes  ./openweathermaporg examples/10days.xml examples/out.xml examples/current.xml examples/hours.xml

