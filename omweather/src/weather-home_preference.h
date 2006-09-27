/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006 Vlad Vasiliev
 * 	for the code
 *        
 * Copyright (C) Superkaramba's Liquid Weather ++ team
 *	for ALL the artwork (icons)
 *        The maintainer (also the main author I believe)
 *        is Matthew <dmbkiwi@yahoo.com>
 *  http://liquidweather.net/icons.html#iconsets
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

#include <gtk/gtk.h>
#include "weather-home_common.h"

#define STATIONS_FILE "/usr/share/weather/stations.txt"
#define COUNTRYS_FILE "/usr/share/weather/iso3166-countrycodes.txt"
extern gchar *_weather_dir_name;
extern gchar *_weather_country_name;
extern gchar *_weather_state_name;
extern gchar *_weather_station_name;
extern gchar *_weather_station_id;
extern gchar *_weather_icon_size;


void weather_window_preference (GtkWidget *widget,GdkEvent *event,gpointer user_data);

GtkWidget  *countrys , *states , *stations , *icon_size; 

static GSList *stations_list = NULL;

struct station_and_weather_code {
        gchar *station_name;
        gchar *station_code;
};

gchar *weather_station_id;
int index_state = 0; // Position state of the list 
int index_station = 0; // Position station of the list 
