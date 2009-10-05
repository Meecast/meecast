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

helpfile_dede_DATA = de_DE/omweather.xml
helpfile_enus_DATA = en_US/omweather.xml
helpfile_engb_DATA = en_GB/omweather.xml
helpfile_ruru_DATA = ru_RU/omweather.xml
helpfile_fifi_DATA = fi_FI/omweather.xml
helpfile_frfr_DATA = fr_FR/omweather.xml
helpfile_itit_DATA = it_IT/omweather.xml
helpfile_eses_DATA = es_ES/omweather.xml
helpfile_esmx_DATA = es_MX/omweather.xml

EXTRA_DIST = $(helpfile_engb_DATA) $(helpfile_enus_DATA)
ALL_LINGUAS=ru_RU fr_FR fi_FI en_GB en_US it_IT de_DE es_ES es_MX


#  # Replace version in help files
%.xml: %.xml.in
	sed "s/__VERSION__/$(VERSION)/g" $< >$@


#Remark for work in Fremantle Alpha
#%.xml.in:
#	for lang in $(ALL_LINGUAS); do \
#	    ./intltool-merge4help -x "po/$$lang.po" omweather.xml.in "$$lang/omweather.xml.in" ; \
#	done
#Temporary for Fremantle
%.xml.in:
	for lang in $(ALL_LINGUAS); do \
	    cp omweather.xml.in "$$lang/omweather.xml.in" ; \
	done


clean:
	for lang in $(ALL_LINGUAS); do \
	    rm -f "$$lang/omweather.xml.in" "$$lang/omweather.xml" ; \
	done


