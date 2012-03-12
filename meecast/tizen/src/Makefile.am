# vim: set sw=4 ts=4 et:
#
# This file is part of omweather-foreca-com-source
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

bin_PROGRAMS = meecasttizen 
meecasttizen_SOURCES  =  main.cpp main.h main_window.cpp main_window.h
meecasttizen_CXXFLAGS = $(GTK_CFLAGS) $(LIBXML2_CFLAGS) $(SQLITE_CFLGAS) $(LIBCURL_CFLAGS) \
                       $(ELEMENTARY_CFLAGS) $(EDJE_CFLAGS) -I../../core 
meecasttizen_LDFLAGS = -avoid-version  
meecasttizen_LDADD = -L../../core/.libs/ ../../core/.libs/libmeecast-core.a \
                     $(ELEMENTARY_LIBS) $(GTK_LIBS) $(LIBXML2_LIBS) $(SQLITE_LIBS) $(LIBCURL_LIBS) \
                     $(EDJE_LIBS)


EDJE_CC = edje_cc
EDJE_FLAGS = -v -id ../../core/data -fd $(top_srcdir)/edc
files_DATA = mainwindow.edj
filesdir = $(datadir)/edje
EXTRA_DIST = edc/mainwindow.edc

mainwindow.edj: Makefile edc/mainwindow.edc
	$(EDJE_CC) $(EDJE_FLAGS) \
	edc/mainwindow.edc \
	mainwindow.edj

desktopdir    = /opt/share/applications
desktop_files = com.meecast.omweather.desktop
desktop_DATA  = $(desktop_files)

icondir    = /opt/apps/com.meecast.omweather/res/icons/default/small/
icon_files = com.meecast.omweather.png
icon_DATA  = $(icon_files)

clean-local: $(RM)  *.edj *.core core core.* stamp-*.h *~
