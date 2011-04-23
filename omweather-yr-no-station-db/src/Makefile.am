# vim: set sw=4 ts=4 et:
#
# This file is part of omweather-gismeteo-ru-source
#
# Copyright (C) 2011 Vlad Vasiliev
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

libhhdir = /usr/lib/omweather
libhh_LTLIBRARIES = libomweather-yr-no-source.la
libomweather_yr_no_source_la_SOURCES = hash.c hash.h hash_description.data \
                                             hash_icons.data main.c main.h
libomweather_yr_no_source_la_CFLAGS = $(GTK_CFLAGS) $(LIBXML2_CFLAGS) \
					    -Wall -pedantic
libomweather_yr_no_source_la_LDFLAGS = -module -avoid-version 
libomweather_yr_no_source_la_LIBADD = $(GTK_LIBS) $(LIBXML2_LIBS) 

clean-local:
	$(RM) *.core core core.* stamp-*.h *~
