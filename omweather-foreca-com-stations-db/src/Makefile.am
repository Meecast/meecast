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

bin_PROGRAMS = forecacom 
forecacom_SOURCES  =  hash.c hash.h hash_description.data \
                                             hash_icons.data meego-main.c meego-main.h
forecacom_CFLAGS = $(GTK_CFLAGS) $(LIBXML2_CFLAGS) \
					    -Wall -pedantic
forecacom_LDFLAGS = -module -avoid-version 
forecacom_LDADD = $(GTK_LIBS) $(LIBXML2_LIBS) 

clean-local:
	$(RM) *.core core core.* stamp-*.h *~
