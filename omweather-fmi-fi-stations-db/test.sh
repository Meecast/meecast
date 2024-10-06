#!/bin/bash
./fmifi examples/Helsinki.html ./out.xml examples/Helsinki_detail.xml 
#valgrind --tool=memcheck --log-file=1.log -v --track-origins=yes  --leak-check=full --show-reachable=yes ./fmifi examples/Helsinki.html ./out.xml examples/Helsinki_detail.xml 
