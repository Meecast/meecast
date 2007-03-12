#
# This file is part of omweather
#
# Copyright (C) 2006 Vlad Vasiliev
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
SUBDIRS = 24x24 36x36 scalable
 
helpfile_engb_DATA = help/en_US/maemoweather.xml
test_testdir = $(datadir)/omweather
test_test_DATA = stations.txt iso3166-countrycodes.txt
icons_crystallargedir = $(datadir)/omweather/icons/Crystal
icons_crystallarge_DATA = icons/Crystal/*.png 
homeapplet_desktopdir = $(datadir)/applications/hildon-home
homeapplet_desktop_DATA = omweather-home.desktop

EXTRA_DIST = $(homeapplet_desktop_DATA) $(helpfile_engb_DATA) $(test_test_DATA) $(icons_crystallarge_DATA) 

