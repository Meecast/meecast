# This file is part of Other Maemo Weather(omweather)
#
# Copyright (C) 2006 Vlad Vasiliev.
# Copyright (C) 2006 Pavel Fialko.
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


# The Home plugin
if OS2008 
libhhdir = $(libdir)/hildon-desktop
else
libhhdir = $(libdir)/hildon-home
endif

libhh_LTLIBRARIES = libweather-home.la

libweather_home_la_SOURCES = weather-home.c \
			     weather-home.h \
			     weather-parser.h \
			     weather-parser.c \
			     weather-config.h \
			     weather-config.c \
			     weather-settings.h \
			     weather-settings.c \
			     weather-common.h \
			     weather-event.c \
			     weather-event.h \
			     weather-hash.h \
			     weather-hash.c \
			     weather-download.h \
			     weather-download.c \
			     weather-locations.h \
			     weather-locations.c \
			     weather-data.h \
			     weather-data.c \
			     weather-popup.h \
			     weather-popup.c \
			     weather-gps.h \
			     weather-gps.c \
			     weather-gpsdistance.h \
			     weather-gpsdistance.c \
			     weather-help.h \
			     weather-help.c \
			     weather-utils.h \
			     weather-utils.c \
			     weather-sensor.h \
			     weather-sensor.c \
			     weather-applet-expose.c \
			     weather-applet-expose.h

weather-hash.c:		hash.data
	if test -f hash.data; then \
	    TMP_FILE=`mktemp`; \
	    sort -bu hash.data > $$TMP_FILE; \
	    rm hash.data; \
	    cat $$TMP_FILE > hash.data; \
	    rm $$TMP_FILE; \
	fi

weather-help.c:	build
	if test -f build; then \
	    rm build; \
	    echo "#define BUILD \""`date +%j`"\"" > build; \
	fi

libweather_home_la_CFLAGS = $(GTK_CFLAGS) $(OSSO_CFLAGS) $(LIBXML2_CFLAGS) $(GNOME_VFS_CFLAGS) \
			    $(GCONF_CFLAGS) $(CONIC_CFLAGS) $(DBUS_CFLAGS) $(HILDONDESKTOP_CFLAGS) \
			    $(DESKTOP_CFLAGS) $(OSSOIC_CFLAGS) $(LOCATION_CFLAGS) $(LIBOSSOHELP_CFLAGS) \
			    -Wall -pedantic -Wmissing-prototypes -DDISPLAY_BUILD

libweather_home_la_LDFLAGS =  -module -avoid-version 

libweather_home_la_LIBADD = $(GTK_LIBS) $(LIBXML2_LIBS) $(GNOME_VFS_LIBS) $(GCONF_LIBS) $(DBUS_LIBS) \
			    $(LIBCURL_LIBS) $(CONIC_LIBS) $(HILDONDESKTOP_LIBS) $(DESKTOP_LIBS)  $(OSSOIC_LIBS) \
			    $(LOCATION_LIBS) $(LIBOSSOHELP_LIBS)

clean-local:
	$(RM) *.core core core.* stamp-*.h *~


