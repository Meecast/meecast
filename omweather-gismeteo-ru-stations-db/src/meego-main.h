/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of omweather-weather-com-source - MeeCast
 *
 * Copyright (C) 2006-2012 Vlad Vasilyeu
 * Copyright (C) 2006-2009 Pavel Fialko
 * 	for the code
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details.
 *
 * You should have received a copy of the GNU  General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/
#ifndef _main_h
#define _main_h 1
/*******************************************************************************/
#include <libxml/parser.h>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <stdlib.h>
#ifdef GLIB
    #include <glib.h>
#endif
#ifdef QT
    #include <QHash>
#endif


#include <locale.h>
#include <sys/stat.h>
/*******************************************************************************/
//gint parse_xml_data(const gchar *station_id, xmlNode *root_node, GHashTable *data);
//void fill_detail_data(xmlNode *root_node, GHashTable *day, gint part_of_day, GHashTable *hash_for_translate, GHashTable *hash_for_icons)
//void  fill_day (xmlNode *root_node, GHashTable *day, gint part_of_day, GHashTable *hash_for_translate, GHashTable *hash_for_icons)
#ifdef GLIB
    GHashTable* hash_description_gismeteo_table_create(void);
    int get_station_weather_data(const char *station_id_with_path, GHashTable *data, gboolean get_detail_data);
    int parse_xml_data(const char *station_id, htmlDocPtr doc, GHashTable *data);
    int parse_xml_detail_data(const char *station_id, htmlDocPtr doc, GHashTable *data);
    GHashTable* hash_icons_gismeteo_table_create(void);
    gchar* hash_gismeteo_table_find(GHashTable *hash, char* key, gboolean search_short_name);
#endif
/*******************************************************************************/
#endif
