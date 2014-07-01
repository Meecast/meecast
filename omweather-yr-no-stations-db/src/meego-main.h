/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of omweather-weather-com-source - MeeCast
 *
 * Copyright (C) 2006-2013 Vlad Vasilyeu
 * 	for the code
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU  General Public License
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
#ifdef GLIB
    #include <glib.h>
#endif
#ifdef QT
    #include <QHash>
#endif

#include <libxml/parser.h>
#include <stdlib.h>
#include <limits.h>
/*******************************************************************************/
int source_init(void);
void source_destroy(void);
void free_fields( void* key, void* val, void* user_data);
#ifdef GLIB
    int get_station_weather_data(const char *station_id_with_path, GHashTable *data, int get_detail_data);
#endif
int parse_and_write_yrno_xml_data(const char *station_id, xmlNode *root_node, const char *result_file);
#ifdef GLIB
    GHashTable* hash_description_yrno_table_create(void);
    GHashTable* hash_icons_yrno_table_create(void);
    gpointer hash_yrno_table_find(GHashTable *hash, gpointer key, gboolean search_short_name);
#endif
#ifdef QT
    QHash<QString, QString> *hash_description_yrno_table_create(void);
    QString hash_yrno_icon_table_find(QHash<QString, QString> *hash_for_icons, char *key);
    QString hash_yrno_description_table_find(QHash<QString, QString> *hash_for_description, char *key);
#endif
xmlHashTablePtr hash_icons_yrno_table_create();
xmlHashTablePtr hash_description_yrno_table_create();

/*******************************************************************************/
#endif
