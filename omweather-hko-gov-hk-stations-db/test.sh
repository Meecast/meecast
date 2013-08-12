#!/bin/bash
# ./hkogovhk ./examples/nday.htm ./examples/out.xml ./examples/englishwx2.htm

valgrind --tool=memcheck --log-file=1.log -v --track-origins=yes  --leak-check=full --show-reachable=yes ./hkogovhk ./examples/nday.htm ./examples/out.xml ./examples/englishwx2.htm

