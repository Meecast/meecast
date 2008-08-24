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
#ifndef _weather_parser_h
#define _weather_parser_h 1
/*******************************************************************************/
#include "weather-common.h"
#include <libxml/HTMLparser.h>
/*******************************************************************************/
gint parse_weather_file_data(const gchar *station_id, const gint station_source,
							WeatherStationData *wsd);
gint parse_weather_file_hour_data(const gchar *station_id, const gint station_source,
                                                        WeatherStationData *wsd);
weather_com_parser *weather_parser_new_from_file(const gchar *filename,
						    const gchar *encoding);
weather_com_parser *weather_parser_hour_new_from_file(const gchar *filename);
gint parse_weather_com_xml(const gchar *station_id, weather_com_parser *parser,
							WeatherStationData *wsd);
gint parse_weather_com_xml_hour(const gchar *station_id, weather_com_parser *parser,
							WeatherStationData *wsd);
gint parse_rp5_ru_xml(const gchar *station_id, weather_com_parser *parser,
							WeatherStationData *wsd);
gint parse_underground_com_data(const gchar *station);
void process_undeground_com_current_weather(const htmlNodePtr node);
void process_undeground_com_forecast_weather(const htmlNodePtr node);
/*******************************************************************************/
extern WeatherSource	weather_sources[];
/*******************************************************************************/
#endif
