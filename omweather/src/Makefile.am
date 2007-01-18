# This file is part of Other Maemo Weather(omweather)
#
# Copyright (C) 2006 Vlad Vasiliev.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#

SUBDIRS = lib 

# The Home plugin

libhhdir = $(libdir)/hildon-home
libhh_LTLIBRARIES = libweather-home.la

libweather_home_la_SOURCES = weather-home.c \
			     weather-home.h \
			     weather-home_parser.c \
			     weather-home_config.c \
			     weather-home_preference.c \
			     weather-home_config.h \
			     weather-home_prefernce.h \
			     weather-home_common.h \
			     weather-event.c \
			     weather-event.h \
			     weather-window_popup.c \
			     weather-window_popup.h

libweather_home_la_CFLAGS = $(GTK_CFLAGS) $(OSSO_CFLAGS) $(LIBXML2_CFLAGS) $(GNOME_VFS_CFLAGS) $(GCONF_CFLAGS) -Ilib -Wall -pedantic
libweather_home_la_LDFLAGS = -module -avoid-version 
libweather_home_la_LIBADD = $(GTK_LIBS) $(LIBXML2_LIBS) $(GNOME_VFS_LIBS) $(GCONF_LIBS) lib/.libs/libweather.la

clean-local:
	$(RM) *.core core core.* stamp-*.h *~


