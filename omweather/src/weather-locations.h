/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006 Vlad Vasiliev
 * Copyright (C) 2006 Pavel Fialko
 * 	for the code
 *        
 * Copyright (C) 2008 Andrew Zhilin
 *		      az@pocketpcrussia.com 
 *	for default icon set (Glance)
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/
#ifndef _weather_locations_h
#define _weather_locations_h 1
/*******************************************************************************/
#include "weather-common.h"

GtkListStore* create_items_list(const char *filename, long start, long end,
				long *items_number);
int parse_country_string(const char *string, Country_item *result);
int parse_region_string(const char *string, Region_item *result);
int parse_station_string(const char *string, Station *result);
gboolean find_iterator(GtkListStore *store, gint field_number, GType type,
					gchar *field_value, GtkTreeIter *result);
/*******************************************************************************/
#endif
