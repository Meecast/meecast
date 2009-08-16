/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of omweather-weather-com-source
 *
 * Copyright (C) 2006-2009 Vlad Vasiliev
 * Copyright (C) 2006-2009 Pavel Fialko
 * 	for the code
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
#ifndef _main_h
#define _main_h 1
/*******************************************************************************/
#include <gtk/gtk.h>
#include <libxml/parser.h>
#include <stdlib.h>
/*******************************************************************************/
gint get_station_weather_data(const gchar *station_id_with_path, GHashTable *data, gboolean get_detail_data);
gint parse_xml_data(const gchar *station_id, xmlNode *root_node, GHashTable *data);
gint parse_xml_detail_data(const gchar *station_id, xmlNode *root_node, GHashTable *data);
/*******************************************************************************/
#endif
