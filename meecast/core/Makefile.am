# vim: set sw=4 ts=4 et:
#
# This file is part of MeeCast 
#
# Copyright (C) 2012 Vlad Vasilyeu
# 	for the code
#
# This software is free software; you can redistribute it and/or
# modify it under the terms of the GNU  General Public License
# as published by the Free Software Foundation; either version 2.1 of
# the License, or (at your option) any later version.
#
# This software is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
#  General Public License for more details.
#
# You should have received a copy of the GNU  General Public
# License along with this software; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
# 02110-1301 USA

libhhdir = /lib
libhh_LTLIBRARIES = libmeecast-core.la
libmeecast_core_la_SOURCES = abstractconfig.cpp \
abstractconfig.h \
autogen.sh \
config.cpp \
connection.cpp \
connection.h \
core.h \
databaseabstract.h \
databasesqlite.cpp \
databasesqlite.h \
data.cpp \
data.h \
datalist.cpp \
datalist.h \
dataparser.cpp \
dataparser.h \
downloader.cpp \
downloader.h \
parser.cpp \
parser.h \
pressure.cpp \
pressure.h \
source.cpp \
source.h \
sourcelist.cpp \
sourcelist.h \
station.cpp \
station.h \
stationlist.cpp \
stationlist.h \
temperature.cpp \
temperature.h \
windspeed.cpp \
windspeed.h 
                             
libmeecast_core_la_CPPFLAGS = $(GTK_CFLAGS) $(LIBXML2_CFLAGS) \
					    -Wall -pedantic
libmeecast_core_la_LDFLAGS = -module -avoid-version 
libmeecast_core_la_LIBADD = $(GTK_LIBS) $(LIBXML2_LIBS) 

clean-local:
	$(RM) *.core core core.* stamp-*.h *~
