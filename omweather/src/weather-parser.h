/* vim: set sw=4 ts=4 et: */
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
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU weather-config.h General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/
#ifndef _weather_parser_h
#define _weather_parser_h 1
/*******************************************************************************/
#include "weather-common.h"
/*******************************************************************************/
typedef struct{
    gint	error;
    xmlDoc	*doc;
    xmlNode	*weather_com_root;
}weather_com_parser;
/*******************************************************************************/
typedef struct{
    GSList	*location;
    GSList	*current;
    GSList	*days;
    gboolean	current_data_is_invalid;
    gboolean	hours_data_is_invalid;
    GSList      *hours_weather;
}WeatherStationData;
/*******************************************************************************/
typedef struct weather_data_source{
    gchar	*name;
    gchar	*url;
    gchar	*hour_url;
    gchar	*encoding;
    gint 	(*parser)(const gchar *station_id, weather_com_parser *parser,
			    WeatherStationData *wsd);
    gint 	(*parser_hour)(const gchar *station_id, weather_com_parser *parser,
				WeatherStationData *wsd);
}WeatherSource;
/*******************************************************************************/
gint parse_weather_file_data(const gchar *station_id, gchar *station_source,
					WeatherStationData *wsd,
						gboolean selected_detail_weather);
/* weather.com */
weather_com_parser *weather_parser_new_from_file(const gchar *filename,
							const gchar *encoding);

gint parse_weather_com_xml(const gchar *station_id, weather_com_parser *parser,
							WeatherStationData *wsd);
gint parse_weather_com_xml_hour(const gchar *station_id, weather_com_parser *parser,
							WeatherStationData *wsd);
/*******************************************************************************/
#endif
