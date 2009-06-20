/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2009 Vlad Vasiliev
 * Copyright (C) 2006-2009 Pavel Fialko
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
#ifndef _weather_sources_h
#define _weather_sourcess_h 1
/*******************************************************************************/
#include "weather-common.h"
/*******************************************************************************/
GtkListStore* create_sources_list(gchar *sources_path, gint *sources_number,
				    GSList **handles);
GHashTable* parse_source_file(const gchar *filename, const gchar *encoding);
void unload_parsers(GSList *list);
gboolean source_name_valid(GHashTable *data);
gboolean source_library_valid(GHashTable *data, GSList **handles);
gboolean source_forecast_url_valid(GHashTable *data);
gboolean source_detail_url_valid(GHashTable *data);
gboolean source_search_url_valid(GHashTable *data);
gboolean source_stations_database_valid(GHashTable *data);
gboolean source_logo_file_valid(GHashTable *data);
void free_hashtable_with_source(GHashTable* hashtable);
void parse_children(xmlNode *node, GHashTable *object);
/*******************************************************************************/
#endif

