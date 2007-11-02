#
# This file is part of omweather
#
# Copyright (C) 2007 Vlad Vasiliev
#
## This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# # the Free Software Foundation; either version 2 of the License, or
# # (at your option) any later version.
# #
# # This program is distributed in the hope that it will be useful,
# # but WITHOUT ANY WARRANTY; without even the implied warranty of
# # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# # GNU General Public License for more details.
# #
# # You should have received a copy of the GNU General Public License along
# # with this program; if not, write to the Free Software Foundation, Inc.,
# # 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
# #
#  

helpfile_enus_DATA = en_US/omweather.xml
helpfile_engb_DATA = en_GB/omweather.xml

EXTRA_DIST = $(helpfile_engb_DATA) $(helpfile_enus_DATA)

xmldir = $(datadir)/xml

xml_in_files = omweather.xml.in
xml_DATA = $(xml_in_files:.xml.in=.xml)

@INTLTOOL_XML_RULE@

#EXTRA_DIST=$(xml_in_files) $(xml_DATA)

#  # Replace version in help files
%.xml: %.xml.in
	sed "s/__VERSION__/$(VERSION)/g" $< >$@

#%.xml.in: ../%.xml.en
#	cat $< >$@

