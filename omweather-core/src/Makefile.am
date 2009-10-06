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

libhhdir = $(libdir)

libhh_LTLIBRARIES = libomweather-core.la

libomweather_core_la_SOURCES = omweather-config.hpp \
                                omweather-config.cpp \
                                omweather-common.hpp \
                                omweather-common.cpp \
                                omweather-events.hpp \
                                omweather-events.cpp \
                                omweather-stations.hpp \
                                omweather-stations.cpp \
                                omweather-sensor.hpp \
                                omweather-sensor.cpp \
                                omweather-sources.hpp \
                                omweather-sources.cpp \
                                omweather-main.hpp \
                                omweather-main.cpp \
                                omweather-database.hpp \
                                omweather-database.cpp

libomweather_core_la_CXXFLAGS = $(LIBXML2_CFLAGS) \
                                $(LIBCURL_CFLAGS) \
                                $(SQLITE_CFLAGS) \
                                -Wall -pedantic

libomweather_core_la_LDFLAGS = -module -avoid-version

libomweather_core_la_LIBADD = $(LIBXML2_LIBS) \
                                $(LIBCURL_LIBS)

#omweather-text.cpp:     omweather-text.data
#	if test -f omweather-text.data; then \
#		TMP_FILE=`mktemp`; \
#		sort -bu omweather-text.data > $$TMP_FILE; \
#		rm omweather-text.data; \
#		cat $$TMP_FILE > omweather-text.data; \
#		rm $$TMP_FILE; \
#	fi
clean-local:
	$(RM) *.core core core.* stamp-*.h *~