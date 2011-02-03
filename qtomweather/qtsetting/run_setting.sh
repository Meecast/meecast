#!/bin/sh
#qmake CONFIG+=localdebug  
make
export LD_LIBRARY_PATH=$PWD/../core:$LD_LIBRARY_PATH
./omweather-settings
