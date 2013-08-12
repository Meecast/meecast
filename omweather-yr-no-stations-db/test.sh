#!/bin/bash
# ./yrno examples/varsel.xml out.xml examples/varsel.xml 
valgrind --tool=memcheck --log-file=1.log -v --track-origins=yes  --leak-check=full --show-reachable=yes ./yrno examples/varsel.xml out.xml examples/varsel.xml 

