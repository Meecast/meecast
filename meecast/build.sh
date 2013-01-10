#!/bin/sh 
CC=/home/additional_disk/emc/clang+llvm-3.0-i386-linux-Ubuntu-11_10/bin/clang
#cd core/zlib
#/home/additional_disk/emc/emscripten/emconfigure  ./configure --static
#/home/additional_disk/emc/emscripten/emmake make
#cd -
#cd core/libxml2
#/home/additional_disk/emc/emscripten/emconfigure ./autogen.sh --without-debug --without-ftp --without-http --without-python --without-regexps --without-threads --without-modules
#/home/additional_disk/emc/emscripten/emmake make
#cd -
#cd core/libxml2
#/home/additional_disk/emc/emscripten/emconfigure ./autogen.sh
#/home/additional_disk/emc/emscripten/emconfigure ./configure --enable-readline=no
#/home/additional_disk/emc/emscripten/emmake make
#cd -
cd core
#/home/additional_disk/emc/emscripten/emconfigure ./autogen.sh
#/home/additional_disk/emc/emscripten/emconfigure ./configure
/home/additional_disk/emc/emscripten/emmake make
cd -
cd jstest
#/home/additional_disk/emc/emscripten/emconfigure ./autogen.sh
#/home/additional_disk/emc/emscripten/emconfigure ./configure
/home/additional_disk/emc/emscripten/emmake make
/home/additional_disk/emc/emscripten/emcc  -s EXPORTED_FUNCTIONS="['_main', '_prepareconfig', '_preparedatabase', '_current_station_name', '_create_sources_list', '_currentstationname_js']" --minify 1 -O0 ../core/libxml2/.libs/libxml2.a \
                                       ../core/libxml2/.libs/libxml2.a \
                                       ../core/.libs/libmeecast-core.a \
                                       src/meecasttizen-main.o \
                                       ../core/.libs/libmeecast_core_la-abstractconfig.o \
                                       ../core/.libs/libmeecast_core_la-config.o \
                                       ../core/.libs/libmeecast_core_la-connection.o \
                                       ../core/.libs/libmeecast_core_la-data.o \
                                       ../core/.libs/libmeecast_core_la-databasesqlite.o \
                                       ../core/.libs/libmeecast_core_la-datalist.o \
                                       ../core/.libs/libmeecast_core_la-dataparser.o \
                                       ../core/.libs/libmeecast_core_la-downloader.o \
                                       ../core/.libs/libmeecast_core_la-parser.o \
                                       ../core/.libs/libmeecast_core_la-pressure.o \
                                       ../core/.libs/libmeecast_core_la-source.o \
                                       ../core/.libs/libmeecast_core_la-sourcelist.o \
                                       ../core/.libs/libmeecast_core_la-stationlist.o \
                                       ../core/.libs/libmeecast_core_la-station.o \
                                       ../core/.libs/libmeecast_core_la-temperature.o \
                                       ../core/.libs/libmeecast_core_la-windspeed.o \
                                        ../core/libxml2/.libs/buf.o \
                                        ../core/libxml2/.libs/c14n.o \
                                        ../core/libxml2/.libs/catalog.o \
                                        ../core/libxml2/.libs/chvalid.o \
                                        ../core/libxml2/.libs/debugXML.o \
                                        ../core/libxml2/.libs/dict.o \
                                        ../core/libxml2/.libs/encoding.o \
                                        ../core/libxml2/.libs/entities.o \
                                        ../core/libxml2/.libs/error.o \
                                        ../core/libxml2/.libs/globals.o \
                                        ../core/libxml2/.libs/hash.o \
                                        ../core/libxml2/.libs/HTMLparser.o \
                                        ../core/libxml2/.libs/HTMLtree.o \
                                        ../core/libxml2/.libs/legacy.o \
                                        ../core/libxml2/.libs/list.o \
                                        ../core/libxml2/.libs/nanoftp.o \
                                        ../core/libxml2/.libs/nanohttp.o \
                                        ../core/libxml2/.libs/parserInternals.o \
                                        ../core/libxml2/.libs/parser.o \
                                        ../core/libxml2/.libs/pattern.o \
                                        ../core/libxml2/.libs/relaxng.o \
                                        ../core/libxml2/.libs/SAX2.o \
                                        ../core/libxml2/.libs/SAX.o \
                                        ../core/libxml2/.libs/schematron.o \
                                        ../core/libxml2/.libs/testdso.o \
                                        ../core/libxml2/.libs/threads.o \
                                        ../core/libxml2/.libs/tree.o \
                                        ../core/libxml2/.libs/uri.o \
                                        ../core/libxml2/.libs/valid.o \
                                        ../core/libxml2/.libs/xinclude.o \
                                        ../core/libxml2/.libs/xlink.o \
                                        ../core/libxml2/.libs/xmlIO.o \
                                        ../core/libxml2/.libs/xmlmemory.o \
                                        ../core/libxml2/.libs/xmlmodule.o \
                                        ../core/libxml2/.libs/xmlreader.o \
                                        ../core/libxml2/.libs/xmlregexp.o \
                                        ../core/libxml2/.libs/xmlsave.o \
                                        ../core/libxml2/.libs/xmlschemas.o \
                                        ../core/libxml2/.libs/xmlschemastypes.o \
                                        ../core/libxml2/.libs/xmlstring.o \
                                        ../core/libxml2/.libs/xmlunicode.o \
                                        ../core/libxml2/.libs/xmlwriter.o \
                                        ../core/libxml2/.libs/xpath.o \
                                        ../core/libxml2/.libs/xpointer.o \
                                        ../core/libxml2/.libs/xzlib.o \
                                        ../core/zlib/adler32.o \
                                        ../core/zlib/compress.o \
                                        ../core/zlib/crc32.o \
                                        ../core/zlib/deflate.o \
                                        ../core/zlib/gzclose.o \
                                        ../core/zlib/gzlib.o \
                                        ../core/zlib/gzread.o \
                                        ../core/zlib/gzwrite.o \
                                        ../core/zlib/infback.o \
                                        ../core/zlib/inffast.o \
                                        ../core/zlib/inflate.o \
                                        ../core/zlib/inftrees.o \
                                        ../core/zlib/trees.o \
                                        ../core/zlib/uncompr.o \
                                        ../core/zlib/zutil.o \
                                        ../core/sqlite/sqlite3.o \
                                       src/meecasttizen-configefl.o  -o meecast.js \
                                       --js-library library.js --js-pre pre.js \
                                       --embed-file dbsources/yr.no.xml \
                                       --embed-file dbsources/bom.gov.au.xml \
                                       --embed-file dbsources/foreca.com.xml \
                                       --embed-file dbsources/weather.com.xml \
                                       --embed-file dbsources/gismeteo.ru.xml \
                                       --embed-file dbsources/hko.gov.hk.xml \
                                       --embed-file dbsources/yr.no.xml 
#/home/additional_disk/emc/emscripten/emcc  --minify 1 -O0  src/meecasttizen-main.o -o meecast.js 
cp meecast.js ../meecastjs/js


cd -


