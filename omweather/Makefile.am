#
# This file is part of Other Maemo Weather(omweather)
#
# Copyright (C) 2006 Vlad Vasiliev.
#
# This software is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public License
# version 2.1 as published by the Free Software Foundation.
#
# This software is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this software; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
# 02110-1301 USA
#


SUBDIRS =	src \
		data \
                po



EXTRA_DIST =					\
        autogen.sh				\
        make-package				\
        debian/changelog			\
        debian/compat				\
        debian/copyright			\
        debian/control				\
        debian/rules				
#	intltool-extract.in			\
#	intltool-merge.in			\
#	intltool-update.in			
	
#DISTCLEANFILES =				\
#	intltool-extract.in			\
#	intltool-merge.in			\
#	intltool-update.in			
    

deb:
	./make-package $(PACKAGE) $(VERSION)
