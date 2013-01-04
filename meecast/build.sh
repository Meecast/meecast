#!/bin/sh
CC=/home/additional_disk/emc/clang+llvm-3.0-i386-linux-Ubuntu-11_10/bin/clang
#cd core/libxml2
#/home/additional_disk/emc/emscripten/emconfigure ./autogen.sh --without-debug --without-ftp --without-http --without-python --without-regexps --without-threads --without-modules
#/home/additional_disk/emc/emscripten/emmake make
#cd -
#cd core/sqlite
#/home/additional_disk/emc/emscripten/emconfigure ./autogen.sh
#/home/additional_disk/emc/emscripten/emconfigure ./configure --enable-readline=no
#/home/additional_disk/emc/emscripten/emmake make
#cd -
#cd core
#/home/additional_disk/emc/emscripten/emconfigure ./autogen.sh
#/home/additional_disk/emc/emscripten/emconfigure ./configure
#/home/additional_disk/emc/emscripten/emmake make
#cd -
cd jstest
#/home/additional_disk/emc/emscripten/emconfigure ./autogen.sh
#/home/additional_disk/emc/emscripten/emconfigure ./configure
/home/additional_disk/emc/emscripten/emmake make
/home/additional_disk/emc/emscripten/emcc  --minify 1 -O0 ../core/libxml2/.libs/libxml2.a ../core/sqlite/.libs/libsqlite3.a  ../core/.libs/libmeecast-core.a src/meecasttizen-main.o -o meecast.js 
cd -


