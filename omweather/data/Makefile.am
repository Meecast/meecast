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
 
omweatherdir = $(datadir)/omweather
omweather_DATA = city_in_clock.txt countries.list regions.list locations.list 
icons_crystallargedir = $(datadir)/omweather/icons/Glance
icons_crystallarge_DATA = icons/Glance/*.png 
moon_iconsdir = $(datadir)/omweather/moon_icons
moon_icons_DATA = moon_icons/*.png
homeapplet_desktopdir = $(datadir)/applications/hildon-home
if OS2008 
homeapplet_desktop_DATA = os2008/omweather-home.desktop
else
homeapplet_desktop_DATA = os2007/omweather-home.desktop
endif
EXTRA_DIST = $(homeapplet_desktop_DATA) \
	     $(omweather_DATA) \
	     $(icons_crystallarge_DATA) \
	     $(moon_icons_DATA) \
	     omweather.png

SUBDIRS = help
