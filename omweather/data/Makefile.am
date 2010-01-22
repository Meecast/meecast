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
omweather_DATA = city_in_clock.txt style.rc
icons_crystallargedir = $(datadir)/omweather/icons/Glance
icons_crystallarge_DATA = icons/Glance/*.png 
clutter_scripts_crystallargedir = $(datadir)/omweather/icons/Glance
clutter_scripts_crystallarge_DATA = icons/Glance/*.json
animation_crystallargedir = $(datadir)/omweather/icons/Glance
animation_crystallarge_DATA = icons/Glance/animation.xml
moon_iconsdir = $(datadir)/omweather/moon_icons
moon_icons_DATA = moon_icons/*.png
button_iconsdir = $(datadir)/omweather/button_icons
button_icons_DATA = button_icons/*.png
backupdir = /etc/osso-backup/applications
backup_DATA = omweather.conf
#if NONMAEMO
imagedir = $(datadir)/omweather/images
image_DATA = images/*.png
#endif

# The icons

icondir = $(datadir)/icons/hicolor

install-data-local:
	i=omweather; \
	$(mkinstalldirs) $(DESTDIR)$(icondir)/26x26/hildon; \
	$(mkinstalldirs) $(DESTDIR)$(icondir)/40x40/hildon; \
	$(mkinstalldirs) $(DESTDIR)$(icondir)/scalable/hildon; \
	$(INSTALL_DATA) $(srcdir)/desktop_icons/$$i.26.png $(DESTDIR)$(icondir)/26x26/hildon/$$i.png; \
	$(INSTALL_DATA) $(srcdir)/desktop_icons/$$i.40.png $(DESTDIR)$(icondir)/40x40/hildon/$$i.png; \
	$(INSTALL_DATA) $(srcdir)/desktop_icons/$$i.64.png $(DESTDIR)$(icondir)/scalable/hildon/$$i.png;

uninstall-local:
	i=omweather; \
	rm $(DESTDIR)$(icondir)/26x26/hildon/$$i.png; \
	rm $(DESTDIR)$(icondir)/40x40/hildon/$$i.png; \
	rm $(DESTDIR)$(icondir)/scalable/hildon/$$i.png;


dbus_servicedir = $(datadir)/dbus-1/services
dbus_service_DATA = com.nokia.omweather.service

desktopdir    = $(datadir)/applications/hildon
desktop_files = omweather.desktop
desktop_DATA  = $(desktop_files)



#homeapplet_desktopdir = $(DESKTOPDIR)
#homeapplet_desktopdir = /usr/share/applications/hildon-home
if OS2008
    homeapplet_desktopdir = /usr/share/omweather
    homeapplet_desktop_DATA = os2008/omweather-home.desktop
else
if OS2009
    homeapplet_desktopdir = /usr/share/omweather
    homeapplet_desktop_DATA = os2009/omweather-home.desktop
else
    homeapplet_desktopdir = /usr/share/applications/hildon-home
    homeapplet_desktop_DATA = os2007/omweather-home.desktop
endif
endif

EXTRA_DIST = $(homeapplet_desktop_DATA) \
	     $(omweather_DATA) \
	     $(icons_crystallarge_DATA) \
	     $(moon_icons_DATA) \
	     $(clutter_scripts_crystallarge_DATA) \
	     $(animation_crystallarge_DATA) \
	     $(button_icons_DATA) \
	     units.xml \
	     $(dbus_service_DATA) \
	     omweather.desktop.in \
	     omweather.png \
	     $(image_DATA) \
	     $(backup_DATA)

omweather.desktop: omweather.desktop.in
	sed "s/__VERSION__/$(VERSION)/g" $< >$@.tmp && \
	sed "s/__PACKAGE__/$(PACKAGE)/g" $@.tmp >$@ && \
	$(RM) *.tmp

if OS2009
else
    SUBDIRS = help
endif
