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

bin_PROGRAMS = omweather

omweather_SOURCES = 	weather-home.c \
			weather-home.h \
			weather-config.h \
			weather-config.c \
			weather-settings.h \
			weather-settings.c \
			weather-simple_settings.h \
			weather-simple_settings.c \
			weather-common.h \
			weather-event.c \
			weather-event.h \
			weather-hash.h \
			weather-hash.c \
			weather-download.h \
			weather-download.c \
			weather-stations.h \
			weather-stations.c \
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
			weather-applet-expose.h \
			weather-alerts.h \
			weather-alerts.c \
			weather-sources.h \
			weather-sources.c \
			weather-main.h \
			weather-main.c \
			weather-clutter.h \
			weather-clutter.c \
			weather-portrait.h \
			weather-portrait.c \
			weather-dbus.h \
			weather-dbus.c \
			weather-presets.c \
			weather-presets.h


omweather_CFLAGS =	$(GTK_CFLAGS) \
			$(OSSO_CFLAGS) \
			$(DBUS_CFLAGS) \
			$(GNOME_VFS_CFLAGS) \
			$(GCONF_CFLAGS) \
			$(LIBXML2_CFLAGS) \
			$(HILDON_CFLAGS) \
			$(CONIC_CFLAGS) \
			$(LIBOSSOHELP_CFLAGS) \
			$(LIBCURL_CFLAGS) \
			$(SQLITE_CFLAGS) \
			$(HILDONDESKTOP_CFLAGS) \
			$(LOCATION_CFLAGS) \
			$(CLUTTER_CFLAGS) \
			-DAPPLICATION \
			-Wall -pedantic \
			-Wmissing-prototypes -DDEBUGFUNCTIONCALL

omweather_LDADD =	$(GTK_LIBS) \
			$(OSSO_LIBS) \
			$(DBUS_LIBS) \
			$(GNOME_VFS_LIBS) \
			$(GCONF_LIBS) \
			$(LIBXML2_LIBS) \
			$(HILDON_LIBS) \
			$(CONIC_LIBS) \
			$(SQLITE_LIBS) \
			$(LIBOSSOHELP_LIBS) \
			$(CLUTTER_LIBS) \
			$(LOCATION_LIBS) \
			$(LIBCURL_LIBS)

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
	    echo "#define BUILD \""`date +%V`"\"" > build; \
	fi

if !NONMAEMO
# The Home plugin
if OS2008 
libhhdir = $(libdir)/hildon-desktop
else
libhhdir = $(libdir)/hildon-home
endif

libhh_LTLIBRARIES = libweather-home.la

libweather_home_la_SOURCES  = 	weather-home.c \
			weather-home.h \
			weather-config.h \
			weather-config.c \
			weather-settings.h \
			weather-settings.c \
			weather-simple_settings.h \
			weather-simple_settings.c \
			weather-common.h \
			weather-event.c \
			weather-event.h \
			weather-hash.h \
			weather-hash.c \
			weather-download.h \
			weather-download.c \
			weather-stations.h \
			weather-stations.c \
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
			weather-applet-expose.h \
			weather-alerts.h \
			weather-alerts.c \
			weather-sources.h \
			weather-sources.c \
			weather-main.h \
			weather-main.c \
			weather-clutter.h \
			weather-clutter.c \
			weather-portrait.h \
			weather-portrait.c \
			weather-dbus.h \
			weather-dbus.c \
			weather-presets.c \
			weather-presets.h


libweather_home_la_CFLAGS =  $(GTK_CFLAGS) \
			$(OSSO_CFLAGS) \
			$(DBUS_CFLAGS) \
			$(GNOME_VFS_CFLAGS) \
			$(GCONF_CFLAGS) \
			$(LIBXML2_CFLAGS) \
			$(HILDON_CFLAGS) \
			$(CONIC_CFLAGS) \
			$(LIBOSSOHELP_CFLAGS) \
			$(LIBCURL_CFLAGS) \
			$(LOCATION_CFLAGS) \
			$(SQLITE_CFLAGS) \
			$(CLUTTER_CFLAGS) \
			$(HILDONDESKTOP_CFLAGS) \
			-Wall -pedantic \
			-Wmissing-prototypes

libweather_home_la_LDFLAGS =  -module -avoid-version 


libweather_home_la_LIBADD = $(GTK_LIBS) \
			    $(LIBXML2_LIBS) \
			    $(GNOME_VFS_LIBS) \
			    $(GCONF_LIBS) \
			    $(DBUS_LIBS) \
			    $(LIBCURL_LIBS) \
			    $(CONIC_LIBS) \
			    $(HILDONDESKTOP_LIBS) \
			    $(DESKTOP_LIBS) \
			    $(OSSOIC_LIBS) \
			    $(LOCATION_LIBS) \
			    $(LIBOSSOHELP_LIBS) \
			    $(CLUTTER_LIBS)
endif


clean-local:
	$(RM) *.core core core.* stamp-*.h *~

