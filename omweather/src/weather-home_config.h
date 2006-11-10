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
#include <libgnomevfs/gnome-vfs.h>
#include <gconf/gconf-client.h>
#include "weather-home_common.h"

#define GCONF_KEY_PREFIX "/apps/maemo/omweather"
#define GCONF_KEY_WEATHER_DIR_NAME        GCONF_KEY_PREFIX"/weather-dir"
#define GCONF_KEY_WEATHER_COUNTRY_NAME    GCONF_KEY_PREFIX"/country-name"
#define GCONF_KEY_WEATHER_STATE_NAME      GCONF_KEY_PREFIX"/state-name"
#define GCONF_KEY_WEATHER_STATION_NAME    GCONF_KEY_PREFIX"/station-name"
#define GCONF_KEY_WEATHER_STATION_ID      GCONF_KEY_PREFIX"/station-id"
#define GCONF_KEY_WEATHER_STATION_IDS      GCONF_KEY_PREFIX"/station-ids"
#define GCONF_KEY_WEATHER_ICON_SIZE       GCONF_KEY_PREFIX"/icon-size"
#define GCONF_KEY_WEATHER_FONT_COLOR      GCONF_KEY_PREFIX"/font-color"
#define GCONF_KEY_WEATHER_PERIODIC_UPDATE GCONF_KEY_PREFIX"/periodic-update"
extern gchar *_weather_dir_name;
extern gchar *_weather_country_name;
extern gchar *_weather_state_name;
extern gchar *_weather_station_name;
extern gchar *_weather_station_id;
extern gchar *_weather_icon_size;
extern int    _weather_periodic_update;
extern GdkColor _weather_font_color;
extern GdkColor DEFAULT_FONT_COLOR;
extern GSList *time_update_list;


gboolean config_set_weather_dir_name(gchar *new_weather_dir_name);


